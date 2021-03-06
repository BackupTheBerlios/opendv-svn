/*
 *   Copyright (C) 2011,2012 by Jonathan Naylor G4KLX
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "CCITTChecksumReverse.h"
#include "DVRPTRControllerV2.h"
#include "CCITTChecksum.h"
#include "DStarDefines.h"
#include "Timer.h"

#if defined(__WINDOWS__)
#include <setupapi.h>
#else
#include <wx/dir.h>
#endif

const unsigned char DVRPTR_HEADER_LENGTH = 5U;

const unsigned char DVRPTR_FRAME_START = 0xD0U;

const unsigned char DVRPTR_GET_STATUS  = 0x10U;
const unsigned char DVRPTR_GET_VERSION = 0x11U;
const unsigned char DVRPTR_GET_SERIAL  = 0x12U;
const unsigned char DVRPTR_GET_CONFIG  = 0x13U;
const unsigned char DVRPTR_SET_CONFIG  = 0x14U;
const unsigned char DVRPTR_RXPREAMBLE  = 0x15U;
const unsigned char DVRPTR_START       = 0x16U;
const unsigned char DVRPTR_HEADER      = 0x17U;
const unsigned char DVRPTR_RXSYNC      = 0x18U;
const unsigned char DVRPTR_DATA        = 0x19U;
const unsigned char DVRPTR_EOT         = 0x1AU;
const unsigned char DVRPTR_RXLOST      = 0x1BU;
const unsigned char DVRPTR_MSG_RSVD1   = 0x1CU;
const unsigned char DVRPTR_MSG_RSVD2   = 0x1DU;
const unsigned char DVRPTR_MSG_RSVD3   = 0x1EU;
const unsigned char DVRPTR_SET_TESTMDE = 0x1FU;

const unsigned char DVRPTR_ACK = 0x06U;
const unsigned char DVRPTR_NAK = 0x15U;

const unsigned int MAX_RESPONSES = 30U;

wxArrayString CDVRPTRControllerV2::getDevices()
{
#if defined(__WINDOWS__)
	return CSerialDataController::getDevices();
#else
	wxArrayString devices;
	devices.Alloc(5U);

	devices.Add(wxT("/dev/ttyACM0"));
	devices.Add(wxT("/dev/ttyACM1"));
	devices.Add(wxT("/dev/ttyACM2"));
	devices.Add(wxT("/dev/ttyACM3"));
	devices.Add(wxT("/dev/ttyACM4"));
	devices.Add(wxT("/dev/ttyACM5"));
	devices.Add(wxT("/dev/ttyACM6"));
	devices.Add(wxT("/dev/ttyACM7"));
	devices.Add(wxT("/dev/ttyACM8"));
	devices.Add(wxT("/dev/ttyACM9"));

	return devices;
#endif
}

const unsigned int BUFFER_LENGTH = 200U;

CDVRPTRControllerV2::CDVRPTRControllerV2(const wxString& port, const wxString& path, bool rxInvert, bool txInvert, bool channel, unsigned int modLevel, unsigned int txDelay) :
wxThread(wxTHREAD_JOINABLE),
m_port(port),
m_path(path),
m_rxInvert(rxInvert),
m_txInvert(txInvert),
m_channel(channel),
m_modLevel(modLevel),
m_txDelay(txDelay),
m_serial(port, SERIAL_115200),
m_buffer(NULL),
m_rxData(1000U),
m_txData(1000U),
m_txCounter(0U),
m_pktCounter(0U),
m_ptt(false),
m_rx(false),
m_txSpace(0U),
m_txEnabled(false),
m_checksum(false),
m_space(0U),
m_stopped(false),
m_mutex()
{
	wxASSERT(!port.IsEmpty());

	m_buffer = new unsigned char[BUFFER_LENGTH];
}

CDVRPTRControllerV2::~CDVRPTRControllerV2()
{
	delete[] m_buffer;
}

bool CDVRPTRControllerV2::open()
{
	findPort();

	bool ret = openModem();
	if (!ret)
		return false;

	findPath();

	Create();
	SetPriority(100U);
	Run();

	return true;
}

void* CDVRPTRControllerV2::Entry()
{
	wxLogMessage(wxT("Starting DV-RPTR Modem Controller thread"));

	// Clock every 5ms-ish
	CTimer pollTimer(200U, 0U, 250U);

	pollTimer.start();

	while (!m_stopped) {
		// Poll the modem status every 250ms
		if (pollTimer.hasExpired()) {
			bool ret = getStatus();
			if (!ret) {
				ret = findModem();
				if (!ret) {
					wxLogMessage(wxT("Stopping DV-RPTR Modem Controller thread"));
					return NULL;
				}
			}

			pollTimer.reset();
		}

		unsigned int length;
		RESP_TYPE_V2 type = getResponse(m_buffer, length);

		switch (type) {
			case RT2_TIMEOUT:
				break;

			case RT2_ERROR: {
					bool ret = findModem();
					if (!ret) {
						wxLogMessage(wxT("Stopping DV-RPTR Modem Controller thread"));
						return NULL;
					}
				}
				break;

			case RT2_RXPREAMBLE:
				// wxLogMessage(wxT("RT_PREAMBLE"));
				break;

			case RT2_START:
				// wxLogMessage(wxT("RT_START"));
				break;

			case RT2_HEADER:
				// CUtils::dump(wxT("RT_HEADER"), m_buffer, length);
				if (length == 7U) {
					if (m_buffer[4U] == DVRPTR_NAK)
						wxLogWarning(wxT("Received a header NAK from the modem"));
				} else {
					bool correct = (m_buffer[5U] & 0x80U) == 0x00U;
					if (correct) {
						m_mutex.Lock();

						unsigned int space = m_rxData.freeSpace();
						if (space < 43U) {
							wxLogMessage(wxT("Out of space in the DV-RPTR RX queue"));
						} else {
							unsigned char data[2U];
							data[0U] = DQT_HEADER;
							data[1U] = RADIO_HEADER_LENGTH_BYTES;
							m_rxData.addData(data, 2U);
							m_rxData.addData(m_buffer + 8U, RADIO_HEADER_LENGTH_BYTES);

							m_rx = true;
						}

						m_mutex.Unlock();
					}
				}
				break;

			case RT2_RXSYNC:
				// wxLogMessage(wxT("RT_RXSYNC"));
				break;

			case RT2_DATA:
				// CUtils::dump(wxT("RT_DATA"), m_buffer, length);
				if (length == 7U) {
					if (m_buffer[4U] == DVRPTR_NAK)
						wxLogWarning(wxT("Received a data NAK from the modem"));
				} else {
					m_mutex.Lock();

					unsigned int space = m_rxData.freeSpace();
					if (space < 14U) {
						wxLogMessage(wxT("Out of space in the DV-RPTR RX queue"));
					} else {
						unsigned char data[2U];
						data[0U] = DQT_DATA;
						data[1U] = DV_FRAME_LENGTH_BYTES;
						m_rxData.addData(data, 2U);
						m_rxData.addData(m_buffer + 8U, DV_FRAME_LENGTH_BYTES);
					}

					m_mutex.Unlock();
				}
				break;

			case RT2_EOT: {
					m_mutex.Lock();

					unsigned int space = m_rxData.freeSpace();
					if (space < 2U) {
						wxLogMessage(wxT("Out of space in the DV-RPTR RX queue"));
					} else {
						unsigned char data[2U];
						data[0U] = DQT_EOT;
						data[1U] = 0U;
						m_rxData.addData(data, 2U);

						m_rx = false;
					}

					m_mutex.Unlock();
					// wxLogMessage(wxT("RT_EOT"));
				}
				break;

			case RT2_RXLOST: {
					m_mutex.Lock();

					unsigned int space = m_rxData.freeSpace();
					if (space < 2U) {
						wxLogMessage(wxT("Out of space in the DV-RPTR RX queue"));
					} else {
						unsigned char data[2U];
						data[0U] = DQT_LOST;
						data[1U] = 0U;
						m_rxData.addData(data, 2U);

						m_rx = false;
					}

					m_mutex.Unlock();
					// wxLogMessage(wxT("RT_LOST"));
				}
				break;

			case RT2_GET_STATUS: {
					m_txEnabled = (m_buffer[4U] & 0x02U) == 0x02U;
					m_checksum  = (m_buffer[4U] & 0x08U) == 0x08U;
					m_ptt       = (m_buffer[5U] & 0x02U) == 0x02U;
					m_txSpace   = m_buffer[8U];
					m_space     = m_txSpace - m_buffer[9U];
					// CUtils::dump(wxT("GET_STATUS"), m_buffer, length);
					// wxLogMessage(wxT("PTT=%d tx=%u space=%u cksum=%d, tx enabled=%d"), int(m_ptt), m_txSpace, m_space, int(m_checksum), int(m_txEnabled));
				}
				break;

			// These should not be received in this loop, but don't complain if we do
			case RT2_GET_VERSION:
			case RT2_GET_SERIAL:
			case RT2_GET_CONFIG:
				break;

			default:
				wxLogMessage(wxT("Unknown message, type: %02X"), m_buffer[3U]);
				CUtils::dump(wxT("Buffer dump"), m_buffer, length);
				break;
		}

		if (m_space > 0U) {
			if (!m_txData.isEmpty()) {
				m_mutex.Lock();

				unsigned char len = 0U;
				m_txData.getData(&len, 1U);

				unsigned char data[100U];
				m_txData.getData(data, len);

				m_mutex.Unlock();

				// CUtils::dump(wxT("Write"), data, len);

				int ret = m_serial.write(data, len);
				if (ret == -1) {
					bool ret = findModem();
					if (!ret) {
						wxLogMessage(wxT("Stopping DV-RPTR Modem Controller thread"));
						return NULL;
					}
				} else {
					m_space--;
				}
			}
		}

		Sleep(5UL);

		pollTimer.clock();
	}

	wxLogMessage(wxT("Stopping DV-RPTR Modem Controller thread"));

	setEnabled(false);

	m_serial.close();

	return NULL;
}

void CDVRPTRControllerV2::purgeRX()
{
	wxMutexLocker locker(m_mutex);

	m_rxData.clear();
}

void CDVRPTRControllerV2::purgeTX()
{
	wxMutexLocker locker(m_mutex);

	m_txData.clear();
}

DATA_QUEUE_TYPE CDVRPTRControllerV2::readQueue(unsigned char* data, unsigned int& length)
{
	wxMutexLocker locker(m_mutex);

	if (m_rxData.isEmpty())
		return DQT_NONE;

	unsigned char header[2U];
	m_rxData.getData(header, 2U);

	DATA_QUEUE_TYPE type = DATA_QUEUE_TYPE(header[0U]);

	length = header[1U];

	if (length > 0U)
		m_rxData.getData(data, length);

	return type;
}

bool CDVRPTRControllerV2::writeStart()
{
	if (!m_txEnabled)
		return false;

	m_txCounter++;
	if (m_txCounter == 0U)
		m_txCounter = 1U;

	unsigned char buffer[10U];

	buffer[0U] = DVRPTR_FRAME_START;

	buffer[1U] = 0x03U;
	buffer[2U] = 0x00U;

	buffer[3U] = DVRPTR_START;

	buffer[4U] = m_txCounter;
	buffer[5U] = 0x00U;

	if (m_checksum) {
		CCCITTChecksum cksum;
		cksum.update(buffer + 0U, 6U);
		cksum.result(buffer + 6U);
	} else {
		buffer[6U] = 0x00U;
		buffer[7U] = 0x0BU;
	}

	wxMutexLocker locker(m_mutex);

	unsigned int space = m_txData.freeSpace();
	if (space < 9U)
		return false;

	unsigned char len = 8U;
	m_txData.addData(&len, 1U);

	m_txData.addData(buffer, 8U);

	return true;
}

bool CDVRPTRControllerV2::writeHeader(const CHeaderData& header)
{
	if (!m_txEnabled)
		return false;

	unsigned char buffer[60U];

	buffer[0U] = DVRPTR_FRAME_START;

	buffer[1U] = 0x2FU;
	buffer[2U] = 0x00U;

	buffer[3U] = DVRPTR_HEADER;

	buffer[4U] = m_txCounter;
	buffer[5U] = 0x00U;

	buffer[6U] = 0x00U;
	buffer[7U] = 0x00U;

	::memset(buffer + 8U, ' ', RADIO_HEADER_LENGTH_BYTES);

	buffer[8U]  = header.getFlag1();
	buffer[9U]  = header.getFlag2();
	buffer[10U] = header.getFlag3();

	wxString rpt2 = header.getRptCall2();
	for (unsigned int i = 0U; i < rpt2.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer[i + 11U]  = rpt2.GetChar(i);

	wxString rpt1 = header.getRptCall1();
	for (unsigned int i = 0U; i < rpt1.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer[i + 19U] = rpt1.GetChar(i);

	wxString your = header.getYourCall();
	for (unsigned int i = 0U; i < your.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer[i + 27U] = your.GetChar(i);

	wxString my1 = header.getMyCall1();
	for (unsigned int i = 0U; i < my1.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer[i + 35U] = my1.GetChar(i);

	wxString my2 = header.getMyCall2();
	for (unsigned int i = 0U; i < my2.Len() && i < SHORT_CALLSIGN_LENGTH; i++)
		buffer[i + 43U] = my2.GetChar(i);

	CCCITTChecksumReverse cksum1;
	cksum1.update(buffer + 8U, RADIO_HEADER_LENGTH_BYTES - 2U);
	cksum1.result(buffer + 47U);

	buffer[49U] = 0x00U;

	if (m_checksum) {
		CCCITTChecksum cksum2;
		cksum2.update(buffer + 0U, 50U);
		cksum2.result(buffer + 50U);
	} else {
		buffer[50U] = 0x00U;
		buffer[51U] = 0x0BU;
	}

	m_pktCounter = 0U;

	wxMutexLocker locker(m_mutex);

	unsigned int space = m_txData.freeSpace();
	if (space < 53U)
		return false;

	unsigned char len = 52U;
	m_txData.addData(&len, 1U);

	m_txData.addData(buffer, 52U);

	return true;
}

bool CDVRPTRControllerV2::writeData(const unsigned char* data, unsigned int length)
{
	if (!m_txEnabled)
		return false;

	unsigned char buffer[30U];

	buffer[0U] = DVRPTR_FRAME_START;

	buffer[1U] = 0x13U;
	buffer[2U] = 0x00U;

	buffer[3U] = DVRPTR_DATA;

	buffer[4U] = m_txCounter;
	buffer[5U] = m_pktCounter;

	m_pktCounter++;
	if (m_pktCounter >= m_txSpace)
		m_pktCounter = 0U;

	buffer[6U] = 0x00U;
	buffer[7U] = 0x00U;

	::memcpy(buffer + 8U, data, DV_FRAME_LENGTH_BYTES);

	buffer[20U] = 0x00U;
	buffer[21U] = 0x00U;

	if (m_checksum) {
		CCCITTChecksum cksum;
		cksum.update(buffer + 0U, 22U);
		cksum.result(buffer + 22U);
	} else {
		buffer[22U] = 0x00U;
		buffer[23U] = 0x0BU;
	}

	wxMutexLocker locker(m_mutex);

	unsigned int space = m_txData.freeSpace();
	if (space < 25U)
		return false;

	unsigned char len = 24U;
	m_txData.addData(&len, 1U);

	m_txData.addData(buffer, 24U);

	return true;
}

bool CDVRPTRControllerV2::writeEnd()
{
	if (!m_txEnabled)
		return false;

	unsigned char buffer[10U];

	buffer[0U] = DVRPTR_FRAME_START;

	buffer[1U] = 0x03U;
	buffer[2U] = 0x00U;

	buffer[3U] = DVRPTR_EOT;

	buffer[4U] = m_txCounter;
	buffer[5U] = 0xFFU;

	if (m_checksum) {
		CCCITTChecksum cksum;
		cksum.update(buffer + 0U, 6U);
		cksum.result(buffer + 6U);
	} else {
		buffer[6U] = 0x00U;
		buffer[7U] = 0x0BU;
	}

	wxMutexLocker locker(m_mutex);

	unsigned int space = m_txData.freeSpace();
	if (space < 9U)
		return false;

	unsigned char len = 8U;
	m_txData.addData(&len, 1U);

	m_txData.addData(buffer, 8U);

	return true;
}

bool CDVRPTRControllerV2::getPTT() const
{
	return m_ptt;
}

bool CDVRPTRControllerV2::hasSpace()
{
	wxMutexLocker locker(m_mutex);

	return m_txData.freeSpace() > 70U;
}

void CDVRPTRControllerV2::close()
{
	m_stopped = true;

	Wait();
}

bool CDVRPTRControllerV2::getVersion()
{
	unsigned char buffer[10U];

	buffer[0U] = DVRPTR_FRAME_START;

	buffer[1U] = 0x01U;
	buffer[2U] = 0x00U;

	buffer[3U] = DVRPTR_GET_VERSION;

	if (m_checksum) {
		CCCITTChecksum cksum;
		cksum.update(buffer + 0U, 4U);
		cksum.result(buffer + 4U);
	} else {
		buffer[4U] = 0x00U;
		buffer[5U] = 0x0BU;
	}

	// CUtils::dump(wxT("Written"), buffer, 6U);

	int ret = m_serial.write(buffer, 6U);
	if (ret != 6)
		return false;

	unsigned int count = 0U;
	unsigned int length;
	RESP_TYPE_V2 resp;
	do {
		::wxMilliSleep(10UL);

		resp = getResponse(m_buffer, length);

		if (resp != RT2_GET_VERSION) {
			count++;
			if (count >= MAX_RESPONSES) {
				wxLogError(wxT("The DV-RPTR modem is not responding to the version command"));
				return false;
			}
		}
	} while (resp != RT2_GET_VERSION);

	wxString firmware;
	if ((m_buffer[4U] & 0x0FU) > 0x00U)
		firmware.Printf(wxT("%u.%u%u%c"), (m_buffer[5U] & 0xF0U) >> 4, m_buffer[5U] & 0x0FU, (m_buffer[4U] & 0xF0U) >> 4, (m_buffer[4U] & 0x0FU) + wxT('a') - 1U);
	else
		firmware.Printf(wxT("%u.%u%u"), (m_buffer[5U] & 0xF0U) >> 4, m_buffer[5U] & 0x0FU, (m_buffer[4U] & 0xF0U) >> 4);

	wxString hardware((char*)(m_buffer + 6U), wxConvLocal, length - DVRPTR_HEADER_LENGTH - 3U);

	wxLogInfo(wxT("DV-RPTR Modem Firmware version: %s, hardware: %s"), firmware.c_str(), hardware.c_str());

	return true;
}

bool CDVRPTRControllerV2::getStatus()
{
	unsigned char buffer[10U];

	buffer[0U] = DVRPTR_FRAME_START;

	buffer[1U] = 0x01U;
	buffer[2U] = 0x00U;

	buffer[3U] = DVRPTR_GET_STATUS;

	if (m_checksum) {
		CCCITTChecksum cksum;
		cksum.update(buffer + 0U, 4U);
		cksum.result(buffer + 4U);
	} else {
		buffer[4U] = 0x00U;
		buffer[5U] = 0x0BU;
	}

	return m_serial.write(buffer, 6U) == 6;
}

bool CDVRPTRControllerV2::setConfig()
{
	unsigned char buffer[20U];

	buffer[0U] = DVRPTR_FRAME_START;

	buffer[1U] = 0x07U;
	buffer[2U] = 0x00U;

	buffer[3U] = DVRPTR_SET_CONFIG;

	buffer[4U] = 0xC0U;		// Physical layer

	buffer[5U] = 0x04U;		// Block length

	buffer[6U] = 0x00U;
	if (m_rxInvert)
		buffer[6U] |= 0x01U;
	if (m_txInvert)
		buffer[6U] |= 0x02U;
	if (m_channel)
		buffer[6U] |= 0x04U;

	buffer[7U] = (m_modLevel * 256U) / 100U;

	wxUint16* txDelay = (wxUint16*)(buffer + 8U);
	*txDelay = wxUINT16_SWAP_ON_BE(m_txDelay);

	if (m_checksum) {
		CCCITTChecksum cksum;
		cksum.update(buffer + 0U, 10U);
		cksum.result(buffer + 10U);
	} else {
		buffer[10U] = 0x00U;
		buffer[11U] = 0x0BU;
	}

	// CUtils::dump(wxT("Written"), buffer, 12U);

	int ret = m_serial.write(buffer, 12U);
	if (ret != 12)
		return false;

	unsigned int count = 0U;
	unsigned int length;
	RESP_TYPE_V2 resp;
	do {

		::wxMilliSleep(10UL);

		resp = getResponse(m_buffer, length);

		if (resp != RT2_SET_CONFIG) {
			count++;
			if (count >= MAX_RESPONSES) {
				wxLogError(wxT("The DV-RPTR modem is not responding to the SET_CONFIG command"));
				return false;
			}
		}
	} while (resp != RT2_SET_CONFIG);

	// CUtils::dump(wxT("Response"), m_buffer, length);

	unsigned char type = m_buffer[4U];
	if (type != DVRPTR_ACK) {
		wxLogError(wxT("Received a NAK to the SET_CONFIG command from the modem"));
		return false;
	}

	return true;
}

bool CDVRPTRControllerV2::setEnabled(bool enable)
{
	unsigned char buffer[10U];

	buffer[0U] = DVRPTR_FRAME_START;

	buffer[1U] = 0x02U;
	buffer[2U] = 0x00U;

	buffer[3U] = DVRPTR_GET_STATUS;

	// Enable RX, TX, and Watchdog
	if (enable)
		buffer[4U] = 0x01U | 0x02U | 0x04U;
	else
		buffer[4U] = 0x00U;

	if (m_checksum) {
		CCCITTChecksum cksum;
		cksum.update(buffer + 0U, 5U);
		cksum.result(buffer + 5U);
	} else {
		buffer[5U] = 0x00U;
		buffer[6U] = 0x0BU;
	}

	// CUtils::dump(wxT("Written"), buffer, 7U);

	int ret = m_serial.write(buffer, 7U);
	if (ret != 7)
		return false;

	unsigned int count = 0U;
	unsigned int length;
	RESP_TYPE_V2 resp;
	do {
		::wxMilliSleep(10UL);

		resp = getResponse(m_buffer, length);

		if (resp != RT2_GET_STATUS) {
			count++;
			if (count >= MAX_RESPONSES) {
				wxLogError(wxT("The DV-RPTR modem is not responding to the SET_STATUS command"));
				return false;
			}
		}
	} while (resp != RT2_GET_STATUS);

	// CUtils::dump(wxT("Response"), m_buffer, length);

	unsigned char type = m_buffer[4U];
	if (type != DVRPTR_ACK) {
		wxLogError(wxT("Received a NAK to the SET_STATUS command from the modem"));
		return false;
	}

	return true;
}

RESP_TYPE_V2 CDVRPTRControllerV2::getResponse(unsigned char *buffer, unsigned int& length)
{
	// Get the start of the frame or nothing at all
	int ret = m_serial.read(buffer, 1U);
	if (ret < 0) {
		wxLogError(wxT("Error when reading from the DV-RPTR"));
		return RT2_ERROR;
	}

	if (ret == 0)
		return RT2_TIMEOUT;

	if (buffer[0U] != DVRPTR_FRAME_START) {
		wxLogError(wxT("DV-RPTR frame start is incorrect - 0x%02X"), buffer[0U]);
		return RT2_UNKNOWN;
	}

	unsigned int offset = 1U;

	while (offset < DVRPTR_HEADER_LENGTH) {
		int ret = m_serial.read(buffer + offset, DVRPTR_HEADER_LENGTH - offset);
		if (ret < 0) {
			wxLogError(wxT("Error when reading from the DV-RPTR"));
			return RT2_ERROR;
		}

		if (ret > 0)
			offset += ret;

		if (ret == 0)
			Sleep(5UL);
	}

	length = buffer[1U] + buffer[2U] * 256U;

	// Remove the response bit
	unsigned int type = buffer[3U] & 0x7FU;

	offset = 0U;

	while (offset < length) {
		int ret = m_serial.read(buffer + offset + DVRPTR_HEADER_LENGTH, length - offset);
		if (ret < 0) {
			wxLogError(wxT("Error when reading from the DV-RPTR"));
			return RT2_ERROR;
		}

		if (ret > 0)
			offset += ret;

		if (ret == 0)
			Sleep(5UL);
	}

	length += DVRPTR_HEADER_LENGTH;

	// CUtils::dump(wxT("Received"), buffer, length);

	switch (type) {
		case DVRPTR_GET_STATUS:
			return RT2_GET_STATUS;
		case DVRPTR_GET_VERSION:
			return RT2_GET_VERSION;
		case DVRPTR_GET_SERIAL:
			return RT2_GET_SERIAL;
		case DVRPTR_GET_CONFIG:
			return RT2_GET_CONFIG;
		case DVRPTR_SET_CONFIG:
			return RT2_SET_CONFIG;
		case DVRPTR_RXPREAMBLE:
			return RT2_RXPREAMBLE;
		case DVRPTR_START:
			return RT2_START;
		case DVRPTR_HEADER:
			return RT2_HEADER;
		case DVRPTR_RXSYNC:
			return RT2_RXSYNC;
		case DVRPTR_DATA:
			return RT2_DATA;
		case DVRPTR_EOT:
			return RT2_EOT;
		case DVRPTR_RXLOST:
			return RT2_RXLOST;
		case DVRPTR_SET_TESTMDE:
			return RT2_SET_TESTMDE;
		default:
			return RT2_UNKNOWN;
	}
}

wxString CDVRPTRControllerV2::getPath() const
{
	return m_path;
}

bool CDVRPTRControllerV2::findPort()
{
	if (m_path.IsEmpty())
		return false;

#if defined(__WINDOWS__)
#else
	wxDir dir;
	bool ret1 = dir.Open(wxT("/sys/class/tty"));
	if (!ret1) {
		wxLogError(wxT("Cannot open directory /sys/class/tty"));
		return false;
	}

	wxString fileName;
	ret1 = dir.GetFirst(&fileName, wxT("ttyACM*"));
	while (ret1) {
		wxString path;
		path.Printf(wxT("/sys/class/tty/%s"), fileName.c_str());

		char cpath[255U];
		::memset(cpath, 0x00U, 255U);

		for (unsigned int i = 0U; i < path.Len(); i++)
			cpath[i] = path.GetChar(i);

		char symlink[255U];
		int ret2 = ::readlink(cpath, symlink, 255U);
		if (ret2 < 0) {
			wxLogError(wxT("Error from readlink()"));
			return false;
		}

		// Get all but the last section
		wxString fullPath = wxString(symlink, wxConvLocal, ret2);
		path = fullPath.BeforeLast(wxT('/'));

		if (path.IsSameAs(m_path)) {
			m_port.Printf(wxT("/dev/%s"), fileName.c_str());

			wxLogMessage(wxT("Found modem port of %s based on the path"), m_port.c_str());

			return true;
		}

		ret1 = dir.GetNext(&fileName);
	}
#endif

	return false;
}

bool CDVRPTRControllerV2::findPath()
{
#if defined(__WINDOWS__)
#ifdef notdef
	GUID guids[5U];

	DWORD count;
	BOOL res = ::SetupDiClassGuidsFromName(L"Multifunction", guids, 5U, &count);
	if (!res) {
		wxLogError(wxT("Error from SetupDiClassGuidsFromName: err=%u"), ::GetLastError());
		return false;
	}

	for (DWORD i = 0U; i < count; i++) {
		HDEVINFO devInfo = ::SetupDiGetClassDevs(&guids[i], NULL, NULL, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);
		if (devInfo == INVALID_HANDLE_VALUE) {
			wxLogError(wxT("Error from SetupDiGetClassDevs: err=%u"), ::GetLastError());
			return false;
		}

		SP_DEVICE_INTERFACE_DATA devInfoData;
		devInfoData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

		for (DWORD index = 0U; ::SetupDiEnumDeviceInterfaces(devInfo, NULL, &guids[i], index, &devInfoData); index++) {
			// Find the required length of the device structure
			DWORD length;
			::SetupDiGetDeviceInterfaceDetail(devInfo, &devInfoData, NULL, 0U, &length, NULL);

			PSP_DEVICE_INTERFACE_DETAIL_DATA detailData = PSP_DEVICE_INTERFACE_DETAIL_DATA(::malloc(length));
			detailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

			// Get the detailed data into the newly allocated device structure
			DWORD required;
			res = ::SetupDiGetDeviceInterfaceDetail(devInfo, &devInfoData, detailData, length, &required, NULL);
			if (!res) {
				wxLogError(wxT("Error from SetupDiGetDeviceInterfaceDetail: err=%u"), ::GetLastError());
				::SetupDiDestroyDeviceInfoList(devInfo);
				::free(detailData);
				return false;
			}

			::free(detailData);
		}

		::SetupDiDestroyDeviceInfoList(devInfo);
	}

	return false;
#endif
#else
	wxString path;
	path.Printf(wxT("/sys/class/tty/%s"), m_port.Mid(5U).c_str());	

	char cpath[255U];
	::memset(cpath, 0x00U, 255U);

	for (unsigned int i = 0U; i < path.Len(); i++)
		cpath[i] = path.GetChar(i);

	char symlink[255U];
	int ret = ::readlink(cpath, symlink, 255U);
	if (ret < 0) {
		wxLogError(wxT("Error from readlink()"));
		return false;
	}

	wxString fullPath = wxString(symlink, wxConvLocal, ret);
	path = fullPath.BeforeLast(wxT('/'));

	if (m_path.IsEmpty())
		wxLogMessage(wxT("Found modem path of %s"), path.c_str());

	m_path = path;
#endif

	return true;
}

bool CDVRPTRControllerV2::findModem()
{
	m_serial.close();

	// Tell the repeater that the signal has gone away
	if (m_rx) {
		purgeRX();

		m_mutex.Lock();
		unsigned char data[2U];
		data[0U] = DQT_EOT;
		data[1U] = 0U;
		m_rxData.addData(data, 2U);
		m_mutex.Unlock();

		m_rx = false;
	}

	unsigned int count = 0U;

	// Purge the transmit buffer every 500ms to avoid overflow, but only try and reopen the modem every 2s
	while (!m_stopped) {
		purgeTX();

		count++;
		if (count >= 4U) {
			wxLogMessage(wxT("Trying to reopen the modem"));

			bool ret = findPort();
			if (ret) {
				ret = openModem();
				if (ret)
					return true;
			}

			count = 0U;
		}

		Sleep(500UL);
	}

	return false;
}

bool CDVRPTRControllerV2::openModem()
{
	bool ret = m_serial.open();
	if (!ret)
		return false;

	ret = getVersion();
	if (!ret) {
		m_serial.close();
		return false;
	}

	ret = setConfig();
	if (!ret) {
		m_serial.close();
		return false;
	}

	ret = setEnabled(true);
	if (!ret) {
		m_serial.close();
		return false;
	}

	return true;
}


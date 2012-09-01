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
#include "DVRPTRController.h"
#include "CCITTChecksum.h"
#include "DStarDefines.h"
#include "Timer.h"

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

wxArrayString CDVRPTRController::getDevices()
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

	return devices;
#endif
}

const unsigned int BUFFER_LENGTH = 200U;

CDVRPTRController::CDVRPTRController(const wxString& port, bool rxInvert, bool txInvert, bool channel, unsigned int modLevel, unsigned int txDelay) :
wxThread(wxTHREAD_JOINABLE),
m_port(port),
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

CDVRPTRController::~CDVRPTRController()
{
	delete[] m_buffer;
}

bool CDVRPTRController::open()
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

	Create();
	SetPriority(100U);
	Run();

	return true;
}

void* CDVRPTRController::Entry()
{
	wxLogMessage(wxT("Starting DV-RPTR Modem Controller thread"));

	// Clock every 5ms-ish
	CTimer pollTimer(200U, 0U, 250U);

	pollTimer.start();

	while (!m_stopped) {
		// Poll the modem status every 250ms
		if (pollTimer.hasExpired()) {
			getStatus();
			pollTimer.reset();
		}

		unsigned int length;
		RESP_TYPE type = getResponse(m_buffer, length);

		switch (type) {
			case RT_TIMEOUT:
				break;

			case RT_ERROR:
				wxLogMessage(wxT("Stopping DV-RPTR Modem Controller thread"));
				m_serial.close();
				return NULL;

			case RT_RXPREAMBLE:
				// wxLogMessage(wxT("RT_PREAMBLE"));
				break;

			case RT_START:
				// wxLogMessage(wxT("RT_START"));
				break;

			case RT_HEADER:
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
						}

						m_mutex.Unlock();
					}
				}
				break;

			case RT_RXSYNC:
				// wxLogMessage(wxT("RT_RXSYNC"));
				break;

			case RT_DATA:
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

			case RT_EOT: {
					m_mutex.Lock();

					unsigned int space = m_rxData.freeSpace();
					if (space < 2U) {
						wxLogMessage(wxT("Out of space in the DV-RPTR RX queue"));
					} else {
						unsigned char data[2U];
						data[0U] = DQT_EOT;
						data[1U] = 0U;
						m_rxData.addData(data, 2U);
					}

					m_mutex.Unlock();
					// wxLogMessage(wxT("RT_EOT"));
				}
				break;

			case RT_RXLOST: {
					m_mutex.Lock();

					unsigned int space = m_rxData.freeSpace();
					if (space < 2U) {
						wxLogMessage(wxT("Out of space in the DV-RPTR RX queue"));
					} else {
						unsigned char data[2U];
						data[0U] = DQT_LOST;
						data[1U] = 0U;
						m_rxData.addData(data, 2U);
					}

					m_mutex.Unlock();
					// wxLogMessage(wxT("RT_LOST"));
				}
				break;

			case RT_GET_STATUS: {
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
			case RT_GET_VERSION:
			case RT_GET_SERIAL:
			case RT_GET_CONFIG:
				break;

			default:
				wxLogMessage(wxT("Unknown message, type: %02X"), m_buffer[3U]);
				CUtils::dump(wxT("Buffer dump"), m_buffer, length);
				break;
		}

		if (m_space > 0U) {
			m_mutex.Lock();
			if (!m_txData.isEmpty()) {
				unsigned char len;
				m_txData.getData(&len, 1U);

				unsigned char data[100U];
				m_txData.getData(data, len);
				m_mutex.Unlock();

				// CUtils::dump(wxT("Write"), data, len);

				int ret = m_serial.write(data, len);
				if (ret != int(len))
					wxLogWarning(wxT("Error when writing data to the modem"));

				m_space--;
			} else {
				m_mutex.Unlock();
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

DATA_QUEUE_TYPE CDVRPTRController::readQueue(unsigned char* data, unsigned int& length)
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

bool CDVRPTRController::writeStart()
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
	if (space < 9U) {
		wxLogMessage(wxT("Out of space in the DV-RPTR TX queue"));
		return false;
	}

	unsigned char len = 8U;
	m_txData.addData(&len, 1U);

	m_txData.addData(buffer, 8U);

	return true;
}

bool CDVRPTRController::writeHeader(const CHeaderData& header)
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
	if (space < 53U) {
		wxLogMessage(wxT("Out of space in the DV-RPTR TX queue"));
		return false;
	}

	unsigned char len = 52U;
	m_txData.addData(&len, 1U);

	m_txData.addData(buffer, 52U);

	return true;
}

bool CDVRPTRController::writeData(const unsigned char* data, unsigned int length)
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
	if (space < 25U) {
		wxLogMessage(wxT("Out of space in the DV-RPTR TX queue"));
		return false;
	}

	unsigned char len = 24U;
	m_txData.addData(&len, 1U);

	m_txData.addData(buffer, 24U);

	return true;
}

bool CDVRPTRController::writeEnd()
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
	if (space < 9U) {
		wxLogMessage(wxT("Out of space in the DV-RPTR TX queue"));
		return false;
	}

	unsigned char len = 8U;
	m_txData.addData(&len, 1U);

	m_txData.addData(buffer, 8U);

	return true;
}

bool CDVRPTRController::getPTT() const
{
	return m_ptt;
}

bool CDVRPTRController::hasSpace()
{
	wxMutexLocker locker(m_mutex);

	return m_txData.freeSpace() > 70U;
}

void CDVRPTRController::close()
{
	m_stopped = true;

	Wait();
}

bool CDVRPTRController::getVersion()
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
	RESP_TYPE resp;
	do {
		::wxMilliSleep(10UL);

		resp = getResponse(m_buffer, length);

		if (resp != RT_GET_VERSION) {
			count++;
			if (count >= MAX_RESPONSES) {
				wxLogError(wxT("The DV-RPTR modem is not responding to the version command"));
				return false;
			}
		}
	} while (resp != RT_GET_VERSION);

	wxString firmware;
	if ((m_buffer[4U] & 0x0FU) > 0x00U)
		firmware.Printf(wxT("%u.%u%u%c"), (m_buffer[5U] & 0xF0U) >> 4, m_buffer[5U] & 0x0FU, (m_buffer[4U] & 0xF0U) >> 4, (m_buffer[4U] & 0x0FU) + wxT('a') - 1U);
	else
		firmware.Printf(wxT("%u.%u%u"), (m_buffer[5U] & 0xF0U) >> 4, m_buffer[5U] & 0x0FU, (m_buffer[4U] & 0xF0U) >> 4);

	wxString hardware((char*)(m_buffer + 6U), wxConvLocal, length - DVRPTR_HEADER_LENGTH - 3U);

	wxLogInfo(wxT("DV-RPTR Modem Firmware version: %s, hardware: %s"), firmware.c_str(), hardware.c_str());

	return true;
}

void CDVRPTRController::getStatus()
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

	int ret = m_serial.write(buffer, 6U);
	if (ret != 6)
		wxLogWarning(wxT("Error when writing data to the modem"));
}

bool CDVRPTRController::setConfig()
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
	RESP_TYPE resp;
	do {
		::wxMilliSleep(10UL);

		resp = getResponse(m_buffer, length);

		if (resp != RT_SET_CONFIG) {
			count++;
			if (count >= MAX_RESPONSES) {
				wxLogError(wxT("The DV-RPTR modem is not responding to the SET_CONFIG command"));
				return false;
			}
		}
	} while (resp != RT_SET_CONFIG);

	// CUtils::dump(wxT("Response"), m_buffer, length);

	unsigned char type = m_buffer[4U];
	if (type != DVRPTR_ACK) {
		wxLogError(wxT("Received a NAK to the SET_CONFIG command from the modem"));
		return false;
	}

	return true;
}

bool CDVRPTRController::setEnabled(bool enable)
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
	RESP_TYPE resp;
	do {
		::wxMilliSleep(10UL);

		resp = getResponse(m_buffer, length);

		if (resp != RT_GET_STATUS) {
			count++;
			if (count >= MAX_RESPONSES) {
				wxLogError(wxT("The DV-RPTR modem is not responding to the SET_STATUS command"));
				return false;
			}
		}
	} while (resp != RT_GET_STATUS);

	// CUtils::dump(wxT("Response"), m_buffer, length);

	unsigned char type = m_buffer[4U];
	if (type != DVRPTR_ACK) {
		wxLogError(wxT("Received a NAK to the SET_STATUS command from the modem"));
		return false;
	}

	return true;
}

RESP_TYPE CDVRPTRController::getResponse(unsigned char *buffer, unsigned int& length)
{
	// Get the start of the frame or nothing at all
	int ret = m_serial.read(buffer, 1U);
	if (ret < 0) {
		wxLogError(wxT("Error when reading from the DV-RPTR"));
		return RT_ERROR;
	}

	if (ret == 0)
		return RT_TIMEOUT;

	if (buffer[0U] != DVRPTR_FRAME_START) {
		wxLogError(wxT("DV-RPTR frame start is incorrect - 0x%02X"), buffer[0U]);
		return RT_UNKNOWN;
	}

	unsigned int offset = 1U;

	while (offset < DVRPTR_HEADER_LENGTH) {
		int ret = m_serial.read(buffer + offset, DVRPTR_HEADER_LENGTH - offset);
		if (ret < 0) {
			wxLogError(wxT("Error when reading from the DV-RPTR"));
			return RT_ERROR;
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
			return RT_ERROR;
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
			return RT_GET_STATUS;
		case DVRPTR_GET_VERSION:
			return RT_GET_VERSION;
		case DVRPTR_GET_SERIAL:
			return RT_GET_SERIAL;
		case DVRPTR_GET_CONFIG:
			return RT_GET_CONFIG;
		case DVRPTR_SET_CONFIG:
			return RT_SET_CONFIG;
		case DVRPTR_RXPREAMBLE:
			return RT_RXPREAMBLE;
		case DVRPTR_START:
			return RT_START;
		case DVRPTR_HEADER:
			return RT_HEADER;
		case DVRPTR_RXSYNC:
			return RT_RXSYNC;
		case DVRPTR_DATA:
			return RT_DATA;
		case DVRPTR_EOT:
			return RT_EOT;
		case DVRPTR_RXLOST:
			return RT_RXLOST;
		case DVRPTR_SET_TESTMDE:
			return RT_SET_TESTMDE;
		default:
			return RT_UNKNOWN;
	}
}

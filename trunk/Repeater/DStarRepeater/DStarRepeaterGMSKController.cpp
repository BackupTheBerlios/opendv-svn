/*
 *   Copyright (C) 2011,2012,2013 by Jonathan Naylor G4KLX
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

#include "DStarRepeaterGMSKController.h"
#if defined(__WINDOWS__)
#include "GMSKModemWinUSB.h"
#endif
#include "GMSKModemLibUsb.h"
#include "Timer.h"

const unsigned char DVRPTR_HEADER_LENGTH = 5U;

const unsigned int BUFFER_LENGTH = 200U;

const unsigned char TAG_HEADER   = 0x00U;
const unsigned char TAG_DATA     = 0x01U;
const unsigned char TAG_DATA_END = 0x02U;

CDStarRepeaterGMSKController::CDStarRepeaterGMSKController(USB_INTERFACE iface, unsigned int address) :
wxThread(wxTHREAD_JOINABLE),
m_modem(NULL),
m_buffer(NULL),
m_rxData(1000U),
m_txData(1000U),
m_tx(false),
m_space(0U),
m_stopped(false),
m_mutex(),
m_readType(),
m_readLength(0U),
m_readBuffer(NULL)
{
	wxASSERT(address > 0U);

	m_buffer     = new unsigned char[BUFFER_LENGTH];
	m_readBuffer = new unsigned char[BUFFER_LENGTH];

#if defined(__WINDOWS__)
	switch (iface) {
		case UI_LIBUSB:
			m_modem = new CGMSKModemLibUsb(address);
			break;
		case UI_WINUSB:
			m_modem = new CGMSKModemWinUSB(address);
			break;
		default:
			wxLogError(wxT("Unknown GMSM modem driver type - %d"), int(iface));
			break;
	}
#else
	m_modem = new CGMSKModemLibUsb(address);
#endif
}

CDStarRepeaterGMSKController::~CDStarRepeaterGMSKController()
{
	delete[] m_buffer;
	delete[] m_readBuffer;

	delete m_modem;
}

bool CDStarRepeaterGMSKController::start()
{
	if (m_modem == NULL)
		return false;

	bool ret = m_modem->open();
	if (!ret)
		return false;

	Create();
	SetPriority(100U);
	Run();

	return true;
}

void* CDStarRepeaterGMSKController::Entry()
{
	wxLogMessage(wxT("Starting GMSK Modem Controller thread"));

	CTimer txTimer(200U, 0U, 100U);
	txTimer.start();

	CTimer hdrTimer(200U, 0U, 50U);
	hdrTimer.start();

	CTimer spaceTimer(200U, 0U, 100U);
	spaceTimer.start();

	CTimer dataTimer(200U, 0U, 100U);

	bool rx = false;

	unsigned char  writeType   = TAG_HEADER;;
	unsigned char  writeLength = 0U;
	unsigned char* writeBuffer = new unsigned char[BUFFER_LENGTH];

	unsigned char  readLength = 0U;
	unsigned char* readBuffer = new unsigned char[DV_FRAME_LENGTH_BYTES];

	while (!m_stopped) {
		if (rx) {
			if (!dataTimer.isRunning() || dataTimer.hasExpired()) {
				dataTimer.stop();

				unsigned char buffer[GMSK_MODEM_DATA_LENGTH];
				bool end;
				int ret = m_modem->readData(buffer, GMSK_MODEM_DATA_LENGTH, end);
				if (ret >= 0) {
					// CUtils::dump(wxT("Read Data"), buffer, ret);

					if (end) {
						unsigned char data[2U];
						data[0U] = TAG_DATA_END;
						data[1U] = DV_FRAME_LENGTH_BYTES;
						m_rxData.addData(data, 2U);

						m_rxData.addData(END_PATTERN_BYTES, DV_FRAME_LENGTH_BYTES);

						dataTimer.stop();
						hdrTimer.start();
						rx = false;
					}

					for (int i = 0; i < ret; i++) {
						readBuffer[readLength] = buffer[i];

						readLength++;
						if (readLength >= DV_FRAME_LENGTH_BYTES) {
							unsigned char data[2U];
							data[0U] = TAG_DATA;
							data[1U] = DV_FRAME_LENGTH_BYTES;
							m_rxData.addData(data, 2U);

							m_rxData.addData(readBuffer, DV_FRAME_LENGTH_BYTES);
							readLength = 0U;
						}
					}
				}
			}
		} else {
			if (hdrTimer.isRunning() && hdrTimer.hasExpired()) {
				unsigned char buffer[90U];
				bool ret = m_modem->readHeader(buffer, 90U);
				if (ret) {
					// CUtils::dump(wxT("Read Header"), buffer, ret);

					unsigned char data[2U];
					data[0U] = TAG_HEADER;
					data[1U] = RADIO_HEADER_LENGTH_BYTES - 2U;
					m_rxData.addData(data, 2U);

					m_rxData.addData(buffer, RADIO_HEADER_LENGTH_BYTES - 2U);

					dataTimer.start();
					hdrTimer.stop();
					readLength = 0U;
					rx = true;
				} else {
					hdrTimer.reset();
				}
			}
		}

		if (writeLength == 0U && m_txData.hasData()) {
			m_mutex.Lock();

			m_txData.getData(&writeType, 1U);
			m_txData.getData(&writeLength, 1U);
			m_txData.getData(writeBuffer, writeLength);

			m_mutex.Unlock();
		}

		if (writeLength > 0U) {
			if (writeType == TAG_HEADER) {
				// CUtils::dump(wxT("Write Header"), writeBuffer, writeLength);
				m_modem->writeHeader(writeBuffer, writeLength);
				m_modem->setPTT(true);
				writeLength = 0U;
			} else {
				if (m_space > 0U) {
					// CUtils::dump(wxT("Write Data"), writeBuffer, writeLength);
					int ret = m_modem->writeData(writeBuffer, writeLength);
					if (ret > 0) {
						writeLength -= ret;
						m_space--;

						if (writeType == TAG_DATA_END)
							m_modem->setPTT(false);
					}
				}
			}
		}

		Sleep(5UL);

		dataTimer.clock();
		hdrTimer.clock();

		// Check the state of the PTT every 100ms or so
		txTimer.clock();
		if (txTimer.hasExpired()) {
			TRISTATE ret = m_modem->getPTT();
			m_tx = ret == STATE_TRUE;
			txTimer.reset();
		}

		// Check the transmit space every 100ms or so, or when there is no space
		spaceTimer.clock();
		if (spaceTimer.hasExpired() || m_space == 0U) {
			int ret = m_modem->getSpace();
			m_space = ret > 0 ? ret : 0U;
			spaceTimer.reset();
		}
	}

	wxLogMessage(wxT("Stopping GMSK Modem Controller thread"));

	m_modem->close();

	delete[] writeBuffer;
	delete[] readBuffer;

	return NULL;
}

DSMT_TYPE CDStarRepeaterGMSKController::read()
{
	m_readLength = 0U;

	if (m_rxData.isEmpty())
		return DSMTT_NONE;

	wxMutexLocker locker(m_mutex);

	unsigned char hdr[2U];
	m_rxData.getData(hdr, 2U);

	m_readType   = hdr[0U];
	m_readLength = hdr[1U];
	m_rxData.getData(m_readBuffer, m_readLength);

	switch (m_readType) {
		case TAG_HEADER:
			return DSMTT_HEADER;

		case TAG_DATA:
		case TAG_DATA_END:
			return DSMTT_DATA;

		default:
			return DSMTT_NONE;
	}
}

CHeaderData* CDStarRepeaterGMSKController::readHeader()
{
	if (m_readType != TAG_HEADER || m_readLength == 0U)
		return NULL;

	return new CHeaderData(m_readBuffer, RADIO_HEADER_LENGTH_BYTES, false);
}

unsigned int CDStarRepeaterGMSKController::readData(unsigned char* data, unsigned int length, bool& end)
{
	end = false;

	if (m_readType != TAG_DATA && m_readType != TAG_DATA_END)
		return 0U;

	end = m_readType == TAG_DATA_END;

	if (length < m_readLength) {
		::memcpy(data, m_readBuffer, length);
		return length;
	} else {
		::memcpy(data, m_readBuffer, m_readLength);
		return m_readLength;
	}
}

bool CDStarRepeaterGMSKController::writeHeader(const CHeaderData& header)
{
	unsigned char buffer[50U];

	::memset(buffer, ' ', RADIO_HEADER_LENGTH_BYTES - 2U);

	buffer[0U] = header.getFlag1();
	buffer[1U] = header.getFlag2();
	buffer[2U] = header.getFlag3();

	wxString rpt2 = header.getRptCall2();
	for (unsigned int i = 0U; i < rpt2.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer[i + 3U]  = rpt2.GetChar(i);

	wxString rpt1 = header.getRptCall1();
	for (unsigned int i = 0U; i < rpt1.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer[i + 11U] = rpt1.GetChar(i);

	wxString your = header.getYourCall();
	for (unsigned int i = 0U; i < your.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer[i + 19U] = your.GetChar(i);

	wxString my1 = header.getMyCall1();
	for (unsigned int i = 0U; i < my1.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer[i + 27U] = my1.GetChar(i);

	wxString my2 = header.getMyCall2();
	for (unsigned int i = 0U; i < my2.Len() && i < SHORT_CALLSIGN_LENGTH; i++)
		buffer[i + 35U] = my2.GetChar(i);

	wxMutexLocker locker(m_mutex);

	bool ret = m_txData.hasSpace(RADIO_HEADER_LENGTH_BYTES);
	if (!ret)
		return false;

	unsigned char data[2U];
	data[0U] = TAG_HEADER;
	data[1U] = RADIO_HEADER_LENGTH_BYTES - 2U;
	m_txData.addData(data, 2U);

	m_txData.addData(buffer, RADIO_HEADER_LENGTH_BYTES - 2U);

	return true;
}

bool CDStarRepeaterGMSKController::writeData(const unsigned char* data, unsigned int length, bool end)
{
	wxMutexLocker locker(m_mutex);

	bool ret = m_txData.hasSpace(DV_FRAME_LENGTH_BYTES + 2U);
	if (!ret)
		return false;

	unsigned char buffer[2U];
	buffer[0U] = end ? TAG_DATA_END : TAG_DATA;
	buffer[1U] = DV_FRAME_LENGTH_BYTES;
	m_txData.addData(buffer, 2U);

	m_txData.addData(data, DV_FRAME_LENGTH_BYTES);

	return true;
}

unsigned int CDStarRepeaterGMSKController::getSpace()
{
	return m_space;
}

bool CDStarRepeaterGMSKController::getTX()
{
	return m_tx;
}

void CDStarRepeaterGMSKController::stop()
{
	m_stopped = true;

	Wait();
}

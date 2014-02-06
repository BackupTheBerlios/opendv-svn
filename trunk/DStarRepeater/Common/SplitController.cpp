/*
 *   Copyright (C) 2011-2014 by Jonathan Naylor G4KLX
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

#include "SplitController.h"
#include "Timer.h"

const unsigned char DVRPTR_HEADER_LENGTH = 5U;

const unsigned int BUFFER_LENGTH = 200U;

CSplitController::CSplitController(const wxString& localAddress, unsigned int localPort, const wxString& transmitter1Address, unsigned int transmitter1Port, const wxString& transmitter2Address, unsigned int transmitter2Port, const wxString& transmitter3Address, unsigned int transmitter3Port, const wxString& receiver1Address, unsigned int receiver1Port, const wxString& receiver2Address, unsigned int receiver2Port, const wxString& receiver3Address, unsigned int receiver3Port, unsigned int timeout) :
CModem(),
m_handler(localAddress, localPort),
m_timeout(timeout),
m_tx1Address(),
m_tx1Port(transmitter1Port),
m_tx2Address(),
m_tx2Port(transmitter2Port),
m_tx3Address(),
m_tx3Port(transmitter3Port),
m_rx1Address(),
m_rx1Port(receiver1Port),
m_rx2Address(),
m_rx2Port(receiver2Port),
m_rx3Address(),
m_rx3Port(receiver3Port),
m_txData(1000U),
m_outId(0x00U),
m_outSeq(0U)
{
	if (transmitter1Port > 0U)
		m_tx1Address = CUDPReaderWriter::lookup(transmitter1Address);
	if (transmitter2Port > 0U)
		m_tx2Address = CUDPReaderWriter::lookup(transmitter2Address);
	if (transmitter3Port > 0U)
		m_tx3Address = CUDPReaderWriter::lookup(transmitter3Address);

	if (receiver1Port > 0U)
		m_rx1Address = CUDPReaderWriter::lookup(receiver1Address);
	if (receiver2Port > 0U)
		m_rx2Address = CUDPReaderWriter::lookup(receiver2Address);
	if (receiver3Port > 0U)
		m_rx3Address = CUDPReaderWriter::lookup(receiver3Address);

	m_buffer = new unsigned char[BUFFER_LENGTH];
}

CSplitController::~CSplitController()
{
	delete[] m_buffer;
}

bool CSplitController::start()
{
	if (m_tx1Port > 0U && m_tx1Address.s_addr == INADDR_NONE) {
		wxLogError(wxT("The address for Transmitter 1 is invalid"));
		return false;
	}

	if (m_tx2Port > 0U && m_tx2Address.s_addr == INADDR_NONE) {
		wxLogError(wxT("The address for Transmitter 2 is invalid"));
		return false;
	}

	if (m_tx3Port > 0U && m_tx3Address.s_addr == INADDR_NONE) {
		wxLogError(wxT("The address for Transmitter 3 is invalid"));
		return false;
	}

	if (m_rx1Port > 0U && m_rx1Address.s_addr == INADDR_NONE) {
		wxLogError(wxT("The address for Receiver 1 is invalid"));
		return false;
	}

	if (m_rx2Port > 0U && m_rx2Address.s_addr == INADDR_NONE) {
		wxLogError(wxT("The address for Receiver 2 is invalid"));
		return false;
	}

	if (m_rx3Port > 0U && m_rx3Address.s_addr == INADDR_NONE) {
		wxLogError(wxT("The address for Receiver 3 is invalid"));
		return false;
	}

	bool ret = m_handler.open();
	if (!ret)
		return false;

	Create();
	SetPriority(100U);
	Run();

	return true;
}

void* CSplitController::Entry()
{
	wxLogMessage(wxT("Starting Split Controller thread"));

	while (!m_stopped) {
		transmit();

		Sleep(10UL);
	}

	wxLogMessage(wxT("Stopping Split Controller thread"));

	m_handler.close();

	return NULL;
}

bool CSplitController::writeHeader(const CHeaderData& header)
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
	data[0U] = DSMTT_HEADER;
	data[1U] = RADIO_HEADER_LENGTH_BYTES - 2U;
	m_txData.addData(data, 2U);

	m_txData.addData(buffer, RADIO_HEADER_LENGTH_BYTES - 2U);

	return true;
}

bool CSplitController::writeData(const unsigned char* data, unsigned int length, bool end)
{
	wxMutexLocker locker(m_mutex);

	bool ret = m_txData.hasSpace(DV_FRAME_LENGTH_BYTES + 2U);
	if (!ret)
		return false;

	unsigned char buffer[2U];
	buffer[0U] = end ? DSMTT_EOT : DSMTT_DATA;
	buffer[1U] = DV_FRAME_LENGTH_BYTES;
	m_txData.addData(buffer, 2U);

	m_txData.addData(data, DV_FRAME_LENGTH_BYTES);

	return true;
}

unsigned int CSplitController::getSpace()
{
	return m_txData.freeSpace() / (DV_FRAME_LENGTH_BYTES + 2U);
}

void CSplitController::transmit()
{
	if (!m_txData.hasData())
		return;

	unsigned char type;
	unsigned char length;

	m_mutex.Lock();

	m_txData.getData(&type, 1U);

	m_txData.getData(&length, 1U);
	m_txData.getData(m_buffer, length);

	m_mutex.Unlock();

	if (type == DSMTT_HEADER) {
		m_outId  = (::rand() % 65535U) + 1U;
		m_outSeq = 0U;

		if (m_tx1Port > 0U)
			m_handler.writeHeader(m_buffer, m_outId, m_tx1Address, m_tx1Port);
		if (m_tx2Port > 0U)
			m_handler.writeHeader(m_buffer, m_outId, m_tx2Address, m_tx2Port);
		if (m_tx3Port > 0U)
			m_handler.writeHeader(m_buffer, m_outId, m_tx3Address, m_tx3Port);
	} else {
		// If this is a data sync, reset the sequence to zero
		if (m_buffer[9] == 0x55U && m_buffer[10] == 0x2DU && m_buffer[11] == 0x16U)
			m_outSeq = 0U;

		if (type == DSMTT_EOT)
			m_outSeq |= 0x40U;

		if (m_tx1Port > 0U)
			m_handler.writeData(m_buffer, length, m_outId, m_outSeq, m_tx1Address, m_tx1Port);
		if (m_tx2Port > 0U)
			m_handler.writeData(m_buffer, length, m_outId, m_outSeq, m_tx2Address, m_tx2Port);
		if (m_tx3Port > 0U)
			m_handler.writeData(m_buffer, length, m_outId, m_outSeq, m_tx3Address, m_tx3Port);

		m_outSeq++;
		if (m_outSeq > 0x14U)
			m_outSeq = 0U;
	}
}

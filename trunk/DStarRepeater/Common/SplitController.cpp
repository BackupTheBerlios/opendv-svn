/*
 *   Copyright (C) 2012-2014 by Jonathan Naylor G4KLX
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

const unsigned char DVRPTR_HEADER_LENGTH = 5U;

const unsigned int BUFFER_LENGTH = 200U;

CAMBESlot::CAMBESlot(unsigned int timeout) :
m_valid(NULL),
m_errors(999U),
m_best(99U),
m_ambe(NULL),
m_length(0U),
m_end(NULL),
m_timer(100U, 0U, timeout)
{
	m_ambe  = new unsigned char[DV_FRAME_MAX_LENGTH_BYTES];
	m_valid = new bool[3U];
	m_end   = new bool[3U];

	reset();
}

CAMBESlot::~CAMBESlot() 
{
	delete[] m_end;
	delete[] m_valid;
	delete[] m_ambe;
}

void CAMBESlot::reset()
{
	m_errors = 999U;
	m_best   = 99U;
	m_length = 0U;

	for (unsigned int i = 0U; i < 3U; i++) {
		m_valid[i] = false;
		m_end[i]   = false;
	}

	m_timer.stop();
}

bool CAMBESlot::isFirst() const
{
	return m_length == 0U;
}

CSplitController::CSplitController(const wxString& localAddress, unsigned int localPort, const wxString& transmitter1Address, unsigned int transmitter1Port, const wxString& transmitter2Address, unsigned int transmitter2Port, const wxString& transmitter3Address, unsigned int transmitter3Port, const wxString& receiver1Address, unsigned int receiver1Port, const wxString& receiver2Address, unsigned int receiver2Port, const wxString& receiver3Address, unsigned int receiver3Port, unsigned int timeout) :
CModem(),
m_handler(localAddress, localPort),
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
m_outSeq(0U),
m_endTimer(100U, 1U),
m_listening(true),
m_seqNo(0x00U),
m_header(NULL),
m_id(NULL),
m_valid(NULL),
m_slots(NULL)
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

	m_header = new unsigned char[RADIO_HEADER_LENGTH_BYTES];

	m_id    = new wxUint16[3U];
	m_valid = new bool[3U];

	for (unsigned int i = 0U; i < 3U; i++) {
		m_valid[i] = false;
		m_id[i] = 0x00U;
	}

	m_slots = new CAMBESlot*[21U];
	for (unsigned int i = 0U; i < 21U; i++)
		m_slots[i] = new CAMBESlot(timeout);
}

CSplitController::~CSplitController()
{
	delete[] m_header;
	delete[] m_id;
	delete[] m_valid;

	for (unsigned int i = 0U; i < 21U; i++)
		delete m_slots[i];
	delete[] m_slots;
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

		receive();

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
	unsigned char buffer[RADIO_HEADER_LENGTH_BYTES];

	m_mutex.Lock();

	m_txData.getData(&type, 1U);

	m_txData.getData(&length, 1U);
	m_txData.getData(buffer, length);

	m_mutex.Unlock();

	if (type == DSMTT_HEADER) {
		m_outId  = (::rand() % 65535U) + 1U;
		m_outSeq = 0U;

		if (m_tx1Port > 0U)
			m_handler.writeHeader(buffer, m_outId, m_tx1Address, m_tx1Port);
		if (m_tx2Port > 0U)
			m_handler.writeHeader(buffer, m_outId, m_tx2Address, m_tx2Port);
		if (m_tx3Port > 0U)
			m_handler.writeHeader(buffer, m_outId, m_tx3Address, m_tx3Port);
	} else {
		// If this is a data sync, reset the sequence to zero
		if (buffer[9] == 0x55U && buffer[10] == 0x2DU && buffer[11] == 0x16U)
			m_outSeq = 0U;

		if (type == DSMTT_EOT)
			m_outSeq |= 0x40U;

		if (m_tx1Port > 0U)
			m_handler.writeData(buffer, length, m_outId, m_outSeq, m_tx1Address, m_tx1Port);
		if (m_tx2Port > 0U)
			m_handler.writeData(buffer, length, m_outId, m_outSeq, m_tx2Address, m_tx2Port);
		if (m_tx3Port > 0U)
			m_handler.writeData(buffer, length, m_outId, m_outSeq, m_tx3Address, m_tx3Port);

		m_outSeq++;
		if (m_outSeq > 0x14U)
			m_outSeq = 0U;
	}
}

void CSplitController::receive()
{
	NETWORK_TYPE type = NETWORK_NONE;

	while (type != NETWORK_NONE) {
		wxUint16 id;
		in_addr address;
		unsigned int port;

		type = m_handler.read(id, address, port);

		if (type == NETWORK_HEADER) {
			unsigned char header[RADIO_HEADER_LENGTH_BYTES];
			unsigned int length = m_handler.readHeader(header, RADIO_HEADER_LENGTH_BYTES);

			if (address.s_addr == m_rx1Address.s_addr && port == m_rx1Port) {
				processHeader(0U, id, header, length);
			} else if (address.s_addr == m_rx2Address.s_addr && port == m_rx2Port) {
				processHeader(1U, id, header, length);
			} else if (address.s_addr == m_rx3Address.s_addr && port == m_rx3Port) {
				processHeader(2U, id, header, length);
			} else {
				wxString addr(::inet_ntoa(address), wxConvLocal);
				wxLogError(wxT("Header received from unknown repeater - %s:%u"), addr.c_str(), port);
			}
		} else if (type == NETWORK_DATA) {
			unsigned char ambe[DV_FRAME_MAX_LENGTH_BYTES];
			wxUint8 seqNo;
			unsigned int errors;
			unsigned int length = m_handler.readData(ambe, DV_FRAME_MAX_LENGTH_BYTES, seqNo, errors);

			if (address.s_addr == m_rx1Address.s_addr && port == m_rx1Port) {
				processAMBE(0U, id, ambe, length, seqNo, errors);
			} else if (address.s_addr == m_rx2Address.s_addr && port == m_rx2Port) {
				processAMBE(1U, id, ambe, length, seqNo, errors);
			} else if (address.s_addr == m_rx3Address.s_addr && port == m_rx3Port) {
				processAMBE(2U, id, ambe, length, seqNo, errors);
			} else {
				wxString addr(::inet_ntoa(address), wxConvLocal);
				wxLogError(wxT("Data received from unknown repeater - %s:%u"), addr.c_str(), port);
			}
		} else {
			wxString addr(::inet_ntoa(address), wxConvLocal);
			wxLogError(wxT("Received invalid frame type %d from %s:%u"), int(type), addr.c_str(), port);
		}
	}

	m_endTimer.clock();
	if (m_endTimer.isRunning() && m_endTimer.hasExpired()) {
		wxMutexLocker locker(m_mutex);
		unsigned char data[2U];
		data[0U] = DSMTT_EOT;
		data[1U] = 0U;
		m_rxData.addData(data, 2U);

		m_listening = true;
		m_endTimer.stop();

		return;
	}

	// Run the slot timers
	for (unsigned int i = 0U; i < 21U; i++)
		m_slots[i]->m_timer.clock();

	// Check for expired timers
	unsigned int slotNo = m_seqNo;
	for (unsigned int i = 0U; i < 21U; i++) {
		CAMBESlot* slot = m_slots[slotNo];

		if (slot->m_timer.isRunning() && slot->m_timer.hasExpired()) {
			// Is there any data?
			if (slot->m_length > 0U) {
				m_mutex.Lock();
				unsigned char data[2U];
				data[0U] = DSMTT_DATA;
				data[1U] = DV_FRAME_LENGTH_BYTES;
				m_rxData.addData(data, 2U);
				m_rxData.addData(slot->m_ambe, DV_FRAME_LENGTH_BYTES);
				m_mutex.Unlock();

				// The last frame?
				if (isEnd(slot)) {
					wxMutexLocker locker(m_mutex);
					unsigned char data[2U];
					data[0U] = DSMTT_EOT;
					data[1U] = 0U;
					m_rxData.addData(data, 2U);

					m_listening = true;
					m_endTimer.stop();

					return;
				} else {
					slot->reset();
				}				
			} else {
				// Send a silence frame to the repeater
				wxMutexLocker locker(m_mutex);
				unsigned char data[2U];
				data[0U] = DSMTT_DATA;
				data[1U] = DV_FRAME_LENGTH_BYTES;
				m_rxData.addData(data, 2U);
				m_rxData.addData(NULL_FRAME_DATA_BYTES, DV_FRAME_LENGTH_BYTES);

				slot->reset();
			}
		} else {
			break;
		}

		slotNo++;
		if (slotNo >= 21U)
			slotNo = 0U;
	}
}

void CSplitController::processHeader(unsigned int n, wxUint16 id, const unsigned char* header, unsigned int length)
{
	if (m_listening) {
		// Send the header to the repeater
		m_mutex.Lock();
		unsigned char data[2U];
		data[0U] = DSMTT_HEADER;
		data[1U] = RADIO_HEADER_LENGTH_BYTES - 2U;
		m_rxData.addData(data, 2U);
		m_rxData.addData(header, RADIO_HEADER_LENGTH_BYTES - 2U);
		m_mutex.Unlock();

		::memcpy(m_header, header, RADIO_HEADER_LENGTH_BYTES - 2U);

		for (unsigned int i = 0U; i < 21U; i++)
			m_slots[i]->reset();

		for (unsigned int i = 0U; i < 3U; i++) {
			m_valid[i] = false;
			m_id[i] = 0x00U;
		}
		m_valid[n] = true;
		m_id[n] = id;

		m_seqNo = 0U;
		m_listening = false;
		m_endTimer.start();
	} else {
		m_valid[n] = ::memcmp(m_header, header, RADIO_HEADER_LENGTH_BYTES - 2U) == 0;
		m_id[n] = id;		// Makes no difference as the valid flag will ensure we ignore bad ids
	}
}

void CSplitController::processAMBE(unsigned int n, wxUint16 id, const unsigned char* ambe, unsigned int length, wxUint8 seqNo, unsigned char errors)
{
	if (m_listening)
		return;

	if (!m_valid[n] || id != m_id[n])
		return;

	CAMBESlot* slot = m_slots[seqNo & 0x3FU];

	m_endTimer.reset();

	slot->m_end[n]   = (seqNo & 0x40U) == 0x40U;
	slot->m_valid[n] = true;

	// If the existing data is of a better quality....
	if (slot->m_errors < errors)
		return;

	::memcpy(slot->m_ambe, ambe, length);
	slot->m_length = length;

	slot->m_errors = errors;
	slot->m_best   = n;
}

bool CSplitController::isEnd(CAMBESlot* slot) const
{
	bool hasNoEnd = false;
	bool hasEnd   = false;

	for (unsigned int i = 0U; i < 3U; i++) {
		if (slot->m_valid[i] && slot->m_end[i])
			hasEnd = true;

		if (slot->m_valid[i] && !slot->m_end[i])
			hasNoEnd = true;
	}

	return hasEnd && !hasNoEnd;	
}

/*
 *   Copyright (C) 2010,2011,2012 by Jonathan Naylor G4KLX
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

#include "GMSKRepeaterRXThread.h"
#include "GMSKRepeaterApp.h"
#include "DStarDefines.h"
#include "Version.h"

const unsigned int NORMAL_CYCLE_TIME = 15U;
const unsigned int BROKEN_CYCLE_TIME = 7U;

const unsigned int NETWORK_QUEUE_COUNT = 2U;

CGMSKRepeaterRXThread::CGMSKRepeaterRXThread() :
wxThread(wxTHREAD_JOINABLE),
m_modem(NULL),
m_protocolHandler(NULL),
m_ambeBuffer(NULL),
m_ambeLength(0U),
m_rxHeader(NULL),
m_radioSeqNo(0U),
m_pollTimer(1000U, 60U),			// 60s
m_headerReadTimer(1000U, 0U, 50U),	// 50ms
m_state(DSRS_LISTENING),
m_killed(false),
m_broken(false),
m_ambe(),
m_ambeFrames(0U),
m_ambeSilence(0U),
m_ambeBits(1U),
m_ambeErrors(0U),
m_lastAMBEBits(0U),
m_lastAMBEErrors(0U),
m_cycleTime(NORMAL_CYCLE_TIME)
{
	m_ambeBuffer = new unsigned char[DV_FRAME_LENGTH_BYTES];
}

CGMSKRepeaterRXThread::~CGMSKRepeaterRXThread()
{
	delete[] m_ambeBuffer;
	delete   m_rxHeader;
}

void CGMSKRepeaterRXThread::start()
{
	Create();

	SetPriority(100U);

	Run();
}

void* CGMSKRepeaterRXThread::Entry()
{
	// Wait here until we have the essentials to run
	while (!m_killed && (m_modem == NULL || m_protocolHandler == NULL))
		Sleep(500UL);		// 1/2 sec

	if (m_killed)
		return NULL;

	m_broken = m_modem->isBroken();

	m_cycleTime = m_broken ? BROKEN_CYCLE_TIME : NORMAL_CYCLE_TIME;

	m_headerReadTimer.start();
	m_pollTimer.start();

	wxLogMessage(wxT("Starting the GMSK receiver thread"));

	wxStopWatch stopWatch;

	while (!m_killed) {
		stopWatch.Start();

		if (m_state == DSRS_LISTENING) {
			// Only check for the RF header every 100ms or so
			if (m_headerReadTimer.hasExpired()) {
				bool error = receiveRadioHeader();
				if (error)
					reopenModem();

				m_headerReadTimer.reset();
			}
		} else {
			bool error = receiveRadioData();
			if (error)
				reopenModem();
		}

		if (m_killed)
			break;

		// Listen all the time on the network for status packets at least
		receiveNetwork();

		// Send the network poll if needed and restart the timer
		if (m_pollTimer.hasExpired()) {
#if defined(__WINDOWS__)
			m_protocolHandler->writePoll(wxT("win_gmsk-") + VERSION);
#else
			m_protocolHandler->writePoll(wxT("linux_gmsk-") + VERSION);
#endif
			m_pollTimer.reset();
		}

		unsigned long ms = stopWatch.Time();

		// Don't sleep when reading from the modem
		if (m_state != DSRS_VALID) {
			if (ms < m_cycleTime)
				Sleep(m_cycleTime - ms);

			ms = stopWatch.Time();
		}

		// Catch up with the clock
		clock(ms);
	}

	wxLogMessage(wxT("Stopping the GMSK receiver thread"));

	if (m_modem != NULL) {
		m_modem->close();
		delete m_modem;
	}

	m_protocolHandler->close();
	delete m_protocolHandler;

	return NULL;
}

void CGMSKRepeaterRXThread::kill()
{
	m_killed = true;
}

void CGMSKRepeaterRXThread::wait()
{
	Wait();
}

void CGMSKRepeaterRXThread::setCallsign(const wxString& callsign, const wxString& gateway, DSTAR_MODE mode, ACK_TYPE ack, bool restriction, bool rpt1Validation)
{
}

void CGMSKRepeaterRXThread::setProtocolHandler(CRepeaterProtocolHandler* handler)
{
	wxASSERT(handler != NULL);

	m_protocolHandler = handler;
}

void CGMSKRepeaterRXThread::setModem(IGMSKModem* modem)
{
	wxASSERT(modem != NULL);

	m_modem = modem;
}

void CGMSKRepeaterRXThread::setTimes(unsigned int timeout, unsigned int ackTime)
{
}

void CGMSKRepeaterRXThread::setBeacon(unsigned int time, const wxString& text, bool voice, TEXT_LANG language)
{
}

void CGMSKRepeaterRXThread::setController(CExternalController* controller, unsigned int activeHangTime)
{
}

void CGMSKRepeaterRXThread::setControl(bool enabled, const wxString& rpt1Callsign, const wxString& rpt2Callsign, const wxString& shutdown, const wxString& startup, const wxString& status1, const wxString& status2, const wxString& status3, const wxString& status4, const wxString& status5, const wxString& command1, const wxString& command1Line, const wxString& command2, const wxString& command2Line, const wxString& command3, const wxString& command3Line, const wxString& command4, const wxString& command4Line, const wxString& output1, const wxString& output2, const wxString& output3, const wxString& output4)
{
}

void CGMSKRepeaterRXThread::setOutputs(bool out1, bool out2, bool out3, bool out4)
{
}

void CGMSKRepeaterRXThread::setLogging(bool logging, const wxString& dir)
{
}

void CGMSKRepeaterRXThread::setWhiteList(CCallsignList* list)
{
}

void CGMSKRepeaterRXThread::setBlackList(CCallsignList* list)
{
}

bool CGMSKRepeaterRXThread::receiveRadioHeader()
{
	bool error;
	CHeaderData* header = m_modem->readHeader(error);
	if (header == NULL)
		return error;

	wxLogMessage(wxT("Header decoded - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s  Flags: %02X %02X %02X"), header->getMyCall1().c_str(), header->getMyCall2().c_str(), header->getYourCall().c_str(), header->getRptCall1().c_str(), header->getRptCall2().c_str(), header->getFlag1(), header->getFlag2(), header->getFlag3());

	bool res = processRadioHeader(header);
	if (res) {
		// A valid header and is a DV packet
		m_radioSeqNo     = 20U;
		m_ambeFrames     = 0U;
		m_ambeSilence    = 0U;
		m_ambeBits       = 1U;
		m_ambeErrors     = 0U;
		m_ambeLength     = 0U;
		m_lastAMBEBits   = 0U;
		m_lastAMBEErrors = 0U;
	} else {
		// This is a DD packet or some other problem
		// wxLogMessage(wxT("Invalid header"));
	}

	return false;
}

bool CGMSKRepeaterRXThread::receiveRadioData()
{
	int length;

	do {
		unsigned char data[GMSK_MODEM_DATA_LENGTH];
		bool end;

		length = m_modem->readData(data, GMSK_MODEM_DATA_LENGTH, end);
		if (length == -1)
			return true;

		if (end) {
			// wxLogMessage(wxT("Found end sync"));
			processRadioFrame(m_ambeBuffer, FRAME_END);
			endOfRadioData();
			return false;
		}

		for (int i = 0; i < length; i++) {
			m_ambeBuffer[m_ambeLength] = data[i];
			m_ambeLength++;

			if (m_ambeLength >= DV_FRAME_LENGTH_BYTES) {
				m_ambeLength = 0U;

				bool res = ::memcmp(m_ambeBuffer + VOICE_FRAME_LENGTH_BYTES, DATA_SYNC_BYTES, DATA_FRAME_LENGTH_BYTES) == 0;
				if (res) {
					// wxLogMessage(wxT("Found data sync at frame %u"), m_radioSyncCounter);
					m_radioSeqNo = 0U;
					processRadioFrame(m_ambeBuffer, FRAME_SYNC);
				} else if (m_radioSeqNo == 20U) {
					// wxLogMessage(wxT("Regenerating data sync"));
					m_radioSeqNo = 0U;
					processRadioFrame(m_ambeBuffer, FRAME_SYNC);
				} else {
					m_radioSeqNo++;
					processRadioFrame(m_ambeBuffer, FRAME_NORMAL);
				}
			}
		}
	} while (length == int(GMSK_MODEM_DATA_LENGTH));

	return false;
}

void CGMSKRepeaterRXThread::receiveNetwork()
{
	NETWORK_TYPE type;

	do {
		type = m_protocolHandler->read();
	} while (type != NETWORK_NONE);
}

bool CGMSKRepeaterRXThread::processRadioHeader(CHeaderData* header)
{
	wxASSERT(header != NULL);

	// We don't handle DD data packets
	if (header->isDataPacket()) {
		wxLogMessage(wxT("Received a DD packet, ignoring"));
		delete header;
		return false;
	}

	m_state = DSRS_VALID;

	delete m_rxHeader;
	m_rxHeader = header;

	CHeaderData netHeader(*m_rxHeader);
	netHeader.setRptCall1(m_rxHeader->getRptCall2());
	netHeader.setRptCall2(m_rxHeader->getRptCall1());

	m_protocolHandler->writeHeader(netHeader);

	return true;
}

void CGMSKRepeaterRXThread::processRadioFrame(unsigned char* data, FRAME_TYPE type)
{
	m_ambeFrames++;

	// If a sync frame, regenerate the sync bytes
	if (type == FRAME_SYNC)
		::memcpy(data + VOICE_FRAME_LENGTH_BYTES, DATA_SYNC_BYTES, DATA_FRAME_LENGTH_BYTES);

	// Only regenerate the AMBE on received radio data
	unsigned int errors = 0U;
	if (type != FRAME_END) {
		errors = m_ambe.regenerate(data);
		m_ambeErrors += errors;
		m_ambeBits   += 48U;		// Only count the bits with FEC added
	}

	if (::memcmp(data, NULL_AMBE_DATA_BYTES, VOICE_FRAME_LENGTH_BYTES) == 0)
		m_ambeSilence++;

	if (type == FRAME_END) {
		// Send null data and the end marker over the network, and the statistics
		unsigned char bytes[DV_FRAME_MAX_LENGTH_BYTES];
		::memcpy(bytes, NULL_AMBE_DATA_BYTES, VOICE_FRAME_LENGTH_BYTES);
		::memcpy(bytes + VOICE_FRAME_LENGTH_BYTES, END_PATTERN_BYTES, END_PATTERN_LENGTH_BYTES);
		m_protocolHandler->writeData(bytes, DV_FRAME_MAX_LENGTH_BYTES, 0U, true);
	} else {
		// Send the data to the network
		m_protocolHandler->writeData(data, DV_FRAME_LENGTH_BYTES, errors, false);
	}
}

void CGMSKRepeaterRXThread::endOfRadioData()
{
	wxLogMessage(wxT("AMBE for %s  Frames: %.1fs, Silence: %.1f%%, BER: %.1f%%"), m_rxHeader->getMyCall1().c_str(), float(m_ambeFrames) / 50.0F, float(m_ambeSilence * 100U) / float(m_ambeFrames), float(m_ambeErrors * 100U) / float(m_ambeBits));

	m_state = DSRS_LISTENING;

	m_protocolHandler->reset();
}

CGMSKRepeaterStatusData* CGMSKRepeaterRXThread::getStatus()
{
	float   bits = float(m_ambeBits - m_lastAMBEBits);
	float errors = float(m_ambeErrors - m_lastAMBEErrors);
	if (bits == 0.0F)
		bits = 1.0F;

	m_lastAMBEBits   = m_ambeBits;
	m_lastAMBEErrors = m_ambeErrors;

	if (m_state == DSRS_LISTENING)
		return new CGMSKRepeaterStatusData(wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString,
				wxEmptyString, 0x00, 0x00, 0x00, false, m_state, 0U, 0U, 0U, 0U, 0.0F, wxEmptyString,
				wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString);
	else
		return new CGMSKRepeaterStatusData(m_rxHeader->getMyCall1(), m_rxHeader->getMyCall2(),
				m_rxHeader->getYourCall(), m_rxHeader->getRptCall1(), m_rxHeader->getRptCall2(), 
				m_rxHeader->getFlag1(), m_rxHeader->getFlag2(), m_rxHeader->getFlag3(), false, m_state,
				0U, 0U, 0U, 0U, (errors * 100.0F) / bits, wxEmptyString, wxEmptyString, wxEmptyString,
				wxEmptyString, wxEmptyString, wxEmptyString);
}

void CGMSKRepeaterRXThread::clock(unsigned int ms)
{
	m_pollTimer.clock(ms);
	m_headerReadTimer.clock(ms);
}

void CGMSKRepeaterRXThread::shutdown()
{
}

void CGMSKRepeaterRXThread::startup()
{
}

void CGMSKRepeaterRXThread::command1()
{
}

void CGMSKRepeaterRXThread::command2()
{
}

void CGMSKRepeaterRXThread::command3()
{
}

void CGMSKRepeaterRXThread::command4()
{
}

bool CGMSKRepeaterRXThread::reopenModem()
{
	m_modem->close();

	while (!m_killed) {
		bool ret = m_modem->open();
		if (ret) {
			wxLogMessage(wxT("Reopened the GMSK modem"));
			return true;
		}

		// Reset the repeaters state
		m_state = DSRS_LISTENING;

		Sleep(1000UL);
	}

	delete m_modem;
	m_modem = NULL;

	return false;
}

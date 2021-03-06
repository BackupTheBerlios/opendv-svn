/*
 *   Copyright (C) 2010-2013 by Jonathan Naylor G4KLX
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

#include "GMSKRepeaterTRXThread.h"
#include "GMSKRepeaterApp.h"
#include "DStarDefines.h"
#include "Version.h"

const unsigned char DTMF_MASK[] = {0x82U, 0x08U, 0x20U, 0x82U, 0x00U, 0x00U, 0x82U, 0x00U, 0x00U};
const unsigned char DTMF_SIG[]  = {0x82U, 0x08U, 0x20U, 0x82U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U};

const unsigned int NORMAL_CYCLE_TIME = 15U;
const unsigned int BROKEN_CYCLE_TIME = 7U;

const unsigned int NETWORK_QUEUE_COUNT = 2U;

const unsigned int SILENCE_THRESHOLD = 2U;

CGMSKRepeaterTRXThread::CGMSKRepeaterTRXThread() :
m_modem(NULL),
m_protocolHandler(NULL),
m_controller(NULL),
m_stopped(true),
m_ambeBuffer(NULL),
m_ambeLength(0U),
m_rptCallsign(),
m_gwyCallsign(),
m_beacon(NULL),
m_rxHeader(NULL),
m_localQueue((DV_FRAME_LENGTH_BYTES + 2U) * 50U, LOCAL_RUN_FRAME_COUNT),			// 1s worth of data
m_radioQueue((DV_FRAME_LENGTH_BYTES + 2U) * 50U, RADIO_RUN_FRAME_COUNT),			// 1s worth of data
m_networkQueue(NULL),
m_writeNum(0U),
m_readNum(0U),
m_radioSeqNo(0U),
m_networkSeqNo(0U),
m_timeoutTimer(1000U, 180U),		// 180s
m_watchdogTimer(1000U, 2U),			// 2s
m_pollTimer(1000U, 60U),			// 60s
m_ackTimer(1000U, 0U, 500U),		// 0.5s
m_status1Timer(1000U, 3U),			// 3s
m_status2Timer(1000U, 3U),			// 3s
m_status3Timer(1000U, 3U),			// 3s
m_status4Timer(1000U, 3U),			// 3s
m_status5Timer(1000U, 3U),			// 3s
m_beaconTimer(1000U, 600U),			// 10 mins
m_headerReadTimer(1000U, 0U, 50U),	// 50ms
m_dataWaitTimer(1000U, 0U, 100U),	// 100ms
m_state(DSRS_LISTENING),
m_ackEncoder(),
m_linkEncoder(),
m_headerEncoder(),
m_status1Encoder(),
m_status2Encoder(),
m_status3Encoder(),
m_status4Encoder(),
m_status5Encoder(),
m_tx(false),
m_killed(false),
m_mode(MODE_DUPLEX),
m_ack(AT_BER),
m_restriction(false),
m_rpt1Validation(true),
m_controlEnabled(false),
m_controlRPT1(),
m_controlRPT2(),
m_controlShutdown(),
m_controlStartup(),
m_controlStatus1(),
m_controlStatus2(),
m_controlStatus3(),
m_controlStatus4(),
m_controlStatus5(),
m_controlCommand1(),
m_controlCommand1Line(),
m_controlCommand2(),
m_controlCommand2Line(),
m_controlCommand3(),
m_controlCommand3Line(),
m_controlCommand4(),
m_controlCommand4Line(),
m_controlOutput1(),
m_controlOutput2(),
m_controlOutput3(),
m_controlOutput4(),
m_output1(false),
m_output2(false),
m_output3(false),
m_output4(false),
m_activeHangTimer(1000U),
m_shutdown(false),
m_disable(false),
m_logging(NULL),
m_broken(false),
m_lastData(NULL),
m_ambe(),
m_ambeFrames(0U),
m_ambeSilence(0U),
m_ambeBits(1U),
m_ambeErrors(0U),
m_lastAMBEBits(0U),
m_lastAMBEErrors(0U),
m_ackText(),
m_tempAckText(),
m_linkStatus(LS_NONE),
m_reflector(),
m_status1Text(),
m_status2Text(),
m_status3Text(),
m_status4Text(),
m_status5Text(),
m_regEx(wxT("^[A-Z0-9]{1}[A-Z0-9]{0,1}[0-9]{1}[A-Z]{1,4} {0,4}[ A-RT-Z]{1}$")),
m_headerTime(),
m_packetTime(),
m_packetCount(0U),
m_modemBuffer(NULL),
m_modemLength(0U),
m_modemEnd(false),
m_cycleTime(NORMAL_CYCLE_TIME),
m_packetSilence(0U),
m_whiteList(NULL),
m_blackList(NULL),
m_greyList(NULL),
m_blocked(false),
m_busyData(false)
{
	m_networkQueue = new COutputQueue*[NETWORK_QUEUE_COUNT];
	for (unsigned int i = 0U; i < NETWORK_QUEUE_COUNT; i++)
		m_networkQueue[i] = new COutputQueue((DV_FRAME_LENGTH_BYTES + 2U) * 200U, NETWORK_RUN_FRAME_COUNT);		// 4s worth of data);

	m_modemBuffer = new unsigned char[DV_FRAME_LENGTH_BYTES];
	m_ambeBuffer  = new unsigned char[DV_FRAME_LENGTH_BYTES];
	m_lastData    = new unsigned char[DV_FRAME_MAX_LENGTH_BYTES];

	setRepeaterState(DSRS_LISTENING);
}

CGMSKRepeaterTRXThread::~CGMSKRepeaterTRXThread()
{
	for (unsigned int i = 0U; i < NETWORK_QUEUE_COUNT; i++)
		delete m_networkQueue[i];
	delete[] m_networkQueue;
	delete[] m_modemBuffer;
	delete[] m_ambeBuffer;
	delete[] m_lastData;
	delete   m_rxHeader;
}

void CGMSKRepeaterTRXThread::run()
{
	// Wait here until we have the essentials to run
	while (!m_killed && (m_modem == NULL || m_controller == NULL || m_rptCallsign.IsEmpty() || m_rptCallsign.IsSameAs(wxT("        "))))
		::wxMilliSleep(500UL);		// 1/2 sec

	if (m_killed)
		return;

	m_broken = m_modem->isBroken();

	m_stopped = false;

	m_cycleTime = m_broken ? BROKEN_CYCLE_TIME : NORMAL_CYCLE_TIME;

	m_beaconTimer.start();
	m_headerReadTimer.start();
	m_controller->setActive(false);
	m_controller->setRadioTransmit(false);

	if (m_protocolHandler != NULL)
		m_pollTimer.start();

	wxLogMessage(wxT("Starting the GMSK repeater thread"));

	unsigned int count = 0U;

	wxStopWatch stopWatch;

	while (!m_killed) {
		stopWatch.Start();

		switch (m_state) {
			case DSRS_SHUTDOWN:
			case DSRS_LISTENING:
				// Only check for the RF header every 100ms or so
				if (m_headerReadTimer.hasExpired()) {
					bool error = receiveRadioHeader();
					if (error)
						reopenModem();

					m_headerReadTimer.reset();
				}
				break;

			case DSRS_NETWORK:
				if (m_mode == MODE_DUPLEX) {
					if (!m_busyData) {
						// Only check for the RF header every 100ms or so for a busy header
						if (m_headerReadTimer.hasExpired()) {
							bool error = receiveRadioHeader();
							if (error)
								reopenModem();

							m_headerReadTimer.reset();
						}
					} else {
						// Receiving busy data
						bool error = receiveRadioData();
						if (error)
							reopenModem();
					}
				}
				break;

			case DSRS_VALID:
			case DSRS_INVALID:
			case DSRS_TIMEOUT: {
					bool error = receiveRadioData();
					if (error)
						reopenModem();
				}
				break;

			default:			// All the DSRS_*_WAIT states
				break;
		}

		if (m_killed)
			break;

		// Listen all the time on the network for status packets at least
		receiveNetwork();

		repeaterStateMachine();

		// Send the network poll if needed and restart the timer
		if (m_pollTimer.hasExpired()) {
#if defined(__WINDOWS__)
			m_protocolHandler->writePoll(wxT("win_gmsk-") + VERSION);
#else
			m_protocolHandler->writePoll(wxT("linux_gmsk-") + VERSION);
#endif
			m_pollTimer.reset();
		}

		// Send the beacon and restart the timer
		if (m_beaconTimer.isRunning() && m_beaconTimer.hasExpired()) {
			m_beacon->sendBeacon();
			m_beaconTimer.reset();
		}

		// Send the status 1 message after a few seconds
		if (m_status1Timer.isRunning() && m_status1Timer.hasExpired()) {
			m_status1Timer.stop();
			if (m_state == DSRS_LISTENING)
				transmitUserStatus(0U);
		}

		// Send the status 2 message after a few seconds
		if (m_status2Timer.isRunning() && m_status2Timer.hasExpired()) {
			m_status2Timer.stop();
			if (m_state == DSRS_LISTENING)
				transmitUserStatus(1U);
		}

		// Send the status 3 message after a few seconds
		if (m_status3Timer.isRunning() && m_status3Timer.hasExpired()) {
			m_status3Timer.stop();
			if (m_state == DSRS_LISTENING)
				transmitUserStatus(2U);
		}

		// Send the status 4 message after a few seconds
		if (m_status4Timer.isRunning() && m_status4Timer.hasExpired()) {
			m_status4Timer.stop();
			if (m_state == DSRS_LISTENING)
				transmitUserStatus(3U);
		}

		// Send the status 5 message after a few seconds
		if (m_status5Timer.isRunning() && m_status5Timer.hasExpired()) {
			m_status5Timer.stop();
			if (m_state == DSRS_LISTENING)
				transmitUserStatus(4U);
		}

		// Clock the heartbeat output every one second
		count++;
		if (count == 60U) {
			m_controller->setHeartbeat();
			count = 0U;
		}

		// Set the output state
		if (m_state == DSRS_VALID      || m_state == DSRS_INVALID      || m_state == DSRS_TIMEOUT      ||
			m_state == DSRS_VALID_WAIT || m_state == DSRS_INVALID_WAIT || m_state == DSRS_TIMEOUT_WAIT ||
			m_state == DSRS_NETWORK    || (m_activeHangTimer.isRunning() && !m_activeHangTimer.hasExpired())) {
			m_controller->setActive(true);
		} else {
			m_controller->setActive(false);
			m_activeHangTimer.stop();
		}

		// Check the shutdown state, state changes are done here to bypass the state machine which is
		// frozen when m_disable or m_shutdown are asserted
		m_disable = m_controller->getDisable();
		if (m_disable || m_shutdown) {
			if (m_state != DSRS_SHUTDOWN) {
				m_timeoutTimer.stop();
				m_watchdogTimer.stop();
				m_activeHangTimer.stop();
				m_ackTimer.stop();
				m_beaconTimer.stop();
				m_dataWaitTimer.stop();
				m_localQueue.reset();
				m_radioQueue.reset();
				for (unsigned int i = 0U; i < NETWORK_QUEUE_COUNT; i++)
					m_networkQueue[i]->reset();
				m_tx = false;
				m_controller->setActive(false);
				m_controller->setRadioTransmit(false);
				m_state = DSRS_SHUTDOWN;
			}
		} else {
			if (m_state == DSRS_SHUTDOWN) {
				m_timeoutTimer.stop();
				m_watchdogTimer.stop();
				m_ackTimer.stop();
				m_beaconTimer.start();
				m_state = DSRS_LISTENING;
				if (m_protocolHandler != NULL)	// Tell the protocol handler
					m_protocolHandler->reset();
			}
		}

		if (m_radioQueue.dataReady()) {
			bool ret = transmitRadioData();
			if (!ret)
				reopenModem();
		} else if (m_localQueue.dataReady()) {
			bool ret = transmitLocalData();
			if (!ret)
				reopenModem();
		} else if (m_networkQueue[m_readNum]->dataReady()) {
			bool ret = transmitNetworkData();
			if (!ret)
				reopenModem();
		} else if (m_radioQueue.headerReady()) {
			bool ret = transmitRadioHeader();
			if (!ret)
				reopenModem();
		} else if (m_localQueue.headerReady()) {
			bool ret = transmitLocalHeader();
			if (!ret)
				reopenModem();
		} else if (m_networkQueue[m_readNum]->headerReady()) {
			bool ret = transmitNetworkHeader();
			if (!ret)
				reopenModem();
		}

		m_controller->setRadioTransmit(m_tx);

		unsigned long ms = stopWatch.Time();

		// Don't sleep when reading from the modem
		if (m_state != DSRS_VALID && m_state != DSRS_INVALID && m_state != DSRS_TIMEOUT) {
			if (ms < m_cycleTime)
				::wxMilliSleep(m_cycleTime - ms);

			ms = stopWatch.Time();
		}

		// Catch up with the clock
		clock(ms);
	}

	wxLogMessage(wxT("Stopping the GMSK repeater thread"));

	if (m_modem != NULL) {
		m_modem->close();
		delete m_modem;
	}

	m_controller->setActive(false);
	m_controller->setRadioTransmit(false);
	m_controller->close();
	delete m_controller;

	if (m_protocolHandler != NULL) {
		m_protocolHandler->close();
		delete m_protocolHandler;
	}
}

void CGMSKRepeaterTRXThread::kill()
{
	m_killed = true;
}

void CGMSKRepeaterTRXThread::setCallsign(const wxString& callsign, const wxString& gateway, DSTAR_MODE mode, ACK_TYPE ack, bool restriction, bool rpt1Validation)
{
	// Pad the callsign up to eight characters
	m_rptCallsign = callsign;
	m_rptCallsign.resize(LONG_CALLSIGN_LENGTH, wxT(' '));

	if (gateway.IsEmpty()) {
		m_gwyCallsign = callsign;
		m_gwyCallsign.resize(LONG_CALLSIGN_LENGTH - 1U, wxT(' '));
		m_gwyCallsign.Append(wxT("G"));
	} else {
		m_gwyCallsign = gateway;
		m_gwyCallsign.resize(LONG_CALLSIGN_LENGTH, wxT(' '));
	}

	m_mode           = mode;
	m_ack            = ack;
	m_restriction    = restriction;
	m_rpt1Validation = rpt1Validation;
}

void CGMSKRepeaterTRXThread::setProtocolHandler(CRepeaterProtocolHandler* handler)
{
	wxASSERT(handler != NULL);

	m_protocolHandler = handler;
}

void CGMSKRepeaterTRXThread::setModem(IGMSKModem* modem)
{
	wxASSERT(modem != NULL);

	m_modem = modem;
}

void CGMSKRepeaterTRXThread::setTimes(unsigned int timeout, unsigned int ackTime)
{
	m_timeoutTimer.setTimeout(timeout);
	m_ackTimer.setTimeout(0U, ackTime);
}

void CGMSKRepeaterTRXThread::setBeacon(unsigned int time, const wxString& text, bool voice, TEXT_LANG language)
{
	m_beaconTimer.setTimeout(time);

	if (time > 0U)
		m_beacon = new CBeaconUnit(this, m_rptCallsign, text, voice, language);
}

void CGMSKRepeaterTRXThread::setController(CExternalController* controller, unsigned int activeHangTime)
{
	wxASSERT(controller != NULL);

	m_controller = controller;
	m_activeHangTimer.setTimeout(activeHangTime);
}

void CGMSKRepeaterTRXThread::setControl(bool enabled, const wxString& rpt1Callsign, const wxString& rpt2Callsign, const wxString& shutdown, const wxString& startup, const wxString& status1, const wxString& status2, const wxString& status3, const wxString& status4, const wxString& status5, const wxString& command1, const wxString& command1Line, const wxString& command2, const wxString& command2Line, const wxString& command3, const wxString& command3Line, const wxString& command4, const wxString& command4Line, const wxString& output1, const wxString& output2, const wxString& output3, const wxString& output4)
{
	m_controlEnabled      = enabled;
	m_controlRPT1         = rpt1Callsign;
	m_controlRPT2         = rpt2Callsign;
	m_controlShutdown     = shutdown;
	m_controlStartup      = startup;
	m_controlStatus1      = status1;
	m_controlStatus2      = status2;
	m_controlStatus3      = status3;
	m_controlStatus4      = status4;
	m_controlStatus5      = status5;
	m_controlCommand1     = command1;
	m_controlCommand1Line = command1Line;
	m_controlCommand2     = command2;
	m_controlCommand2Line = command2Line;
	m_controlCommand3     = command3;
	m_controlCommand3Line = command3Line;
	m_controlCommand4     = command4;
	m_controlCommand4Line = command4Line;
	m_controlOutput1      = output1;
	m_controlOutput2      = output2;
	m_controlOutput3      = output3;
	m_controlOutput4      = output4;

	m_controlRPT1.Append(wxT(' '), LONG_CALLSIGN_LENGTH);
	m_controlRPT2.Append(wxT(' '), LONG_CALLSIGN_LENGTH);
	m_controlShutdown.Append(wxT(' '), LONG_CALLSIGN_LENGTH);
	m_controlStartup.Append(wxT(' '), LONG_CALLSIGN_LENGTH);
	m_controlStatus1.Append(wxT(' '), LONG_CALLSIGN_LENGTH);
	m_controlStatus2.Append(wxT(' '), LONG_CALLSIGN_LENGTH);
	m_controlStatus3.Append(wxT(' '), LONG_CALLSIGN_LENGTH);
	m_controlStatus4.Append(wxT(' '), LONG_CALLSIGN_LENGTH);
	m_controlStatus5.Append(wxT(' '), LONG_CALLSIGN_LENGTH);
	m_controlCommand1.Append(wxT(' '), LONG_CALLSIGN_LENGTH);
	m_controlCommand2.Append(wxT(' '), LONG_CALLSIGN_LENGTH);
	m_controlCommand3.Append(wxT(' '), LONG_CALLSIGN_LENGTH);
	m_controlCommand4.Append(wxT(' '), LONG_CALLSIGN_LENGTH);
	m_controlOutput1.Append(wxT(' '), LONG_CALLSIGN_LENGTH);
	m_controlOutput2.Append(wxT(' '), LONG_CALLSIGN_LENGTH);
	m_controlOutput3.Append(wxT(' '), LONG_CALLSIGN_LENGTH);
	m_controlOutput4.Append(wxT(' '), LONG_CALLSIGN_LENGTH);

	m_controlRPT1.Truncate(LONG_CALLSIGN_LENGTH);
	m_controlRPT2.Truncate(LONG_CALLSIGN_LENGTH);
	m_controlShutdown.Truncate(LONG_CALLSIGN_LENGTH);
	m_controlStartup.Truncate(LONG_CALLSIGN_LENGTH);
	m_controlStatus1.Truncate(LONG_CALLSIGN_LENGTH);
	m_controlStatus2.Truncate(LONG_CALLSIGN_LENGTH);
	m_controlStatus3.Truncate(LONG_CALLSIGN_LENGTH);
	m_controlStatus4.Truncate(LONG_CALLSIGN_LENGTH);
	m_controlStatus5.Truncate(LONG_CALLSIGN_LENGTH);
	m_controlCommand1.Truncate(LONG_CALLSIGN_LENGTH);
	m_controlCommand2.Truncate(LONG_CALLSIGN_LENGTH);
	m_controlCommand3.Truncate(LONG_CALLSIGN_LENGTH);
	m_controlCommand4.Truncate(LONG_CALLSIGN_LENGTH);
	m_controlOutput1.Truncate(LONG_CALLSIGN_LENGTH);
	m_controlOutput2.Truncate(LONG_CALLSIGN_LENGTH);
	m_controlOutput3.Truncate(LONG_CALLSIGN_LENGTH);
	m_controlOutput4.Truncate(LONG_CALLSIGN_LENGTH);
}

void CGMSKRepeaterTRXThread::setOutputs(bool out1, bool out2, bool out3, bool out4)
{
	if (m_controller == NULL)
		return;

	m_output1 = out1;
	m_output2 = out2;
	m_output3 = out3;
	m_output4 = out4;

	m_controller->setOutput1(m_output1);
	m_controller->setOutput2(m_output2);
	m_controller->setOutput3(m_output3);
	m_controller->setOutput4(m_output4);
}

void CGMSKRepeaterTRXThread::setLogging(bool logging, const wxString& dir)
{
	if (logging && m_logging == NULL) {
		m_logging = new CDVTOOLFileWriter;
		m_logging->setDirectory(dir);
		return;
	}

	if (!logging && m_logging != NULL) {
		delete m_logging;
		m_logging = NULL;
		return;
	}
}

void CGMSKRepeaterTRXThread::setWhiteList(CCallsignList* list)
{
	wxASSERT(list != NULL);

	m_whiteList = list;
}

void CGMSKRepeaterTRXThread::setBlackList(CCallsignList* list)
{
	wxASSERT(list != NULL);

	m_blackList = list;
}

void CGMSKRepeaterTRXThread::setGreyList(CCallsignList* list)
{
	wxASSERT(list != NULL);

	m_greyList = list;
}

bool CGMSKRepeaterTRXThread::receiveRadioHeader()
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

bool CGMSKRepeaterTRXThread::receiveRadioData()
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

void CGMSKRepeaterTRXThread::receiveNetwork()
{
	if (m_protocolHandler == NULL)
		return;

	NETWORK_TYPE type;

	for (;;) {
		type = m_protocolHandler->read();

		// Get the data from the network
		if (type == NETWORK_NONE) {					// Nothing received
			break;
		} else if (type == NETWORK_HEADER) {		// A header
			CHeaderData* header = m_protocolHandler->readHeader();
			if (header != NULL) {
				::memcpy(m_lastData, NULL_FRAME_DATA_BYTES, DV_FRAME_LENGTH_BYTES);

				processNetworkHeader(header);

				m_headerTime.Start();
				m_packetTime.Start();
				m_packetCount   = 0U;
				m_packetSilence = 0U;
			}
		} else if (type == NETWORK_DATA) {			// AMBE data and slow data
			unsigned char data[2U * DV_FRAME_MAX_LENGTH_BYTES];
			::memset(data, 0x00U, 2U * DV_FRAME_MAX_LENGTH_BYTES);

			unsigned char seqNo;
			unsigned int length = m_protocolHandler->readData(data, DV_FRAME_MAX_LENGTH_BYTES, seqNo);
			if (length != 0U) {
				::memcpy(m_lastData, data, length);
				m_packetCount += processNetworkFrame(data, length, seqNo);
				m_watchdogTimer.reset();
			}
		} else if (type == NETWORK_TEXT) {			// Slow data text for the Ack
			m_protocolHandler->readText(m_ackText, m_linkStatus, m_reflector);
			m_linkEncoder.setTextData(m_ackText);
			wxLogMessage(wxT("Slow data set to \"%s\""), m_ackText.c_str());
		} else if (type == NETWORK_TEMPTEXT) {			// Temporary slow data text for the Ack
			m_protocolHandler->readTempText(m_tempAckText);
			wxLogMessage(wxT("Temporary slow data set to \"%s\""), m_tempAckText.c_str());
		} else if (type == NETWORK_STATUS1) {		// Status 1 data text
			m_status1Text = m_protocolHandler->readStatus1();
			m_status1Encoder.setTextData(m_status1Text);
			wxLogMessage(wxT("Status 1 data set to \"%s\""), m_status1Text.c_str());
		} else if (type == NETWORK_STATUS2) {		// Status 2 data text
			m_status2Text = m_protocolHandler->readStatus2();
			m_status2Encoder.setTextData(m_status2Text);
			wxLogMessage(wxT("Status 2 data set to \"%s\""), m_status2Text.c_str());
		} else if (type == NETWORK_STATUS3) {		// Status 3 data text
			m_status3Text = m_protocolHandler->readStatus3();
			m_status3Encoder.setTextData(m_status3Text);
			wxLogMessage(wxT("Status 3 data set to \"%s\""), m_status3Text.c_str());
		} else if (type == NETWORK_STATUS4) {		// Status 4 data text
			m_status4Text = m_protocolHandler->readStatus4();
			m_status4Encoder.setTextData(m_status4Text);
			wxLogMessage(wxT("Status 4 data set to \"%s\""), m_status4Text.c_str());
		} else if (type == NETWORK_STATUS5) {		// Status 5 data text
			m_status5Text = m_protocolHandler->readStatus5();
			m_status5Encoder.setTextData(m_status5Text);
			wxLogMessage(wxT("Status 5 data set to \"%s\""), m_status5Text.c_str());
		}
	}

	// Have we missed any data frames?
	if (m_state == DSRS_NETWORK && m_packetTime.Time() > 200L) {
		unsigned int packetsNeeded = m_headerTime.Time() / DSTAR_FRAME_TIME_MS;

		// wxLogMessage(wxT("Time: %u ms, need %u packets and received %u packets"), ms - m_headerMS, packetsNeeded, m_packetCount);

		if (packetsNeeded > m_packetCount) {
			unsigned int count = packetsNeeded - m_packetCount;

			if (count > 5U) {
				// wxLogMessage(wxT("Inserting silence into the network data stream"));

				count -= 2U;

				// Create silence frames
				for (unsigned int i = 0U; i < count; i++) {
					unsigned char data[DV_FRAME_LENGTH_BYTES];
					::memcpy(data, NULL_FRAME_DATA_BYTES, DV_FRAME_LENGTH_BYTES);
					m_packetCount += processNetworkFrame(data, DV_FRAME_LENGTH_BYTES, m_networkSeqNo);
					m_packetSilence++;
				}
			}
		}

		m_packetTime.Start();
	}
}

void CGMSKRepeaterTRXThread::transmitLocalHeader(CHeaderData* header)
{
	wxLogMessage(wxT("Transmitting to - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s  Flags: %02X %02X %02X"), header->getMyCall1().c_str(), header->getMyCall2().c_str(), header->getYourCall().c_str(), header->getRptCall1().c_str(), header->getRptCall2().c_str(), header->getFlag1(), header->getFlag2(), header->getFlag3());

	m_headerEncoder.setHeaderData(*header);

	m_localQueue.reset();
	m_localQueue.setHeader(header);
}

void CGMSKRepeaterTRXThread::transmitBeaconHeader()
{
	CHeaderData* header = new CHeaderData(m_rptCallsign, wxT("RPTR"), wxT("CQCQCQ  "), m_gwyCallsign, m_rptCallsign);
	transmitLocalHeader(header);
}

void CGMSKRepeaterTRXThread::transmitBeaconData(const unsigned char* data, unsigned int length, bool end)
{
	m_localQueue.addData(data, length, end);
}

void CGMSKRepeaterTRXThread::transmitRadioHeader(CHeaderData* header)
{
	wxLogMessage(wxT("Transmitting to - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s  Flags: %02X %02X %02X"), header->getMyCall1().c_str(), header->getMyCall2().c_str(), header->getYourCall().c_str(), header->getRptCall1().c_str(), header->getRptCall2().c_str(), header->getFlag1(), header->getFlag2(), header->getFlag3());

	m_headerEncoder.setHeaderData(*header);

	m_radioQueue.reset();
	m_radioQueue.setHeader(header);
}

void CGMSKRepeaterTRXThread::transmitNetworkHeader(CHeaderData* header)
{
	wxLogMessage(wxT("Transmitting to - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s  Flags: %02X %02X %02X"), header->getMyCall1().c_str(), header->getMyCall2().c_str(), header->getYourCall().c_str(), header->getRptCall1().c_str(), header->getRptCall2().c_str(), header->getFlag1(), header->getFlag2(), header->getFlag3());

	m_headerEncoder.setHeaderData(*header);

	bool empty = m_networkQueue[m_readNum]->isEmpty();
	if (!empty) {
		bool headerReady = m_networkQueue[m_readNum]->headerReady();
		if (headerReady) {
			// Transmission has never started, so just purge the queue
			m_networkQueue[m_readNum]->reset();

			m_readNum++;
			if (m_readNum >= NETWORK_QUEUE_COUNT)
				m_readNum = 0U;
		} else {
			// Purge the currently transmitting buffer and append an end of stream
			m_networkQueue[m_readNum]->reset();
			m_networkQueue[m_readNum]->addData(END_PATTERN_BYTES, DV_FRAME_LENGTH_BYTES, true);
		}
	}

	m_networkQueue[m_writeNum]->reset();
	m_networkQueue[m_writeNum]->setHeader(header);
}

void CGMSKRepeaterTRXThread::transmitStatus()
{
	CHeaderData* header = new CHeaderData(m_rptCallsign, wxT("    "), m_rxHeader->getMyCall1(), m_gwyCallsign, m_rptCallsign, RELAY_UNAVAILABLE);
	transmitLocalHeader(header);

	// Filler data
	for (unsigned int i = 0U; i < 21U; i++) {
		unsigned char buffer[DV_FRAME_LENGTH_BYTES];

		if (i == 0U) {
			m_ackEncoder.sync();

			::memcpy(buffer + 0U, NULL_AMBE_DATA_BYTES, VOICE_FRAME_LENGTH_BYTES);
			::memcpy(buffer + VOICE_FRAME_LENGTH_BYTES, DATA_SYNC_BYTES, DATA_FRAME_LENGTH_BYTES);
		} else {
			unsigned char text[DATA_FRAME_LENGTH_BYTES];
			m_ackEncoder.getTextData(text);

			::memcpy(buffer + 0U, NULL_AMBE_DATA_BYTES, VOICE_FRAME_LENGTH_BYTES);
			::memcpy(buffer + VOICE_FRAME_LENGTH_BYTES, text, DATA_FRAME_LENGTH_BYTES);
		}

		m_localQueue.addData(buffer, DV_FRAME_LENGTH_BYTES, false);
	}

	m_localQueue.addData(END_PATTERN_BYTES, DV_FRAME_LENGTH_BYTES, true);
}

void CGMSKRepeaterTRXThread::transmitErrorStatus()
{
	CHeaderData* header = new CHeaderData(m_rptCallsign, wxT("    "), m_rxHeader->getMyCall1(), m_rptCallsign, m_rptCallsign, RELAY_UNAVAILABLE);
	transmitLocalHeader(header);

	// Filler data
	for (unsigned int i = 0U; i < 21U; i++) {
		unsigned char buffer[DV_FRAME_LENGTH_BYTES];

		if (i == 0U) {
			m_linkEncoder.sync();

			::memcpy(buffer + 0U, NULL_AMBE_DATA_BYTES, VOICE_FRAME_LENGTH_BYTES);
			::memcpy(buffer + VOICE_FRAME_LENGTH_BYTES, DATA_SYNC_BYTES, DATA_FRAME_LENGTH_BYTES);
		} else {
			unsigned char text[DATA_FRAME_LENGTH_BYTES];
			m_linkEncoder.getTextData(text);

			::memcpy(buffer + 0U, NULL_AMBE_DATA_BYTES, VOICE_FRAME_LENGTH_BYTES);
			::memcpy(buffer + VOICE_FRAME_LENGTH_BYTES, text, DATA_FRAME_LENGTH_BYTES);
		}

		m_localQueue.addData(buffer, DV_FRAME_LENGTH_BYTES, false);
	}

	m_localQueue.addData(END_PATTERN_BYTES, DV_FRAME_LENGTH_BYTES, true);
}

void CGMSKRepeaterTRXThread::transmitUserStatus(unsigned int n)
{
	CSlowDataEncoder* encoder = NULL;
	CHeaderData* header = NULL;
	switch (n) {
		case 0U:
			header = new CHeaderData(m_rptCallsign, wxT("    "), wxT("STATUS 1"), m_gwyCallsign, m_rptCallsign);
			encoder = &m_status1Encoder;
			break;
		case 1U:
			header = new CHeaderData(m_rptCallsign, wxT("    "), wxT("STATUS 2"), m_gwyCallsign, m_rptCallsign);
			encoder = &m_status2Encoder;
			break;
		case 2U:
			header = new CHeaderData(m_rptCallsign, wxT("    "), wxT("STATUS 3"), m_gwyCallsign, m_rptCallsign);
			encoder = &m_status3Encoder;
			break;
		case 3U:
			header = new CHeaderData(m_rptCallsign, wxT("    "), wxT("STATUS 4"), m_gwyCallsign, m_rptCallsign);
			encoder = &m_status4Encoder;
			break;
		case 4U:
			header = new CHeaderData(m_rptCallsign, wxT("    "), wxT("STATUS 5"), m_gwyCallsign, m_rptCallsign);
			encoder = &m_status5Encoder;
			break;
		default:
			wxLogWarning(wxT("Invalid status number - %u"), n);
			return;
	}

	transmitLocalHeader(header);

	// Filler data
	for (unsigned int i = 0U; i < 21U; i++) {
		unsigned char buffer[DV_FRAME_LENGTH_BYTES];

		if (i == 0U) {
			encoder->sync();

			::memcpy(buffer + 0U, NULL_AMBE_DATA_BYTES, VOICE_FRAME_LENGTH_BYTES);
			::memcpy(buffer + VOICE_FRAME_LENGTH_BYTES, DATA_SYNC_BYTES, DATA_FRAME_LENGTH_BYTES);
		} else {
			unsigned char text[DATA_FRAME_LENGTH_BYTES];
			encoder->getTextData(text);

			::memcpy(buffer + 0U, NULL_AMBE_DATA_BYTES, VOICE_FRAME_LENGTH_BYTES);
			::memcpy(buffer + VOICE_FRAME_LENGTH_BYTES, text, DATA_FRAME_LENGTH_BYTES);
		}

		m_localQueue.addData(buffer, DV_FRAME_LENGTH_BYTES, false);
	}

	m_localQueue.addData(END_PATTERN_BYTES, DV_FRAME_LENGTH_BYTES, true);
}

bool CGMSKRepeaterTRXThread::transmitLocalHeader()
{
	TRISTATE ptt = m_modem->getPTT();
	if (ptt == STATE_UNKNOWN)
		return false;

	if (ptt == STATE_TRUE)
		return true;

	CHeaderData* header = m_localQueue.getHeader();
	if (header == NULL)
		return true;

	bool ret = m_modem->writeHeader(*header);
	if (!ret)
		return false;

	delete header;

	m_tx = true;

	m_modemLength = 0U;
	m_modemEnd    = false;

	m_headerReadTimer.stop();
	m_dataWaitTimer.start();

	return true;
}

bool CGMSKRepeaterTRXThread::transmitLocalData()
{
	if (m_dataWaitTimer.isRunning() && !m_dataWaitTimer.hasExpired())
		return true;

	m_dataWaitTimer.stop();

	TRISTATE space = m_modem->hasSpace();
	if (space == STATE_UNKNOWN)
		return false;

	if (space == STATE_FALSE)
		return true;

	if (m_modemLength == 0U)
		m_modemLength = m_localQueue.getData(m_modemBuffer, DV_FRAME_LENGTH_BYTES, m_modemEnd);

	if (m_broken) {
		// PTT restoration is needed when using DUTCH*Star 0.1.00
		TRISTATE ptt = m_modem->getPTT();
		if (ptt == STATE_UNKNOWN)
			return false;

		if (ptt == STATE_FALSE) {
			bool ret = m_modem->setPTT(true);
			if (!ret)
				return false;
		}
	}

	// If nothing to do then leave
	if (m_modemLength == 0U)
		return true;

	int length = m_modem->writeData(m_modemBuffer, m_modemLength);
	if (length == -1)
		return false;

	if (int(m_modemLength) == length) {
		if (m_modemEnd) {
			bool ret = m_modem->setPTT(false);
			if (!ret)
				return false;

			m_localQueue.reset();
			m_tx = false;

			m_headerReadTimer.start();

			return true;
		} else {
			m_modemLength = 0U;

			return true;
		}
	} else {
		m_modemLength -= length;

		for (unsigned int i = 0U; i < m_modemLength; i++)
			m_modemBuffer[i + 0U] = m_modemBuffer[i + length];

		return true;
	}
}

bool CGMSKRepeaterTRXThread::transmitRadioHeader()
{
	TRISTATE ptt = m_modem->getPTT();
	if (ptt == STATE_UNKNOWN)
		return false;

	if (ptt == STATE_TRUE)
		return true;

	CHeaderData* header = m_radioQueue.getHeader();
	if (header == NULL)
		return true;

	bool ret = m_modem->writeHeader(*header);
	if (!ret)
		return false;

	delete header;

	m_tx = true;

	m_modemLength = 0U;
	m_modemEnd    = false;

	m_headerReadTimer.stop();
	m_dataWaitTimer.start();

	return true;
}

bool CGMSKRepeaterTRXThread::transmitRadioData()
{
	if (m_dataWaitTimer.isRunning() && !m_dataWaitTimer.hasExpired())
		return true;

	m_dataWaitTimer.stop();

	// TRISTATE space = m_modem->hasSpace();
	// if (space == STATE_UNKNOWN)
	//	return false;
	//
	// if (space == STATE_FALSE)
	//	return true;

	if (m_modemLength == 0U)
		m_modemLength = m_radioQueue.getData(m_modemBuffer, DV_FRAME_LENGTH_BYTES, m_modemEnd);

	// if (m_broken) {
		// PTT restoration is needed when using DUTCH*Star 0.1.00
		TRISTATE ptt = m_modem->getPTT();
		if (ptt == STATE_UNKNOWN)
			return false;

		if (ptt == STATE_FALSE) {
			bool ret = m_modem->setPTT(true);
			if (!ret)
				return false;
		}
	// }

	// If nothing to do then leave
	if (m_modemLength == 0U)
		return true;

	int length = m_modem->writeData(m_modemBuffer, m_modemLength);
	if (length == -1)
		return false;

	if (int(m_modemLength) == length) {
		if (m_modemEnd) {
			bool ret = m_modem->setPTT(false);
			if (!ret)
				return false;

			m_radioQueue.reset();
			m_tx = false;

			m_headerReadTimer.start();

			return true;
		} else {
			m_modemLength = 0U;

			return true;
		}
	} else {
		m_modemLength -= length;

		for (unsigned int i = 0U; i < m_modemLength; i++)
			m_modemBuffer[i + 0U] = m_modemBuffer[i + length];

		return true;
	}
}

bool CGMSKRepeaterTRXThread::transmitNetworkHeader()
{
	TRISTATE ptt = m_modem->getPTT();
	if (ptt == STATE_UNKNOWN)
		return false;

	if (ptt == STATE_TRUE)
		return true;

	CHeaderData* header = m_networkQueue[m_readNum]->getHeader();
	if (header == NULL)
		return true;

	bool ret = m_modem->writeHeader(*header);
	if (!ret)
		return false;

	delete header;

	m_tx = true;

	m_modemLength = 0U;
	m_modemEnd    = false;

	if (m_mode != MODE_DUPLEX)
		m_headerReadTimer.stop();

	m_dataWaitTimer.start();

	return true;
}

bool CGMSKRepeaterTRXThread::transmitNetworkData()
{
	if (m_dataWaitTimer.isRunning() && !m_dataWaitTimer.hasExpired())
		return true;

	m_dataWaitTimer.stop();

	TRISTATE space = m_modem->hasSpace();
	if (space == STATE_UNKNOWN)
		return false;

	if (space == STATE_FALSE)
		return true;

	if (m_modemLength == 0U)
		m_modemLength = m_networkQueue[m_readNum]->getData(m_modemBuffer, DV_FRAME_LENGTH_BYTES, m_modemEnd);

	if (m_broken) {
		// PTT restoration is needed when using DUTCH*Star 0.1.00
		TRISTATE ptt = m_modem->getPTT();
		if (ptt == STATE_UNKNOWN)
			return false;

		if (ptt == STATE_FALSE) {
			bool ret = m_modem->setPTT(true);
			if (!ret)
				return false;
		}
	}

	// If nothing to do then leave
	if (m_modemLength == 0U)
		return true;

	int length = m_modem->writeData(m_modemBuffer, m_modemLength);
	if (length == -1)
		return false;

	if (int(m_modemLength) == length) {
		if (m_modemEnd) {
			bool ret = m_modem->setPTT(false);
			if (!ret)
				return false;

			m_networkQueue[m_readNum]->reset();

			m_readNum++;
			if (m_readNum >= NETWORK_QUEUE_COUNT)
				m_readNum = 0U;

			m_tx = false;

			if (m_mode != MODE_DUPLEX)
				m_headerReadTimer.start();

			return true;
		} else {
			m_modemLength = 0U;

			return true;
		}
	} else {
		m_modemLength -= length;

		for (unsigned int i = 0U; i < m_modemLength; i++)
			m_modemBuffer[i + 0U] = m_modemBuffer[i + length];

		return true;
	}
}

void CGMSKRepeaterTRXThread::repeaterStateMachine()
{
	switch (m_state) {
		case DSRS_VALID:
			if (m_timeoutTimer.isRunning() && m_timeoutTimer.hasExpired()) {
				wxLogMessage(wxT("User has timed out"));
				setRepeaterState(DSRS_TIMEOUT);
			}
			break;

		case DSRS_VALID_WAIT:
			if (m_ackTimer.hasExpired()) {
				if (m_mode != MODE_GATEWAY)
					transmitStatus();
				setRepeaterState(DSRS_LISTENING);
				m_activeHangTimer.start();
			}
			break;

		case DSRS_INVALID_WAIT:
			if (m_ackTimer.hasExpired()) {
				if (m_mode != MODE_GATEWAY)
					transmitErrorStatus();
				setRepeaterState(DSRS_LISTENING);
				m_activeHangTimer.start();
			}
			break;

		case DSRS_TIMEOUT_WAIT:
			if (m_ackTimer.hasExpired()) {
				if (m_mode != MODE_GATEWAY)
					transmitStatus();
				setRepeaterState(DSRS_LISTENING);
				m_activeHangTimer.start();
			}
			break;

		case DSRS_NETWORK:
			if (m_watchdogTimer.hasExpired()) {
				wxLogMessage(wxT("Network watchdog has expired"));
				// Send end of transmission data to the radio
				m_networkQueue[m_writeNum]->addData(END_PATTERN_BYTES, DV_FRAME_LENGTH_BYTES, true);
				endOfNetworkData();
				m_tx = false;
			}
			break;

		default:
			break;
	}
}

bool CGMSKRepeaterTRXThread::setRepeaterState(DSTAR_RPT_STATE state)
{
	// Can't change state when shutdown
	if (m_disable || m_shutdown)
		return false;

	// The "from" state
	switch (m_state) {
		case DSRS_SHUTDOWN:
			m_beaconTimer.start();
			break;

		case DSRS_LISTENING:
			m_beaconTimer.stop();
			break;

		default:
			break;
	}

	// The "to" state
	switch (state) {
		case DSRS_SHUTDOWN:
			m_timeoutTimer.stop();
			m_watchdogTimer.stop();
			m_activeHangTimer.stop();
			m_ackTimer.stop();
			m_beaconTimer.stop();
			m_controller->setActive(false);
			m_controller->setRadioTransmit(false);
			m_state = DSRS_SHUTDOWN;
			break;

		case DSRS_LISTENING:
			m_timeoutTimer.stop();
			m_watchdogTimer.stop();
			m_ackTimer.stop();
			m_beaconTimer.start();
			m_state = DSRS_LISTENING;
			if (m_protocolHandler != NULL)	// Tell the protocol handler
				m_protocolHandler->reset();
			break;

		case DSRS_VALID:
			if (m_state != DSRS_LISTENING)
				return false;

			m_activeHangTimer.stop();
			m_timeoutTimer.start();
			m_state = DSRS_VALID;
			break;

		case DSRS_VALID_WAIT:
			m_ackTimer.start();
			m_timeoutTimer.stop();
			m_state = DSRS_VALID_WAIT;
			break;

		case DSRS_TIMEOUT:
			// Send end of transmission data to the radio and the network
			if (m_mode == MODE_DUPLEX)
				m_radioQueue.addData(END_PATTERN_BYTES, DV_FRAME_LENGTH_BYTES, true);

			if (!m_blocked && m_protocolHandler != NULL) {
				unsigned char bytes[DV_FRAME_MAX_LENGTH_BYTES];
				::memcpy(bytes, NULL_AMBE_DATA_BYTES, VOICE_FRAME_LENGTH_BYTES);
				::memcpy(bytes + VOICE_FRAME_LENGTH_BYTES, END_PATTERN_BYTES, END_PATTERN_LENGTH_BYTES);
				m_protocolHandler->writeData(bytes, DV_FRAME_MAX_LENGTH_BYTES, 0U, true);
			}

			m_timeoutTimer.stop();
			m_state = DSRS_TIMEOUT;
			break;

		case DSRS_TIMEOUT_WAIT:
			m_ackTimer.start();
			m_timeoutTimer.stop();
			m_state = DSRS_TIMEOUT_WAIT;
			break;

		case DSRS_INVALID:
			if (m_state != DSRS_LISTENING)
				return false;

			m_activeHangTimer.stop();
			m_timeoutTimer.stop();
			m_state = DSRS_INVALID;
			break;

		case DSRS_INVALID_WAIT:
			m_ackTimer.start();
			m_timeoutTimer.stop();
			m_state = DSRS_INVALID_WAIT;
			break;

		case DSRS_NETWORK:
			if (m_state != DSRS_LISTENING && m_state != DSRS_VALID_WAIT && m_state != DSRS_INVALID_WAIT && m_state != DSRS_TIMEOUT_WAIT)
				return false;

			m_state = DSRS_NETWORK;
			m_networkSeqNo = 0U;
			m_timeoutTimer.stop();
			m_watchdogTimer.start();
			m_activeHangTimer.stop();
			m_ackTimer.stop();
			break;
	}

	return true;
}

bool CGMSKRepeaterTRXThread::processRadioHeader(CHeaderData* header)
{
	wxASSERT(header != NULL);

	// Check control messages
	bool res = checkControl(*header);
	if (res) {
		delete header;
		return true;
	}

	// If shutdown we ignore incoming headers
	if (m_state == DSRS_SHUTDOWN) {
		delete header;
		return true;
	}

	if (m_whiteList != NULL) {
		bool res = m_whiteList->isInList(header->getMyCall1());
		if (!res) {
			wxLogMessage(wxT("%s rejected due to not being in the white list"), header->getMyCall1().c_str());
			delete header;
			return true;
		}
	}

	if (m_blackList != NULL) {
		bool res = m_blackList->isInList(header->getMyCall1());
		if (res) {
			wxLogMessage(wxT("%s rejected due to being in the black list"), header->getMyCall1().c_str());
			delete header;
			return true;
		}
	}

	m_blocked = false;
	if (m_greyList != NULL) {
		bool res = m_greyList->isInList(header->getMyCall1());
		if (res) {
			wxLogMessage(wxT("%s blocked from the network due to being in the grey list"), header->getMyCall1().c_str());
			m_blocked = true;
		}
	}

	// Check for receiving our own gateway data, and ignore it
	if (m_mode == MODE_GATEWAY) {
		if (header->getFlag2() == 0x01U) {
			wxLogMessage(wxT("Receiving a gateway header, ignoring"));
			delete header;
			return true;
		}

		header->setFlag2(0x00U);
	}

	// We don't handle DD data packets
	if (header->isDataPacket()) {
		wxLogMessage(wxT("Received a DD packet, ignoring"));
		delete header;
		return false;
	}

	TRISTATE valid = checkHeader(*header);
	switch (valid) {
		case STATE_FALSE: {
				bool res = setRepeaterState(DSRS_INVALID);
				if (res) {
					delete m_rxHeader;
					m_rxHeader = header;
				} else {
					delete header;
				}
			}
			return true;

		case STATE_UNKNOWN:
			delete header;
			return true;

		case STATE_TRUE:
			break;
	}

	setRepeaterState(DSRS_VALID);

	// Send the valid header to the gateway if we are accepted
	if (m_state == DSRS_VALID) {
		delete m_rxHeader;
		m_rxHeader = header;

		if (m_logging != NULL)
			m_logging->open(*m_rxHeader);

		// Only send on the network if the user isn't blocked and we have one and RPT2 is not blank or the repeater callsign
		if (!m_blocked && m_protocolHandler != NULL && !m_rxHeader->getRptCall2().IsSameAs(wxT("        ")) && !m_rxHeader->getRptCall2().IsSameAs(m_rptCallsign)) {
			CHeaderData netHeader(*m_rxHeader);
			netHeader.setRptCall1(m_rxHeader->getRptCall2());
			netHeader.setRptCall2(m_rxHeader->getRptCall1());
			netHeader.setFlag1(m_rxHeader->getFlag1() & ~REPEATER_MASK);
			m_protocolHandler->writeHeader(netHeader);
		}

		// Create the new radio header but only in duplex mode
		if (m_mode == MODE_DUPLEX) {
			CHeaderData* rfHeader = new CHeaderData(*m_rxHeader);
			rfHeader->setRptCall1(m_rptCallsign);
			rfHeader->setRptCall2(m_rptCallsign);
			rfHeader->setFlag1(m_rxHeader->getFlag1() & ~REPEATER_MASK);
			transmitRadioHeader(rfHeader);
		}
	}

	// If we're in network mode, send the header as a busy header to the gateway in case it's an unlink
	// command
	if (m_state == DSRS_NETWORK) {
		// Only send on the network if the user isn't blocked and we have one and RPT2 is not blank or the repeater callsign
		if (!header->getRptCall2().IsSameAs(wxT("        ")) && !header->getRptCall2().IsSameAs(m_rptCallsign)) {
			if (!m_blocked && m_protocolHandler != NULL) {
				CHeaderData netHeader(*header);
				netHeader.setRptCall1(header->getRptCall2());
				netHeader.setRptCall2(header->getRptCall1());
				netHeader.setFlag1(header->getFlag1() & ~REPEATER_MASK);
				m_protocolHandler->writeBusyHeader(netHeader);
			}

			m_busyData = true;
		}

		delete header;
	}

	return true;
}

void CGMSKRepeaterTRXThread::processNetworkHeader(CHeaderData* header)
{
	wxASSERT(header != NULL);

	// If shutdown we ignore incoming headers
	if (m_state == DSRS_SHUTDOWN) {
		delete header;
		return;
	}

	wxLogMessage(wxT("Network header received - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s  Flags: %02X %02X %02X"), header->getMyCall1().c_str(), header->getMyCall2().c_str(), header->getYourCall().c_str(), header->getRptCall1().c_str(), header->getRptCall2().c_str(), header->getFlag1(), header->getFlag2(), header->getFlag3());

	// Is it for us?
	if (!header->getRptCall2().IsSameAs(m_rptCallsign)) {
		wxLogMessage(wxT("Invalid network RPT2 value, ignoring"));
		delete header;
		return;
	}

	setRepeaterState(DSRS_NETWORK);

	if (m_state == DSRS_NETWORK) {
		delete m_rxHeader;
		m_rxHeader = header;

		if (m_mode == MODE_GATEWAY) {
			// If in gateway mode, set the repeater bit, set flag 2 to 0x01,
			// and change RPT1 & RPT2 just for transmission
			CHeaderData* header = new CHeaderData(*m_rxHeader);
			header->setRepeaterMode(true);
			header->setFlag2(0x01U);
			header->setRptCall1(m_rptCallsign);
			header->setRptCall2(m_gwyCallsign);
			transmitNetworkHeader(header);
		} else {
			CHeaderData* header = new CHeaderData(*m_rxHeader);

			transmitNetworkHeader(header);
		}
	} else {
		delete header;
	}
}

void CGMSKRepeaterTRXThread::processRadioFrame(unsigned char* data, FRAME_TYPE type)
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

	// Pass background AMBE data to the network
	if (m_busyData) {
		if (type == FRAME_END) {
			if (!m_blocked && m_protocolHandler != NULL) {
				unsigned char bytes[DV_FRAME_MAX_LENGTH_BYTES];
				::memcpy(bytes, NULL_AMBE_DATA_BYTES, VOICE_FRAME_LENGTH_BYTES);
				::memcpy(bytes + VOICE_FRAME_LENGTH_BYTES, END_PATTERN_BYTES, END_PATTERN_LENGTH_BYTES);
				m_protocolHandler->writeBusyData(bytes, DV_FRAME_MAX_LENGTH_BYTES, 0U, true);
			}

			m_busyData = false;
		} else {
			if (!m_blocked && m_protocolHandler != NULL)
				m_protocolHandler->writeBusyData(data, DV_FRAME_LENGTH_BYTES, errors, false);
		}

		return;
	}

	// Don't pass through the frame of an invalid transmission
	if (m_state != DSRS_VALID)
		return;

	if (type == FRAME_END) {
		if (m_logging != NULL)
			m_logging->close();

		// Transmit the end sync on the radio, but only in duplex mode
		if (m_mode == MODE_DUPLEX)
			m_radioQueue.addData(data, DV_FRAME_LENGTH_BYTES, true);

		// Send null data and the end marker over the network, and the statistics
		if (!m_blocked && m_protocolHandler != NULL) {
			unsigned char bytes[DV_FRAME_MAX_LENGTH_BYTES];
			::memcpy(bytes, NULL_AMBE_DATA_BYTES, VOICE_FRAME_LENGTH_BYTES);
			::memcpy(bytes + VOICE_FRAME_LENGTH_BYTES, END_PATTERN_BYTES, END_PATTERN_LENGTH_BYTES);
			m_protocolHandler->writeData(bytes, DV_FRAME_MAX_LENGTH_BYTES, 0U, true);
		}
	} else {
		if (m_logging != NULL)
			m_logging->write(data, DV_FRAME_LENGTH_BYTES);

		// Send the data to the network
		if (!m_blocked && m_protocolHandler != NULL)
			m_protocolHandler->writeData(data, DV_FRAME_LENGTH_BYTES, errors, false);

		// Send the data for transmission, but only in duplex mode
		if (m_mode == MODE_DUPLEX) {
			blankDTMF(data);
			m_radioQueue.addData(data, DV_FRAME_LENGTH_BYTES, false);
		}
	}
}

unsigned int CGMSKRepeaterTRXThread::processNetworkFrame(unsigned char* data, unsigned int length, unsigned char seqNo)
{
	wxASSERT(data != NULL);
	wxASSERT(length > 0U);

	if (m_state != DSRS_NETWORK)
		return 0U;

	bool end = (seqNo & 0x40U) == 0x40U;
	if (end) {
		if (length > DV_FRAME_LENGTH_BYTES) {
			m_networkQueue[m_writeNum]->addData(data, DV_FRAME_LENGTH_BYTES, false);
			m_networkQueue[m_writeNum]->addData(data + DV_FRAME_LENGTH_BYTES, DV_FRAME_LENGTH_BYTES, true);
		} else {
			m_networkQueue[m_writeNum]->addData(data, DV_FRAME_LENGTH_BYTES, true);
		}

		endOfNetworkData();
		return 1U;
	}

	// Mask out the control bits of the sequence number
	seqNo &= 0x1FU;

	// Count the number of silence frames to insert
	unsigned int tempSeqNo = m_networkSeqNo;
	unsigned int count = 0U;
	while (seqNo != tempSeqNo) {
		count++;

		tempSeqNo++;
		if (tempSeqNo >= 21U)
			tempSeqNo = 0U;
	}

	// If the number is too high, then it probably means an old out-of-order frame, ignore it
	if (count > 18U)
		return 0U;

	unsigned int packetCount = 0U;

	// Insert missing frames
	while (seqNo != m_networkSeqNo) {
		unsigned char buffer[DV_FRAME_LENGTH_BYTES];
		if (count > SILENCE_THRESHOLD) {
			::memcpy(buffer, NULL_FRAME_DATA_BYTES, DV_FRAME_LENGTH_BYTES);
		} else {
			::memcpy(buffer, m_lastData, DV_FRAME_LENGTH_BYTES);
			m_ambe.regenerate(buffer);
			blankDTMF(buffer);
		}

		if (m_networkSeqNo == 0U)
			::memcpy(buffer + VOICE_FRAME_LENGTH_BYTES, DATA_SYNC_BYTES, DATA_FRAME_LENGTH_BYTES);

		m_networkQueue[m_writeNum]->addData(buffer, DV_FRAME_LENGTH_BYTES, false);

		packetCount++;
		m_networkSeqNo++;
		m_packetSilence++;
		if (m_networkSeqNo >= 21U)
			m_networkSeqNo = 0U;
	}

	// Regenerate the sync bytes
	if (m_networkSeqNo == 0U) {
		::memcpy(data + VOICE_FRAME_LENGTH_BYTES, DATA_SYNC_BYTES, DATA_FRAME_LENGTH_BYTES);
		m_headerEncoder.sync();
	} else {
		// If in Gateway mode, replace the slow data with our modified header
		if (m_mode == MODE_GATEWAY) {
			unsigned char slowData[DATA_FRAME_LENGTH_BYTES];
			m_headerEncoder.getHeaderData(slowData);
			::memcpy(data + VOICE_FRAME_LENGTH_BYTES, slowData, DATA_FRAME_LENGTH_BYTES);
		}
	}

	packetCount++;
	m_networkSeqNo++;
	if (m_networkSeqNo >= 21U)
		m_networkSeqNo = 0U;

	m_ambe.regenerate(data);
	blankDTMF(data);

	m_networkQueue[m_writeNum]->addData(data, DV_FRAME_LENGTH_BYTES, false);

	return packetCount;
}

void CGMSKRepeaterTRXThread::endOfRadioData()
{
	switch (m_state) {
		case DSRS_VALID:
			wxLogMessage(wxT("AMBE for %s  Frames: %.1fs, Silence: %.1f%%, BER: %.1f%%"), m_rxHeader->getMyCall1().c_str(), float(m_ambeFrames) / 50.0F, float(m_ambeSilence * 100U) / float(m_ambeFrames), float(m_ambeErrors * 100U) / float(m_ambeBits));

			if (m_tempAckText.IsEmpty()) {
				if (m_ack == AT_BER) {
					// Create the ack text with the linked reflector and BER
					wxString ackText;
					if (m_linkStatus == LS_LINKED_DEXTRA || m_linkStatus == LS_LINKED_DPLUS || m_linkStatus == LS_LINKED_DCS || m_linkStatus == LS_LINKED_LOOPBACK)
						ackText.Printf(wxT("%-8s  BER: %.1f%%   "), m_reflector.c_str(), float(m_ambeErrors * 100U) / float(m_ambeBits));
					else
						ackText.Printf(wxT("BER: %.1f%%            "), float(m_ambeErrors * 100U) / float(m_ambeBits));
					m_ackEncoder.setTextData(ackText);
				} else {
					m_ackEncoder.setTextData(m_ackText);
				}
			} else {
				m_ackEncoder.setTextData(m_tempAckText);
				m_tempAckText.Clear();
			}

			if (m_ack != AT_NONE || m_mode == MODE_GATEWAY) {
				setRepeaterState(DSRS_VALID_WAIT);
			} else {
				setRepeaterState(DSRS_LISTENING);
				m_activeHangTimer.start();
			}
			break;

		case DSRS_INVALID:
			wxLogMessage(wxT("AMBE for %s  Frames: %.1fs, Silence: %.1f%%, BER: %.1f%%"), m_rxHeader->getMyCall1().c_str(), float(m_ambeFrames) / 50.0F, float(m_ambeSilence * 100U) / float(m_ambeFrames), float(m_ambeErrors * 100U) / float(m_ambeBits));

			if (m_ack != AT_NONE || m_mode == MODE_GATEWAY) {
				setRepeaterState(DSRS_INVALID_WAIT);
			} else {
				setRepeaterState(DSRS_LISTENING);
				m_activeHangTimer.start();
			}
			break;

		case DSRS_TIMEOUT:
			wxLogMessage(wxT("AMBE for %s  Frames: %.1fs, Silence: %.1f%%, BER: %.1f%%"), m_rxHeader->getMyCall1().c_str(), float(m_ambeFrames) / 50.0F, float(m_ambeSilence * 100U) / float(m_ambeFrames), float(m_ambeErrors * 100U) / float(m_ambeBits));

			if (m_tempAckText.IsEmpty()) {
				if (m_ack == AT_BER) {
					// Create the ack text with the linked reflector and BER
					wxString ackText;
					if (m_linkStatus == LS_LINKED_DEXTRA || m_linkStatus == LS_LINKED_DPLUS || m_linkStatus == LS_LINKED_DCS || m_linkStatus == LS_LINKED_LOOPBACK)
						ackText.Printf(wxT("%-8s  BER: %.1f%%   "), m_reflector.c_str(), float(m_ambeErrors * 100U) / float(m_ambeBits));
					else
						ackText.Printf(wxT("BER: %.1f%%            "), float(m_ambeErrors * 100U) / float(m_ambeBits));
					m_ackEncoder.setTextData(ackText);
				} else {
					m_ackEncoder.setTextData(m_ackText);
				}
			} else {
				m_ackEncoder.setTextData(m_tempAckText);
				m_tempAckText.Clear();
			}

			if (m_ack != AT_NONE || m_mode == MODE_GATEWAY) {
				setRepeaterState(DSRS_TIMEOUT_WAIT);
			} else {
				setRepeaterState(DSRS_LISTENING);
				m_activeHangTimer.start();
			}
			break;

		default:
			break;
	}
}

void CGMSKRepeaterTRXThread::endOfNetworkData()
{
	if (m_watchdogTimer.hasExpired()) {
		m_packetCount   -= 77U;
		m_packetSilence -= 77U;
	}

	float loss = 0.0F;
	if (m_packetCount != 0U)
		loss = float(m_packetSilence) / float(m_packetCount);

	wxLogMessage(wxT("Stats for %s  Frames: %.1fs, Loss: %.1f%%, Packets: %u/%u"), m_rxHeader->getMyCall1().c_str(), float(m_packetCount) / 50.0F, loss * 100.0F, m_packetSilence, m_packetCount);

	setRepeaterState(DSRS_LISTENING);
	m_activeHangTimer.start();

	m_writeNum++;
	if (m_writeNum >= NETWORK_QUEUE_COUNT)
		m_writeNum = 0U;
}

CGMSKRepeaterStatusData* CGMSKRepeaterTRXThread::getStatus()
{
	if (m_state == DSRS_SHUTDOWN || m_state == DSRS_LISTENING) {
		return new CGMSKRepeaterStatusData(wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString,
				wxEmptyString, 0x00, 0x00, 0x00, m_tx, m_state, m_timeoutTimer.getTimer(),
				m_timeoutTimer.getTimeout(), m_beaconTimer.getTimer(), m_beaconTimer.getTimeout(), 0.0F,
				m_ackText, m_status1Text, m_status2Text, m_status3Text, m_status4Text, m_status5Text);
	} else if (m_state == DSRS_NETWORK) {
		float loss = 0.0F;
		if (m_packetCount != 0U)
			loss = float(m_packetSilence) / float(m_packetCount);

		return new CGMSKRepeaterStatusData(m_rxHeader->getMyCall1(), m_rxHeader->getMyCall2(),
				m_rxHeader->getYourCall(), m_rxHeader->getRptCall1(), m_rxHeader->getRptCall2(), 
				m_rxHeader->getFlag1(), m_rxHeader->getFlag2(), m_rxHeader->getFlag3(), m_tx, m_state,
				m_timeoutTimer.getTimer(), m_timeoutTimer.getTimeout(), m_beaconTimer.getTimer(),
				m_beaconTimer.getTimeout(), loss * 100.0F, m_ackText, m_status1Text, m_status2Text,
				m_status3Text, m_status4Text, m_status5Text);
	} else {
		float   bits = float(m_ambeBits - m_lastAMBEBits);
		float errors = float(m_ambeErrors - m_lastAMBEErrors);
		if (bits == 0.0F)
			bits = 1.0F;

		m_lastAMBEBits   = m_ambeBits;
		m_lastAMBEErrors = m_ambeErrors;

		return new CGMSKRepeaterStatusData(m_rxHeader->getMyCall1(), m_rxHeader->getMyCall2(),
				m_rxHeader->getYourCall(), m_rxHeader->getRptCall1(), m_rxHeader->getRptCall2(), 
				m_rxHeader->getFlag1(), m_rxHeader->getFlag2(), m_rxHeader->getFlag3(), m_tx, m_state,
				m_timeoutTimer.getTimer(), m_timeoutTimer.getTimeout(), m_beaconTimer.getTimer(),
				m_beaconTimer.getTimeout(), (errors * 100.0F) / bits, m_ackText, m_status1Text, m_status2Text,
				m_status3Text, m_status4Text, m_status5Text);
	}
}

void CGMSKRepeaterTRXThread::clock(unsigned int ms)
{
	m_pollTimer.clock(ms);
	m_timeoutTimer.clock(ms);
	m_watchdogTimer.clock(ms);
	m_activeHangTimer.clock(ms);
	m_ackTimer.clock(ms);
	m_status1Timer.clock(ms);
	m_status2Timer.clock(ms);
	m_status3Timer.clock(ms);
	m_status4Timer.clock(ms);
	m_status5Timer.clock(ms);
	m_beaconTimer.clock(ms);
	m_headerReadTimer.clock(ms);
	m_dataWaitTimer.clock(ms);
	if (m_beacon != NULL)
		m_beacon->clock();
}

void CGMSKRepeaterTRXThread::shutdown()
{
	m_shutdown = true;
}

void CGMSKRepeaterTRXThread::startup()
{
	m_shutdown = false;
}

void CGMSKRepeaterTRXThread::command1()
{
	if (!m_controlCommand1Line.IsEmpty())
		::wxShell(m_controlCommand1Line);
}

void CGMSKRepeaterTRXThread::command2()
{
	if (!m_controlCommand2Line.IsEmpty())
		::wxShell(m_controlCommand2Line);
}

void CGMSKRepeaterTRXThread::command3()
{
	if (!m_controlCommand3Line.IsEmpty())
		::wxShell(m_controlCommand3Line);
}

void CGMSKRepeaterTRXThread::command4()
{
	if (!m_controlCommand4Line.IsEmpty())
		::wxShell(m_controlCommand4Line);
}

bool CGMSKRepeaterTRXThread::checkControl(const CHeaderData& header)
{
	if (!m_controlEnabled)
		return false;

	if (!m_controlRPT1.IsSameAs(header.getRptCall1()) || !m_controlRPT2.IsSameAs(header.getRptCall2()))
		return false;

	if (m_controlShutdown.IsSameAs(header.getYourCall())) {
		wxLogMessage(wxT("Shutdown requested by %s/%s"), header.getMyCall1().c_str(), header.getMyCall2().c_str());
		shutdown();
	} else if (m_controlStartup.IsSameAs(header.getYourCall())) {
		wxLogMessage(wxT("Startup requested by %s/%s"), header.getMyCall1().c_str(), header.getMyCall2().c_str());
		startup();
	} else if (m_controlStatus1.IsSameAs(header.getYourCall())) {
		wxLogMessage(wxT("Status 1 requested by %s/%s"), header.getMyCall1().c_str(), header.getMyCall2().c_str());
		m_status1Timer.start();
	} else if (m_controlStatus2.IsSameAs(header.getYourCall())) {
		wxLogMessage(wxT("Status 2 requested by %s/%s"), header.getMyCall1().c_str(), header.getMyCall2().c_str());
		m_status2Timer.start();
	} else if (m_controlStatus3.IsSameAs(header.getYourCall())) {
		wxLogMessage(wxT("Status 3 requested by %s/%s"), header.getMyCall1().c_str(), header.getMyCall2().c_str());
		m_status3Timer.start();
	} else if (m_controlStatus4.IsSameAs(header.getYourCall())) {
		wxLogMessage(wxT("Status 4 requested by %s/%s"), header.getMyCall1().c_str(), header.getMyCall2().c_str());
		m_status4Timer.start();
	} else if (m_controlStatus5.IsSameAs(header.getYourCall())) {
		wxLogMessage(wxT("Status 5 requested by %s/%s"), header.getMyCall1().c_str(), header.getMyCall2().c_str());
		m_status5Timer.start();
	} else if (m_controlCommand1.IsSameAs(header.getYourCall())) {
		wxLogMessage(wxT("Command 1 requested by %s/%s"), header.getMyCall1().c_str(), header.getMyCall2().c_str());
		command1();
	} else if (m_controlCommand2.IsSameAs(header.getYourCall())) {
		wxLogMessage(wxT("Command 2 requested by %s/%s"), header.getMyCall1().c_str(), header.getMyCall2().c_str());
		command2();
	} else if (m_controlCommand3.IsSameAs(header.getYourCall())) {
		wxLogMessage(wxT("Command 3 requested by %s/%s"), header.getMyCall1().c_str(), header.getMyCall2().c_str());
		command3();
	} else if (m_controlCommand4.IsSameAs(header.getYourCall())) {
		wxLogMessage(wxT("Command 4 requested by %s/%s"), header.getMyCall1().c_str(), header.getMyCall2().c_str());
		command4();
	} else if (m_controlOutput1.IsSameAs(header.getYourCall())) {
		wxLogMessage(wxT("Output 1 requested by %s/%s"), header.getMyCall1().c_str(), header.getMyCall2().c_str());
		m_output1 = !m_output1;
		m_controller->setOutput1(m_output1);
	} else if (m_controlOutput2.IsSameAs(header.getYourCall())) {
		wxLogMessage(wxT("Output 2 requested by %s/%s"), header.getMyCall1().c_str(), header.getMyCall2().c_str());
		m_output2 = !m_output2;
		m_controller->setOutput2(m_output2);
	} else if (m_controlOutput3.IsSameAs(header.getYourCall())) {
		wxLogMessage(wxT("Output 3 requested by %s/%s"), header.getMyCall1().c_str(), header.getMyCall2().c_str());
		m_output3 = !m_output3;
		m_controller->setOutput3(m_output3);
	} else if (m_controlOutput4.IsSameAs(header.getYourCall())) {
		wxLogMessage(wxT("Output 4 requested by %s/%s"), header.getMyCall1().c_str(), header.getMyCall2().c_str());
		m_output4 = !m_output4;
		m_controller->setOutput4(m_output4);
	} else {
		wxLogMessage(wxT("Invalid command of %s sent by %s/%s"), header.getYourCall().c_str(), header.getMyCall1().c_str(), header.getMyCall2().c_str());
	}

	return true;
}

TRISTATE CGMSKRepeaterTRXThread::checkHeader(CHeaderData& header)
{
	// If not in RPT1 validation mode, then a simplex header is converted to a proper repeater header
	if (!m_rpt1Validation) {
		if (!header.isRepeaterMode()) {
			// Convert to a properly addressed repeater packet
			header.setRepeaterMode(true);
			header.setRptCall1(m_rptCallsign);
			header.setRptCall2(m_gwyCallsign);
		}
	}

	// The repeater bit must be set when not in gateway mode
	if (m_mode != MODE_GATEWAY) {
		if (!header.isRepeaterMode()) {
			wxLogMessage(wxT("Received a non-repeater packet, ignoring"));
			return STATE_FALSE;
		}
	} else {
		// Quietly reject acks when in gateway mode
		if (header.isAck() || header.isNoResponse() || header.isRelayUnavailable())
			return STATE_UNKNOWN;

		// As a second check, reject on own UR call
		wxString ur = header.getYourCall();
		if (ur.IsSameAs(m_rptCallsign) || ur.IsSameAs(m_gwyCallsign))
			return STATE_UNKNOWN;

		// Change RPT2 to be the gateway callsign in gateway mode
		header.setRptCall2(m_gwyCallsign);
	}

	wxString my = header.getMyCall1();

	// Make sure MyCall is not empty, a silly value, or the repeater or gateway callsigns
	if (my.IsSameAs(m_rptCallsign) ||
		my.IsSameAs(m_gwyCallsign) ||
		my.Left(6U).IsSameAs(wxT("NOCALL")) ||
		my.Left(6U).IsSameAs(wxT("N0CALL")) ||
		my.Left(6U).IsSameAs(wxT("MYCALL"))) {
		wxLogMessage(wxT("Invalid MYCALL value of %s, ignoring"), my.c_str());
		return STATE_UNKNOWN;
	}

	// Check the MyCall value against the regular expression
	bool ok = m_regEx.Matches(my);
	if (!ok) {
		wxLogMessage(wxT("Invalid MYCALL value of %s, ignoring"), my.c_str());
		return STATE_UNKNOWN;
	}

	// Is it for us?
	if (!header.getRptCall1().IsSameAs(m_rptCallsign)) {
		wxLogMessage(wxT("Invalid RPT1 value %s, ignoring"), header.getRptCall1().c_str());
		return STATE_FALSE;
	}

	// If using callsign restriction, validate the my callsign
	if (m_restriction) {
		if (!my.Left(LONG_CALLSIGN_LENGTH - 1U).IsSameAs(m_rptCallsign.Left(LONG_CALLSIGN_LENGTH - 1U))) {
			wxLogMessage(wxT("Unauthorised user %s tried to access the repeater"), my.c_str());
			return STATE_UNKNOWN;
		}
	}

	return STATE_TRUE;
}

void CGMSKRepeaterTRXThread::blankDTMF(unsigned char* data)
{
	wxASSERT(data != NULL);

	// DTMF begins with these byte values
	if ((data[0] & DTMF_MASK[0]) == DTMF_SIG[0] && (data[1] & DTMF_MASK[1]) == DTMF_SIG[1] &&
		(data[2] & DTMF_MASK[2]) == DTMF_SIG[2] && (data[3] & DTMF_MASK[3]) == DTMF_SIG[3] &&
		(data[4] & DTMF_MASK[4]) == DTMF_SIG[4] && (data[5] & DTMF_MASK[5]) == DTMF_SIG[5] &&
		(data[6] & DTMF_MASK[6]) == DTMF_SIG[6] && (data[7] & DTMF_MASK[7]) == DTMF_SIG[7] &&
		(data[8] & DTMF_MASK[8]) == DTMF_SIG[8])
		::memcpy(data, NULL_AMBE_DATA_BYTES, VOICE_FRAME_LENGTH_BYTES);
}

bool CGMSKRepeaterTRXThread::reopenModem()
{
	m_modem->close();

	while (!m_killed) {
		bool ret = m_modem->open();
		if (ret) {
			wxLogMessage(wxT("Reopened the GMSK modem"));
			return true;
		}

		// Reset the repeaters state
		setRepeaterState(DSRS_LISTENING);
		m_localQueue.reset();
		m_radioQueue.reset();
		m_busyData = false;
		m_tx = false;
		for (unsigned int j = 0U; j < NETWORK_QUEUE_COUNT; j++)
			m_networkQueue[j]->reset();

		::wxMilliSleep(1000UL);
	}

	delete m_modem;
	m_modem = NULL;

	return false;
}

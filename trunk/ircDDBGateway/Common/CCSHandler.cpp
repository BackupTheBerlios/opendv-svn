/*
 *   Copyright (C) 2013 by Jonathan Naylor G4KLX
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

#include "RepeaterHandler.h"
#include "DStarDefines.h"
#include "CCSHandler.h"
#include "Utils.h"


CCCSHandler**            CCCSHandler::m_handlers = NULL;

unsigned int             CCCSHandler::m_count = 0U;

in_addr                  CCCSHandler::m_ccsAddress;

wxString                 CCCSHandler::m_localAddress;
CHeaderLogger*           CCCSHandler::m_headerLogger = NULL;

void CCCSHandler::initialise(unsigned int count)
{
	wxASSERT(count > 0U);

	m_ccsAddress = CUDPReaderWriter::lookup(CCS_HOSTNAME);
	if (m_ccsAddress.s_addr == INADDR_NONE)
		wxLogError(wxT("Unable to find the IP address for %s"), CCS_HOSTNAME.c_str());

	m_count = count;
	m_handlers = new CCCSHandler*[m_count];

	for (unsigned int i = 0U; i < m_count; i++)
		m_handlers[i] = NULL;
}

void CCCSHandler::setLocalAddress(const wxString& address)
{
	m_localAddress = address;
}

void CCCSHandler::setHeaderLogger(CHeaderLogger* logger)
{
	m_headerLogger = logger;
}

void CCCSHandler::process()
{
	for (unsigned int i = 0U; i < m_count; i++) {
		if (m_handlers[i] != NULL)
			m_handlers[i]->processInt();
	}
}

void CCCSHandler::disconnect()
{
	for (unsigned int i = 0U; i < m_count; i++) {
		if (m_handlers[i] != NULL)
			m_handlers[i]->disconnectInt();
	}
}

void CCCSHandler::clock(unsigned int ms)
{
	for (unsigned int i = 0U; i < m_count; i++) {
		if (m_handlers[i] != NULL)
			m_handlers[i]->clockInt(ms);
	}
}

void CCCSHandler::finalise()
{
	for (unsigned int i = 0U; i < m_count; i++)
		delete m_handlers[i];

	delete[] m_handlers;
}

CCCSHandler::CCCSHandler(IRepeaterCallback* handler, const wxString& callsign, double latitude, double longitude, unsigned int localPort) :
m_handler(handler),
m_callsign(callsign),
m_reflector(),
m_locator(),
m_protocol(localPort, m_localAddress),
m_state(CS_DISABLED),
m_local(),
m_inactivityTimer(1000U, 300U),			// 5 minutes
m_pollInactivityTimer(1000U, 60U),		// 60 seconds
m_pollTimer(1000U, 10U),				// 10 seconds
m_tryTimer(1000U, 1U),					// 1 second
m_tryCount(0U),
m_id(0x00U),
m_seqNo(0U),
m_yourCall(),
m_myCall1(),
m_myCall2()
{
	wxASSERT(handler != NULL);

	if (latitude != 0.0 && longitude != 0.0)
		m_locator = CUtils::latLonToLoc(latitude, longitude);

	// Add to the global list
	for (unsigned int i = 0U; i < m_count; i++) {
		if (m_handlers[i] == NULL) {
			m_handlers[i] = this;
			break;
		}
	}
}

CCCSHandler::~CCCSHandler()
{
}

void CCCSHandler::setReflector(const wxString& callsign)
{
	m_reflector = callsign;

	if (m_reflector.IsEmpty())
		m_reflector = wxT("        ");
}

void CCCSHandler::processInt()
{
	if (m_state == CS_DISABLED)
		return;

	for (;;) {
		CCS_TYPE type = m_protocol.read();

		switch (type) {
			case CT_DATA: {
					CAMBEData* data = m_protocol.readData();
					if (data != NULL) {
						process(*data);
						delete data;
					}
				}
				break;

			case CT_POLL: {
					CPollData* poll = m_protocol.readPoll();
					if (poll != NULL) {
						process(*poll);
						delete poll;
					}
				}
				break;

			case CT_CONNECT: {
					CConnectData* connect = m_protocol.readConnect();
					if (connect != NULL) {
						process(*connect);
						delete connect;
					}
				}
				break;

			case CT_MISC: {
					CCCSData* data = m_protocol.readMisc();
					if (data != NULL) {
						process(*data);
						delete data;
					}
				}
				break;

			default:
				return;
		}
	}
}

void CCCSHandler::process(CAMBEData& data)
{
	CHeaderData& header = data.getHeader();
	unsigned int seqNo = data.getSeq();
	unsigned int id = data.getId();

	if (m_state != CS_CONNECTED && m_state != CS_ACTIVE)
		return;

	// This is a resolved DTMF code
	if (m_state == CS_ACTIVE && m_yourCall.Left(1U).IsSameAs(wxT("*")))
		m_yourCall = header.getMyCall1();

	// This is a new incoming CCS call
	if (m_state == CS_CONNECTED) {
		m_yourCall = header.getMyCall1();
		m_local    = header.getYourCall();
		m_state = CS_ACTIVE;
		m_inactivityTimer.start();
	}

	m_inactivityTimer.reset();

	if (m_id != id) {
		// Write to Header.log if it's enabled
		if (m_headerLogger != NULL)
			m_headerLogger->write(wxT("CCS"), header);

		bool busy = !m_handler->process(header, DIR_INCOMING, AS_CCS);
		if (busy)
			m_protocol.writeBusy(wxT("We're busy!"), m_ccsAddress, CCS_PORT);

		m_id = id;
	} else if (seqNo == 0U) {
		bool busy = !m_handler->process(header, DIR_INCOMING, AS_DUP);
		if (busy)
			m_protocol.writeBusy(wxT("We're busy!"), m_ccsAddress, CCS_PORT);
	}

	bool busy = !m_handler->process(data, DIR_INCOMING, AS_CCS);
	if (busy)
		m_protocol.writeBusy(wxT("We're busy!"), m_ccsAddress, CCS_PORT);
}

void CCCSHandler::process(CCCSData& data)
{
	CC_TYPE type = data.getType();

	if (type == CT_TERMINATE) {
		wxLogMessage(wxT("CCS link between %s and %s has been terminated"), data.getLocal().c_str(), data.getRemote().c_str());
		m_state = CS_CONNECTED;
		m_inactivityTimer.stop();
	}
}

void CCCSHandler::process(CPollData& poll)
{
	m_pollInactivityTimer.reset();
}

void CCCSHandler::process(CConnectData& connect)
{
	CD_TYPE type = connect.getType();

	if (type == CT_ACK && m_state == CS_CONNECTING) {
		m_pollInactivityTimer.start();
		m_pollTimer.start();
		m_tryTimer.stop();
		m_state = CS_CONNECTED;
		return;
	}

	if (type == CT_NAK && m_state == CS_CONNECTING) {
		wxLogMessage(wxT("Connection refused by CCS for %s"), m_callsign.c_str());
		m_tryTimer.stop();
		m_state = CS_DISABLED;
		return;
	}
}

bool CCCSHandler::connect()
{
	// Is CCS disabled?
	if (m_localAddress.IsSameAs(wxT("127.0.0.1")))
		return false;

	// Can we resolve the CCS server address?
	if (m_ccsAddress.s_addr == INADDR_NONE)
		return false;

	bool res = m_protocol.open();
	if (!res)
		return false;

	CConnectData connect(m_callsign, CT_LINK1, m_ccsAddress, CCS_PORT);
	if (!m_locator.IsEmpty())
		connect.setLocator(m_locator);
	m_protocol.writeConnect(connect);

	m_tryTimer.setTimeout(1U);
	m_tryTimer.start();

	m_tryCount = 1U;

	m_state = CS_CONNECTING;

	return true;
}

void CCCSHandler::disconnectInt()
{
	if (m_state == CS_CONNECTED || m_state == CS_ACTIVE) {
		CConnectData connect(m_callsign, CT_UNLINK, m_ccsAddress, CCS_PORT);
		m_protocol.writeConnect(connect);
	}

	m_pollInactivityTimer.stop();
	m_inactivityTimer.stop();
	m_pollTimer.stop();
	m_tryTimer.stop();

	m_state = CS_DISABLED;

	if (m_state != CS_DISABLED)
		m_protocol.close();
}

void CCCSHandler::writeEnd()
{
	if (m_state != CS_ACTIVE)
		return;

	CCCSData data(m_local, m_yourCall, CT_TERMINATE);
	data.setDestination(m_ccsAddress, CCS_PORT);
	m_protocol.writeMisc(data);

	m_state = CS_CONNECTED;
	m_inactivityTimer.stop();
}

void CCCSHandler::writeHeard(const CHeaderData& data)
{
	if (m_state != CS_CONNECTED && m_state != CS_ACTIVE)
		return;

	CHeardData heard(data, m_callsign, m_reflector);
	heard.setDestination(m_ccsAddress, CCS_PORT);
	m_protocol.writeHeard(heard);
}

void CCCSHandler::writeHeader(CHeaderData& header)
{
	m_myCall1 = header.getMyCall1();
	m_myCall2 = header.getMyCall2();

	m_seqNo = 0U;
}

void CCCSHandler::writeAMBE(CAMBEData& data, const wxString& dtmf)
{
	if (m_state != CS_CONNECTED && m_state != CS_ACTIVE)
		return;

	if (m_state == CS_CONNECTED) {
		if (!dtmf.Left(1U).IsSameAs(wxT("*")))
			return;

		m_local    = m_myCall1;
		m_yourCall = dtmf;
		m_seqNo = 0U;

		m_state = CS_ACTIVE;
		m_inactivityTimer.start();
	}

	CHeaderData& header = data.getHeader();
	header.setMyCall1(m_myCall1);
	header.setMyCall2(m_myCall2);
	header.setYourCall(m_yourCall);
	header.setRptCall1(m_callsign);
	header.setRptCall2(m_reflector);

	data.setRptSeq(m_seqNo++);
	data.setDestination(m_ccsAddress, CCS_PORT);
	m_protocol.writeData(data);
}

void CCCSHandler::clockInt(unsigned int ms)
{
	m_pollInactivityTimer.clock(ms);
	m_inactivityTimer.clock(ms);
	m_pollTimer.clock(ms);
	m_tryTimer.clock(ms);

	if (m_pollInactivityTimer.isRunning() && m_pollInactivityTimer.hasExpired()) {
		wxLogMessage(wxT("CCS connection has failed (poll inactivity) for %s, reconnecting"), m_callsign.c_str());

		m_pollInactivityTimer.stop();
		m_inactivityTimer.stop();
		m_pollTimer.stop();

		m_tryTimer.setTimeout(1U);
		m_tryTimer.start();

		m_tryCount = 1U;

		m_state = CS_CONNECTING;
		return;
	}

	if (m_tryTimer.isRunning() && m_tryTimer.hasExpired()) {
		CConnectData connect(m_callsign, CT_LINK1, m_ccsAddress, CCS_PORT);
		m_protocol.writeConnect(connect);

		unsigned int t = calcBackoff();
		m_tryTimer.setTimeout(t);
		m_tryTimer.reset();
	}

	if (m_pollTimer.isRunning() && m_pollTimer.hasExpired()) {
		CPollData poll(m_callsign, m_ccsAddress, CCS_PORT);
		m_protocol.writePoll(poll);

		m_pollTimer.reset();
	}

	if (m_inactivityTimer.isRunning() && m_inactivityTimer.hasExpired()) {
		wxLogMessage(wxT("Activity timeout on CCS link for %s"), m_callsign.c_str());
		m_state = CS_CONNECTED;
		m_inactivityTimer.stop();
	}
}

unsigned int CCCSHandler::calcBackoff()
{
	unsigned int timeout = 1U;

	for (unsigned int i = 0U; i < m_tryCount; i++)
		timeout *= 2U;

	m_tryCount++;

	if (timeout > 60U)
		return 60U;
	else
		return timeout;
}

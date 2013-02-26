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

wxString                 CCCSHandler::m_localAddress;
CHeaderLogger*           CCCSHandler::m_headerLogger = NULL;

wxString                 CCCSHandler::m_ccsHost;

CCCSCache_t              CCCSHandler::m_cache;
wxMutex                  CCCSHandler::m_mutex;


void CCCSHandler::initialise(unsigned int count)
{
	wxASSERT(count > 0U);

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

void CCCSHandler::setHost(const wxString& host)
{
	m_ccsHost = host;
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

CCCSHandler::CCCSHandler(ICCSCallback* handler, const wxString& callsign, unsigned int delay, double latitude, double longitude, unsigned int localPort) :
m_handler(handler),
m_callsign(callsign),
m_reflector(),
m_locator(),
m_ccsAddress(),
m_protocol(localPort, m_localAddress),
m_state(CS_DISABLED),
m_local(),
m_announceTimer(1000U, 20U),			// 20 seconds
m_inactivityTimer(1000U, 300U),			// 5 minutes
m_pollInactivityTimer(1000U, 60U),		// 60 seconds
m_pollTimer(1000U, 10U),				// 10 seconds
m_waitTimer(1000U, delay),
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

	// This is a new incoming CCS call
	if (m_state == CS_CONNECTED) {
		m_yourCall = header.getMyCall1();
		m_local    = header.getYourCall();
		m_state    = CS_ACTIVE;
		m_inactivityTimer.start();

		m_handler->ccsLinkMade(m_yourCall);

		wxLogMessage(wxT("CCS: New incoming link to %s from %s"), m_local.c_str(), m_yourCall.c_str());
	}

	m_pollInactivityTimer.reset();
	m_inactivityTimer.reset();

	if (m_id != id) {
		// Write to Header.log if it's enabled
		if (m_headerLogger != NULL)
			m_headerLogger->write(wxT("CCS"), header);

		header.setCQCQCQ();
		m_handler->process(header, DIR_INCOMING, AS_CCS);

		m_id = id;
	} else if (seqNo == 0U) {
		header.setCQCQCQ();
		m_handler->process(header, DIR_INCOMING, AS_DUP);
	}

	m_handler->process(data, DIR_INCOMING, AS_CCS);
}

void CCCSHandler::process(CCCSData& data)
{
	CC_TYPE type = data.getType();

	switch (type) {
		case CT_TERMINATE:
			if (m_state == CS_ACTIVE) {
				wxLogMessage(wxT("CCS: Link between %s and %s has been terminated"), data.getLocal().c_str(), data.getRemote().c_str());
				m_state = CS_CONNECTED;
				m_inactivityTimer.stop();
				m_handler->ccsLinkEnded(data.getRemote());
			}
			break;

		case CT_DTMFNOTFOUND:
			wxLogMessage(wxT("CCS: Cannot map %s to a callsign"), m_yourCall.Mid(1U).c_str());
			m_state = CS_CONNECTED;
			m_inactivityTimer.stop();
			m_handler->ccsLinkFailed(m_yourCall.Mid(1U));
			break;

		case CT_DTMFFOUND:
			wxLogMessage(wxT("CCS: Mapped %s to %s, added to the cache"), m_yourCall.Mid(1U).c_str(), data.getRemote().c_str());
			addToCache(m_yourCall.Mid(1U), data.getRemote());
			m_yourCall = data.getRemote();
			m_handler->ccsLinkMade(m_yourCall);
			break;

		default:
			break;
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
		wxLogMessage(wxT("CCS: %s connected to server %s"), m_callsign.c_str(), m_ccsHost.c_str());

		m_announceTimer.start();
		m_pollInactivityTimer.start();
		m_pollTimer.start();
		m_tryTimer.stop();

		m_state = CS_CONNECTED;

		return;
	}

	if (type == CT_NAK && m_state == CS_CONNECTING) {
		wxLogMessage(wxT("CCS: Connection refused for %s"), m_callsign.c_str());
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
	m_ccsAddress = CUDPReaderWriter::lookup(m_ccsHost);
	if (m_ccsAddress.s_addr == INADDR_NONE) {
		wxLogError(wxT("CCS: Unable to find the IP address for %s"), m_ccsHost.c_str());
		return false;
	}

	bool res = m_protocol.open();
	if (!res)
		return false;

	wxLogMessage(wxT("CCS: Opening UDP port %u for %s"), m_protocol.getPort(), m_callsign.c_str());

	m_waitTimer.start();

	m_state = CS_CONNECTING;

	return true;
}

void CCCSHandler::disconnectInt()
{
	if (m_state == CS_CONNECTED || m_state == CS_ACTIVE) {
		CConnectData connect(m_callsign, CT_UNLINK, m_ccsAddress, CCS_PORT);
		m_protocol.writeConnect(connect);
	}

	m_announceTimer.stop();
	m_pollInactivityTimer.stop();
	m_inactivityTimer.stop();
	m_pollTimer.stop();
	m_tryTimer.stop();

	if (m_state != CS_DISABLED)
		m_protocol.close();

	m_state = CS_DISABLED;
}

void CCCSHandler::writeEnd()
{
	if (m_state != CS_ACTIVE)
		return;

	wxLogMessage(wxT("CCS: Link to %s from %s has been terminated locally"), m_yourCall.c_str(), m_local.c_str());

	CCCSData data(m_local, m_yourCall, CT_TERMINATE);
	data.setDestination(m_ccsAddress, CCS_PORT);

	m_protocol.writeMisc(data);
	m_protocol.writeMisc(data);
	m_protocol.writeMisc(data);
	m_protocol.writeMisc(data);
	m_protocol.writeMisc(data);

	m_state = CS_CONNECTED;
	m_inactivityTimer.stop();

	m_handler->ccsLinkEnded(m_yourCall);
}

void CCCSHandler::writeHeard(CHeaderData& header)
{
	if (m_state != CS_CONNECTED && m_state != CS_ACTIVE)
		return;

	CHeardData heard(header, m_callsign, m_reflector);
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

		wxString callsign = findInCache(dtmf.Mid(1U));
		if (!callsign.IsEmpty()) {
			wxLogMessage(wxT("CCS: New outgoing link to %s/%s from %s"), dtmf.Mid(1U).c_str(), callsign.c_str(), m_myCall1.c_str());
			m_handler->ccsLinkMade(callsign);
			m_yourCall = callsign;
		} else {
			wxLogMessage(wxT("CCS: New outgoing link to %s from %s"), dtmf.Mid(1U).c_str(), m_myCall1.c_str());
			m_yourCall = dtmf;
		}

		m_local = m_myCall1;
		m_seqNo = 0U;

		m_state = CS_ACTIVE;
		m_inactivityTimer.start();

	}

	CAMBEData temp(data);

	CHeaderData& header = temp.getHeader();
	header.setMyCall1(m_myCall1);
	header.setMyCall2(m_myCall2);
	header.setYourCall(m_yourCall);
	header.setRptCall1(m_callsign);
	header.setRptCall2(m_reflector);

	temp.setRptSeq(m_seqNo++);
	temp.setDestination(m_ccsAddress, CCS_PORT);
	m_protocol.writeData(temp);
}

CCS_STATUS CCCSHandler::getStatus() const
{
	return m_state;
}

void CCCSHandler::clockInt(unsigned int ms)
{
	m_announceTimer.clock(ms);
	m_pollInactivityTimer.clock(ms);
	m_inactivityTimer.clock(ms);
	m_pollTimer.clock(ms);
	m_waitTimer.clock(ms);
	m_tryTimer.clock(ms);

	if (m_pollInactivityTimer.isRunning() && m_pollInactivityTimer.hasExpired()) {
		wxLogMessage(wxT("CCS: Connection has failed (poll inactivity) for %s, reconnecting"), m_callsign.c_str());

		m_announceTimer.stop();
		m_pollInactivityTimer.stop();
		m_inactivityTimer.stop();
		m_pollTimer.stop();

		if (m_state == CS_ACTIVE)
			m_handler->ccsLinkEnded(m_yourCall);

		m_waitTimer.start();

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
		wxLogMessage(wxT("CCS: Activity timeout on link for %s"), m_callsign.c_str());
		m_state = CS_CONNECTED;
		m_inactivityTimer.stop();

		m_handler->ccsLinkEnded(m_yourCall);
	}

	if (m_waitTimer.isRunning() && m_waitTimer.hasExpired()) {
		CConnectData connect(m_callsign, CT_LINK1, m_ccsAddress, CCS_PORT);
		if (!m_locator.IsEmpty())
			connect.setLocator(m_locator);
		m_protocol.writeConnect(connect);

		m_tryTimer.setTimeout(1U);
		m_tryTimer.start();

		m_tryCount = 1U;

		m_waitTimer.stop();
	}

	if (m_announceTimer.isRunning() && m_announceTimer.hasExpired()) {
		CHeaderData header;
		header.setMyCall1(m_callsign.Left(LONG_CALLSIGN_LENGTH - 1U));
		CHeardData heard(header, m_callsign, wxEmptyString);
		heard.setDestination(m_ccsAddress, CCS_PORT);
		m_protocol.writeHeard(heard);

		m_announceTimer.setTimeout(3600U);
		m_announceTimer.reset();
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

void CCCSHandler::addToCache(const wxString& dtmf, const wxString& callsign)
{
	wxMutexLocker locker(m_mutex);

	m_cache[dtmf] = callsign;
}

wxString CCCSHandler::findInCache(const wxString& dtmf)
{
	wxMutexLocker locker(m_mutex);

	return m_cache[dtmf];
}

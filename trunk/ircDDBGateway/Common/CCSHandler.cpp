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

CCS_STATUS               CCCSHandler::m_state = CS_DISABLED;

wxString                 CCCSHandler::m_callsign = wxEmptyString;

CCCSProtocolHandler*     CCCSHandler::m_handler = NULL;

in_addr                  CCCSHandler::m_address;

CHeaderLogger*           CCCSHandler::m_headerLogger = NULL;

CTimer                   CCCSHandler::m_pollInactivityTimer(1000U, 60U);
CTimer                   CCCSHandler::m_pollTimer(1000U, 10U);
CTimer                   CCCSHandler::m_tryTimer(1000U, 1U);
unsigned int             CCCSHandler::m_tryCount = 0U;

void CCCSHandler::initialise()
{
}

void CCCSHandler::setCCSProtocolHandler(CCCSProtocolHandler* handler)
{
	wxASSERT(handler != NULL);

	m_handler = handler;
}

void CCCSHandler::setHeaderLogger(CHeaderLogger* logger)
{
	m_headerLogger = logger;
}

void CCCSHandler::setCallsign(const wxString& callsign)
{
	m_callsign = callsign;
}

void CCCSHandler::process(CAMBEData& data)
{
}

void CCCSHandler::process(CPollData& poll)
{
	m_pollInactivityTimer.reset();
}

void CCCSHandler::process(CConnectData& connect)
{
	CD_TYPE type = connect.getType();

	if (type == CT_ACK && m_state == CS_CONNECTING) {
		m_pollInactivityTimer.reset();
		m_pollTimer.start();
		m_tryTimer.stop();
		m_state = CS_CONNECTED;
		return;
	}

	if (type == CT_NAK && m_state == CS_CONNECTING) {
		m_pollInactivityTimer.stop();
		m_tryTimer.stop();
		m_state = CS_DISABLED;
		return;
	}
}

bool CCCSHandler::link()
{
	m_address = CUDPReaderWriter::lookup(CCS_HOSTNAME);

	CConnectData connect(m_callsign, CT_LINK1, m_address, CCS_PORT);
	m_handler->writeConnect(connect);

	wxLogMessage(wxT("Linking to CCS on host %s"), CCS_HOSTNAME.c_str());

	m_pollInactivityTimer.start();

	m_tryTimer.setTimeout(1U);
	m_tryTimer.start();

	m_tryCount = 1U;

	m_state = CS_CONNECTING;

	return true;
}

void CCCSHandler::unlink()
{
	CConnectData connect(m_callsign, CT_UNLINK, m_address, CCS_PORT);
	m_handler->writeConnect(connect);

	m_pollInactivityTimer.stop();
	m_pollTimer.stop();

	m_state = CS_DISABLED;

	wxLogMessage(wxT("Unlinking from CCS"));
}

void CCCSHandler::writeHeader(IReflectorCallback* handler, CHeaderData& header, DIRECTION direction)
{
}

void CCCSHandler::writeAMBE(IReflectorCallback* handler, CAMBEData& data, DIRECTION direction)
{
}

void CCCSHandler::clock(unsigned int ms)
{
	m_pollInactivityTimer.clock(ms);
	m_pollTimer.clock(ms);
	m_tryTimer.clock(ms);

	if (m_pollInactivityTimer.isRunning() && m_pollInactivityTimer.hasExpired()) {
		switch (m_state) {
			case CS_CONNECTING:
				wxLogMessage(wxT("CCS has failed to connect"));
				m_tryTimer.stop();
				break;
			case CS_CONNECTED:
				wxLogMessage(wxT("CCS connection has failed (poll inactivity)"));
				m_pollTimer.stop();
				break;
			default:
				break;
		}

		m_pollInactivityTimer.stop();
		m_state = CS_DISABLED;
		return;
	}

	if (m_tryTimer.isRunning() && m_tryTimer.hasExpired()) {
		CConnectData connect(m_callsign, CT_LINK1, m_address, CCS_PORT);
		m_handler->writeConnect(connect);

		unsigned int t = calcBackoff();
		m_tryTimer.setTimeout(t);
		m_tryTimer.reset();
	}

	if (m_pollTimer.isRunning() && m_pollTimer.hasExpired()) {
		CPollData poll(m_callsign, m_address, CCS_PORT);
		m_handler->writePoll(poll);

		m_pollTimer.reset();
	}
}

void CCCSHandler::finalise()
{
}

CCS_STATUS CCCSHandler::getState()
{
	return m_state;
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

/*
 *   Copyright (C) 2012 by Jonathan Naylor G4KLX
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
#include "DCSHandler.h"
#include "Utils.h"

unsigned int         CDCSHandler::m_maxReflectors = 0U;
CDCSHandler**        CDCSHandler::m_reflectors = NULL;

wxString             CDCSHandler::m_callsign;
CDCSProtocolHandler* CDCSHandler::m_handler = NULL;

bool                 CDCSHandler::m_stateChange = false;

CHeaderLogger*       CDCSHandler::m_headerLogger = NULL;


CDCSHandler::CDCSHandler(IReflectorCallback* handler, const wxString& reflector, const wxString& repeater, const in_addr& address, unsigned int port, DIRECTION direction) :
m_reflector(reflector),
m_repeater(repeater),
m_address(address),
m_port(port),
m_direction(direction),
m_linkState(DCS_LINKING),
m_destination(handler),
m_time(),
m_pollTimer(1000U, 5U),
m_pollInactivityTimer(1000U, 60U),
m_tryTimer(1000U, 5U),
m_dcsId(0x00U),
m_dcsSeq(0x00U),
m_rptrId(0x00U),
m_seqNo(0x00U),
m_inactivityTimer(1000U, 2U),
m_yourCall(),
m_myCall1(),
m_myCall2(),
m_rptCall1(),
m_rptCall2()
{
	wxASSERT(handler != NULL);
	wxASSERT(port > 0U);

	m_pollInactivityTimer.start();

	m_time = ::time(NULL);

	if (direction == DIR_INCOMING) {
		m_pollTimer.start();
		m_stateChange = true;
		m_linkState = DCS_LINKED;
	} else {
		m_linkState = DCS_LINKING;
		m_tryTimer.start();
	}
}

CDCSHandler::CDCSHandler(const wxString& reflector, const in_addr& address, unsigned int port, DIRECTION direction) :
m_reflector(reflector),
m_repeater(),
m_address(address),
m_port(port),
m_direction(direction),
m_linkState(DCS_LINKING),
m_destination(NULL),
m_time(),
m_pollTimer(1000U, 5U),
m_pollInactivityTimer(1000U, 60U),
m_tryTimer(1000U, 1U),
m_dcsId(0x00U),
m_dcsSeq(0x00U),
m_rptrId(0x00U),
m_seqNo(0x00U),
m_inactivityTimer(1000U, 2U),
m_yourCall(),
m_myCall1(),
m_myCall2(),
m_rptCall1(),
m_rptCall2()
{
	wxASSERT(port > 0U);

	m_pollInactivityTimer.start();

	m_time = ::time(NULL);

	if (direction == DIR_INCOMING) {
		m_pollTimer.start();
		m_stateChange = true;
		m_linkState = DCS_LINKED;
	} else {
		m_linkState = DCS_LINKING;
		m_tryTimer.start();
	}
}

CDCSHandler::~CDCSHandler()
{
}

void CDCSHandler::initialise(unsigned int maxReflectors)
{
	wxASSERT(maxReflectors > 0U);

	m_maxReflectors = maxReflectors;

	m_reflectors = new CDCSHandler*[m_maxReflectors];
	for (unsigned int i = 0U; i < m_maxReflectors; i++)
		m_reflectors[i] = NULL;
}

void CDCSHandler::setCallsign(const wxString& callsign)
{
	m_callsign = callsign;
	m_callsign.SetChar(LONG_CALLSIGN_LENGTH - 1U, wxT(' '));
}

void CDCSHandler::setDCSProtocolHandler(CDCSProtocolHandler* handler)
{
	wxASSERT(handler != NULL);

	m_handler = handler;
}

void CDCSHandler::setHeaderLogger(CHeaderLogger* logger)
{
	m_headerLogger = logger;
}

wxString CDCSHandler::getIncoming(const wxString& callsign)
{
	wxString incoming;

	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		CDCSHandler* reflector = m_reflectors[i];
		if (reflector != NULL && reflector->m_direction == DIR_INCOMING && reflector->m_repeater.IsSameAs(callsign)) {
			incoming.Append(reflector->m_reflector);
			incoming.Append(wxT("  "));
		}
	}

	return incoming;
}

void CDCSHandler::getInfo(IReflectorCallback* handler, CRemoteRepeaterData& data)
{
	wxASSERT(handler != NULL);

	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		CDCSHandler* reflector = m_reflectors[i];
		if (reflector != NULL) {
			if (reflector->m_destination == handler) {
				if (reflector->m_direction == DIR_INCOMING && reflector->m_repeater.IsEmpty())
					data.addLink(reflector->m_reflector, PROTO_DCS, reflector->m_linkState == DCS_LINKED, DIR_INCOMING, true);
				else
					data.addLink(reflector->m_reflector, PROTO_DCS, reflector->m_linkState == DCS_LINKED, reflector->m_direction, false);
			}
		}
	}
}

void CDCSHandler::process(CAMBEData& data)
{
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		CDCSHandler* reflector = m_reflectors[i];
		if (reflector != NULL) {
			if (reflector->m_address.s_addr == data.getAddress().s_addr &&
				reflector->m_port           == data.getPort())
				reflector->processInt(data);
		}
	}	
}

void CDCSHandler::process(CPollData& poll)
{
	wxString reflector = poll.getData1();
	in_addr  address   = poll.getAddress();
	unsigned int port  = poll.getPort();

	bool found = false;

	// Check to see if we already have a link
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		CDCSHandler* handler = m_reflectors[i];

		if (handler != NULL) {
			if (handler->m_reflector.Left(LONG_CALLSIGN_LENGTH - 1U).IsSameAs(reflector.Left(LONG_CALLSIGN_LENGTH - 1U)) &&
				handler->m_address.s_addr == address.s_addr && handler->m_port == port && handler->m_linkState == DCS_LINKED) {
				handler->m_pollInactivityTimer.reset();
				found = true;

				if (handler->m_direction == DIR_OUTGOING) {
					wxString data = poll.getData1();
					poll.setData1(handler->m_callsign);
					poll.setData2(data);
					m_handler->writePoll(poll);
				}
			}
		}
	}

	if (!found)
		wxLogMessage(wxT("Incoming DCS poll from %s"), reflector.c_str());
}

void CDCSHandler::process(CConnectData& connect)
{
	CD_TYPE type = connect.getType();

	if (type == CT_ACK || type == CT_NAK || type == CT_UNLINK) {
		for (unsigned int i = 0U; i < m_maxReflectors; i++) {
			if (m_reflectors[i] != NULL) {
				bool res = m_reflectors[i]->processInt(connect, type);
				if (res) {
					delete m_reflectors[i];
					m_reflectors[i] = NULL;
				}
			}
		}

		return;
	}

	// else if type == CT_LINK1 or type == CT_LINK2
	in_addr   address = connect.getAddress();
	unsigned int port = connect.getPort();

	wxString repeaterCallsign = connect.getRepeater();
	wxString reflectorCallsign = connect.getReflector();

	// Check that it isn't a duplicate
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		if (m_reflectors[i] != NULL) {
			if (m_reflectors[i]->m_direction      == DIR_INCOMING &&
			    m_reflectors[i]->m_address.s_addr == address.s_addr &&
			    m_reflectors[i]->m_port           == port &&
			    m_reflectors[i]->m_repeater.IsSameAs(reflectorCallsign) &&
			    m_reflectors[i]->m_reflector.IsSameAs(repeaterCallsign))
				return;
		}
	}

	// Check the validity of our repeater callsign
	IReflectorCallback* handler = CRepeaterHandler::findDVRepeater(reflectorCallsign);
	if (handler == NULL) {
		wxLogMessage(wxT("DCS connect to unknown reflector %s from %s"), reflectorCallsign.c_str(), repeaterCallsign.c_str());
		CConnectData reply(repeaterCallsign, reflectorCallsign, CT_NAK, connect.getAddress(), connect.getPort());
		m_handler->writeConnect(reply);
		return;
	}

	// A new connect packet indicates the need for a new entry
	wxLogMessage(wxT("New incoming DCS link to %s from %s"), reflectorCallsign.c_str(), repeaterCallsign.c_str());

	CDCSHandler* dextra = new CDCSHandler(handler, repeaterCallsign, reflectorCallsign, address, port, DIR_INCOMING);

	bool found = false;
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		if (m_reflectors[i] == NULL) {
			m_reflectors[i] = dextra;
			found = true;
			break;
		}
	}

	if (found) {
		CConnectData reply(repeaterCallsign, reflectorCallsign, CT_ACK, address, port);
		m_handler->writeConnect(reply);
	} else {
		CConnectData reply(repeaterCallsign, reflectorCallsign, CT_NAK, address, port);
		m_handler->writeConnect(reply);

		wxLogError(wxT("No space to add new DCS link, ignoring"));
		delete dextra;
	}
}

void CDCSHandler::link(IReflectorCallback* handler, const wxString& repeater, const wxString &gateway, const in_addr& address)
{
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		if (m_reflectors[i] != NULL) {
			if (m_reflectors[i]->m_direction == DIR_OUTGOING && m_reflectors[i]->m_destination == handler && m_reflectors[i]->m_linkState != DCS_UNLINKING)
				return;
		}
	}	

	CDCSHandler* dcs = new CDCSHandler(handler, gateway, repeater, address, DCS_PORT, DIR_OUTGOING);

	bool found = false;

	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		if (m_reflectors[i] == NULL) {
			m_reflectors[i] = dcs;
			found = true;
			break;
		}
	}

	if (found) {
		CConnectData reply(repeater, gateway, CT_LINK1, address, DCS_PORT);
		m_handler->writeConnect(reply);
	} else {
		wxLogError(wxT("No space to add new DCS link, ignoring"));
		delete dcs;
	}
}

void CDCSHandler::unlink(IReflectorCallback* handler, const wxString& exclude)
{
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		CDCSHandler* reflector = m_reflectors[i];

		if (reflector != NULL) {
			if (reflector->m_direction == DIR_OUTGOING && reflector->m_destination == handler && !reflector->m_reflector.IsSameAs(exclude)) {
				wxLogMessage(wxT("Removing outgoing DCS link %s, %s"), reflector->m_repeater.c_str(), reflector->m_reflector.c_str());

				if (reflector->m_linkState == DCS_LINKING || reflector->m_linkState == DCS_LINKED) {
					CConnectData connect(reflector->m_repeater, reflector->m_reflector, CT_UNLINK, reflector->m_address, reflector->m_port);
					m_handler->writeConnect(connect);

					reflector->m_linkState = DCS_UNLINKING;
					reflector->m_tryTimer.start();
				}

				// If an active link with incoming traffic, send an EOT to the repeater
				if (reflector->m_dcsId != 0x00U) {
					unsigned int seq = reflector->m_dcsSeq + 1U;
					if (seq == 21U)
						seq = 0U;

					CAMBEData data;
					data.setData(END_PATTERN_BYTES, DV_FRAME_LENGTH_BYTES);
					data.setSeq(seq);
					data.setEnd(true);
					data.setId(reflector->m_dcsId);

					reflector->m_destination->process(data, AS_DCS);
				}

				m_stateChange = true;
			}
		}
	}	
}

void CDCSHandler::unlink()
{
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		CDCSHandler* reflector = m_reflectors[i];

		if (reflector != NULL) {
			if (!reflector->m_repeater.IsEmpty()) {
				wxLogMessage(wxT("Unlinking from DCS reflector %s"), reflector->m_reflector.c_str());

				CConnectData connect(reflector->m_repeater, reflector->m_reflector, CT_UNLINK, reflector->m_address, reflector->m_port);
				m_handler->writeConnect(connect);

				reflector->m_linkState = DCS_UNLINKING;
				reflector->m_tryTimer.start();
			}
		}
	}	
}

void CDCSHandler::writeHeader(const wxString& callsign, CHeaderData& header, DIRECTION direction)
{
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		if (m_reflectors[i] != NULL)
			m_reflectors[i]->writeHeaderInt(callsign, header, direction);
	}	
}

void CDCSHandler::writeAMBE(CAMBEData& data, DIRECTION direction)
{
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		if (m_reflectors[i] != NULL)
			m_reflectors[i]->writeAMBEInt(data, direction);
	}	
}

void CDCSHandler::gatewayUpdate(const wxString& reflector, const wxString& address)
{
	wxString gateway = reflector;
	gateway.Truncate(LONG_CALLSIGN_LENGTH - 1U);

	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		CDCSHandler* reflector = m_reflectors[i];
		if (reflector != NULL) {
			if (reflector->m_reflector.Left(LONG_CALLSIGN_LENGTH - 1U).IsSameAs(gateway)) {
				if (!address.IsEmpty()) {
					// A new address, change the value
					wxLogMessage(wxT("Changing IP address of DCS gateway or reflector %s to %s"), reflector->m_reflector.c_str(), address.c_str());
					reflector->m_address.s_addr = ::inet_addr(address.mb_str());
				} else {
					wxLogMessage(wxT("IP address for DCS gateway or reflector %s has been removed"), reflector->m_reflector.c_str());

					// No address, this probably shouldn't happen....
					if (reflector->m_direction == DIR_OUTGOING && reflector->m_destination != NULL)
						reflector->m_destination->linkDown(DP_DCS, reflector->m_reflector, false);

					m_stateChange = true;

					delete m_reflectors[i];
					m_reflectors[i] = NULL;
				}
			}
		}
	}
}

void CDCSHandler::clock(unsigned int ms)
{
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		if (m_reflectors[i] != NULL) {
			bool ret = m_reflectors[i]->clockInt(ms);
			if (ret) {
				delete m_reflectors[i];
				m_reflectors[i] = NULL;
			}
		}
	}
}

void CDCSHandler::finalise()
{
	for (unsigned int i = 0U; i < m_maxReflectors; i++)
		delete m_reflectors[i];

	delete[] m_reflectors;
}

void CDCSHandler::processInt(CAMBEData& data)
{
	unsigned int id = data.getId();
	CHeaderData& header = data.getHeader();
	unsigned int seqNo = data.getSeq();

	if (m_linkState != DCS_LINKED)
		return;

	switch (m_direction) {
		case DIR_OUTGOING:
			if (!m_reflector.IsSameAs(header.getRptCall2()))
				return;

			if (m_dcsId == 0x00U && seqNo != 0U)
				return;

			if (m_dcsId == 0x00U) {		// && seqNo == 0U) {
				// Write to Header.log if it's enabled
				if (m_headerLogger != NULL)
					m_headerLogger->write(wxT("DCS"), header);

				m_dcsId  = id;
				m_dcsSeq = 0x00U;
				m_inactivityTimer.start();

				header.setCQCQCQ();
				header.setFlags(0x00U, 0x00U, 0x00U);

				m_destination->process(header, AS_DCS);
			}
			
			if (id == m_dcsId) {
				m_inactivityTimer.reset();

				m_dcsSeq = seqNo;

				if (m_dcsSeq == 0U) {
					// Send the header every 21 frames
					header.setCQCQCQ();
					header.setFlags(0x00U, 0x00U, 0x00U);

					m_destination->process(header, AS_DUP);
				}

				m_destination->process(data, AS_DCS);

				if (data.isEnd()) {
					m_dcsId  = 0x00U;
					m_dcsSeq = 0x00U;
					m_inactivityTimer.stop();
				}
			}

			break;

		case DIR_INCOMING:
			if (!m_repeater.IsSameAs(header.getRptCall2()))
				return;

			if (m_dcsId == 0x00U && seqNo != 0U)
				return;

			if (m_dcsId == 0x00U) {		// && seqNo == 0U) {
				// Write to Header.log if it's enabled
				if (m_headerLogger != NULL)
					m_headerLogger->write(wxT("DCS"), header);

				m_dcsId  = id;
				m_dcsSeq = 0x00U;
				m_inactivityTimer.start();

				header.setCQCQCQ();
				header.setFlags(0x00U, 0x00U, 0x00U);

				m_destination->process(header, AS_DCS);
			}

			if (id == m_dcsId) {
				m_inactivityTimer.reset();

				m_dcsSeq = seqNo;

				if (m_dcsSeq == 0U) {
					// Send the header every 21 frames
					header.setCQCQCQ();
					header.setFlags(0x00U, 0x00U, 0x00U);

					m_destination->process(header, AS_DUP);
				}

				m_destination->process(data, AS_DCS);

				if (data.isEnd()) {
					m_dcsId  = 0x00U;
					m_dcsSeq = 0x00U;
					m_inactivityTimer.stop();
				}
			}

			break;
	}
}

bool CDCSHandler::processInt(CConnectData& connect, CD_TYPE type)
{
	in_addr address   = connect.getAddress();
	unsigned int port = connect.getPort();
	wxString repeater = connect.getRepeater();

	if (m_address.s_addr != address.s_addr || m_port != port)
		return false;

	switch (type) {
		case CT_ACK:
			if (!m_repeater.IsSameAs(repeater))
				return false;

			if (m_linkState == DCS_LINKING) {
				wxLogMessage(wxT("DCS ACK message received from %s"), m_reflector.c_str());

				if (m_direction == DIR_OUTGOING && m_destination != NULL)
					m_destination->linkUp(DP_DCS, m_reflector);

				m_tryTimer.stop();
				m_stateChange = true;
				m_linkState   = DCS_LINKED;
			}

			return false;

		case CT_NAK:
			if (!m_repeater.IsSameAs(repeater))
				return false;

			if (m_linkState == DCS_UNLINKING) {
				wxLogMessage(wxT("DCS NAK message received from %s"), m_reflector.c_str());

				if (m_direction == DIR_OUTGOING && m_destination != NULL)
					m_destination->linkDown(DP_DCS, m_reflector, false);

				return true;
			}

			return false;

		case CT_UNLINK:
			if (!m_reflector.IsSameAs(repeater))
				return false;

			if (m_linkState == DCS_LINKED) {
				wxLogMessage(wxT("DCS disconnect message received from %s"), m_reflector.c_str());

				if (m_direction == DIR_OUTGOING && m_destination != NULL)
					m_destination->linkDown(DP_DCS, m_reflector, false);

				m_stateChange = true;
			}

			return true;

		default:
			return false;
	}
}

bool CDCSHandler::clockInt(unsigned int ms)
{
	m_pollInactivityTimer.clock(ms);
	m_inactivityTimer.clock(ms);
	m_pollTimer.clock(ms);

	if (m_pollInactivityTimer.isRunning() && m_pollInactivityTimer.hasExpired()) {
		m_pollInactivityTimer.reset();

		m_stateChange = true;
		m_dcsId       = 0x00U;
		m_dcsSeq      = 0x00U;
		m_rptrId      = 0x00U;

		switch (m_linkState) {
			case DCS_LINKING:
				wxLogMessage(wxT("DCS link to %s has failed to connect"), m_reflector.c_str());
				break;
			case DCS_LINKED:
				wxLogMessage(wxT("DCS link to %s has failed (poll inactivity)"), m_reflector.c_str());
				break;
			case DCS_UNLINKING:
				wxLogMessage(wxT("DCS link to %s has failed to disconnect cleanly"), m_reflector.c_str());
				break;
			default:
				break;
		}

		if (m_direction == DIR_OUTGOING) {
			bool reconnect = m_destination->linkDown(DP_DCS, m_reflector, true);
			if (reconnect) {
				CConnectData reply(m_repeater, m_reflector, CT_LINK1, m_address, m_port);
				m_handler->writeConnect(reply);
				m_linkState = DCS_LINKING;
				m_tryTimer.start();
				return false;
			}
		}

		return true;
	}

	if (m_inactivityTimer.isRunning() && m_inactivityTimer.hasExpired()) {
		m_dcsId  = 0x00U;
		m_dcsSeq = 0x00U;
		m_inactivityTimer.stop();
	}

	if (m_pollTimer.isRunning() && m_pollTimer.hasExpired()) {
		m_pollTimer.reset();

		CPollData poll(m_repeater, wxEmptyString, m_address, m_port);
		m_handler->writePoll(poll);
	}

	if (m_linkState == DCS_LINKING) {
		m_tryTimer.clock(ms);

		if (m_tryTimer.hasExpired()) {
			CConnectData reply(m_repeater, m_reflector, CT_LINK1, m_address, m_port);
			m_handler->writeConnect(reply);

			m_tryTimer.reset();
		}
	}

	if (m_linkState == DCS_UNLINKING) {
		m_tryTimer.clock(ms);

		if (m_tryTimer.hasExpired()) {
			CConnectData connect(m_repeater, m_reflector, CT_UNLINK, m_address, m_port);
			m_handler->writeConnect(connect);

			m_tryTimer.reset();
		}
	}

	return false;
}

void CDCSHandler::writeHeaderInt(const wxString& callsign, CHeaderData& header, DIRECTION direction)
{
	if (m_linkState != DCS_LINKED)
		return;

	// Is it link in the right direction
	if (m_direction != direction)
		return;

	// Do the callsigns match?
	if (!callsign.IsSameAs(m_repeater))
		return;

	// Already in use?
	if (m_dcsId != 0x00)
		return;

	m_rptrId   = header.getId();
	m_seqNo    = 0U;

	m_myCall1  = header.getMyCall1();
	m_myCall2  = header.getMyCall2();
	m_yourCall = header.getYourCall();
	m_rptCall1 = header.getRptCall1();
	m_rptCall2 = header.getRptCall2();
}

void CDCSHandler::writeAMBEInt(CAMBEData& data, DIRECTION direction)
{
	if (m_linkState != DCS_LINKED)
		return;

	// Is it link in the right direction
	if (m_direction != direction)
		return;

	// If the ids don't match, reject
	if (data.getId() != m_rptrId)
		return;

	// Already in use?
	if (m_dcsId != 0x00)
		return;

	CHeaderData& header = data.getHeader();
	header.setMyCall1(m_myCall1);
	header.setMyCall2(m_myCall2);
	header.setRptCall1(m_rptCall1);
	header.setRptCall2(m_rptCall2);
	header.setCQCQCQ();

	data.setRptSeq(m_seqNo++);
	data.setDestination(m_address, m_port);
	m_handler->writeData(data);

	if (data.isEnd())
		m_rptrId = 0x00;
}

bool CDCSHandler::stateChange()
{
	bool stateChange = m_stateChange;

	m_stateChange = false;

	return stateChange;
}

void CDCSHandler::writeStatus(wxFFile& file)
{
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		CDCSHandler* reflector = m_reflectors[i];
		if (reflector != NULL) {
			struct tm* tm = ::gmtime(&reflector->m_time);

			switch (reflector->m_direction) {
				case DIR_OUTGOING:
					if (reflector->m_linkState == DCS_LINKED) {
						wxString text;
						text.Printf(wxT("%04d-%02d-%02d %02d:%02d:%02d: DCS link - Type: Repeater Rptr: %s Refl: %s Dir: Outgoing\n"),
							tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec, 
							reflector->m_repeater.c_str(), reflector->m_reflector.c_str());
						file.Write(text);
					}
					break;

				case DIR_INCOMING:
					if (reflector->m_linkState == DCS_LINKED) {
						wxString text;
						text.Printf(wxT("%04d-%02d-%02d %02d:%02d:%02d: DCS link - Type: Repeater Rptr: %s Refl: %s Dir: Incoming\n"),
							tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec, 
							reflector->m_repeater.c_str(), reflector->m_reflector.c_str());
						file.Write(text);
					}
					break;
			}
		}
	}
}

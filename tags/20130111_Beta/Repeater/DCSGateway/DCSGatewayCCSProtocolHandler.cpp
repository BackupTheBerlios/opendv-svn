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

#include "DCSGatewayCCSProtocolHandler.h"

#include "DStarDefines.h"
#include "Version.h"
#include "Utils.h"

// #define	DUMP_TX

const unsigned int BUFFER_LENGTH = 2000U;

CDCSGatewayCCSProtocolHandler::CDCSGatewayCCSProtocolHandler() :
m_socket(NULL),
m_callsign(NULL),
m_locator(NULL),
m_reflector(NULL),
m_remote(NULL),
m_type(CT_NONE),
m_buffer(NULL),
m_length(0U),
m_queryRemote(NULL)
{
	m_locator     = new unsigned char[LOCATOR_LENGTH];
	m_callsign    = new unsigned char[LONG_CALLSIGN_LENGTH];
	m_reflector   = new unsigned char[LONG_CALLSIGN_LENGTH];
	m_remote      = new unsigned char[LONG_CALLSIGN_LENGTH];
	m_queryRemote = new unsigned char[LONG_CALLSIGN_LENGTH];

	::memset(m_locator,     ' ', LOCATOR_LENGTH);
	::memset(m_callsign,    ' ', LONG_CALLSIGN_LENGTH);
	::memset(m_reflector,   ' ', LONG_CALLSIGN_LENGTH);
	::memset(m_remote,      ' ', LONG_CALLSIGN_LENGTH);
	::memset(m_queryRemote, ' ', LONG_CALLSIGN_LENGTH);

	m_buffer = new unsigned char[BUFFER_LENGTH];

	wxDateTime now = wxDateTime::Now();
	::srand(now.GetMillisecond());
}

CDCSGatewayCCSProtocolHandler::~CDCSGatewayCCSProtocolHandler()
{
	delete[] m_locator;
	delete[] m_callsign;
	delete[] m_reflector;
	delete[] m_remote;
	delete[] m_queryRemote;
	delete[] m_buffer;
}

void CDCSGatewayCCSProtocolHandler::setCallsign(const wxString& callsign)
{
	::memset(m_callsign, ' ', LONG_CALLSIGN_LENGTH);

	for (unsigned int i = 0U; i < callsign.Len(); i++)
		m_callsign[i] = callsign.GetChar(i);

	if (m_callsign[7U] == ' ')
		m_callsign[7U] = 'D';
}

void CDCSGatewayCCSProtocolHandler::setLocator(const wxString& locator)
{
	::memset(m_locator, ' ', LONG_CALLSIGN_LENGTH);

	for (unsigned int i = 0U; i < locator.Len(); i++)
		m_locator[i] = locator.GetChar(i);
}

void CDCSGatewayCCSProtocolHandler::setReflector(const wxString& reflector)
{
	::memset(m_reflector, ' ', LONG_CALLSIGN_LENGTH);

	for (unsigned int i = 0U; i < reflector.Len(); i++)
		m_reflector[i] = reflector.GetChar(i);
}

void CDCSGatewayCCSProtocolHandler::setRemote(const wxString& remote)
{
	::memset(m_remote, ' ', LONG_CALLSIGN_LENGTH);

	for (unsigned int i = 0U; i < remote.Len(); i++)
		m_remote[i] = remote.GetChar(i);
}

bool CDCSGatewayCCSProtocolHandler::registration(const wxString& address, unsigned int port)
{
	unsigned char buffer[39U];

	::memset(buffer, ' ', 39U);

	buffer[0U]  = m_callsign[0U];
	buffer[1U]  = m_callsign[1U];
	buffer[2U]  = m_callsign[2U];
	buffer[3U]  = m_callsign[3U];
	buffer[4U]  = m_callsign[4U];
	buffer[5U]  = m_callsign[5U];
	buffer[6U]  = m_callsign[6U];

	buffer[8U]  = m_callsign[7U];

	buffer[9U]  = 'A';

	buffer[10U] = '@';

	::memcpy(buffer + 11U, m_locator, LOCATOR_LENGTH);

	buffer[18U] = '@';

	wxString text;
	text.Printf(wxT("DCSGateway:%s"), VERSION.Left(8U).c_str());

	for (unsigned int i = 0U; i < text.Len(); i++)
		buffer[i + 19U] = text.GetChar(i);

	m_socket = new CUDPReaderWriter(CCS_HOST, CCS_PORT);
	bool ret = m_socket->open();
	if (!ret)
		return false;

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Registration"), buffer, 39U);
	return true;
#else
	return m_socket->write(buffer, 39U);
#endif
}

void CDCSGatewayCCSProtocolHandler::cancelation()
{
	if (m_socket == NULL)
		return;

	unsigned char buffer[19U];

	::memset(buffer, ' ', 19U);

	buffer[0U]  = m_callsign[0U];
	buffer[1U]  = m_callsign[1U];
	buffer[2U]  = m_callsign[2U];
	buffer[3U]  = m_callsign[3U];
	buffer[4U]  = m_callsign[4U];
	buffer[5U]  = m_callsign[5U];
	buffer[6U]  = m_callsign[6U];

	buffer[8U]  = m_callsign[7U];

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Cancelation"), buffer, 19U);
#else
	m_socket->write(buffer, 19U);
#endif

	m_socket->close();

	delete m_socket;
	m_socket = NULL;
}

bool CDCSGatewayCCSProtocolHandler::writeUserStatus(const CDCSGatewayHeaderData& header)
{
	unsigned char buffer[100U];

	unsigned int length = header.getCCSData(buffer, 100U);

	::memcpy(buffer + 7U, m_reflector, LONG_CALLSIGN_LENGTH);

	::memcpy(buffer + 15U, m_callsign, LONG_CALLSIGN_LENGTH);

// #if defined(DUMP_TX)
	CUtils::dump(wxT("Sending User Status"), buffer, length);
//	return true;
// #else
	return m_socket->write(buffer, length);
// #endif
}

bool CDCSGatewayCCSProtocolHandler::writeData(const CDCSGatewayHeaderData& header, const CDCSGatewayAMBEData& data)
{
	unsigned char buffer[100U];

	header.getCCSData(buffer, 100U);
	data.getCCSData(buffer, 100U);

	::memcpy(buffer + 7U, m_reflector, LONG_CALLSIGN_LENGTH);

	::memcpy(buffer + 15U, m_callsign, LONG_CALLSIGN_LENGTH);

// #if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Data"), buffer, 100U);
//	return true;
// #else
	return m_socket->write(buffer, 100U);
// #endif
}

bool CDCSGatewayCCSProtocolHandler::writeTerminate()
{
	unsigned char buffer[38U];

	::memset(buffer, 0x00U, 38U);

	::memcpy(buffer + 0U, m_remote, LONG_CALLSIGN_LENGTH);

	buffer[8U]  = '0';
	buffer[9U]  = '0';
	buffer[10U] = '0';
	buffer[11U] = '1';

	::memcpy(buffer + 12U, m_callsign, LONG_CALLSIGN_LENGTH);

// #if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Termination"), buffer, 38U);
//	return true;
// #else
	return m_socket->write(buffer, 38U);
// #endif
}

bool CDCSGatewayCCSProtocolHandler::writeQuery()
{
	unsigned char buffer[38U];

	::memset(buffer, 0x00U, 38U);

	::memcpy(buffer + 0U, m_remote, LONG_CALLSIGN_LENGTH);

	buffer[8U]  = '0';
	buffer[9U]  = '0';
	buffer[10U] = '0';
	buffer[11U] = '2';

	::memcpy(buffer + 12U, m_callsign, LONG_CALLSIGN_LENGTH);

// #if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Query"), buffer, 38U);
//	return true;
// #else
	return m_socket->write(buffer, 38U);
// #endif
}

bool CDCSGatewayCCSProtocolHandler::writeQueryReply(bool busy, const wxString& callsign, unsigned int timeout)
{
	unsigned char buffer[38U];

	::memset(buffer, ' ', 38U);

	::memcpy(buffer + 0U, m_queryRemote, LONG_CALLSIGN_LENGTH);

	buffer[8U]  = '1';
	buffer[9U]  = '0';
	buffer[10U] = '0';
	buffer[11U] = '2';

	::memcpy(buffer + 12U, m_reflector, LONG_CALLSIGN_LENGTH);

	buffer[20U] = '0';

	if (busy) {
		buffer[20U] = '1';

		buffer[21U] = callsign.GetChar(0U);
		buffer[22U] = callsign.GetChar(1U);
		buffer[23U] = callsign.GetChar(2U);
		buffer[24U] = callsign.GetChar(3U);
		buffer[25U] = callsign.GetChar(4U);
		buffer[26U] = callsign.GetChar(5U);
		buffer[27U] = callsign.GetChar(6U);
		buffer[28U] = callsign.GetChar(7U);

		::sprintf((char*)(buffer + 29U), "%us", timeout);
	}

// #if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Query Reply"), buffer, 38U);
//	return true;
// #else
	return m_socket->write(buffer, 38U);
// #endif
}

CCS_TYPE CDCSGatewayCCSProtocolHandler::read()
{
	if (m_socket == NULL) {
		m_type = CT_NONE;
		return CT_NONE;
	}

	bool res = true;

	// Loop until we have no more data from the socket or we have data for the higher layers
	while (res)
		res = readPackets();

	return m_type;
}

bool CDCSGatewayCCSProtocolHandler::readPackets()
{
	m_type = CT_NONE;

	// No more data?
	m_length = m_socket->read(m_buffer, BUFFER_LENGTH);
	if (m_length == 0U)
		return false;

	if (m_length == 25U) {
		writeHeartbeatReply();
		return true;
	} else if (m_length == 14U) {
		return true;
	} else if (m_buffer[0] == '0' && m_buffer[1] == '0' && m_buffer[2] == '0' && m_buffer[3] == '1') {
		CUtils::dump(wxT("Data"), m_buffer, m_length);
		m_type = CT_DATA;
		return false;
	} else if (m_buffer[0] == 'D' && m_buffer[1] == 'D' && m_buffer[2] == 'D' && m_buffer[3] == 'D') {
		// CUtils::dump(wxT("Status data"), m_buffer, m_length);
		return true;
	} else if (m_buffer[0] == 'G' && m_buffer[1] == 'G' && m_buffer[2] == 'G' && m_buffer[3] == 'G') {
		// CUtils::dump(wxT("Status data"), m_buffer, m_length);
		return true;
	} else if (m_length == 100U) {
		if (m_buffer[8U] == '0' && m_buffer[9U] == '0' && m_buffer[10U] == '0' && m_buffer[11U] == '1') {
			CUtils::dump(wxT("Termination"), m_buffer, m_length);
			m_type = CT_TERMINATE;
			return false;
		} else if (m_buffer[8U] == '0' && m_buffer[9U] == '0' && m_buffer[10U] == '0' && m_buffer[11U] == '2') {
			CUtils::dump(wxT("Query"), m_buffer, m_length);
			::memcpy(m_queryRemote, m_buffer + 0U, LONG_CALLSIGN_LENGTH);
			m_type = CT_QUERY;
			return false;
		} else if (m_buffer[8U] == '1' && m_buffer[9U] == '0' && m_buffer[10U] == '0' && m_buffer[11U] == '2') {
			CUtils::dump(wxT("Query Reply"), m_buffer, m_length);
			m_type = CT_QUERY_REPLY;
			return false;
		}
	}

	// An unknown type
	CUtils::dump(wxT("Unknown packet type from CCS"), m_buffer, m_length);

	return true;
}

CDCSGatewayHeaderData* CDCSGatewayCCSProtocolHandler::getHeader()
{
	if (m_type != CT_DATA)
		return NULL;

	CDCSGatewayHeaderData* header = new CDCSGatewayHeaderData;

	header->setCCSData(m_buffer, m_length);

	return header;
}

CDCSGatewayAMBEData* CDCSGatewayCCSProtocolHandler::readData()
{
	if (m_type != CT_DATA)
		return NULL;

	CDCSGatewayAMBEData* data = new CDCSGatewayAMBEData;

	bool res = data->setCCSData(m_buffer, m_length);
	if (!res) {
		delete data;
		return NULL;
	}

	return data;
}

bool CDCSGatewayCCSProtocolHandler::writeHeartbeatReply()
{
	unsigned char buffer[25U];

	::memset(buffer, ' ', 25U);

	buffer[0U]  = m_callsign[0U];
	buffer[1U]  = m_callsign[1U];
	buffer[2U]  = m_callsign[2U];
	buffer[3U]  = m_callsign[3U];
	buffer[4U]  = m_callsign[4U];
	buffer[5U]  = m_callsign[5U];
	buffer[6U]  = m_callsign[6U];

	buffer[8U]  = m_callsign[7U];

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Heartbeat Reply"), buffer, 25U);
	return true;
#else
	return m_socket->write(buffer, 25U);
#endif
}

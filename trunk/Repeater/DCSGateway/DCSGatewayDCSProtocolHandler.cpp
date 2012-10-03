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

#include "DCSGatewayDCSProtocolHandler.h"

#include "DStarDefines.h"
#include "Utils.h"

// #define	DUMP_TX

const unsigned int BUFFER_LENGTH = 2000U;

CDCSGatewayDCSProtocolHandler::CDCSGatewayDCSProtocolHandler() :
m_socket(NULL),
m_callsign(NULL),
m_reflector(NULL),
m_type(DT_NONE),
m_buffer(NULL),
m_length(0U),
m_connectTimer(1000U, 0U, 200U),
m_linked(false)
{
	m_callsign  = new unsigned char[LONG_CALLSIGN_LENGTH];
	m_reflector = new unsigned char[LONG_CALLSIGN_LENGTH];

	::memset(m_callsign,  ' ', LONG_CALLSIGN_LENGTH);
	::memset(m_reflector, ' ', LONG_CALLSIGN_LENGTH);

	m_buffer = new unsigned char[BUFFER_LENGTH];

	wxDateTime now = wxDateTime::Now();
	::srand(now.GetMillisecond());
}

CDCSGatewayDCSProtocolHandler::~CDCSGatewayDCSProtocolHandler()
{
	delete[] m_callsign;
	delete[] m_reflector;
	delete[] m_buffer;

	if (m_socket != NULL) {
		m_socket->close();
		delete m_socket;
	}
}

void CDCSGatewayDCSProtocolHandler::setCallsign(const wxString& callsign)
{
	::memset(m_callsign, ' ', LONG_CALLSIGN_LENGTH);

	for (unsigned int i = 0U; i < callsign.Len(); i++)
		m_callsign[i] = callsign.GetChar(i);

	if (m_callsign[7U] == ' ')
		m_callsign[7U] = 'D';
}

bool CDCSGatewayDCSProtocolHandler::link(const wxString& reflector, const wxString& address, unsigned int port)
{
	if (m_socket != NULL) {
		m_socket->close();
		delete m_socket;
		m_socket = NULL;
	}

	CUDPReaderWriter* socket = new CUDPReaderWriter(address, port);
	bool res = socket->open();
	if (!res) {
		delete socket;
		return false;
	}

	for (unsigned int i = 0U; i < reflector.Len(); i++)
		m_reflector[i] = reflector.GetChar(i);

	m_socket = socket;

	return writeConnect();
}

bool CDCSGatewayDCSProtocolHandler::writeData(const CDCSGatewayHeaderData& header, const CDCSGatewayAMBEData& data)
{
	wxASSERT(m_socket != NULL);

	unsigned char buffer[600U];

	unsigned int length = data.getDCSData(buffer, 600U);

	header.getDCSData(buffer, 600U);

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Data"), buffer, length);
	return true;
#else
	return m_socket->write(buffer, length);
#endif
}

DCS_TYPE CDCSGatewayDCSProtocolHandler::read()
{
	if (m_socket == NULL) {
		m_type = DT_NONE;
		return DT_NONE;
	}

	bool res = true;

	// Loop until we have no more data from the socket or we have data for the higher layers
	while (res)
		res = readPackets();

	return m_type;
}

bool CDCSGatewayDCSProtocolHandler::readPackets()
{
	m_type = DT_NONE;

	// No more data?
	m_length = m_socket->read(m_buffer, BUFFER_LENGTH);
	if (m_length == 0U)
		return false;

	if (m_buffer[0] == '0' && m_buffer[1] == '0' && m_buffer[2] == '0' && m_buffer[3] == '1') {
		if (m_length == 100U || m_length == 600U) {
			m_type = DT_DATA;
			return false;
		}
	} else if (m_buffer[0] == 'E' && m_buffer[1] == 'E' && m_buffer[2] == 'E' && m_buffer[3] == 'E') {
		// CUtils::dump(wxT("Status data"), m_buffer, m_length);
		return true;
	} else {
		switch (m_length) {
			case 9U:
				writePollReply();
				m_connectTimer.reset();
				return false;
			case 14U:
				if (!m_linked) {
					m_linked = true;
					m_connectTimer.setTimeout(60U);
				}
				return false;
			case 35U:
				// CUtils::dump(wxT("Status data"), m_buffer, m_length);
				return true;
			default:
				break;
		}
	}

	// An unknown type
	CUtils::dump(wxT("Unknown packet type from DCS"), m_buffer, m_length);

	return true;
}

CDCSGatewayHeaderData* CDCSGatewayDCSProtocolHandler::getHeader()
{
	if (m_type != DT_DATA)
		return NULL;

	CDCSGatewayHeaderData* header = new CDCSGatewayHeaderData;

	header->setDCSData(m_buffer, m_length);

	return header;
}

CDCSGatewayAMBEData* CDCSGatewayDCSProtocolHandler::readData()
{
	if (m_type != DT_DATA)
		return NULL;

	CDCSGatewayAMBEData* data = new CDCSGatewayAMBEData;

	bool res = data->setDCSData(m_buffer, m_length);
	if (!res) {
		delete data;
		return NULL;
	}

	return data;
}

void CDCSGatewayDCSProtocolHandler::clock(unsigned int ms)
{
	m_connectTimer.clock(ms);

	if (m_connectTimer.hasExpired()) {
		if (!m_linked)
			writeConnect();
		else
			m_linked = false;
	}
}

bool CDCSGatewayDCSProtocolHandler::isConnected()
{
	return m_socket != NULL && m_linked;
}

void CDCSGatewayDCSProtocolHandler::unlink()
{
	if (m_socket != NULL) {
		if (m_linked)
			writeDisconnect();

		m_socket->close();
		delete m_socket;
		m_socket = NULL;
	}
}

bool CDCSGatewayDCSProtocolHandler::writeConnect()
{
	unsigned char buffer[20U];

	::memset(buffer, 0x20U, 19U);

	::memcpy(buffer, m_callsign, LONG_CALLSIGN_LENGTH - 1U);

	buffer[8U]  = m_callsign[LONG_CALLSIGN_LENGTH - 1U];

	buffer[9U]  = m_reflector[LONG_CALLSIGN_LENGTH - 1U];

	buffer[10U] = 0x00U;

	::memcpy(buffer + 11U, m_reflector, LONG_CALLSIGN_LENGTH - 1U);

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Connect"), buffer, 19U);
	return true;
#else
	return m_socket->write(buffer, 19U);
#endif
}

bool CDCSGatewayDCSProtocolHandler::writePollReply()
{
	unsigned char buffer[20U];

	::memset(buffer, 0x20U, 17U);

	::memcpy(buffer, m_callsign, LONG_CALLSIGN_LENGTH - 1U);

	buffer[8U]  = 0x00U;

	::memcpy(buffer + 9U, m_reflector, LONG_CALLSIGN_LENGTH - 1U);

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Poll Reply"), buffer, 17U);
	return true;
#else
	return m_socket->write(buffer, 17U);
#endif
}

bool CDCSGatewayDCSProtocolHandler::writeDisconnect()
{
	unsigned char buffer[20U];

	::memset(buffer, 0x20U, 19U);

	::memcpy(buffer, m_callsign, LONG_CALLSIGN_LENGTH - 1U);

	buffer[8U]  = m_callsign[LONG_CALLSIGN_LENGTH - 1U];

	buffer[9U]  = 0x20U;

	buffer[10U] = 0x00U;

	::memcpy(buffer + 11U, m_reflector, LONG_CALLSIGN_LENGTH - 1U);

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Disconnect"), buffer, 19U);
	return true;
#else
	return m_socket->write(buffer, 19U);
#endif
}

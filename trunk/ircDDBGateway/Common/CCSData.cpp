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

#include "DStarDefines.h"
#include "CCSData.h"
#include "Utils.h"

CCCSData::CCCSData(const wxString& local, const wxString& remote, CC_TYPE type) :
m_local(local),
m_remote(remote),
m_type(type),
m_busy(false),
m_reflector(),
m_time(),
m_yourAddress(),
m_yourPort(0U),
m_myPort(0U)
{
}

CCCSData::CCCSData(const wxString& remote, CC_TYPE type) :
m_local(),
m_remote(remote),
m_type(type),
m_busy(false),
m_reflector(),
m_time(),
m_yourAddress(),
m_yourPort(0U),
m_myPort(0U)
{
}

CCCSData::CCCSData() :
m_local(),
m_remote(),
m_type(),
m_busy(false),
m_reflector(),
m_time(),
m_yourAddress(),
m_yourPort(0U),
m_myPort(0U)
{
}

CCCSData::~CCCSData()
{
}

bool CCCSData::setCCSData(const unsigned char *data, unsigned int length, const in_addr& yourAddress, unsigned int yourPort, unsigned int myPort)
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 100U);

	m_remote = wxString((char*)(data + 0U), wxConvLocal, LONG_CALLSIGN_LENGTH);

	if (::memcmp(data + 8U, "0001", 4U) == 0) {
		m_type = CT_TERMINATE;
	} else if (::memcmp(data + 8U, "0002", 4U) == 0) {
		m_type = CT_QUERY;
	} else if (::memcmp(data + 8U, "1002", 4U) == 0) {
		m_type = CT_ANSWER;
	} else {
		CUtils::dump(wxT("Invalid CCS packet"), data, length);
		return false;
	}

	if (m_type == CT_TERMINATE || m_type == CT_QUERY) {
		m_local     = wxString((char*)(data + 12U), wxConvLocal, LONG_CALLSIGN_LENGTH);
	} else {
		m_reflector = wxString((char*)(data + 12U), wxConvLocal, LONG_CALLSIGN_LENGTH);

		switch (data[20U]) {
			case '0':
				m_busy = false;
				break;
			case '1':
				m_busy = true;
				break;
			default:
				break;
		}

		m_local    = wxString((char*)(data + 21U), wxConvLocal, LONG_CALLSIGN_LENGTH);

		m_time     = wxString((char*)(data + 29U), wxConvLocal, 9U);
	}

	m_yourAddress = yourAddress;
	m_yourPort    = yourPort;
	m_myPort      = myPort;

	return true;
}

unsigned int CCCSData::getCCSData(unsigned char* data, unsigned int length) const
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 38U);

	::memset(data, ' ', 38U);

	for (unsigned int i = 0U; i < m_remote.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		data[i + 0U] = m_remote.GetChar(i);

	switch (m_type) {
		case CT_TERMINATE:
			::memcpy(data + 8U, "0001", 4U);
			break;
		case CT_QUERY:
			::memcpy(data + 8U, "0002", 4U);
			break;
		case CT_ANSWER:
			::memcpy(data + 8U, "1002", 4U);
			break;
		default:
			wxLogWarning(wxT("Invalid CCS type: %d"), int(m_type));
			break;
	}

	if (m_type == CT_TERMINATE || m_type == CT_QUERY) {
		for (unsigned int i = 0U; i < m_local.Len() && i < LONG_CALLSIGN_LENGTH; i++)
			data[i + 12U] = m_local.GetChar(i);
	} else {
		for (unsigned int i = 0U; i < m_reflector.Len() && i < LONG_CALLSIGN_LENGTH; i++)
			data[i + 12U] = m_reflector.GetChar(i);

		data[20U] = m_busy ? '1' : '0';

		for (unsigned int i = 0U; i < m_local.Len() && i < LONG_CALLSIGN_LENGTH; i++)
			data[i + 21U] = m_local.GetChar(i);

		for (unsigned int i = 0U; i < m_time.Len() && i < 9U; i++)
			data[i + 29U] = m_time.GetChar(i);
	}

	return 38U;
}

void CCCSData::setLocal(const wxString& callsign)
{
	m_local = callsign;
}

void CCCSData::setReflector(const wxString& callsign)
{
	m_reflector = callsign;
}

void CCCSData::setTime(unsigned int seconds)
{
	m_time.Printf(wxT("%us"), seconds);

	m_busy = true;
}

wxString CCCSData::getLocal() const
{
	return m_local;
}

wxString CCCSData::getRemote() const
{
	return m_remote;
}

CC_TYPE CCCSData::getType() const
{
	return m_type;
}

bool CCCSData::isBusy() const
{
	return m_busy;
}

wxString CCCSData::getReflector() const
{
	return m_reflector;
}

wxString CCCSData::getTime() const
{
	return m_time;
}

void CCCSData::setDestination(const in_addr& address, unsigned int port)
{
	m_yourAddress = address;
	m_yourPort    = port;
}

in_addr CCCSData::getYourAddress() const
{
	return m_yourAddress;
}

unsigned int CCCSData::getYourPort() const
{
	return m_yourPort;
}

unsigned int CCCSData::getMyPort() const
{
	return m_myPort;
}

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
m_yourAddress(),
m_yourPort(0U),
m_myPort(0U)
{
}

CCCSData::CCCSData() :
m_local(),
m_remote(),
m_type(),
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

	switch (length) {
		case 100U:
			m_remote = wxString((char*)(data + 0U), wxConvLocal, LONG_CALLSIGN_LENGTH);

			if (::memcmp(data + 8U, "0001", 4U) == 0) {
				m_type = CT_TERMINATE;
			} else {
				CUtils::dump(wxT("Invalid CCS packet"), data, length);
				return false;
			}

			m_local = wxString((char*)(data + 12U), wxConvLocal, LONG_CALLSIGN_LENGTH);
			break;

		case 20U:
			if (::memcmp(data + 0U, "DTMF_CALL:", 10U) == 0) {
				m_type = CT_DTMFFOUND;
			} else {
				CUtils::dump(wxT("Invalid CCS packet"), data, length);
				return false;
			}

			m_remote = wxString((char*)(data + 10U), wxConvLocal, LONG_CALLSIGN_LENGTH);
			break;

		case 17U:
			if (::memcmp(data + 0U, "NODTMFCALL", 10U) == 0) {
				m_type = CT_DTMFNOTFOUND;
			} else {
				CUtils::dump(wxT("Invalid CCS packet"), data, length);
				return false;
			}
			break;

		default:
			CUtils::dump(wxT("Invalid CCS packet"), data, length);
			return false;
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

	::memcpy(data + 8U, "0001", 4U);

	for (unsigned int i = 0U; i < m_local.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		data[i + 12U] = m_local.GetChar(i);

	return 38U;
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

/*
 *   Copyright (C) 2012,2013 by Jonathan Naylor G4KLX
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
#include "HeardData.h"

CHeardData::CHeardData() :
m_repeater(),
m_user(),
m_address(),
m_port(0U)
{
}

CHeardData::CHeardData(const CHeardData& data) :
m_repeater(data.m_repeater),
m_user(data.m_user),
m_address(data.m_address),
m_port(data.m_port)
{
}

CHeardData::~CHeardData()
{
}

bool CHeardData::setIcomRepeaterData(const unsigned char *data, unsigned int length, const in_addr& address, unsigned int port)
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 26U);

	m_user     = wxString((char*)(data + 10U), wxConvLocal, LONG_CALLSIGN_LENGTH);
	m_repeater = wxString((char*)(data + 18U), wxConvLocal, LONG_CALLSIGN_LENGTH);

	m_address = address;
	m_port    = port;

	return true;
}

unsigned int CHeardData::getCCSData(unsigned char *data, unsigned int length) const
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 100U);

	::memset(data, 0x00U, 100U);

	data[0U] = '0';
	data[1U] = '0';
	data[2U] = '0';
	data[3U] = '1';

	::memcpy(data + 23U, "CQCQCQ  ", LONG_CALLSIGN_LENGTH);

	data[61U] = 0x01U;

	data[63U] = 0x21U;

	::memset(data + 64U, ' ', 20U);

	data[93U] = 0x30U;		// XXX

	return 100U;
}

wxString CHeardData::getRepeater() const
{
	return m_repeater;
}

wxString CHeardData::getUser() const
{
	return m_user;
}

in_addr CHeardData::getAddress() const
{
	return m_address;
}

unsigned int CHeardData::getPort() const
{
	return m_port;
}

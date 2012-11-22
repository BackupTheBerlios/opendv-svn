/*
 *   Copyright (C) 2010,2012 by Jonathan Naylor G4KLX
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

#include "PollData.h"

#include "DStarDefines.h"
#include "Utils.h"

CPollData::CPollData(const wxString& data, const in_addr& address, unsigned int port) :
m_data1(data),
m_data2(),
m_dongle(false),
m_length(0U),
m_address(address),
m_port(port)
{
	wxASSERT(port > 0U);
}

CPollData::CPollData(const in_addr& address, unsigned int port) :
m_data1(),
m_data2(),
m_dongle(false),
m_length(0U),
m_address(address),
m_port(port)
{
	wxASSERT(port > 0U);
}

CPollData::CPollData() :
m_data1(),
m_data2(),
m_dongle(false),
m_length(0U),
m_address(),
m_port(0U)
{
}

CPollData::~CPollData()
{
}

bool CPollData::setDExtraData(const unsigned char* data, unsigned int length, const in_addr& address, unsigned int port)
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 9U);
	wxASSERT(port > 0U);

	m_data1   = wxString((const char*)data, wxConvLocal, LONG_CALLSIGN_LENGTH);
	m_dongle  = data[LONG_CALLSIGN_LENGTH] != 0x00;

	m_length  = length;
	m_address = address;
	m_port    = port;

	return true;
}

bool CPollData::setDCSData(const unsigned char* data, unsigned int length, const in_addr& address, unsigned int port)
{
	wxASSERT(data != NULL);
	wxASSERT(port > 0U);

	if (length == 9U) {
		m_data1   = wxString((const char*)(data + 0U), wxConvLocal, LONG_CALLSIGN_LENGTH);
		m_length  = length;
		m_address = address;
		m_port    = port;
	} else if (length == 17U) {
		m_data1   = wxString((const char*)(data + 0U), wxConvLocal, LONG_CALLSIGN_LENGTH);
		m_data2   = wxString((const char*)(data + 9U), wxConvLocal, LONG_CALLSIGN_LENGTH);
		m_length  = length;
		m_address = address;
		m_port    = port;
	} else if (length == 22U) {
		m_data1   = wxString((const char*)(data + 0U), wxConvLocal, LONG_CALLSIGN_LENGTH);
		m_data2   = wxString((const char*)(data + 9U), wxConvLocal, LONG_CALLSIGN_LENGTH - 1U);
		m_data2.Append(wxString((const char*)(data + 17U), wxConvLocal, 1U));
		m_length  = length;
		m_address = address;
		m_port    = port;
	}

	return true;
}

bool CPollData::setDPlusData(const unsigned char* data, unsigned int length, const in_addr& address, unsigned int port)
{
	wxASSERT(port > 0U);

	m_length  = length;
	m_address = address;
	m_port    = port;

	return true;
}

unsigned int CPollData::getDExtraData(unsigned char *data, unsigned int length) const
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 9U);

	::memset(data, ' ', LONG_CALLSIGN_LENGTH);

	for (unsigned int i = 0U; i < m_data1.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		data[i] = m_data1.GetChar(i);

	data[LONG_CALLSIGN_LENGTH] = 0x00;

	return 9U;
}

unsigned int CPollData::getDCSData(unsigned char *data, unsigned int length) const
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 8U);

	::memset(data, ' ', LONG_CALLSIGN_LENGTH);

	for (unsigned int i = 0U; i < m_data1.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		data[i] = m_data1.GetChar(i);

	return 8U;
}

unsigned int CPollData::getDPlusData(unsigned char *data, unsigned int length) const
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 3U);

	data[0U] = 0x03;
	data[1U] = 0x60;
	data[2U] = 0x00;

	return 3U;
}

wxString CPollData::getData1() const
{
	return m_data1;
}

void CPollData::setData1(const wxString& data)
{
	m_data1 = data;
}

wxString CPollData::getData2() const
{
	return m_data2;
}

void CPollData::setData2(const wxString& data)
{
	m_data2 = data;
}

bool CPollData::isDongle() const
{
	return m_dongle;
}

in_addr CPollData::getAddress() const
{
	return m_address;
}

unsigned int CPollData::getPort() const
{
	return m_port;
}

unsigned int CPollData::getLength() const
{
	return m_length;
}

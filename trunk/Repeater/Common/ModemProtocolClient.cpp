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

#include "ModemProtocolClient.h"
#include "DStarDefines.h"
#include "Utils.h"

// #define	DUMP_TX

const unsigned int BUFFER_LENGTH = 255U;

CModemProtocolClient::CModemProtocolClient(const wxString& name) :
m_client(NULL),
m_type(MMT_NONE),
m_buffer(NULL),
m_length(0U)
{
	m_buffer = new unsigned char[BUFFER_LENGTH];

#if defined(__WINDOWS__)
	m_client = new CNamedPipeClient(name);
#else
	wxString path = wxT("/tmp/";
	path.Append(name);

	path.Replace(wxT(" "), wxT("_"));

	m_client = new CUNIXSocketClient(path);
#endif
}

CModemProtocolClient::~CModemProtocolClient()
{
	delete m_client;

	delete[] m_buffer;
}

bool CModemProtocolClient::open()
{
	bool ret = m_client->open();
	if (!ret)
		return false;

	for (unsigned int i = 0U; i < 10U; i++) {
		unsigned char buffer[100U];
		buffer[0U] = 0x00U;
		m_client->write(buffer, 1U);

		unsigned int length = m_client->read(buffer, 100U);
		if (length == 1U && m_buffer[0U] == 0x00U)
			return true;

		wxMilliSleep(100UL);
	}

	wxLogError(wxT("No reply to the modem initialisation command"));

	m_client->close();
	return false;
}

bool CModemProtocolClient::writeHeader(const CHeaderData& header)
{
	unsigned char buffer[50U];

	buffer[0U] = 0x01U;

	buffer[1U] = header.getFlag1();
	buffer[2U] = header.getFlag2();
	buffer[3U] = header.getFlag3();

	for (unsigned int i = 0U; i < LONG_CALLSIGN_LENGTH; i++)
		buffer[4U + i] = header.getRptCall1().GetChar(i);

	for (unsigned int i = 0U; i < LONG_CALLSIGN_LENGTH; i++)
		buffer[12U + i] = header.getRptCall2().GetChar(i);

	for (unsigned int i = 0U; i < LONG_CALLSIGN_LENGTH; i++)
		buffer[20U + i] = header.getYourCall().GetChar(i);

	for (unsigned int i = 0U; i < LONG_CALLSIGN_LENGTH; i++)
		buffer[28U + i] = header.getMyCall1().GetChar(i);

	for (unsigned int i = 0U; i < SHORT_CALLSIGN_LENGTH; i++)
		buffer[36U + i] = header.getMyCall2().GetChar(i);

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Header"), buffer, 40U);
	return true;
#else
	return m_client->write(buffer, 40U);
#endif
}

bool CModemProtocolClient::writeData(const unsigned char* data, unsigned int length, bool end)
{
	wxASSERT(data != NULL);
	wxASSERT(length == DV_FRAME_LENGTH_BYTES || length == DV_FRAME_MAX_LENGTH_BYTES);

	unsigned char buffer[20U];

	buffer[0U] = 0x02U;

	buffer[1U] = end ? 1U : 0U;

	::memcpy(buffer + 2U, data, length);

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Data"), buffer, length + 2U);
	return true;
#else
	return m_client->write(buffer, length + 2U);
#endif
}

bool CModemProtocolClient::writeTX(bool on)
{
	unsigned char buffer[5U];

	buffer[0U] = 0x03U;

	buffer[1U] = on ? 1U : 0U;

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending TX"), buffer, 2U);
	return true;
#else
	return m_client->write(buffer, 2U);
#endif
}

bool CModemProtocolClient::getStatus()
{
	unsigned char buffer[5U];

	buffer[0] = 0x04U;

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Status"), buffer, 1U);
	return true;
#else
	return m_client->write(buffer, 1U);
#endif
}

MODEM_MSG_TYPE CModemProtocolClient::read()
{
	bool res = true;

	// Loop until we have no more data from the socket or we have data for the higher layers
	while (res)
		res = readPackets();

	return m_type;
}

bool CModemProtocolClient::readPackets()
{
	m_type = MMT_NONE;

	// No more data?
	int length = m_client->read(m_buffer, BUFFER_LENGTH);
	if (length <= 0)
		return false;

	m_length = length;

	switch (m_buffer[0U]) {
		case 0x01U:
			m_type = MMT_HEADER;
			return false;

		case 0x02U:
			m_type = MMT_DATA;
			return false;

		case 0x05U:
			m_type = MMT_SPACE;
			return false;

		case 0x06U:
			m_type = MMT_TX;
			return false;

		case 0x10U:
			m_type = MMT_TEXT;
			return false;

		default:
			CUtils::dump(wxT("Unknown packet from the modem"), m_buffer, m_length);
			return true;
	}
}

CHeaderData* CModemProtocolClient::readHeader()
{
	if (m_type != MMT_HEADER)
		return NULL;

	return new CHeaderData(m_buffer + 1U, RADIO_HEADER_LENGTH_BYTES, false);
}

unsigned int CModemProtocolClient::readData(unsigned char* buffer, unsigned int length, bool& end)
{
	if (m_type != MMT_DATA)
		return 0U;

	unsigned int dataLen = m_length - 2U;

	// Is our buffer too small?
	if (dataLen > length)
		dataLen = length;

	end = m_buffer[1U] == 1U;

	::memcpy(buffer, m_buffer + 2U, dataLen);

	return dataLen;
}

wxString CModemProtocolClient::readText()
{
	if (m_type != MMT_TEXT)
		return wxEmptyString;

	return wxString((char*)(m_buffer + 1U), wxConvLocal, m_length - 1U);
}

unsigned int CModemProtocolClient::readSpace()
{
	if (m_type != MMT_SPACE)
		return 0U;

	unsigned int space;
	::memcpy(&space, m_buffer + 1U, sizeof(unsigned int));

	return space;
}

bool CModemProtocolClient::readTX()
{
	if (m_type != MMT_TX)
		return false;

	return m_buffer[1U] == 1U;
}

void CModemProtocolClient::close()
{
	for (unsigned int i = 0U; i < 10U; i++) {
		unsigned char buffer[100U];
		buffer[0U] = 0xFFU;
		m_client->write(buffer, 1U);

		unsigned int length = m_client->read(buffer, 100U);
		if (length == 1U && m_buffer[0U] == 0xFFU)
			break;

		wxMilliSleep(100UL);
	}

	m_client->close();
}

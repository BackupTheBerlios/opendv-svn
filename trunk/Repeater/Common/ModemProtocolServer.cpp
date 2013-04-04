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

#include "ModemProtocolServer.h"
#include "DStarDefines.h"
#include "Utils.h"

// #define	DUMP_TX

const unsigned int BUFFER_LENGTH = 255U;

CModemProtocolServer::CModemProtocolServer(const wxString& name) :
m_server(NULL),
m_type(MMT_NONE),
m_buffer(NULL),
m_length(0U)
{
	m_buffer = new unsigned char[BUFFER_LENGTH];

#if defined(__WINDOWS__)
	m_server = new CNamedPipeServer(name);
#else
	wxString path = wxT("/tmp/");
	path.Append(name);

	path.Replace(wxT(" "), wxT("_"));

	m_server = new CUNIXSocketServer(path);
#endif
}

CModemProtocolServer::~CModemProtocolServer()
{
	delete m_server;

	delete[] m_buffer;
}

bool CModemProtocolServer::open()
{
	return m_server->start();
}

bool CModemProtocolServer::writeBegin()
{
	unsigned char buffer[2U];

	buffer[0U] = 0x00U;

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Begin"), buffer, 1U);
	return true;
#else
	return m_server->write(buffer, 1U);
#endif
}

bool CModemProtocolServer::writeHeader(const CHeaderData& header)
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
	return m_server->write(buffer, 40U);
#endif
}

bool CModemProtocolServer::writeData(const unsigned char* data, unsigned int length, bool end)
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
	return m_server->write(buffer, length + 2U);
#endif
}

bool CModemProtocolServer::writeSpace(unsigned int space)
{
	unsigned char buffer[10U];

	buffer[0U] = 0x05U;

	::memcpy(buffer + 1U, &space, sizeof(unsigned int));

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Space"), buffer, 1U + sizeof(unsigned int));
	return true;
#else
	return m_server->write(buffer, 1U + sizeof(unsigned int));
#endif
}

bool CModemProtocolServer::writeTX(bool on)
{
	unsigned char buffer[5U];

	buffer[0U] = 0x06U;

	buffer[1U] = on ? 1U : 0U;

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending TX"), buffer, 2U);
	return true;
#else
	return m_server->write(buffer, 2U);
#endif
}

bool CModemProtocolServer::writeText(const wxString& text)
{
	unsigned char buffer[200U];

	unsigned int length = text.Len();

	buffer[0U] = 0x10U;

	for (unsigned int i = 0U; i < length; i++)
		buffer[i + 1U] = text.GetChar(i);

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending Text"), buffer, length + 1U);
	return true;
#else
	return m_server->write(buffer, length + 1U);
#endif
}

bool CModemProtocolServer::writeEnd()
{
	unsigned char buffer[2U];

	buffer[0U] = 0xFFU;

#if defined(DUMP_TX)
	CUtils::dump(wxT("Sending End"), buffer, 1U);
	return true;
#else
	return m_server->write(buffer, 1U);
#endif
}

MODEM_MSG_TYPE CModemProtocolServer::read()
{
	bool res = true;

	// Loop until we have no more data from the socket or we have data for the higher layers
	while (res)
		res = readPackets();

	return m_type;
}

bool CModemProtocolServer::readPackets()
{
	m_type = MMT_NONE;

	if (!m_server->isConnected())
		return false;

	// No more data?
	int length = m_server->read(m_buffer, BUFFER_LENGTH);
	if (length <= 0)
		return false;

	m_length = length;

	switch (m_buffer[0U]) {
		case 0x00U:
			m_type = MMT_BEGIN;
			return false;

		case 0x01U:
			m_type = MMT_HEADER;
			return false;

		case 0x02U:
			m_type = MMT_DATA;
			return false;

		case 0x03U:
			m_type = MMT_TX;
			return false;

		case 0x04U:
			m_type = MMT_STATUS;
			return false;

		case 0xFFU:
			m_type = MMT_END;
			return false;

		default:
			CUtils::dump(wxT("Unknown packet from the repeater"), m_buffer, m_length);
			return true;
	}
}

CHeaderData* CModemProtocolServer::readHeader()
{
	if (m_type != MMT_HEADER)
		return NULL;

	return new CHeaderData(m_buffer + 1U, RADIO_HEADER_LENGTH_BYTES, false);
}

unsigned int CModemProtocolServer::readData(unsigned char* buffer, unsigned int length, bool& end)
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

bool CModemProtocolServer::readTX()
{
	if (m_type != MMT_TX)
		return false;

	return m_buffer[1U] == 1U;
}

void CModemProtocolServer::close()
{
	m_server->stop();
}

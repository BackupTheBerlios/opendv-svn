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

#ifndef	ModemProtocolClient_H
#define	ModemProtocolClient_H

#include "DStarDefines.h"
#include "HeaderData.h"

#if defined(__WINDOWS__)
#include "NamedPipeClient.h"
#else
#include "UnixSocketReaderWriter.h"
#endif

#include <wx/wx.h>

enum MODEM_MSG_TYPE {
	MMT_NONE,
	MMT_HEADER,
	MMT_DATA,
	MMT_SPACE,
	MMT_TX,
	MMT_TEXT
};

class CModemProtocolClient {
public:
	CModemProtocolClient(const wxString& name);
	~CModemProtocolClient();

	bool open();

	bool writeHeader(const CHeaderData& header);
	bool writeData(const unsigned char* data, unsigned int length, bool end);
	bool writeTX(bool on);
	bool getStatus();

	MODEM_MSG_TYPE read();
	wxString       readText();
	unsigned int   readSpace();
	CHeaderData*   readHeader();
	unsigned int   readData(unsigned char* data, unsigned int length, bool& end);
	bool           readTX();

	void close();

private:
#if defined(__WINDOWS__)
	CNamedPipeClient*        m_client;
#else
	CUnixSocketReaderWriter* m_client;
#endif
	MODEM_MSG_TYPE           m_type;
	unsigned char*           m_buffer;
	unsigned int             m_length;

	bool readPackets();
};

#endif

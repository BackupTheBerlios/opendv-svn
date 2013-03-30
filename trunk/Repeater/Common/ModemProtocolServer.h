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

#ifndef	ModemProtocolServer_H
#define	ModemProtocolServer_H

#include "DStarDefines.h"
#include "HeaderData.h"

#if defined(__WINDOWS__)
#include "NamedPipeServer.h"
#else
#include "UNIXSocketServer.h"
#endif

#include <wx/wx.h>

enum MODEM_MSG_TYPE {
	MMT_NONE,
	MMT_BEGIN,		// 0x00
	MMT_HEADER,		// 0x01
	MMT_DATA,		// 0x02
	MMT_TX,			// 0x03
	MMT_STATUS,		// 0x04
	MMT_END			// 0xFF
};

class CModemProtocolServer {
public:
	CModemProtocolServer(const wxString& name);
	~CModemProtocolServer();

	bool open();

	bool writeBegin();
	bool writeHeader(const CHeaderData& header);
	bool writeData(const unsigned char* data, unsigned int length, bool end);
	bool writeTX(bool on);
	bool writeSpace(unsigned int space);
	bool writeText(const wxString& text);
	bool writeEnd();

	MODEM_MSG_TYPE read();
	CHeaderData*   readHeader();
	unsigned int   readData(unsigned char* data, unsigned int length, bool& end);
	bool           readTX();

	void close();

private:
#if defined(__WINDOWS__)
	CNamedPipeServer*  m_server;
#else
	CUNIXSocketServer* m_server;
#endif
	MODEM_MSG_TYPE     m_type;
	unsigned char*     m_buffer;
	unsigned int       m_length;

	bool readPackets();
};

#endif

/*
 *   Copyright (C) 2010,2011,2013 by Jonathan Naylor G4KLX
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

#ifndef	DongleProtocolHandler_H
#define	DongleProtocolHandler_H

#include "UDPReaderWriter.h"
#include "DStarDefines.h"
#include "ConnectData.h"
#include "HeaderData.h"
#include "AMBEData.h"
#include "PollData.h"

#if defined(__WINDOWS__)
#include "Inaddr.h"
#else
#include <netinet/in.h>
#endif

#include <wx/wx.h>

enum DONGLE_TYPE {
	DP_NONE,

	DP_DPLUS_HEADER,
	DP_DPLUS_AMBE,
	DP_DPLUS_POLL,
	DP_DPLUS_CONNECT,

	DP_DEXTRA_HEADER,
	DP_DEXTRA_AMBE,
	DP_DEXTRA_POLL
};

class CDongleProtocolHandler {
public:
	CDongleProtocolHandler(unsigned int port, const wxString& addr = wxEmptyString);
	~CDongleProtocolHandler();

	bool open();

	unsigned int getPort() const;

	bool writeDPlusHeader(const CHeaderData& header);
	bool writeDPlusAMBE(const CAMBEData& data);
	bool writeDPlusConnect(const CConnectData& connect);
	bool writeDPlusPoll(const CPollData& poll);

	bool writeDExtraHeader(const CHeaderData& header);
	bool writeDExtraAMBE(const CAMBEData& data);
	bool writeDExtraPoll(const CPollData& poll);

	DONGLE_TYPE   read();

	CHeaderData*  readDPlusHeader();
	CAMBEData*    readDPlusAMBE();
	CPollData*    readDPlusPoll();
	CConnectData* readDPlusConnect();

	CHeaderData*  readDExtraHeader();
	CAMBEData*    readDExtraAMBE();
	CPollData*    readDExtraPoll();

	void close();

private:
	CUDPReaderWriter m_socket;
	DONGLE_TYPE      m_type;
	unsigned char*   m_buffer;
	unsigned int     m_length;
	in_addr          m_yourAddress;
	unsigned int     m_yourPort;
	unsigned int     m_myPort;

	bool readPackets();
};

#endif

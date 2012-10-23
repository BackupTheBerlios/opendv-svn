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

#ifndef	DCSGatewayCCSProtocolHander_H
#define	DCSGatewayCCSProtocolHander_H

#include "DCSGatewayHeaderData.h"
#include "DCSGatewayAMBEData.h"
#include "UDPReaderWriter.h"
#include "Timer.h"

#include <wx/wx.h>

enum CCS_TYPE {
	CT_NONE,
	CT_DATA,
	CT_TERMINATE,
	CT_QUERY,
	CT_QUERY_REPLY
};

class CDCSGatewayCCSProtocolHandler {
public:
	CDCSGatewayCCSProtocolHandler();
	~CDCSGatewayCCSProtocolHandler();

	void setCallsign(const wxString& callsign);
	void setLocator(const wxString& locator);
	void setReflector(const wxString& reflector);
	void setRemote(const wxString& remote);

	bool registration(const wxString& address, unsigned int port);
	void cancelation();

	bool writeUserStatus(const CDCSGatewayHeaderData& header);
	bool writeData(const CDCSGatewayHeaderData& header, const CDCSGatewayAMBEData& data);
	bool writeTerminate();
	bool writeQuery();
	bool writeQueryReply(bool busy, const wxString& callsign, unsigned int timeout);

	CCS_TYPE read();
	CDCSGatewayAMBEData* readData();
	CDCSGatewayHeaderData* getHeader();

private:
	CUDPReaderWriter* m_socket;
	unsigned char*    m_callsign;
	unsigned char*    m_locator;
	unsigned char*    m_reflector;
	unsigned char*    m_remote;
	CCS_TYPE          m_type;
	unsigned char*    m_buffer;
	unsigned int      m_length;
	unsigned char*    m_queryRemote;

	bool readPackets();
	bool writeHeartbeatReply();
};

#endif

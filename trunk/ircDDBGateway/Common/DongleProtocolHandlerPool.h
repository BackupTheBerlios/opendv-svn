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

#ifndef DongleProtocolHandlerPool_H
#define	DongleProtocolHandlerPool_H

#include <wx/wx.h>

#include "DongleProtocolHandler.h"

class CDongleProtocolHandlerEntry {
public:
	CDongleProtocolHandler* m_handler;
	unsigned int            m_port;
	bool                    m_inUse;
};

class CDongleProtocolHandlerPool {
public:
	CDongleProtocolHandlerPool(unsigned int n, unsigned int port, const wxString& addr = wxEmptyString);
	~CDongleProtocolHandlerPool();

	bool open();

	CDongleProtocolHandler* getHandler(unsigned int port = 0U);
	void release(CDongleProtocolHandler* handler);

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
	CDongleProtocolHandlerEntry* m_pool;
	unsigned int                 m_n;
	unsigned int                 m_index;
};

#endif

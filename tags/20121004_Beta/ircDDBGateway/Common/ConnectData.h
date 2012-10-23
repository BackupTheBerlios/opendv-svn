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

#ifndef	ConnectData_H
#define	ConnectData_H

#include <wx/wx.h>

#if defined(__WINDOWS__)
#include "Inaddr.h"
#else
#include <netinet/in.h>
#endif

enum CD_TYPE {
	CT_LINK1,
	CT_LINK2,
	CT_UNLINK,
	CT_ACK,
	CT_NAK
};

class CConnectData {
public:
	CConnectData(const wxString& repeater, const wxString& reflector, CD_TYPE type, const in_addr& address, unsigned int port);
	CConnectData(const wxString& repeater, const in_addr& address, unsigned int port);
	CConnectData(const wxString& repeater, CD_TYPE type, const in_addr& address, unsigned int port);
	CConnectData(CD_TYPE type, const in_addr& address, unsigned int port);
	CConnectData();
	~CConnectData();

	bool setDExtraData(const unsigned char* data, unsigned int length, const in_addr& address, unsigned int port);
	bool setDPlusData(const unsigned char* data, unsigned int length, const in_addr& address, unsigned int port);
	bool setDCSData(const unsigned char* data, unsigned int length, const in_addr& address, unsigned int port);

	unsigned int getDExtraData(unsigned char* data, unsigned int length) const;
	unsigned int getDPlusData(unsigned char* data, unsigned int length) const;
	unsigned int getDCSData(unsigned char* data, unsigned int length) const;

	wxString      getRepeater() const;
	wxString      getReflector() const;
	CD_TYPE       getType() const;

	in_addr       getAddress() const;
	unsigned int  getPort() const;

private:
	wxString      m_repeater;
	wxString      m_reflector;
	CD_TYPE       m_type;
	in_addr       m_address;
	unsigned int  m_port;
};

#endif

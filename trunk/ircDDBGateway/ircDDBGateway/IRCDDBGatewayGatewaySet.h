/*
 *   Copyright (C) 2010,2011,2012 by Jonathan Naylor G4KLX
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

#ifndef	IRCDDBGatewayGatewaySet_H
#define	IRCDDBGatewayGatewaySet_H

#include "CallsignTextCtrl.h"
#include "AddressTextCtrl.h"
#include "PortTextCtrl.h"
#include "Defs.h"

#include <wx/wx.h>

class CIRCDDBGatewayGatewaySet : public wxPanel {
public:
	CIRCDDBGatewayGatewaySet(wxWindow* parent, int id, const wxString& title, const wxString& callsign, const wxString& address, const wxString& icomAddress, unsigned int icomPort, const wxString& hbAddress, unsigned int hbPort);
	virtual ~CIRCDDBGatewayGatewaySet();

	virtual bool Validate();

	virtual wxString     getCallsign() const;
	virtual wxString     getAddress() const;
	virtual wxString     getIcomAddress() const;
	virtual unsigned int getIcomPort() const;
	virtual wxString     getHBAddress() const;
	virtual unsigned int getHBPort() const;

private:
	wxString           m_title;
	CCallsignTextCtrl* m_callsign;
	CAddressTextCtrl*  m_address;
	CAddressTextCtrl*  m_icomAddress;
	CPortTextCtrl*     m_icomPort;
	CAddressTextCtrl*  m_hbAddress;
	CPortTextCtrl*     m_hbPort;
};

#endif

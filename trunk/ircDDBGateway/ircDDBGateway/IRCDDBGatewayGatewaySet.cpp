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

#include "DStarDefines.h"
#include "IRCDDBGatewayGatewaySet.h"
#include <cctype>

const unsigned int ADDRESS_WIDTH     = 120U;
const unsigned int PORT_WIDTH        = 80U;

const unsigned int ADDRESS_LENGTH     = 15U;
const unsigned int PORT_LENGTH        = 5U;

const unsigned int BORDER_SIZE = 5U;

CIRCDDBGatewayGatewaySet::CIRCDDBGatewayGatewaySet(wxWindow* parent, int id, const wxString& title, const wxString& callsign, const wxString& address, const wxString& icomAddress, unsigned int icomPort, const wxString& hbAddress, unsigned int hbPort) :
wxPanel(parent, id),
m_title(title),
m_callsign(NULL),
m_address(NULL),
m_icomAddress(NULL),
m_icomPort(NULL),
m_hbAddress(NULL),
m_hbPort(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(3);

	wxStaticText* callsignLabel = new wxStaticText(this, -1, _("Callsign"));
	sizer->Add(callsignLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxString call = callsign;
	call.Truncate(LONG_CALLSIGN_LENGTH - 1U);
	call.Trim();

	m_callsign = new CCallsignTextCtrl(this, -1, call, wxDefaultPosition, wxSize(ADDRESS_WIDTH, -1));
	m_callsign->SetMaxLength(LONG_CALLSIGN_LENGTH);
	sizer->Add(m_callsign, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* gLabel = new wxStaticText(this, -1, wxT("G"));
	sizer->Add(gLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* addressLabel = new wxStaticText(this, -1, _("Gateway Address"));
	sizer->Add(addressLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_address = new CAddressTextCtrl(this, -1, address, wxDefaultPosition, wxSize(ADDRESS_WIDTH, -1));
	m_address->SetMaxLength(ADDRESS_LENGTH);
	sizer->Add(m_address, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* dummy0Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy0Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* icomAddressLabel = new wxStaticText(this, -1, _("Local Icom Address"));
	sizer->Add(icomAddressLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_icomAddress = new CAddressTextCtrl(this, -1, icomAddress, wxDefaultPosition, wxSize(ADDRESS_WIDTH, -1));
	m_icomAddress->SetMaxLength(ADDRESS_LENGTH);
	sizer->Add(m_icomAddress, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* dummy1Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy1Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* icomPortLabel = new wxStaticText(this, -1, _("Local Icom Port"));
	sizer->Add(icomPortLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxString buffer;
	buffer.Printf(wxT("%u"), icomPort);

	m_icomPort = new CPortTextCtrl(this, -1, buffer, wxDefaultPosition, wxSize(PORT_WIDTH, -1));
	m_icomPort->SetMaxLength(PORT_LENGTH);
	sizer->Add(m_icomPort, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* dummy2Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy2Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* hbAddressLabel = new wxStaticText(this, -1, _("Local HB Address"));
	sizer->Add(hbAddressLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_hbAddress = new CAddressTextCtrl(this, -1, hbAddress, wxDefaultPosition, wxSize(ADDRESS_WIDTH, -1));
	m_hbAddress->SetMaxLength(ADDRESS_LENGTH);
	sizer->Add(m_hbAddress, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* dummy3Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy3Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* hbPortLabel = new wxStaticText(this, -1, _("Local HB Port"));
	sizer->Add(hbPortLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	buffer.Printf(wxT("%u"), hbPort);

	m_hbPort = new CPortTextCtrl(this, -1, buffer, wxDefaultPosition, wxSize(PORT_WIDTH, -1));
	m_hbPort->SetMaxLength(PORT_LENGTH);
	sizer->Add(m_hbPort, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	SetAutoLayout(true);

	SetSizer(sizer);
}


CIRCDDBGatewayGatewaySet::~CIRCDDBGatewayGatewaySet()
{
}

bool CIRCDDBGatewayGatewaySet::Validate()
{
	wxString callsign = getCallsign();

	if (callsign.IsEmpty()) {
		wxMessageDialog dialog(this, _("The Gateway Callsign is not valid"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	unsigned int port = getIcomPort();

	if (port == 0U || port > 65535U) {
		wxMessageDialog dialog(this, _("The Icom Port is not valid"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	port = getHBPort();

	if (port == 0U || port > 65535U) {
		wxMessageDialog dialog(this, _("The Homebrew Port is not valid"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	return true;
}

wxString CIRCDDBGatewayGatewaySet::getCallsign() const
{
	wxString callsign = m_callsign->GetValue();

	callsign.MakeUpper();

	return callsign;
}

wxString CIRCDDBGatewayGatewaySet::getAddress() const
{
	return m_address->GetValue();
}

wxString CIRCDDBGatewayGatewaySet::getIcomAddress() const
{
	return m_icomAddress->GetValue();
}

unsigned int CIRCDDBGatewayGatewaySet::getIcomPort() const
{
	unsigned long n;
	m_icomPort->GetValue().ToULong(&n);

	return n;
}

wxString CIRCDDBGatewayGatewaySet::getHBAddress() const
{
	return m_hbAddress->GetValue();
}

unsigned int CIRCDDBGatewayGatewaySet::getHBPort() const
{
	unsigned long n;
	m_hbPort->GetValue().ToULong(&n);

	return n;
}

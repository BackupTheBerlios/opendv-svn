/*
 *   Copyright (C) 2010 by Jonathan Naylor G4KLX
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

#include "DCSGatewayRepeaterSet.h"
#include "DStarDefines.h"

const unsigned int ADDRESS_WIDTH   = 120U;
const unsigned int PORT_WIDTH      = 80U;

const unsigned int ADDRESS_LENGTH  = 15U;
const unsigned int PORT_LENGTH     = 5U;

const unsigned int BORDER_SIZE = 5U;

CDCSGatewayRepeaterSet::CDCSGatewayRepeaterSet(wxWindow* parent, int id, const wxString& title, const wxString& repeaterCallsign, const wxString& repeaterAddress, unsigned int repeaterPort, const wxString& localAddress, unsigned int localPort) :
wxPanel(parent, id),
m_title(title),
m_repeaterCallsign(NULL),
m_repeaterAddress(NULL),
m_repeaterPort(NULL),
m_localAddress(NULL),
m_localPort(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxStaticText* repeaterCallsignLabel = new wxStaticText(this, -1, _("Repeater Callsign"));
	sizer->Add(repeaterCallsignLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_repeaterCallsign = new CCallsignTextCtrl(this, -1, repeaterCallsign, wxDefaultPosition, wxSize(ADDRESS_WIDTH, -1));
	m_repeaterCallsign->SetMaxLength(LONG_CALLSIGN_LENGTH);
	sizer->Add(m_repeaterCallsign, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* repeaterAddressLabel = new wxStaticText(this, -1, _("Repeater Address"));
	sizer->Add(repeaterAddressLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_repeaterAddress = new CAddressTextCtrl(this, -1, repeaterAddress, wxDefaultPosition, wxSize(ADDRESS_WIDTH, -1));
	m_repeaterAddress->SetMaxLength(ADDRESS_LENGTH);
	sizer->Add(m_repeaterAddress, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* repeaterPortLabel = new wxStaticText(this, -1, _("Repeater Port"));
	sizer->Add(repeaterPortLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxString buffer;
	buffer.Printf(wxT("%u"), repeaterPort);

	m_repeaterPort = new CPortTextCtrl(this, -1, buffer, wxDefaultPosition, wxSize(PORT_WIDTH, -1));
	m_repeaterPort->SetMaxLength(PORT_LENGTH);
	sizer->Add(m_repeaterPort, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* localAddressLabel = new wxStaticText(this, -1, _("Local Address"));
	sizer->Add(localAddressLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_localAddress = new CAddressTextCtrl(this, -1, localAddress, wxDefaultPosition, wxSize(ADDRESS_WIDTH, -1));
	m_localAddress->SetMaxLength(ADDRESS_LENGTH);
	sizer->Add(m_localAddress, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* localPortLabel = new wxStaticText(this, -1, _("Local Port"));
	sizer->Add(localPortLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	buffer.Printf(wxT("%u"), localPort);

	m_localPort = new CPortTextCtrl(this, -1, buffer, wxDefaultPosition, wxSize(PORT_WIDTH, -1));
	m_localPort->SetMaxLength(PORT_LENGTH);
	sizer->Add(m_localPort, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	SetAutoLayout(true);

//	sizer->Fit(this);
//	sizer->SetSizeHints(this);

	SetSizer(sizer);
}


CDCSGatewayRepeaterSet::~CDCSGatewayRepeaterSet()
{
}

bool CDCSGatewayRepeaterSet::Validate()
{
	wxString callsign = getRepeaterCallsign();

	if (callsign.IsEmpty()) {
		wxMessageDialog dialog(this, _("The Repeater Callsign is not valid"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	wxString address = getRepeaterAddress();

	if (address.IsEmpty()) {
		wxMessageDialog dialog(this, _("The Repeater Address is not valid"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	unsigned int port = getRepeaterPort();

	if (port == 0U || port > 65535U) {
		wxMessageDialog dialog(this, _("The Repeater Port is not valid"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	port = getLocalPort();

	if (port == 0U || port > 65535U) {
		wxMessageDialog dialog(this, _("The Local Port is not valid"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	return true;
}

wxString CDCSGatewayRepeaterSet::getRepeaterCallsign() const
{
	return m_repeaterCallsign->GetValue();
}

wxString CDCSGatewayRepeaterSet::getRepeaterAddress() const
{
	return m_repeaterAddress->GetValue();
}

unsigned int CDCSGatewayRepeaterSet::getRepeaterPort() const
{
	unsigned long n;

	m_repeaterPort->GetValue().ToULong(&n);

	return n;
}

wxString CDCSGatewayRepeaterSet::getLocalAddress() const
{
	return m_localAddress->GetValue();
}

unsigned int CDCSGatewayRepeaterSet::getLocalPort() const
{
	unsigned long n;

	m_localPort->GetValue().ToULong(&n);

	return n;
}

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

#include "GMSKClientModemSet.h"
#include "GMSKClientDefs.h"

CGMSKClientModemSet::CGMSKClientModemSet(wxWindow* parent, int id, const wxString& title, unsigned int address) :
wxPanel(parent, id),
m_title(title),
m_address(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxStaticText* addressLabel = new wxStaticText(this, -1, _("Address"));
	sizer->Add(addressLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_address = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	m_address->Append(wxT("0x300"));
	m_address->Append(wxT("0x301"));
	m_address->Append(wxT("0x302"));
	m_address->Append(wxT("0x303"));
	sizer->Add(m_address, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_address->SetSelection(address - 0x0300U);

	SetAutoLayout(true);

	sizer->Fit(this);
	sizer->SetSizeHints(this);

	SetSizer(sizer);
}

CGMSKClientModemSet::~CGMSKClientModemSet()
{
}

bool CGMSKClientModemSet::Validate()
{
	return m_address->GetCurrentSelection() != wxNOT_FOUND;
}

unsigned int CGMSKClientModemSet::getAddress() const
{
	int n = m_address->GetCurrentSelection();

	if (n == wxNOT_FOUND)
		return 0x0300U;
	else
		return n + 0x0300U;
}

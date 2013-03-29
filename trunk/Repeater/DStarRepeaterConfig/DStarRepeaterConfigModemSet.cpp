/*
 *   Copyright (C) 2011,2012,2013 by Jonathan Naylor G4KLX
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

#include "DStarRepeaterConfigModemSet.h"

const unsigned int BORDER_SIZE   = 5U;
const unsigned int CONTROL_WIDTH = 100U;


CDStarRepeaterConfigModemSet::CDStarRepeaterConfigModemSet(wxWindow* parent, int id, const wxString& title, const wxString& name) :
wxPanel(parent, id),
m_title(title),
m_name(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(2);

	wxStaticText* versionLabel = new wxStaticText(this, -1, _("Name"));
	sizer->Add(versionLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_name = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	m_name->Append(_("Modem 1"));
	m_name->Append(_("Modem 2"));
	m_name->Append(_("Modem 3"));
	m_name->Append(_("Modem 4"));
	sizer->Add(m_name, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	if (name.IsEmpty()) {
		m_name->SetSelection(0);
	} else {
		int n = m_name->SetStringSelection(name);
		if (n == wxNOT_FOUND)
			m_name->SetSelection(0);
	}

	SetAutoLayout(true);

	sizer->Fit(this);
	sizer->SetSizeHints(this);

	SetSizer(sizer);
}

CDStarRepeaterConfigModemSet::~CDStarRepeaterConfigModemSet()
{
}

bool CDStarRepeaterConfigModemSet::Validate()
{
	return m_name->GetCurrentSelection() != wxNOT_FOUND;
}

wxString CDStarRepeaterConfigModemSet::getName() const
{
	int n = m_name->GetCurrentSelection();

	if (n == wxNOT_FOUND)
		return _("Modem 1");

	return m_name->GetStringSelection();
}

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
 
#include "SplitRepeaterCallsignSet.h"

const unsigned int CALLSIGN_WIDTH  = 80U;
const unsigned int SUFFIX_WIDTH    = 50U;

const unsigned int BORDER_SIZE = 5U;


CSplitRepeaterCallsignSet::CSplitRepeaterCallsignSet(wxWindow* parent, int id, const wxString& title, const wxString& callsign, const wxString& gateway, DSTAR_MODE mode, ACK_TYPE ack, bool restriction, bool rpt1Validation) :
wxPanel(parent, id),
m_title(title),
m_callsign(NULL),
m_gateway(NULL),
m_suffix(NULL),
m_mode(NULL),
m_ack(NULL),
m_restriction(NULL),
m_rpt1Validation(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(3);

	wxStaticText* callsignLabel = new wxStaticText(this, -1, _("Callsign"));
	sizer->Add(callsignLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxString call = callsign;
	call.Append(wxT("        "));
	call.Truncate(LONG_CALLSIGN_LENGTH);

	m_callsign = new CCallsignTextCtrl(this, -1, call.Left(LONG_CALLSIGN_LENGTH - 1U).Trim(), wxDefaultPosition, wxSize(CALLSIGN_WIDTH, -1));
	m_callsign->SetMaxLength(LONG_CALLSIGN_LENGTH - 1U);
	sizer->Add(m_callsign, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	m_suffix = new wxChoice(this, -1, wxDefaultPosition, wxSize(SUFFIX_WIDTH, -1));
#if defined(__WXDEBUG__)
	m_suffix->Append(wxT(" "));
#endif
	m_suffix->Append(wxT("A"));
	m_suffix->Append(wxT("B"));
	m_suffix->Append(wxT("C"));
	m_suffix->Append(wxT("D"));
	sizer->Add(m_suffix, 0, wxALL, BORDER_SIZE);

	wxString suffix = callsign.Right(1U);
	bool res = m_suffix->SetStringSelection(suffix);
	if (!res)
		m_suffix->SetStringSelection(wxT("B"));

	wxStaticText* gatewayLabel = new wxStaticText(this, -1, _("Gateway"));
	sizer->Add(gatewayLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	call = gateway;
	call.Append(wxT("        "));
	call.Truncate(LONG_CALLSIGN_LENGTH);

	m_gateway = new CCallsignTextCtrl(this, -1, call.Left(LONG_CALLSIGN_LENGTH - 1U).Trim(), wxDefaultPosition, wxSize(CALLSIGN_WIDTH, -1));
	m_gateway->SetMaxLength(LONG_CALLSIGN_LENGTH - 1U);
	sizer->Add(m_gateway, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* gLabel = new wxStaticText(this, -1, wxT("G"));
	sizer->Add(gLabel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* modeLabel = new wxStaticText(this, -1, _("Mode"));
	sizer->Add(modeLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_mode = new wxChoice(this, -1, wxDefaultPosition, wxSize(CALLSIGN_WIDTH, -1));
	m_mode->Append(_("Duplex"));
	m_mode->Append(_("Simplex"));
	sizer->Add(m_mode, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_mode->SetSelection(int(mode));

	wxStaticText* dummy1 = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy1, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* ackLabel = new wxStaticText(this, -1, _("Ack"));
	sizer->Add(ackLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_ack = new wxChoice(this, -1, wxDefaultPosition, wxSize(CALLSIGN_WIDTH, -1));
	m_ack->Append(_("Off"));
	m_ack->Append(wxT("BER"));
	m_ack->Append(_("Status"));
	sizer->Add(m_ack, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_ack->SetSelection(int(ack));

	wxStaticText* dummy2 = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy2, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* restrictionLabel = new wxStaticText(this, -1, _("Restrict"));
	sizer->Add(restrictionLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_restriction = new wxChoice(this, -1, wxDefaultPosition, wxSize(CALLSIGN_WIDTH, -1));
	m_restriction->Append(_("Off"));
	m_restriction->Append(_("On"));
	sizer->Add(m_restriction, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_restriction->SetSelection(restriction ? 1 : 0);

	wxStaticText* dummy3 = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy3, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* rpt1ValidationLabel = new wxStaticText(this, -1, _("RPT1 Validation"));
	sizer->Add(rpt1ValidationLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_rpt1Validation = new wxChoice(this, -1, wxDefaultPosition, wxSize(CALLSIGN_WIDTH, -1));
	m_rpt1Validation->Append(_("Off"));
	m_rpt1Validation->Append(_("On"));
	sizer->Add(m_rpt1Validation, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_rpt1Validation->SetSelection(rpt1Validation ? 1 : 0);

	SetAutoLayout(true);

	sizer->Fit(this);
	sizer->SetSizeHints(this);

	SetSizer(sizer);
}


CSplitRepeaterCallsignSet::~CSplitRepeaterCallsignSet()
{
}

bool CSplitRepeaterCallsignSet::Validate()
{
	bool res = getCallsign().IsEmpty();
	if (res) {
		wxMessageDialog dialog(this, _("The Callsign may not be empty"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	res = getGateway().IsEmpty();
	if (res) {
		wxMessageDialog dialog(this, _("The Gateway may not be empty"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	if (m_suffix->GetSelection() == wxNOT_FOUND) {
		wxMessageDialog dialog(this, _("The Callsign suffix is not set"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	if (m_mode->GetSelection() == wxNOT_FOUND) {
		wxMessageDialog dialog(this, _("Mode is not set"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	if (m_ack->GetSelection() == wxNOT_FOUND) {
		wxMessageDialog dialog(this, _("Ack is not set"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	if (m_restriction->GetSelection() == wxNOT_FOUND) {
		wxMessageDialog dialog(this, _("Restrict is not set"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	if (m_rpt1Validation->GetSelection() == wxNOT_FOUND) {
		wxMessageDialog dialog(this, _("RPT1 Validation is not set"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	return true;
}

wxString CSplitRepeaterCallsignSet::getCallsign() const
{
	wxString callsign = m_callsign->GetValue().MakeUpper();
	wxString suffix   = m_suffix->GetStringSelection();

	callsign.Append(wxT("        "));
	callsign.Truncate(LONG_CALLSIGN_LENGTH - 1U);
	callsign.Append(suffix);

	return callsign;
}

wxString CSplitRepeaterCallsignSet::getGateway() const
{
	wxString gateway = m_gateway->GetValue().MakeUpper();

	gateway.Append(wxT("        "));
	gateway.Truncate(LONG_CALLSIGN_LENGTH - 1U);
	gateway.Append(wxT("G"));

	return gateway;
}

DSTAR_MODE CSplitRepeaterCallsignSet::getMode() const
{
	int n = m_mode->GetCurrentSelection();

	return DSTAR_MODE(n);
}

ACK_TYPE CSplitRepeaterCallsignSet::getAck() const
{
	int n = m_ack->GetCurrentSelection();
	if (n == wxNOT_FOUND)
		return AT_BER;

	return ACK_TYPE(n);
}

bool CSplitRepeaterCallsignSet::getRestriction() const
{
	int n = m_restriction->GetCurrentSelection();

	return n == 1;
}

bool CSplitRepeaterCallsignSet::getRPT1Validation() const
{
	int n = m_rpt1Validation->GetCurrentSelection();

	return n == 1;
}
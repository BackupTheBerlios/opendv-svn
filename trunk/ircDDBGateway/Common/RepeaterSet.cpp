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
#include "RepeaterSet.h"
#include "HostFile.h"

#include <wx/filename.h>

const unsigned int CONTROL_WIDTH1 = 130U;
const unsigned int CONTROL_WIDTH2 = 80U;
const unsigned int CONTROL_WIDTH3 = 40U;

const unsigned int DESCRIPTION_LENGTH = 20U;
const unsigned int ADDRESS_LENGTH     = 15U;
const unsigned int PORT_LENGTH        = 5U;
const unsigned int FREQUENCY_LENGTH   = 8U;
const unsigned int OFFSET_LENGTH      = 6U;

const unsigned int BORDER_SIZE = 5U;


const int CHOICE_BAND = 8745;
const int CHOICE_TYPE = 8746;

BEGIN_EVENT_TABLE(CRepeaterSet, wxPanel)
	EVT_CHOICE(CHOICE_BAND, CRepeaterSet::onBand)
	EVT_CHOICE(CHOICE_TYPE, CRepeaterSet::onType)
END_EVENT_TABLE()


CRepeaterSet::CRepeaterSet(wxWindow* parent, int id, const wxString& title, const wxString& band, HW_TYPE type, const wxString& address, unsigned int port, unsigned char band1, unsigned char band2, unsigned char band3, bool dplusEnabled, bool dExtraEnabled, bool dcsEnabled, const wxString& reflector, bool atStartup, RECONNECT reconnect, double frequency, double offset, double range, double latitude, double longitude, double agl, const wxString& description1, const wxString& description2, const wxString& url) :
wxPanel(parent, id),
m_title(title),
m_band(NULL),
m_type(NULL),
m_address(NULL),
m_port(NULL),
m_band1(NULL),
m_band2(NULL),
m_band3(NULL),
m_reflector(NULL),
m_channel(NULL),
m_startup(NULL),
m_reconnect(NULL),
m_frequency(NULL),
m_offset(NULL),
m_range(NULL),
m_latitude(NULL),
m_longitude(NULL),
m_agl(NULL),
m_description1(NULL),
m_description2(NULL),
m_url(NULL)
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(3);

	wxStaticText* bandLabel = new wxStaticText(this, -1, _("Band"));
	sizer->Add(bandLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_band = new wxChoice(this, CHOICE_BAND, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1));
	m_band->Append(_("None"));
	m_band->Append(wxT("A"));
	m_band->Append(wxT("B"));
	m_band->Append(wxT("C"));
	m_band->Append(wxT("D"));
	m_band->Append(wxT("E"));
	m_band->Append(wxT("AD"));
	m_band->Append(wxT("BD"));
	m_band->Append(wxT("CD"));
	m_band->Append(wxT("DD"));
	m_band->Append(wxT("ED"));
	sizer->Add(m_band, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	bool res = m_band->SetStringSelection(band);
	if (!res)
		m_band->SetSelection(0);

	wxStaticText* dummy1Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy1Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* hardwareTypeLabel = new wxStaticText(this, -1, _("Type"));
	sizer->Add(hardwareTypeLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_type = new wxChoice(this, CHOICE_TYPE, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_type->Append(_("Homebrew"));
	m_type->Append(wxT("Icom"));
	sizer->Add(m_type, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_type->SetSelection(int(type));

	wxStaticText* dummy2Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy2Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* addressLabel = new wxStaticText(this, -1, _("Address"));
	sizer->Add(addressLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_address = new CAddressTextCtrl(this, -1, address, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_address->SetMaxLength(ADDRESS_LENGTH);
	sizer->Add(m_address, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* dummy3Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy3Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* portLabel = new wxStaticText(this, -1, _("Port"));
	sizer->Add(portLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxString buffer;
	buffer.Printf(wxT("%u"), port);

	m_port = new CPortTextCtrl(this, -1, buffer, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1));
	m_port->SetMaxLength(PORT_LENGTH);
	sizer->Add(m_port, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* dummy4Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy4Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* icomBandsLabel = new wxStaticText(this, -1, _("Bands"));
	sizer->Add(icomBandsLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxBoxSizer* bandSizer = new wxBoxSizer(wxHORIZONTAL);

	buffer.Printf(wxT("%u"), band1);
	m_band1 = new CPortTextCtrl(this, -1, buffer, wxDefaultPosition, wxSize(CONTROL_WIDTH3, -1));
	m_band1->SetMaxLength(2U);
	bandSizer->Add(m_band1, 0, wxALIGN_LEFT, BORDER_SIZE);

	buffer.Printf(wxT("%u"), band2);
	m_band2 = new CPortTextCtrl(this, -1, buffer, wxDefaultPosition, wxSize(CONTROL_WIDTH3, -1));
	m_band2->SetMaxLength(2U);
	bandSizer->Add(m_band2, 0, wxLEFT | wxALIGN_LEFT, BORDER_SIZE);

	buffer.Printf(wxT("%u"), band3);
	m_band3 = new CPortTextCtrl(this, -1, buffer, wxDefaultPosition, wxSize(CONTROL_WIDTH3, -1));
	m_band3->SetMaxLength(2U);
	bandSizer->Add(m_band3, 0, wxLEFT | wxALIGN_LEFT, BORDER_SIZE);

	sizer->Add(bandSizer, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* dummy0Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy0Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* reflectorLabel = new wxStaticText(this, -1, _("Reflector"));
	sizer->Add(reflectorLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_reflector = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_reflector->Append(_("None"));

	if (dplusEnabled) {
		wxFileName fileName(wxFileName::GetHomeDir(), DPLUS_HOSTS_FILE_NAME);
		if (!fileName.IsFileReadable()) {
#if defined(__WINDOWS__)
			fileName.Assign(::wxGetCwd(), DPLUS_HOSTS_FILE_NAME);
#else
			fileName.Assign(wxT(DATA_DIR), DPLUS_HOSTS_FILE_NAME);
#endif
		}

		CHostFile file(fileName.GetFullPath(), true);

		for (unsigned int i = 0U; i < file.getCount(); i++)
			m_reflector->Append(file.getName(i));
	}

	if (dExtraEnabled) {
		wxFileName fileName(wxFileName::GetHomeDir(), DEXTRA_HOSTS_FILE_NAME);
		if (!fileName.IsFileReadable()) {
#if defined(__WINDOWS__)
			fileName.Assign(::wxGetCwd(), DEXTRA_HOSTS_FILE_NAME);
#else
			fileName.Assign(wxT(DATA_DIR), DEXTRA_HOSTS_FILE_NAME);
#endif
		}

		CHostFile file(fileName.GetFullPath(), true);

		for (unsigned int i = 0U; i < file.getCount(); i++)
			m_reflector->Append(file.getName(i));
	}

	if (dcsEnabled) {
		wxFileName fileName(wxFileName::GetHomeDir(), DCS_HOSTS_FILE_NAME);
		if (!fileName.IsFileReadable()) {
#if defined(__WINDOWS__)
			fileName.Assign(::wxGetCwd(), DCS_HOSTS_FILE_NAME);
#else
			fileName.Assign(wxT(DATA_DIR), DCS_HOSTS_FILE_NAME);
#endif
		}

		CHostFile file(fileName.GetFullPath(), true);

		for (unsigned int i = 0U; i < file.getCount(); i++)
			m_reflector->Append(file.getName(i));
	}

	sizer->Add(m_reflector, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	if (reflector.IsEmpty()) {
		m_reflector->SetSelection(0);
	} else {
		wxString name = reflector;
		name.SetChar(LONG_CALLSIGN_LENGTH - 1U, wxT(' '));
		bool res = m_reflector->SetStringSelection(name);
		if (!res)
			m_reflector->SetSelection(0);
	}

	m_channel = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1));
	m_channel->Append(wxT("A"));
	m_channel->Append(wxT("B"));
	m_channel->Append(wxT("C"));
	m_channel->Append(wxT("D"));
	m_channel->Append(wxT("E"));
	m_channel->Append(wxT("F"));
	m_channel->Append(wxT("G"));
	m_channel->Append(wxT("H"));
	m_channel->Append(wxT("I"));
	m_channel->Append(wxT("J"));
	m_channel->Append(wxT("K"));
	m_channel->Append(wxT("L"));
	m_channel->Append(wxT("M"));
	m_channel->Append(wxT("N"));
	m_channel->Append(wxT("O"));
	m_channel->Append(wxT("P"));
	m_channel->Append(wxT("Q"));
	m_channel->Append(wxT("R"));
	m_channel->Append(wxT("S"));
	m_channel->Append(wxT("T"));
	m_channel->Append(wxT("U"));
	m_channel->Append(wxT("V"));
	m_channel->Append(wxT("W"));
	m_channel->Append(wxT("X"));
	m_channel->Append(wxT("Y"));
	m_channel->Append(wxT("Z"));
	sizer->Add(m_channel, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	res = m_channel->SetStringSelection(reflector.Right(1U));
	if (!res)
		m_channel->SetSelection(0);

	wxStaticText* startupLabel = new wxStaticText(this, -1, _("Startup"));
	sizer->Add(startupLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_startup = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_startup->Append(_("No"));
	m_startup->Append(_("Yes"));
	sizer->Add(m_startup, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_startup->SetSelection(atStartup ? 1 : 0);

	wxStaticText* dummy5Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy5Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* reconnectLabel = new wxStaticText(this, -1, _("Reconnect"));
	sizer->Add(reconnectLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_reconnect = new wxChoice(this, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_reconnect->Append(_("Never"));
	m_reconnect->Append(_("Fixed"));
	m_reconnect->Append(_("5 minutes"));
	m_reconnect->Append(_("10 minutes"));
	m_reconnect->Append(_("15 minutes"));
	m_reconnect->Append(_("20 minutes"));
	m_reconnect->Append(_("25 minutes"));
	m_reconnect->Append(_("30 minutes"));
	m_reconnect->Append(_("60 minutes"));
	m_reconnect->Append(_("90 minutes"));
	m_reconnect->Append(_("120 minutes"));
	m_reconnect->Append(_("180 minutes"));
	sizer->Add(m_reconnect, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);
	m_reconnect->SetSelection(int(reconnect));

	wxStaticText* dummy6Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy6Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* frequencyLabel = new wxStaticText(this, -1, _("Frequency (MHz)"));
	sizer->Add(frequencyLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	buffer.Printf(wxT("%.4lf"), frequency);

	m_frequency = new wxTextCtrl(this, -1, buffer, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_frequency->SetMaxLength(FREQUENCY_LENGTH);
	sizer->Add(m_frequency, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* dummy7Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy7Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* offsetLabel = new wxStaticText(this, -1, _("Offset (MHz)"));
	sizer->Add(offsetLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	buffer.Printf(wxT("%.4lf"), offset);

	m_offset = new wxTextCtrl(this, -1, buffer, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_offset->SetMaxLength(OFFSET_LENGTH);
	sizer->Add(m_offset, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* dummy8Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy8Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* rangeLabel = new wxStaticText(this, -1, _("Range (kms)"));
	sizer->Add(rangeLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	buffer.Printf(wxT("%.0lf"), range);

	m_range = new wxTextCtrl(this, -1, buffer, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1));
	m_range->SetMaxLength(PORT_LENGTH);
	sizer->Add(m_range, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* dummy9Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy9Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* latitudeLabel = new wxStaticText(this, -1, _("Latitude"));
	sizer->Add(latitudeLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	buffer.Printf(wxT("%lf"), latitude);

	m_latitude = new wxTextCtrl(this, -1, buffer, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	sizer->Add(m_latitude, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* dummy10Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy10Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* longitudeLabel = new wxStaticText(this, -1, _("Longitude"));
	sizer->Add(longitudeLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	buffer.Printf(wxT("%lf"), longitude);

	m_longitude = new wxTextCtrl(this, -1, buffer, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	sizer->Add(m_longitude, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* dummy11Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy11Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* aglLabel = new wxStaticText(this, -1, _("AGL (m)"));
	sizer->Add(aglLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	buffer.Printf(wxT("%.0lf"), agl);

	m_agl = new wxTextCtrl(this, -1, buffer, wxDefaultPosition, wxSize(CONTROL_WIDTH2, -1));
	m_agl->SetMaxLength(PORT_LENGTH);
	sizer->Add(m_agl, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* dummy12Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy12Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* descriptionLabel = new wxStaticText(this, -1, _("QTH"));
	sizer->Add(descriptionLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_description1 = new wxTextCtrl(this, -1, description1, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_description1->SetMaxLength(DESCRIPTION_LENGTH);
	sizer->Add(m_description1, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* dummy13Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy13Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* dummy14Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy14Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_description2 = new wxTextCtrl(this, -1, description2, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	m_description2->SetMaxLength(DESCRIPTION_LENGTH);
	sizer->Add(m_description2, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	wxStaticText* dummy15Label = new wxStaticText(this, -1, wxEmptyString);
	sizer->Add(dummy15Label, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	wxStaticText* urlLabel = new wxStaticText(this, -1, _("URL"));
	sizer->Add(urlLabel, 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	m_url = new wxTextCtrl(this, -1, url, wxDefaultPosition, wxSize(CONTROL_WIDTH1, -1));
	sizer->Add(m_url, 0, wxALL | wxALIGN_LEFT, BORDER_SIZE);

	if (isDDMode()) {
		m_type->SetSelection(1);
		m_reflector->SetSelection(0);
		m_channel->SetSelection(0);
		m_startup->SetSelection(0);
		m_reconnect->SetSelection(0);
		m_offset->Clear();

		m_type->Disable();
		m_reflector->Disable();
		m_channel->Disable();
		m_startup->Disable();
		m_reconnect->Disable();
		m_offset->Disable();
	} else {
		m_type->Enable();
		m_reflector->Enable();
		m_channel->Enable();
		m_startup->Enable();
		m_reconnect->Enable();
		m_offset->Enable();
	}

	if (type == HW_ICOM) {
		m_band1->Enable();
		m_band2->Enable();
		m_band3->Enable();
	} else {
		m_band1->Disable();
		m_band2->Disable();
		m_band3->Disable();
	}

	SetAutoLayout(true);

	SetSizer(sizer);
}


CRepeaterSet::~CRepeaterSet()
{
}

bool CRepeaterSet::Validate()
{
	int band = m_band->GetCurrentSelection();
	if (band == wxNOT_FOUND)
		return false;

	if (band == 0)
		return true;

	if (m_type->GetCurrentSelection() == wxNOT_FOUND) {
		wxMessageDialog dialog(this, _("The Hardware Type is not set"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	wxString address = getAddress();

	if (address.IsEmpty()) {
		wxMessageDialog dialog(this, _("The Repeater Address is not valid"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	unsigned int port = getPort();

	if (port == 0U || port > 65535U) {
		wxMessageDialog dialog(this, _("The Repeater Port is not valid"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	if (m_reflector->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	if (m_channel->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	if (m_startup->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	if (m_reconnect->GetCurrentSelection() == wxNOT_FOUND)
		return false;

	double latitude = getLatitude();

	if (latitude < -90.0 || latitude > 90.0) {
		wxMessageDialog dialog(this, _("The Latitude is invalid"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	double longitude = getLongitude();

	if (longitude < -180.0 || longitude > 180.0) {
		wxMessageDialog dialog(this, _("The Longitude is invalid"), m_title + _(" Error"), wxICON_ERROR);
		dialog.ShowModal();
		return false;
	}

	return true;
}

wxString CRepeaterSet::getBand() const
{
	int c = m_band->GetCurrentSelection();

	switch (c) {
		case 0:
			return wxT(" ");
		case 1:
			return wxT("A");
		case 2:
			return wxT("B");
		case 3:
			return wxT("C");
		case 4:
			return wxT("D");
		case 5:
			return wxT("E");
		case 6:
			return wxT("AD");
		case 7:
			return wxT("BD");
		case 8:
			return wxT("CD");
		case 9:
			return wxT("DD");
		case 10:
			return wxT("ED");
		default:
			return wxT(" ");
	}
}

wxString CRepeaterSet::getAddress() const
{
	return m_address->GetValue();
}

HW_TYPE CRepeaterSet::getType() const
{
	if (isDDMode())
		return HW_ICOM;

	int n = m_type->GetCurrentSelection();
	if (n == wxNOT_FOUND)
		n = 0;

	return HW_TYPE(n);
}

unsigned int CRepeaterSet::getPort() const
{
	unsigned long n;
	m_port->GetValue().ToULong(&n);

	return n;
}

unsigned char CRepeaterSet::getBand1() const
{
	unsigned long n;
	m_band1->GetValue().ToULong(&n);

	return n;
}

unsigned char CRepeaterSet::getBand2() const
{
	unsigned long n;
	m_band2->GetValue().ToULong(&n);

	return n;
}

unsigned char CRepeaterSet::getBand3() const
{
	unsigned long n;
	m_band3->GetValue().ToULong(&n);

	return n;
}

wxString CRepeaterSet::getReflector() const
{
	if (isDDMode())
		return wxEmptyString;

	int n = m_reflector->GetCurrentSelection();
	int c = m_channel->GetCurrentSelection();

	if (n == 0)
		return wxEmptyString;

	wxString reflector = m_reflector->GetStringSelection();

	reflector.Append(wxT("        "));
	reflector.Truncate(LONG_CALLSIGN_LENGTH - 1U);
	reflector.Append(wxT('A') + c);

	return reflector;
}

bool CRepeaterSet::atStartup() const
{
	if (isDDMode())
		return false;

	int n = m_startup->GetCurrentSelection();

	return n == 1;
}

RECONNECT CRepeaterSet::getReconnect() const
{
	if (isDDMode())
		return RECONNECT_NEVER;

	int n = m_reconnect->GetCurrentSelection();

	return RECONNECT(n);
}

double CRepeaterSet::getFrequency() const
{
	double n;
	m_frequency->GetValue().ToDouble(&n);

	return n;
}

double CRepeaterSet::getOffset() const
{
	if (isDDMode())
		return 0.0;

	double n;
	m_offset->GetValue().ToDouble(&n);

	return n;
}

double CRepeaterSet::getRange() const
{
	double n;
	m_range->GetValue().ToDouble(&n);

	return n;
}

double CRepeaterSet::getLatitude() const
{
	double val;
	
	m_latitude->GetValue().ToDouble(&val);

	return val;
}

double CRepeaterSet::getLongitude() const
{
	double val;
	
	m_longitude->GetValue().ToDouble(&val);

	return val;
}

wxString CRepeaterSet::getDescription1() const
{
	return m_description1->GetValue();
}

wxString CRepeaterSet::getDescription2() const
{
	return m_description2->GetValue();
}

wxString CRepeaterSet::getURL() const
{
	return m_url->GetValue();
}

double CRepeaterSet::getAGL() const
{
	double n;
	m_agl->GetValue().ToDouble(&n);

	return n;
}

void CRepeaterSet::onBand(wxCommandEvent &event)
{
	if (isDDMode()) {
		m_type->SetSelection(1);
		m_reflector->SetSelection(0);
		m_channel->SetSelection(0);
		m_startup->SetSelection(0);
		m_reconnect->SetSelection(0);
		m_offset->Clear();

		m_type->Disable();
		m_reflector->Disable();
		m_channel->Disable();
		m_startup->Disable();
		m_reconnect->Disable();
		m_offset->Disable();
	} else {
		m_type->Enable();
		m_reflector->Enable();
		m_channel->Enable();
		m_startup->Enable();
		m_reconnect->Enable();
		m_offset->Enable();
	}
}

void CRepeaterSet::onType(wxCommandEvent &event)
{
	int n = m_type->GetCurrentSelection();
	if (n != 1) {
		m_band1->Disable();
		m_band2->Disable();
		m_band3->Disable();
	} else {
		m_band1->Enable();
		m_band2->Enable();
		m_band3->Enable();
	}
}

bool CRepeaterSet::isDDMode() const
{
	int c = m_band->GetCurrentSelection();

	switch (c) {
		case 5:
		case 6:
		case 7:
		case 8:
			return true;
		default:
			return false;
	}
}

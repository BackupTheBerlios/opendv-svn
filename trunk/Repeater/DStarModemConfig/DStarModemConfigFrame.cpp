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

#include "DStarModemConfigFrame.h"
#include "DStarModemDVRPTR1Set.h"
#include "DStarModemDVRPTR2Set.h"
#include "DStarModemConfigApp.h"
#include "DStarModemGMSKSet.h"
#include "DStarModemDVAPSet.h"
#include "DStarModemConfig.h"
#include "DStarDefines.h"
#include "Version.h"

#include <wx/aboutdlg.h>

enum {
	Choice_Name = 7088,
	Button_Configure
};

BEGIN_EVENT_TABLE(CDStarModemConfigFrame, wxFrame)
	EVT_MENU(wxID_EXIT,          CDStarModemConfigFrame::onQuit)
	EVT_MENU(wxID_ABOUT,         CDStarModemConfigFrame::onAbout)

	EVT_CHOICE(Choice_Name,      CDStarModemConfigFrame::onName)

	EVT_BUTTON(Button_Configure, CDStarModemConfigFrame::onConfigure)

	EVT_CLOSE(CDStarModemConfigFrame::onClose)
END_EVENT_TABLE()

#if defined(__WINDOWS__)
const unsigned int BORDER_SIZE    = 5U;
const unsigned int CONTROL_WIDTH  = 100U;
const unsigned int LABEL_WIDTH    = 100U;
#else
const unsigned int BORDER_SIZE    = 5U;
const unsigned int CONTROL_WIDTH  = 150U;
const unsigned int LABEL_WIDTH    = 150U;
#endif

#if defined(__WINDOWS__)
CDStarModemConfigFrame::CDStarModemConfigFrame(const wxString& title) :
wxFrame(NULL, -1, title),
#else
CDStarModemConfigFrame::CDStarModemConfigFrame(const wxString& title, const wxString& confDir) :
wxFrame(NULL, -1, title),
m_confDir(confDir),
#endif
m_name(NULL),
m_type(NULL)
{
	SetMenuBar(createMenuBar());

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	wxPanel* panel = new wxPanel(this, -1);

	wxFlexGridSizer* sizer = new wxFlexGridSizer(3);

	wxStaticText* dummy1 = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(LABEL_WIDTH, -1));
	sizer->Add(dummy1, 0, wxALL, BORDER_SIZE);

	wxStaticText* dummy2 = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	sizer->Add(dummy2, 0, wxALL, BORDER_SIZE);

	wxStaticText* dummy3 = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(LABEL_WIDTH, -1));
	sizer->Add(dummy3, 0, wxALL, BORDER_SIZE);

	wxStaticText* dummy4 = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(LABEL_WIDTH, -1));
	sizer->Add(dummy4, 0, wxALL, BORDER_SIZE);

	m_name = new wxChoice(panel, Choice_Name, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	m_name->Append(wxT("GMSK 1"));
	m_name->Append(wxT("GMSK 2"));
	m_name->Append(wxT("GMSK 3"));
	m_name->Append(wxT("GMSK 4"));
	sizer->Add(m_name, 0, wxALL, BORDER_SIZE);

	wxStaticText* dummy5 = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(LABEL_WIDTH, -1));
	sizer->Add(dummy5, 0, wxALL, BORDER_SIZE);

	wxStaticText* dummy6 = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(LABEL_WIDTH, -1));
	sizer->Add(dummy6, 0, wxALL, BORDER_SIZE);

	m_type = new wxChoice(panel, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	m_type->Append(_("None"));
	m_type->Append(wxT("DVAP"));
	m_type->Append(wxT("GMSK Modem"));
	m_type->Append(wxT("DV-RPTR V1"));
	m_type->Append(wxT("DV-RPTR V2"));
	m_type->Append(wxT("Raspberry PI"));
	sizer->Add(m_type, 0, wxALL, BORDER_SIZE);

	wxStaticText* dummy7 = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(LABEL_WIDTH, -1));
	sizer->Add(dummy7, 0, wxALL, BORDER_SIZE);

	wxStaticText* dummy8 = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(LABEL_WIDTH, -1));
	sizer->Add(dummy8, 0, wxALL, BORDER_SIZE);

	wxButton* configure = new wxButton(panel, Button_Configure, _("Configure..."), wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	sizer->Add(configure, 0, wxALL, BORDER_SIZE);

	wxStaticText* dummy9 = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(LABEL_WIDTH, -1));
	sizer->Add(dummy9, 0, wxALL, BORDER_SIZE);

	wxStaticText* dummy10 = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(LABEL_WIDTH, -1));
	sizer->Add(dummy10, 0, wxALL, BORDER_SIZE);

#if defined(__WINDOWS__)
	CDStarModemConfig config(new wxConfig(APPLICATION_NAME), wxT("GMSK 1"));
#else
	if (m_confDir.IsEmpty())
		m_confDir = CONF_DIR;

	CDStarModemConfig config(m_confDir, CONFIG_FILE_NAME, wxT("GMSK 1"));
#endif

	MODEM_TYPE type;
	config.getType(type);

	m_name->SetSelection(0);
	m_type->SetSelection(int(type));

	panel->SetAutoLayout(true);

	// sizer->Fit(panel);
	// sizer->SetSizeHints(panel);

	panel->SetSizer(sizer);

	mainSizer->Add(panel);

	SetSizer(mainSizer);
	mainSizer->SetSizeHints(this);
}

CDStarModemConfigFrame::~CDStarModemConfigFrame()
{
}

wxMenuBar* CDStarModemConfigFrame::createMenuBar()
{
	wxMenu* fileMenu = new wxMenu();
	fileMenu->Append(wxID_EXIT, _("Exit"));

	wxMenu* helpMenu = new wxMenu();
	helpMenu->Append(wxID_ABOUT, _("About D-Star Modem Config"));

	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append(fileMenu,     _("File"));
	menuBar->Append(helpMenu,     _("Help"));

	return menuBar;
}

void CDStarModemConfigFrame::onQuit(wxCommandEvent& event)
{
	Close(false);
}

void CDStarModemConfigFrame::onClose(wxCloseEvent& event)
{
	Destroy();
}

void CDStarModemConfigFrame::onAbout(wxCommandEvent& event)
{
	wxAboutDialogInfo info;
	info.AddDeveloper(wxT("Jonathan Naylor, G4KLX"));
	info.SetCopyright(wxT("(C) 2013 using GPL v2 or later"));
	info.SetName(APPLICATION_NAME);
	info.SetVersion(VERSION);
	info.SetDescription(_("This program configures the D-Star Modem."));

	::wxAboutBox(info);
}

void CDStarModemConfigFrame::onName(wxCommandEvent& event)
{
	int n = m_name->GetSelection();
	if (n == wxNOT_FOUND)
		return;

	wxString name = m_name->GetStringSelection();

#if defined(__WINDOWS__)
	CDStarModemConfig config(new wxConfig(APPLICATION_NAME), name);
#else
	if (m_confDir.IsEmpty())
		m_confDir = CONF_DIR;

	CDStarModemConfig config(m_confDir, CONFIG_FILE_NAME, name);
#endif

	MODEM_TYPE type;
	config.getType(type);

	m_type->SetSelection(int(type));
}

void CDStarModemConfigFrame::onConfigure(wxCommandEvent& event)
{
	int n = m_name->GetSelection();
	if (n == wxNOT_FOUND)
		return;

	n = m_type->GetSelection();
	if (n == wxNOT_FOUND)
		return;

	wxString name = m_name->GetStringSelection();

#if defined(__WINDOWS__)
	CDStarModemConfig config(new wxConfig(APPLICATION_NAME), name);
#else
	if (m_confDir.IsEmpty())
		m_confDir = CONF_DIR;

	CDStarModemConfig config(m_confDir, CONFIG_FILE_NAME, name);
#endif

	MODEM_TYPE type = MODEM_TYPE(n);

	switch (type) {
		case MT_NONE:
			config.setType(MT_NONE);
			config.write();
			break;

		case MT_RASPBERRY_PI:
			config.setType(MT_RASPBERRY_PI);
			config.write();
			break;

		case MT_DVAP: {
				wxString port;
				unsigned int frequency;
				int power, squelch;
				config.getDVAP(port, frequency, power, squelch);
				CDStarModemDVAPSet modem(this, -1, port, frequency, power, squelch);
				if (modem.ShowModal() == wxID_OK) {
					if (modem.Validate()) {
						port      = modem.getPort();
						frequency = modem.getFrequency();
						power     = modem.getPower();
						squelch   = modem.getSquelch();
						config.setType(MT_DVAP);
						config.setDVAP(port, frequency, power, squelch);
						config.write();
					}
				}
			}
			break;

		case MT_GMSK_MODEM: {
				USB_INTERFACE type;
				unsigned int address;
				config.getGMSK(type, address);
				CDStarModemGMSKSet modem(this, -1, type, address);
				if (modem.ShowModal() == wxID_OK) {
					if (modem.Validate()) {
						type    = modem.getType();
						address = modem.getAddress();
						config.setType(MT_GMSK_MODEM);
						config.setGMSK(type, address);
						config.write();
					}
				}
			}
			break;

		case MT_DVRPTR_V1: {
				wxString port;
				bool txInvert, rxInvert, channel;
				unsigned int modLevel, txDelay;
				config.getDVRPTR1(port, rxInvert, txInvert, channel, modLevel, txDelay);
				CDStarModemDVRPTR1Set modem(this, -1, port, rxInvert, txInvert, channel, modLevel, txDelay);
				if (modem.ShowModal() == wxID_OK) {
					if (modem.Validate()) {
						port     = modem.getPort();
						rxInvert = modem.getRXInvert();
						txInvert = modem.getTXInvert();
						channel  = modem.getChannel();
						modLevel = modem.getModLevel();
						txDelay  = modem.getTXDelay();
						config.setType(MT_DVRPTR_V1);
						config.setDVRPTR1(port, rxInvert, txInvert, channel, modLevel, txDelay);
						config.write();
					}
				}
			}
			break;

		case MT_DVRPTR_V2: {
				CONNECTION_TYPE type;
				wxString usbPort, address;
				bool txInvert;
				unsigned int port, modLevel;
				config.getDVRPTR2(type, usbPort, address, port, txInvert, modLevel);
				CDStarModemDVRPTR2Set modem(this, -1, type, usbPort, address, port, txInvert, modLevel);
				if (modem.ShowModal() == wxID_OK) {
					if (modem.Validate()) {
						type     = modem.getConnectionType();
						usbPort  = modem.getUSBPort();
						address  = modem.getAddress();
						port     = modem.getPort();
						txInvert = modem.getTXInvert();
						modLevel = modem.getModLevel();
						config.setType(MT_DVRPTR_V2);
						config.setDVRPTR2(type, usbPort, address, port, txInvert, modLevel);
						config.write();
					}
				}
			}
			break;

		default:
			break;
	}
}

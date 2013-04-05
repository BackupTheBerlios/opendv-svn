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
#include "DStarModemConfigApp.h"
#include "DStarModemConfig.h"
#include "DStarDefines.h"
#include "Version.h"

#include <wx/aboutdlg.h>

enum {
	Button_Configure = 7088
};

BEGIN_EVENT_TABLE(CDStarModemConfigFrame, wxFrame)
	EVT_MENU(wxID_EXIT,          CDStarModemConfigFrame::onQuit)
	EVT_MENU(wxID_ABOUT,         CDStarModemConfigFrame::onAbout)

	EVT_BUTTON(Button_Configure, CDStarModemConfigFrame::onConfigure)

	EVT_CLOSE(CDStarModemConfigFrame::onClose)
END_EVENT_TABLE()

#if defined(__WINDOWS__)
const unsigned int BORDER_SIZE    = 5U;
const unsigned int CONTROL_WIDTH  = 200U;
const unsigned int LABEL_WIDTH    = 70U;
#else
const unsigned int BORDER_SIZE    = 5U;
const unsigned int CONTROL_WIDTH  = 300U;
const unsigned int LABEL_WIDTH    = 70U;
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

	wxFlexGridSizer* sizer = new wxFlexGridSizer(1);

	m_name = new wxChoice(panel, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	m_name->Append(wxT("GMSK 1"));
	m_name->Append(wxT("GMSK 2"));
	m_name->Append(wxT("GMSK 3"));
	m_name->Append(wxT("GMSK 4"));
	sizer->Add(m_name, 0, wxALL, BORDER_SIZE);

	m_type = new wxChoice(panel, -1, wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	m_type->Append(wxT("DVAP"));
	m_type->Append(wxT("GMSK Modem"));
	m_type->Append(wxT("DV-RPTR V1"));
	m_type->Append(wxT("DV-RPTR V2"));
	m_type->Append(wxT("Raspberry PI"));
	sizer->Add(m_type, 0, wxALL, BORDER_SIZE);

	wxButton* configure = new wxButton(panel, Button_Configure, _("Configure..."), wxDefaultPosition, wxSize(CONTROL_WIDTH, -1));
	sizer->Add(configure, 0, wxALL, BORDER_SIZE);

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

	// panel->SetAutoLayout(true);

	// sizer->Fit(this);
	// sizer->SetSizeHints(this);

	panel->SetSizer(sizer);

	mainSizer->Add(panel);

	SetSizer(mainSizer);
	// mainSizer->SetSizeHints(this);
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

void CDStarModemConfigFrame::onConfigure(wxCommandEvent& event)
{
}

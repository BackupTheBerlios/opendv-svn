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

#include "DStarRepeaterConfigPreferences.h"
#include "DStarRepeaterConfigFrame.h"
#include "DStarRepeaterConfigApp.h"
#include "DStarDefines.h"
#include "Version.h"

#include <wx/aboutdlg.h>

enum {
	Menu_Edit_Preferences = 6000,
};

BEGIN_EVENT_TABLE(CDStarRepeaterConfigFrame, wxFrame)
	EVT_MENU(wxID_EXIT,             CDStarRepeaterConfigFrame::onQuit)
	EVT_MENU(Menu_Edit_Preferences, CDStarRepeaterConfigFrame::onPreferences)
	EVT_MENU(wxID_ABOUT,            CDStarRepeaterConfigFrame::onAbout)

	EVT_CLOSE(CDStarRepeaterConfigFrame::onClose)
END_EVENT_TABLE()

#if defined(__WINDOWS__)
const unsigned int BORDER_SIZE    = 5U;
const unsigned int CONTROL_WIDTH  = 100U;
const unsigned int LABEL_WIDTH    = 70U;
#else
const unsigned int BORDER_SIZE    = 5U;
const unsigned int CONTROL_WIDTH  = 150U;
const unsigned int LABEL_WIDTH    = 70U;
#endif

CDStarRepeaterConfigFrame::CDStarRepeaterConfigFrame(const wxString& title) :
wxFrame(NULL, -1, title)
{
	SetMenuBar(createMenuBar());

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	wxPanel* panel = new wxPanel(this, -1, wxDefaultPosition, wxSize(500, 300));

	mainSizer->Add(panel);

	SetSizer(mainSizer);
	mainSizer->SetSizeHints(this);
}

CDStarRepeaterConfigFrame::~CDStarRepeaterConfigFrame()
{
}

wxMenuBar* CDStarRepeaterConfigFrame::createMenuBar()
{
	wxMenu* fileMenu = new wxMenu();
	fileMenu->Append(wxID_EXIT, _("Exit"));

	wxMenu* editMenu = new wxMenu();
	editMenu->Append(Menu_Edit_Preferences, _("Preferences..."));

	wxMenu* helpMenu = new wxMenu();
	helpMenu->Append(wxID_ABOUT, _("About D-Star Repeater Config"));

	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append(fileMenu,     _("File"));
	menuBar->Append(editMenu,     _("Edit"));
	menuBar->Append(helpMenu,     _("Help"));

	return menuBar;
}

void CDStarRepeaterConfigFrame::onQuit(wxCommandEvent& event)
{
	Close(false);
}

void CDStarRepeaterConfigFrame::onClose(wxCloseEvent& event)
{
	Destroy();
}

void CDStarRepeaterConfigFrame::onAbout(wxCommandEvent& event)
{
	wxAboutDialogInfo info;
	info.AddDeveloper(wxT("Jonathan Naylor, G4KLX"));
	info.SetCopyright(wxT("(C) 2013 using GPL v2 or later"));
	info.SetName(APPLICATION_NAME);
	info.SetVersion(VERSION);
	info.SetDescription(_("This program configures the D-Star Repeater."));

	::wxAboutBox(info);
}

void CDStarRepeaterConfigFrame::onPreferences(wxCommandEvent& event)
{
	wxString callsign, gateway;
	DSTAR_MODE mode;
	ACK_TYPE ack;
	bool restriction, rpt1Validation, dtmfBlanking;
	::wxGetApp().getCallsign(callsign, gateway, mode, ack, restriction, rpt1Validation, dtmfBlanking);

	wxString gatewayAddress, localAddress;
	unsigned int gatewayPort, localPort;
	::wxGetApp().getNetwork(gatewayAddress, gatewayPort, localAddress, localPort);

	unsigned int timeout, ackTime;
	::wxGetApp().getTimes(timeout, ackTime);

	unsigned int beaconTime;
	wxString beaconText;
	bool beaconVoice;
	TEXT_LANG language;
	::wxGetApp().getBeacon(beaconTime, beaconText, beaconVoice, language);

	bool announcementEnabled;
	unsigned int announcementTime;
	wxString announcementRecordRPT1, announcementRecordRPT2;
	wxString announcementDeleteRPT1, announcementDeleteRPT2;
	::wxGetApp().getAnnouncement(announcementEnabled, announcementTime, announcementRecordRPT1, announcementRecordRPT2, announcementDeleteRPT1, announcementDeleteRPT2);

	wxString modemName;
	::wxGetApp().getModem(modemName);

	bool enabled;
	wxString rpt1Callsign, rpt2Callsign;
	wxString shutdown, startup;
	wxString status1, status2, status3, status4, status5;
	wxString command1, command1Line, command2, command2Line;
	wxString command3, command3Line, command4, command4Line;
	wxString output1, output2, output3, output4;
	::wxGetApp().getControl(enabled, rpt1Callsign, rpt2Callsign, shutdown, startup, status1, status2, status3, status4, status5, command1, command1Line, command2, command2Line, command3, command3Line, command4, command4Line, output1, output2, output3, output4);

	wxString controllerType;
	unsigned int activeHangTime;
	::wxGetApp().getController(controllerType, activeHangTime);

	CDStarRepeaterConfigPreferences dialog1(this, -1, callsign, gateway, mode, ack, restriction, rpt1Validation, dtmfBlanking,
		gatewayAddress, gatewayPort, localAddress, localPort, timeout, ackTime, beaconTime, beaconText, beaconVoice, language,
		announcementEnabled, announcementTime, announcementRecordRPT1, announcementRecordRPT2, announcementDeleteRPT1,
		announcementDeleteRPT2, modemName, enabled, rpt1Callsign, rpt2Callsign, shutdown, startup, status1, status2, status3,
		status4, status5, command1, command1Line, command2, command2Line, command3, command3Line, command4, command4Line,
		output1, output2, output3, output4, controllerType, activeHangTime);
	if (dialog1.ShowModal() != wxID_OK)
		return;

	callsign       = dialog1.getCallsign();
	gateway        = dialog1.getGateway();
	mode           = dialog1.getMode();
	ack            = dialog1.getAck();
	restriction    = dialog1.getRestriction();
	rpt1Validation = dialog1.getRPT1Validation();
	dtmfBlanking   = dialog1.getDTMFBlanking();
	::wxGetApp().setCallsign(callsign, gateway, mode, ack, restriction, rpt1Validation, dtmfBlanking);

	gatewayAddress = dialog1.getGatewayAddress();
	gatewayPort    = dialog1.getGatewayPort();
	localAddress   = dialog1.getLocalAddress();
	localPort      = dialog1.getLocalPort();
	::wxGetApp().setNetwork(gatewayAddress, gatewayPort, localAddress, localPort);

	timeout    = dialog1.getTimeout();
	ackTime    = dialog1.getAckTime();
	::wxGetApp().setTimes(timeout, ackTime);

	beaconTime  = dialog1.getBeaconTime();
	beaconText  = dialog1.getBeaconText();
	beaconVoice = dialog1.getBeaconVoice();
	language    = dialog1.getLanguage();
	::wxGetApp().setBeacon(beaconTime, beaconText, beaconVoice, language);

	announcementEnabled    = dialog1.getAnnouncementEnabled();
	announcementTime       = dialog1.getAnnouncementTime();
	announcementRecordRPT1 = dialog1.getAnnouncementRecordRPT1();
	announcementRecordRPT2 = dialog1.getAnnouncementRecordRPT2();
	announcementDeleteRPT1 = dialog1.getAnnouncementDeleteRPT1();
	announcementDeleteRPT2 = dialog1.getAnnouncementDeleteRPT2();
	::wxGetApp().setAnnouncement(announcementEnabled, announcementTime, announcementRecordRPT1, announcementRecordRPT2, announcementDeleteRPT1, announcementDeleteRPT2);

	modemName    = dialog1.getModemName();
	::wxGetApp().setModem(modemName);

	enabled      = dialog1.getEnabled();
	rpt1Callsign = dialog1.getRPT1Callsign();
	rpt2Callsign = dialog1.getRPT2Callsign();
	shutdown     = dialog1.getShutdown();
	startup      = dialog1.getStartup();
	status1      = dialog1.getStatus1();
	status2      = dialog1.getStatus2();
	status3      = dialog1.getStatus3();
	status4      = dialog1.getStatus4();
	status5      = dialog1.getStatus5();
	output1      = dialog1.getOutput1();
	output2      = dialog1.getOutput2();
	output3      = dialog1.getOutput3();
	output4      = dialog1.getOutput4();
	command1     = dialog1.getCommand1();
	command1Line = dialog1.getCommand1Line();
	command2     = dialog1.getCommand2();
	command2Line = dialog1.getCommand2Line();
	command3     = dialog1.getCommand3();
	command3Line = dialog1.getCommand3Line();
	command4     = dialog1.getCommand4();
	command4Line = dialog1.getCommand4Line();
	::wxGetApp().setControl(enabled, rpt1Callsign, rpt2Callsign, shutdown, startup, status1, status2, status3, status4, status5, command1, command1Line, command2, command2Line, command3, command3Line, command4, command4Line, output1, output2, output3, output4);

	controllerType = dialog1.getControllerType();
	activeHangTime = dialog1.getActiveHangTime();
	::wxGetApp().setController(controllerType, activeHangTime);

	::wxGetApp().writeConfig();

	wxMessageDialog dialog2(this, _("The changes made will not take effect\nuntil the application is restarted"), _("D-Star Repeater Information"), wxICON_INFORMATION);
	dialog2.ShowModal();
}

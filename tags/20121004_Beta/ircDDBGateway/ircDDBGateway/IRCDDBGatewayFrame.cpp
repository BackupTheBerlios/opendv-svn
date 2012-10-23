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

#include "IRCDDBGatewayPreferences.h"
#include "IRCDDBGatewayStatusData.h"
#include "IRCDDBGatewayFrame.h"
#include "IRCDDBGatewayDefs.h"
#include "IRCDDBGatewayApp.h"
#include "LogEvent.h"
#include "Version.h"

const unsigned int BORDER_SIZE = 5U;

#if defined(__WINDOWS__)
const unsigned int IRCDDB_WIDTH   = 230U;
const unsigned int LINKED_WIDTH   = 160U;
const unsigned int INCOMING_WIDTH = 320U;
const unsigned int DONGLES_WIDTH  = 560U;
const unsigned int LOGTEXT_WIDTH  = 560U;
#else
const unsigned int IRCDDB_WIDTH   = 288U;
const unsigned int LINKED_WIDTH   = 220U;
const unsigned int INCOMING_WIDTH = 385U;
const unsigned int DONGLES_WIDTH  = 700U;
const unsigned int LOGTEXT_WIDTH  = 700U;
#endif

#include <wx/gbsizer.h>
#include <wx/aboutdlg.h>

DEFINE_EVENT_TYPE(LOG_EVENT)

enum {
	Menu_Edit_Preferences = 6000,
	Menu_View_Updates
};

BEGIN_EVENT_TABLE(CIRCDDBGatewayFrame, wxFrame)
	EVT_MENU(wxID_EXIT, CIRCDDBGatewayFrame::onQuit)
	EVT_MENU(Menu_Edit_Preferences, CIRCDDBGatewayFrame::onPreferences)
	EVT_MENU(Menu_View_Updates, CIRCDDBGatewayFrame::onUpdates)
	EVT_MENU(wxID_ABOUT, CIRCDDBGatewayFrame::onAbout)

	EVT_TIMER(-1, CIRCDDBGatewayFrame::onTimer)

	EVT_CLOSE(CIRCDDBGatewayFrame::onClose)

	EVT_CUSTOM(LOG_EVENT, wxID_ANY, CIRCDDBGatewayFrame::onLog)
END_EVENT_TABLE()

CIRCDDBGatewayFrame::CIRCDDBGatewayFrame(const wxString& title, const wxPoint& position, bool gui) :
wxFrame(NULL, -1, title, position),
m_timer(this),
m_ircDDBStatus(NULL),
m_dprsStatus(NULL),
#if defined(__WXDEBUG__)
m_updates(true)
#else
m_updates(gui)
#endif
{
	SetMenuBar(createMenuBar());

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	wxPanel* panel = new wxPanel(this);

	wxBoxSizer* panelSizer = new wxBoxSizer(wxVERTICAL);

	wxStaticBoxSizer* status1Sizer = new wxStaticBoxSizer(new wxStaticBox(panel, -1, _("Status")), wxVERTICAL);
	wxFlexGridSizer* status2Sizer = new wxFlexGridSizer(4);

	wxStaticText* ircDDBStatusLabel = new wxStaticText(panel, -1, _("ircDDB:"));
	status2Sizer->Add(ircDDBStatusLabel, 0, wxALL, BORDER_SIZE);

	m_ircDDBStatus = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(IRCDDB_WIDTH, -1));
	status2Sizer->Add(m_ircDDBStatus, 0, wxALL, BORDER_SIZE);

	wxStaticText* dprsStatusLabel = new wxStaticText(panel, -1, _("D-PRS:"));
	status2Sizer->Add(dprsStatusLabel, 0, wxALL, BORDER_SIZE);

	m_dprsStatus = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(IRCDDB_WIDTH, -1));
	status2Sizer->Add(m_dprsStatus, 0, wxALL, BORDER_SIZE);

	status1Sizer->Add(status2Sizer);
	panelSizer->Add(status1Sizer, 0, wxALL, BORDER_SIZE);

	wxStaticBoxSizer* links1Sizer = new wxStaticBoxSizer(new wxStaticBox(panel, -1, _("Links")), wxVERTICAL);
	wxFlexGridSizer* links2Sizer = new wxFlexGridSizer(3);

	for (unsigned int i = 0U; i < 4U; i++) {
		wxString text;
		text.Printf(_("Repeater %u:"), i + 1U);

		wxStaticText* repeaterLabel = new wxStaticText(panel, -1, text);
		links2Sizer->Add(repeaterLabel, 0, wxALL, BORDER_SIZE);

		m_linkStatus[i] = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(LINKED_WIDTH, -1));
		links2Sizer->Add(m_linkStatus[i], 0, wxALL, BORDER_SIZE);

		m_incoming[i] = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(INCOMING_WIDTH, -1));
		links2Sizer->Add(m_incoming[i], 0, wxALL, BORDER_SIZE);
	}

	links1Sizer->Add(links2Sizer);
	panelSizer->Add(links1Sizer, 0, wxALL, BORDER_SIZE);

	wxStaticBoxSizer* dongles1Sizer = new wxStaticBoxSizer(new wxStaticBox(panel, -1, _("Dongles")), wxVERTICAL);
	wxFlexGridSizer* dongles2Sizer = new wxFlexGridSizer(5);

	m_dongles = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(DONGLES_WIDTH, -1));
	dongles2Sizer->Add(m_dongles, 0, wxALL, BORDER_SIZE);

	dongles1Sizer->Add(dongles2Sizer);
	panelSizer->Add(dongles1Sizer, 0, wxALL, BORDER_SIZE);

	wxStaticBoxSizer* log1Sizer = new wxStaticBoxSizer(new wxStaticBox(panel, -1, _("Log")), wxVERTICAL);
	wxBoxSizer* log2Sizer = new wxBoxSizer(wxVERTICAL);

	for (unsigned int i = 0U; i < 7U; i++) {
		m_logLine[i] = new wxStaticText(panel, -1, wxEmptyString, wxDefaultPosition, wxSize(LOGTEXT_WIDTH, -1));
		m_logLine[i]->Wrap(LOGTEXT_WIDTH);
		log2Sizer->Add(m_logLine[i], 0, wxTOP | wxLEFT | wxRIGHT, BORDER_SIZE);
	}

	log1Sizer->Add(log2Sizer);
	panelSizer->Add(log1Sizer, 0, wxALL, BORDER_SIZE);

	panel->SetSizer(panelSizer);
	panelSizer->SetSizeHints(panel);

	mainSizer->Add(panel);

	SetSizer(mainSizer);
	mainSizer->SetSizeHints(this);

	m_timer.Start(5000);
}

CIRCDDBGatewayFrame::~CIRCDDBGatewayFrame()
{
}

wxMenuBar* CIRCDDBGatewayFrame::createMenuBar()
{
	wxMenu* fileMenu = new wxMenu();
	fileMenu->Append(wxID_EXIT, _("Exit"));

	wxMenu* editMenu = new wxMenu();
	editMenu->Append(Menu_Edit_Preferences, _("Preferences..."));

	wxMenu* viewMenu = new wxMenu();
	viewMenu->AppendCheckItem(Menu_View_Updates, _("GUI Updates"));
	viewMenu->Check(Menu_View_Updates, m_updates);

	wxMenu* helpMenu = new wxMenu();
	helpMenu->Append(wxID_ABOUT, _("About ircDDB Gateway"));

	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append(fileMenu, _("File"));
	menuBar->Append(editMenu, _("Edit"));
	menuBar->Append(viewMenu, _("View"));
	menuBar->Append(helpMenu, _("Help"));

	return menuBar;
}

void CIRCDDBGatewayFrame::onQuit(wxCommandEvent& event)
{
	Close(false);
}

void CIRCDDBGatewayFrame::onClose(wxCloseEvent& event)
{
	int x, y;
	GetPosition(&x, &y);
	if (x >= 0 && y >= 0) {
		::wxGetApp().setPosition(x, y);
		::wxGetApp().writeConfig();
	}

	Destroy();
}

void CIRCDDBGatewayFrame::onPreferences(wxCommandEvent& event)
{
	wxString gatewayCallsign, gatewayAddress, icomAddress, hbAddress, description1, description2, url;
	unsigned int icomPort, hbPort;
	double latitude, longitude;
	::wxGetApp().getGateway(gatewayCallsign, gatewayAddress, icomAddress, icomPort, hbAddress, hbPort, latitude, longitude, description1, description2, url);

	wxString repeaterCall1, repeaterBand1, repeaterAddress1, reflector1;
	double frequency1, offset1, range1, latitude1, longitude1, agl1;
	unsigned char band11, band12, band13;
	unsigned int repeaterPort1;
	HW_TYPE repeaterType1;
	bool atStartup1;
	RECONNECT reconnect1;
	::wxGetApp().getRepeater1(repeaterCall1, repeaterBand1, repeaterType1, repeaterAddress1, repeaterPort1, band11, band12, band13, reflector1, atStartup1, reconnect1, frequency1, offset1, range1, latitude1, longitude1, agl1);

	wxString repeaterCall2, repeaterBand2, repeaterAddress2, reflector2;
	double frequency2, offset2, range2, latitude2, longitude2, agl2;
	unsigned char band21, band22, band23;
	unsigned int repeaterPort2;
	HW_TYPE repeaterType2;
	bool atStartup2;
	RECONNECT reconnect2;
	::wxGetApp().getRepeater2(repeaterCall2, repeaterBand2, repeaterType2, repeaterAddress2, repeaterPort2, band21, band22, band23, reflector2, atStartup2, reconnect2, frequency2, offset2, range2, latitude2, longitude2, agl2);

	wxString repeaterCall3, repeaterBand3, repeaterAddress3, reflector3;
	double frequency3, offset3, range3, latitude3, longitude3, agl3;
	unsigned char band31, band32, band33;
	unsigned int repeaterPort3;
	HW_TYPE repeaterType3;
	bool atStartup3;
	RECONNECT reconnect3;
	::wxGetApp().getRepeater3(repeaterCall3, repeaterBand3, repeaterType3, repeaterAddress3, repeaterPort3, band31, band32, band33, reflector3, atStartup3, reconnect3, frequency3, offset3, range3, latitude3, longitude3, agl3);

	wxString repeaterCall4, repeaterBand4, repeaterAddress4, reflector4;
	double frequency4, offset4, range4, latitude4, longitude4, agl4;
	unsigned char band41, band42, band43;
	unsigned int repeaterPort4;
	HW_TYPE repeaterType4;
	bool atStartup4;
	RECONNECT reconnect4;
	::wxGetApp().getRepeater4(repeaterCall4, repeaterBand4, repeaterType4, repeaterAddress4, repeaterPort4, band41, band42, band43, reflector4, atStartup4, reconnect4, frequency4, offset4, range4, latitude4, longitude4, agl4);

	wxString hostname, username, password;
	::wxGetApp().getIrcDDB(hostname, username, password);

	wxString aprsHostname;
	unsigned int aprsPort;
	bool aprsEnabled;
	::wxGetApp().getDPRS(aprsEnabled, aprsHostname, aprsPort);

	bool dextraEnabled;
	unsigned int maxDExtraDongles;
	::wxGetApp().getDExtra(dextraEnabled, maxDExtraDongles);

	wxString dplusLogin;
	unsigned int maxDPlusDongles;
	bool dplusEnabled;
	::wxGetApp().getDPlus(dplusEnabled, maxDPlusDongles, dplusLogin);

	bool dcsEnabled;
	::wxGetApp().getDCS(dcsEnabled);

	unsigned int remotePort;
	wxString remotePassword;
	bool remoteEnabled;
	::wxGetApp().getRemote(remoteEnabled, remotePassword, remotePort);

	TEXT_LANG language;
	bool infoEnabled, echoEnabled, logEnabled, dratsEnabled, dtmfEnabled;
	::wxGetApp().getMiscellaneous(language, infoEnabled, echoEnabled, logEnabled, dratsEnabled, dtmfEnabled);

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	wxString starNetBand1, starNetCallsign1, starNetLogoff1, starNetInfo1, starNetLink1, starNetPermanent1;
	unsigned int starNetUserTimeout1, starNetGroupTimeout1;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch1;
	bool starNetTXMsgSwitch1;
	::wxGetApp().getStarNet1(starNetBand1, starNetCallsign1, starNetLogoff1, starNetInfo1, starNetPermanent1, starNetUserTimeout1, starNetGroupTimeout1, starNetCallsignSwitch1, starNetTXMsgSwitch1, starNetLink1);

	wxString starNetBand2, starNetCallsign2, starNetLogoff2, starNetInfo2, starNetLink2, starNetPermanent2;
	unsigned int starNetUserTimeout2, starNetGroupTimeout2;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch2;
	bool starNetTXMsgSwitch2;
	::wxGetApp().getStarNet2(starNetBand2, starNetCallsign2, starNetLogoff2, starNetInfo2, starNetPermanent2, starNetUserTimeout2, starNetGroupTimeout2, starNetCallsignSwitch2, starNetTXMsgSwitch2, starNetLink2);

	wxString starNetBand3, starNetCallsign3, starNetLogoff3, starNetInfo3, starNetLink3, starNetPermanent3;
	unsigned int starNetUserTimeout3, starNetGroupTimeout3;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch3;
	bool starNetTXMsgSwitch3;
	::wxGetApp().getStarNet3(starNetBand3, starNetCallsign3, starNetLogoff3, starNetInfo3, starNetPermanent3, starNetUserTimeout3, starNetGroupTimeout3, starNetCallsignSwitch3, starNetTXMsgSwitch3, starNetLink3);

	wxString starNetBand4, starNetCallsign4, starNetLogoff4, starNetInfo4, starNetLink4, starNetPermanent4;
	unsigned int starNetUserTimeout4, starNetGroupTimeout4;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch4;
	bool starNetTXMsgSwitch4;
	::wxGetApp().getStarNet4(starNetBand4, starNetCallsign4, starNetLogoff4, starNetInfo4, starNetPermanent4, starNetUserTimeout4, starNetGroupTimeout4, starNetCallsignSwitch4, starNetTXMsgSwitch4, starNetLink4);

	wxString starNetBand5, starNetCallsign5, starNetLogoff5, starNetInfo5, starNetLink5, starNetPermanent5;
	unsigned int starNetUserTimeout5, starNetGroupTimeout5;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch5;
	bool starNetTXMsgSwitch5;
	::wxGetApp().getStarNet5(starNetBand5, starNetCallsign5, starNetLogoff5, starNetInfo5, starNetPermanent5, starNetUserTimeout5, starNetGroupTimeout5, starNetCallsignSwitch5, starNetTXMsgSwitch5, starNetLink5);

	CIRCDDBGatewayPreferences dialog1(this, -1, gatewayCallsign, gatewayAddress, icomAddress, icomPort, hbAddress, hbPort, latitude, longitude, description1, description2, url,
		repeaterBand1, repeaterType1, repeaterAddress1, repeaterPort1, band11, band12, band13, reflector1, atStartup1, reconnect1, frequency1, offset1, range1, latitude1, longitude1, agl1,
		repeaterBand2, repeaterType2, repeaterAddress2, repeaterPort2, band21, band22, band23, reflector2, atStartup2, reconnect2, frequency2, offset2, range2, latitude2, longitude2, agl2,
		repeaterBand3, repeaterType3, repeaterAddress3, repeaterPort3, band31, band32, band33, reflector3, atStartup3, reconnect3, frequency3, offset3, range3, latitude3, longitude3, agl3,
		repeaterBand4, repeaterType4, repeaterAddress4, repeaterPort4, band41, band42, band43, reflector4, atStartup4, reconnect4, frequency4, offset4, range4, latitude4, longitude4, agl4,
		hostname, username, password, language, infoEnabled, echoEnabled, logEnabled, dratsEnabled, dtmfEnabled,
		aprsEnabled, aprsHostname, aprsPort, dextraEnabled, maxDExtraDongles,
		dplusEnabled, maxDPlusDongles, dplusLogin, dcsEnabled,
		starNetBand1, starNetCallsign1, starNetLogoff1, starNetInfo1, starNetPermanent1, starNetUserTimeout1, starNetGroupTimeout1, starNetCallsignSwitch1, starNetTXMsgSwitch1, starNetLink1,
		starNetBand2, starNetCallsign2, starNetLogoff2, starNetInfo2, starNetPermanent2, starNetUserTimeout2, starNetGroupTimeout2, starNetCallsignSwitch2, starNetTXMsgSwitch2, starNetLink2,
		starNetBand3, starNetCallsign3, starNetLogoff3, starNetInfo3, starNetPermanent3, starNetUserTimeout3, starNetGroupTimeout3, starNetCallsignSwitch3, starNetTXMsgSwitch3, starNetLink3,
		starNetBand4, starNetCallsign4, starNetLogoff4, starNetInfo4, starNetPermanent4, starNetUserTimeout4, starNetGroupTimeout4, starNetCallsignSwitch4, starNetTXMsgSwitch4, starNetLink4,
		starNetBand5, starNetCallsign5, starNetLogoff5, starNetInfo5, starNetPermanent5, starNetUserTimeout5, starNetGroupTimeout5, starNetCallsignSwitch5, starNetTXMsgSwitch5, starNetLink5,
		remoteEnabled, remotePassword, remotePort);
#else
	wxString starNetBand1, starNetCallsign1, starNetLogoff1, starNetInfo1, starNetPermanent1;
	unsigned int starNetUserTimeout1, starNetGroupTimeout1;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch1;
	bool starNetTXMsgSwitch1;
	::wxGetApp().getStarNet1(starNetBand1, starNetCallsign1, starNetLogoff1, starNetInfo1, starNetPermanent1, starNetUserTimeout1, starNetGroupTimeout1, starNetCallsignSwitch1, starNetTXMsgSwitch1);

	wxString starNetBand2, starNetCallsign2, starNetLogoff2, starNetInfo2, starNetPermanent2;
	unsigned int starNetUserTimeout2, starNetGroupTimeout2;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch2;
	bool starNetTXMsgSwitch2;
	::wxGetApp().getStarNet2(starNetBand2, starNetCallsign2, starNetLogoff2, starNetInfo2, starNetPermanent2, starNetUserTimeout2, starNetGroupTimeout2, starNetCallsignSwitch2, starNetTXMsgSwitch2);

	wxString starNetBand3, starNetCallsign3, starNetLogoff3, starNetInfo3, starNetPermanent3;
	unsigned int starNetUserTimeout3, starNetGroupTimeout3;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch3;
	bool starNetTXMsgSwitch3;
	::wxGetApp().getStarNet3(starNetBand3, starNetCallsign3, starNetLogoff3, starNetInfo3, starNetPermanent3, starNetUserTimeout3, starNetGroupTimeout3, starNetCallsignSwitch3, starNetTXMsgSwitch3);

	wxString starNetBand4, starNetCallsign4, starNetLogoff4, starNetInfo4, starNetPermanent4;
	unsigned int starNetUserTimeout4, starNetGroupTimeout4;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch4;
	bool starNetTXMsgSwitch4;
	::wxGetApp().getStarNet4(starNetBand4, starNetCallsign4, starNetLogoff4, starNetInfo4, starNetPermanent4, starNetUserTimeout4, starNetGroupTimeout4, starNetCallsignSwitch4, starNetTXMsgSwitch4);

	wxString starNetBand5, starNetCallsign5, starNetLogoff5, starNetInfo5, starNetPermanent5;
	unsigned int starNetUserTimeout5, starNetGroupTimeout5;
	STARNET_CALLSIGN_SWITCH starNetCallsignSwitch5;
	bool starNetTXMsgSwitch5;
	::wxGetApp().getStarNet5(starNetBand5, starNetCallsign5, starNetLogoff5, starNetInfo5, starNetPermanent5, starNetUserTimeout5, starNetGroupTimeout5, starNetCallsignSwitch5, starNetTXMsgSwitch5);

	CIRCDDBGatewayPreferences dialog1(this, -1, gatewayCallsign, gatewayAddress, icomAddress, icomPort, hbAddress, hbPort, latitude, longitude, description1, description2, url,
		repeaterBand1, repeaterType1, repeaterAddress1, repeaterPort1, band11, band12, band13, reflector1, atStartup1, reconnect1, frequency1, offset1, range1, latitude1, longitude1, agl1,
		repeaterBand2, repeaterType2, repeaterAddress2, repeaterPort2, band21, band22, band23, reflector2, atStartup2, reconnect2, frequency2, offset2, range2, latitude2, longitude2, agl2,
		repeaterBand3, repeaterType3, repeaterAddress3, repeaterPort3, band31, band32, band33, reflector3, atStartup3, reconnect3, frequency3, offset3, range3, latitude3, longitude3, agl3,
		repeaterBand4, repeaterType4, repeaterAddress4, repeaterPort4, band41, band42, band43, reflector4, atStartup4, reconnect4, frequency4, offset4, range4, latitude4, longitude4, agl4,
		hostname, username, password, language, infoEnabled, echoEnabled, logEnabled, dratsEnabled, dtmfEnabled,
		aprsEnabled, aprsHostname, aprsPort, dextraEnabled, maxDExtraDongles,
		dplusEnabled, maxDPlusDongles, dplusLogin, dcsEnabled,
		starNetBand1, starNetCallsign1, starNetLogoff1, starNetInfo1, starNetPermanent1, starNetUserTimeout1, starNetGroupTimeout1, starNetCallsignSwitch1, starNetTXMsgSwitch1,
		starNetBand2, starNetCallsign2, starNetLogoff2, starNetInfo2, starNetPermanent2, starNetUserTimeout2, starNetGroupTimeout2, starNetCallsignSwitch2, starNetTXMsgSwitch2,
		starNetBand3, starNetCallsign3, starNetLogoff3, starNetInfo3, starNetPermanent3, starNetUserTimeout3, starNetGroupTimeout3, starNetCallsignSwitch3, starNetTXMsgSwitch3,
		starNetBand4, starNetCallsign4, starNetLogoff4, starNetInfo4, starNetPermanent4, starNetUserTimeout4, starNetGroupTimeout4, starNetCallsignSwitch4, starNetTXMsgSwitch4,
		starNetBand5, starNetCallsign5, starNetLogoff5, starNetInfo5, starNetPermanent5, starNetUserTimeout5, starNetGroupTimeout5, starNetCallsignSwitch5, starNetTXMsgSwitch5,
		remoteEnabled, remotePassword, remotePort);
#endif
	if (dialog1.ShowModal() != wxID_OK)
		return;

	gatewayCallsign  = dialog1.getGatewayCallsign();
	gatewayAddress   = dialog1.getGatewayAddress();
	icomAddress      = dialog1.getIcomAddress();
	icomPort         = dialog1.getIcomPort();
	hbAddress        = dialog1.getHBAddress();
	hbPort           = dialog1.getHBPort();
	latitude         = dialog1.getLatitude();
	longitude        = dialog1.getLongitude();
	description1     = dialog1.getDescription1();
	description2     = dialog1.getDescription2();
	url              = dialog1.getURL();

	repeaterBand1    = dialog1.getRepeaterBand1();
	repeaterType1    = dialog1.getRepeaterType1();
	repeaterAddress1 = dialog1.getRepeaterAddress1();
	repeaterPort1    = dialog1.getRepeaterPort1();
	band11           = dialog1.getBand11();
	band12           = dialog1.getBand12();
	band13           = dialog1.getBand13();
	reflector1       = dialog1.getReflector1();
	atStartup1       = dialog1.atStartup1();
	reconnect1       = dialog1.getReconnect1();
	frequency1       = dialog1.getFrequency1();
	offset1          = dialog1.getOffset1();
	range1           = dialog1.getRange1();
	latitude1        = dialog1.getLatitude1();
	longitude1       = dialog1.getLongitude1();
	agl1             = dialog1.getAGL1();

	repeaterBand2    = dialog1.getRepeaterBand2();
	repeaterType2    = dialog1.getRepeaterType2();
	repeaterAddress2 = dialog1.getRepeaterAddress2();
	repeaterPort2    = dialog1.getRepeaterPort2();
	band21           = dialog1.getBand21();
	band22           = dialog1.getBand22();
	band23           = dialog1.getBand23();
	reflector2       = dialog1.getReflector2();
	atStartup2       = dialog1.atStartup2();
	reconnect2       = dialog1.getReconnect2();
	frequency2       = dialog1.getFrequency2();
	offset2          = dialog1.getOffset2();
	range2           = dialog1.getRange2();
	latitude2        = dialog1.getLatitude2();
	longitude2       = dialog1.getLongitude2();
	agl2             = dialog1.getAGL2();

	repeaterBand3    = dialog1.getRepeaterBand3();
	repeaterType3    = dialog1.getRepeaterType3();
	repeaterAddress3 = dialog1.getRepeaterAddress3();
	repeaterPort3    = dialog1.getRepeaterPort3();
	band31           = dialog1.getBand31();
	band32           = dialog1.getBand32();
	band33           = dialog1.getBand33();
	reflector3       = dialog1.getReflector3();
	atStartup3       = dialog1.atStartup3();
	reconnect3       = dialog1.getReconnect3();
	frequency3       = dialog1.getFrequency3();
	offset3          = dialog1.getOffset3();
	range3           = dialog1.getRange3();
	latitude3        = dialog1.getLatitude3();
	longitude3       = dialog1.getLongitude3();
	agl3             = dialog1.getAGL3();

	repeaterBand4    = dialog1.getRepeaterBand4();
	repeaterType4    = dialog1.getRepeaterType4();
	repeaterAddress4 = dialog1.getRepeaterAddress4();
	repeaterPort4    = dialog1.getRepeaterPort4();
	band41           = dialog1.getBand41();
	band42           = dialog1.getBand42();
	band43           = dialog1.getBand43();
	reflector4       = dialog1.getReflector4();
	atStartup4       = dialog1.atStartup4();
	reconnect4       = dialog1.getReconnect4();
	frequency4       = dialog1.getFrequency4();
	offset4          = dialog1.getOffset4();
	range4           = dialog1.getRange4();
	latitude4        = dialog1.getLatitude4();
	longitude4       = dialog1.getLongitude4();
	agl4             = dialog1.getAGL4();

	hostname         = dialog1.getHostname();
	username         = dialog1.getUsername();
	password         = dialog1.getPassword();

	aprsEnabled      = dialog1.getAPRSEnabled();
	aprsHostname     = dialog1.getAPRSHostname();
	aprsPort         = dialog1.getAPRSPort();

	dextraEnabled    = dialog1.getDExtraEnabled();
	maxDExtraDongles = dialog1.getMaxDExtraDongles();

	dplusEnabled     = dialog1.getDPlusEnabled();
	maxDPlusDongles  = dialog1.getMaxDPlusDongles();
	dplusLogin       = dialog1.getDPlusLogin();

	dcsEnabled       = dialog1.getDCSEnabled();

	starNetBand1         = dialog1.getStarNetBand1();
	starNetCallsign1     = dialog1.getStarNetCallsign1();
	starNetLogoff1       = dialog1.getStarNetLogoff1();
	starNetInfo1         = dialog1.getStarNetInfo1();
	starNetPermanent1    = dialog1.getStarNetPermanent1();
	starNetUserTimeout1  = dialog1.getStarNetUserTimeout1();
	starNetGroupTimeout1 = dialog1.getStarNetGroupTimeout1();
	starNetCallsignSwitch1 = dialog1.getStarNetCallsignSwitch1();
	starNetTXMsgSwitch1  = dialog1.getStarNetTXMsgSwitch1();
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	starNetLink1         = dialog1.getStarNetLink1();
#endif

	starNetBand2         = dialog1.getStarNetBand2();
	starNetCallsign2     = dialog1.getStarNetCallsign2();
	starNetLogoff2       = dialog1.getStarNetLogoff2();
	starNetInfo2         = dialog1.getStarNetInfo2();
	starNetPermanent2    = dialog1.getStarNetPermanent2();
	starNetUserTimeout2  = dialog1.getStarNetUserTimeout2();
	starNetGroupTimeout2 = dialog1.getStarNetGroupTimeout2();
	starNetGroupTimeout2 = dialog1.getStarNetGroupTimeout2();
	starNetCallsignSwitch2 = dialog1.getStarNetCallsignSwitch2();
	starNetTXMsgSwitch2  = dialog1.getStarNetTXMsgSwitch2();
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	starNetLink2         = dialog1.getStarNetLink2();
#endif

	starNetBand3         = dialog1.getStarNetBand3();
	starNetCallsign3     = dialog1.getStarNetCallsign3();
	starNetLogoff3       = dialog1.getStarNetLogoff3();
	starNetInfo3         = dialog1.getStarNetInfo3();
	starNetPermanent3    = dialog1.getStarNetPermanent3();
	starNetUserTimeout3  = dialog1.getStarNetUserTimeout3();
	starNetGroupTimeout3 = dialog1.getStarNetGroupTimeout3();
	starNetGroupTimeout3 = dialog1.getStarNetGroupTimeout3();
	starNetCallsignSwitch3 = dialog1.getStarNetCallsignSwitch3();
	starNetTXMsgSwitch3  = dialog1.getStarNetTXMsgSwitch3();
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	starNetLink3         = dialog1.getStarNetLink3();
#endif

	starNetBand4         = dialog1.getStarNetBand4();
	starNetCallsign4     = dialog1.getStarNetCallsign4();
	starNetLogoff4       = dialog1.getStarNetLogoff4();
	starNetInfo4         = dialog1.getStarNetInfo4();
	starNetPermanent4    = dialog1.getStarNetPermanent4();
	starNetUserTimeout4  = dialog1.getStarNetUserTimeout4();
	starNetGroupTimeout4 = dialog1.getStarNetGroupTimeout4();
	starNetCallsignSwitch4 = dialog1.getStarNetCallsignSwitch4();
	starNetTXMsgSwitch4  = dialog1.getStarNetTXMsgSwitch4();
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	starNetLink4         = dialog1.getStarNetLink4();
#endif

	starNetBand5         = dialog1.getStarNetBand5();
	starNetCallsign5     = dialog1.getStarNetCallsign5();
	starNetLogoff5       = dialog1.getStarNetLogoff5();
	starNetInfo5         = dialog1.getStarNetInfo5();
	starNetPermanent5    = dialog1.getStarNetPermanent5();
	starNetUserTimeout5  = dialog1.getStarNetUserTimeout5();
	starNetGroupTimeout5 = dialog1.getStarNetGroupTimeout5();
	starNetCallsignSwitch5 = dialog1.getStarNetCallsignSwitch5();
	starNetTXMsgSwitch5  = dialog1.getStarNetTXMsgSwitch5();
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	starNetLink5         = dialog1.getStarNetLink5();
#endif

	remoteEnabled       = dialog1.getRemoteEnabled();
	remotePassword      = dialog1.getRemotePassword();
	remotePort          = dialog1.getRemotePort();

	language            = dialog1.getLanguage();
	infoEnabled         = dialog1.getInfoEnabled();
	echoEnabled         = dialog1.getEchoEnabled();
	logEnabled          = dialog1.getLogEnabled();
	dratsEnabled        = dialog1.getDRATSEnabled();
	dtmfEnabled         = dialog1.getDTMFEnabled();

	::wxGetApp().setGateway(gatewayCallsign, gatewayAddress, icomAddress, icomPort, hbAddress, hbPort, latitude, longitude, description1, description2, url);
	::wxGetApp().setRepeater1(repeaterBand1, repeaterType1, repeaterAddress1, repeaterPort1, band11, band12, band13, reflector1, atStartup1, reconnect1, frequency1, offset1, range1, latitude1, longitude1, agl1);
	::wxGetApp().setRepeater2(repeaterBand2, repeaterType2, repeaterAddress2, repeaterPort2, band21, band22, band23, reflector2, atStartup2, reconnect2, frequency2, offset2, range2, latitude2, longitude2, agl2);
	::wxGetApp().setRepeater3(repeaterBand3, repeaterType3, repeaterAddress3, repeaterPort3, band31, band32, band33, reflector3, atStartup3, reconnect3, frequency3, offset3, range3, latitude3, longitude3, agl3);
	::wxGetApp().setRepeater4(repeaterBand4, repeaterType4, repeaterAddress4, repeaterPort4, band41, band42, band43, reflector4, atStartup4, reconnect4, frequency4, offset4, range4, latitude4, longitude4, agl4);
	::wxGetApp().setIrcDDB(hostname, username, password);
	::wxGetApp().setDPRS(aprsEnabled, aprsHostname, aprsPort);
	::wxGetApp().setDExtra(dextraEnabled, maxDExtraDongles);
	::wxGetApp().setDPlus(dplusEnabled, maxDPlusDongles, dplusLogin);
	::wxGetApp().setDCS(dcsEnabled);
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	::wxGetApp().setStarNet1(starNetBand1, starNetCallsign1, starNetLogoff1, starNetInfo1, starNetPermanent1, starNetUserTimeout1, starNetGroupTimeout1, starNetCallsignSwitch1, starNetTXMsgSwitch1, starNetLink1);
	::wxGetApp().setStarNet2(starNetBand2, starNetCallsign2, starNetLogoff2, starNetInfo2, starNetPermanent2, starNetUserTimeout2, starNetGroupTimeout2, starNetCallsignSwitch2, starNetTXMsgSwitch2, starNetLink2);
	::wxGetApp().setStarNet3(starNetBand3, starNetCallsign3, starNetLogoff3, starNetInfo3, starNetPermanent3, starNetUserTimeout3, starNetGroupTimeout3, starNetCallsignSwitch3, starNetTXMsgSwitch3, starNetLink3);
	::wxGetApp().setStarNet4(starNetBand4, starNetCallsign4, starNetLogoff4, starNetInfo4, starNetPermanent4, starNetUserTimeout4, starNetGroupTimeout4, starNetCallsignSwitch4, starNetTXMsgSwitch4, starNetLink4);
	::wxGetApp().setStarNet5(starNetBand5, starNetCallsign5, starNetLogoff5, starNetInfo5, starNetPermanent5, starNetUserTimeout5, starNetGroupTimeout5, starNetCallsignSwitch5, starNetTXMsgSwitch5, starNetLink5);
#else
	::wxGetApp().setStarNet1(starNetBand1, starNetCallsign1, starNetLogoff1, starNetInfo1, starNetPermanent1, starNetUserTimeout1, starNetGroupTimeout1, starNetCallsignSwitch1, starNetTXMsgSwitch1);
	::wxGetApp().setStarNet2(starNetBand2, starNetCallsign2, starNetLogoff2, starNetInfo2, starNetPermanent2, starNetUserTimeout2, starNetGroupTimeout2, starNetCallsignSwitch2, starNetTXMsgSwitch2);
	::wxGetApp().setStarNet3(starNetBand3, starNetCallsign3, starNetLogoff3, starNetInfo3, starNetPermanent3, starNetUserTimeout3, starNetGroupTimeout3, starNetCallsignSwitch3, starNetTXMsgSwitch3);
	::wxGetApp().setStarNet4(starNetBand4, starNetCallsign4, starNetLogoff4, starNetInfo4, starNetPermanent4, starNetUserTimeout4, starNetGroupTimeout4, starNetCallsignSwitch4, starNetTXMsgSwitch4);
	::wxGetApp().setStarNet5(starNetBand5, starNetCallsign5, starNetLogoff5, starNetInfo5, starNetPermanent5, starNetUserTimeout5, starNetGroupTimeout5, starNetCallsignSwitch5, starNetTXMsgSwitch5);
#endif
	::wxGetApp().setRemote(remoteEnabled, remotePassword, remotePort);
	::wxGetApp().setMiscellaneous(language, infoEnabled, echoEnabled, logEnabled, dratsEnabled, dtmfEnabled);
	::wxGetApp().writeConfig();

	wxMessageDialog dialog2(this, _("The changes made will not take effect\nuntil the application is restarted"), _("ircDDB Gateway Information"), wxICON_INFORMATION);
	dialog2.ShowModal();
}

void CIRCDDBGatewayFrame::onUpdates(wxCommandEvent& event)
{
	m_updates = event.IsChecked();
}

void CIRCDDBGatewayFrame::onAbout(wxCommandEvent& event)
{
	wxAboutDialogInfo info;
	info.AddDeveloper(wxT("Jonathan Naylor, G4KLX"));
	info.AddDeveloper(wxT("Michael Dirska, DL1BFF"));
	info.SetCopyright(wxT("(C) 2010-2012 using GPL v2 or later"));
	info.SetName(APPLICATION_NAME);
	info.SetVersion(VERSION);
	info.SetDescription(_("This program allows a computer running homebrew repeaters\nto access the ircDDB network for G2 callsign and repeater routing,\nas well as D-Plus and DExtra reflectors. It includes a StarNet\nDigital server."));

	::wxAboutBox(info);
}

void CIRCDDBGatewayFrame::onLog(wxEvent& event)
{
	CLogEvent& logEvent = dynamic_cast<CLogEvent&>(event);

	wxString text;

	text = m_logLine[1U]->GetLabel();
	m_logLine[0U]->SetLabel(text);

	text = m_logLine[2U]->GetLabel();
	m_logLine[1U]->SetLabel(text);

	text = m_logLine[3U]->GetLabel();
	m_logLine[2U]->SetLabel(text);

	text = m_logLine[4U]->GetLabel();
	m_logLine[3U]->SetLabel(text);

	text = m_logLine[5U]->GetLabel();
	m_logLine[4U]->SetLabel(text);

	text = m_logLine[6U]->GetLabel();
	m_logLine[5U]->SetLabel(text);

	text = logEvent.getText();
	m_logLine[6U]->SetLabel(text);
}

void CIRCDDBGatewayFrame::showLog(const wxString& text)
{
	if (!m_updates)
		return;

	CLogEvent event(text, LOG_EVENT);

	AddPendingEvent(event);
}

void CIRCDDBGatewayFrame::onTimer(wxTimerEvent& event)
{
	if (!m_updates)
		return;

	CIRCDDBGatewayStatusData* status = ::wxGetApp().getStatus();
	if (status == NULL)
		return;

	switch (status->getIrcDDBStatus()) {
		case IS_DISCONNECTED:
			m_ircDDBStatus->SetLabel(_("Disconnected"));
			break;
		case IS_CONNECTING:
			m_ircDDBStatus->SetLabel(_("Connecting"));
			break;
		default:
			m_ircDDBStatus->SetLabel(_("Connected"));
			break;
	}

	if (status->getDPRSStatus()) {
		if (status->getIrcDDBStatus() == IS_CONNECTED)
			m_dprsStatus->SetLabel(_("Active"));
		else
			m_dprsStatus->SetLabel(_("Waiting"));
	} else {
		m_dprsStatus->SetLabel(_("Inactive"));
	}

	for (unsigned int i = 0U; i < 4U; i++) {
		wxString callsign = status->getCallsign(i);
		if (!callsign.IsEmpty()) {
			wxString text = callsign;

			wxString linkCallsign = status->getLinkCallsign(i);

			switch (status->getLinkStatus(i)) {
				case LS_NONE:
					text.Append(_("  Not linked"));
					break;
				case LS_LINKED_DEXTRA:
				case LS_LINKED_DPLUS:
				case LS_LINKED_DCS:
					text.Append(_("  Linked to  "));
					text.Append(linkCallsign);
					break;
				default:
					text.Append(_("  Linking to  "));
					text.Append(linkCallsign);
					break;
			}
			m_linkStatus[i]->SetLabel(text);

			wxString incoming = status->getIncoming(i);
			if (!incoming.IsEmpty()) {
				incoming.Prepend(wxT("[  "));
				incoming.Append(wxT("]"));
			}
			m_incoming[i]->SetLabel(incoming);
		}
	}

	wxString dongles = status->getDongles();
	m_dongles->SetLabel(dongles);

	delete status;
}

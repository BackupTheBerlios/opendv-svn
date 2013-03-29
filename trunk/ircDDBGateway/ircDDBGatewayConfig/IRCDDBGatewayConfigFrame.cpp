/*
 *   Copyright (C) 2010-2013 by Jonathan Naylor G4KLX
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

#include "IRCDDBGatewayConfigPreferences.h"
#include "IRCDDBGatewayConfigFrame.h"
#include "IRCDDBGatewayConfigDefs.h"
#include "IRCDDBGatewayConfigApp.h"
#include "Version.h"

const unsigned int BORDER_SIZE = 5U;

#include <wx/gbsizer.h>
#include <wx/aboutdlg.h>

enum {
	Menu_Edit_Preferences = 6000
};

BEGIN_EVENT_TABLE(CIRCDDBGatewayConfigFrame, wxFrame)
	EVT_MENU(wxID_EXIT, CIRCDDBGatewayConfigFrame::onQuit)
	EVT_MENU(Menu_Edit_Preferences, CIRCDDBGatewayConfigFrame::onPreferences)
	EVT_MENU(wxID_ABOUT, CIRCDDBGatewayConfigFrame::onAbout)

	EVT_CLOSE(CIRCDDBGatewayConfigFrame::onClose)
END_EVENT_TABLE()

CIRCDDBGatewayConfigFrame::CIRCDDBGatewayConfigFrame(const wxString& title) :
wxFrame(NULL, -1, title)
{
	SetMenuBar(createMenuBar());

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	wxPanel* panel = new wxPanel(this);

	wxBoxSizer* panelSizer = new wxBoxSizer(wxVERTICAL);

	panel->SetSizer(panelSizer);
	panelSizer->SetSizeHints(panel);

	mainSizer->Add(panel);

	SetSizer(mainSizer);
	mainSizer->SetSizeHints(this);
}

CIRCDDBGatewayConfigFrame::~CIRCDDBGatewayConfigFrame()
{
}

wxMenuBar* CIRCDDBGatewayConfigFrame::createMenuBar()
{
	wxMenu* fileMenu = new wxMenu();
	fileMenu->Append(wxID_EXIT, _("Exit"));

	wxMenu* editMenu = new wxMenu();
	editMenu->Append(Menu_Edit_Preferences, _("Preferences..."));

	wxMenu* helpMenu = new wxMenu();
	helpMenu->Append(wxID_ABOUT, _("About ircDDB Gateway Config"));

	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append(fileMenu, _("File"));
	menuBar->Append(editMenu, _("Edit"));
	menuBar->Append(helpMenu, _("Help"));

	return menuBar;
}

void CIRCDDBGatewayConfigFrame::onQuit(wxCommandEvent& event)
{
	Close(false);
}

void CIRCDDBGatewayConfigFrame::onClose(wxCloseEvent& event)
{
	Destroy();
}

void CIRCDDBGatewayConfigFrame::onPreferences(wxCommandEvent& event)
{
	wxString gatewayCallsign, gatewayAddress, icomAddress, hbAddress, description1, description2, url;
	unsigned int icomPort, hbPort;
	double latitude, longitude;
	::wxGetApp().getGateway(gatewayCallsign, gatewayAddress, icomAddress, icomPort, hbAddress, hbPort, latitude, longitude, description1, description2, url);

	wxString repeaterCall1, repeaterBand1, repeaterAddress1, reflector1, description11, description12, url1;
	double frequency1, offset1, range1, latitude1, longitude1, agl1;
	unsigned char band11, band12, band13;
	unsigned int repeaterPort1;
	HW_TYPE repeaterType1;
	bool atStartup1;
	RECONNECT reconnect1;
	::wxGetApp().getRepeater1(repeaterCall1, repeaterBand1, repeaterType1, repeaterAddress1, repeaterPort1, band11, band12, band13, reflector1, atStartup1, reconnect1, frequency1, offset1, range1, latitude1, longitude1, agl1, description11, description12, url1);

	wxString repeaterCall2, repeaterBand2, repeaterAddress2, reflector2, description21, description22, url2;
	double frequency2, offset2, range2, latitude2, longitude2, agl2;
	unsigned char band21, band22, band23;
	unsigned int repeaterPort2;
	HW_TYPE repeaterType2;
	bool atStartup2;
	RECONNECT reconnect2;
	::wxGetApp().getRepeater2(repeaterCall2, repeaterBand2, repeaterType2, repeaterAddress2, repeaterPort2, band21, band22, band23, reflector2, atStartup2, reconnect2, frequency2, offset2, range2, latitude2, longitude2, agl2, description21, description22, url2);

	wxString repeaterCall3, repeaterBand3, repeaterAddress3, reflector3, description31, description32, url3;
	double frequency3, offset3, range3, latitude3, longitude3, agl3;
	unsigned char band31, band32, band33;
	unsigned int repeaterPort3;
	HW_TYPE repeaterType3;
	bool atStartup3;
	RECONNECT reconnect3;
	::wxGetApp().getRepeater3(repeaterCall3, repeaterBand3, repeaterType3, repeaterAddress3, repeaterPort3, band31, band32, band33, reflector3, atStartup3, reconnect3, frequency3, offset3, range3, latitude3, longitude3, agl3, description31, description32, url3);

	wxString repeaterCall4, repeaterBand4, repeaterAddress4, reflector4, description41, description42, url4;
	double frequency4, offset4, range4, latitude4, longitude4, agl4;
	unsigned char band41, band42, band43;
	unsigned int repeaterPort4;
	HW_TYPE repeaterType4;
	bool atStartup4;
	RECONNECT reconnect4;
	::wxGetApp().getRepeater4(repeaterCall4, repeaterBand4, repeaterType4, repeaterAddress4, repeaterPort4, band41, band42, band43, reflector4, atStartup4, reconnect4, frequency4, offset4, range4, latitude4, longitude4, agl4, description41, description42, url4);

	bool ircDDBEnabled;
	wxString ircDDBHostname, ircDDBUsername, ircDDBPassword;
	::wxGetApp().getIrcDDB(ircDDBEnabled, ircDDBHostname, ircDDBUsername, ircDDBPassword);

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

	bool dcsEnabled, ccsEnabled;
	wxString ccsHost;
	::wxGetApp().getDCS(dcsEnabled, ccsEnabled, ccsHost);

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

	CIRCDDBGatewayConfigPreferences dialog1(this, -1, gatewayCallsign, gatewayAddress, icomAddress, icomPort, hbAddress, hbPort, latitude, longitude, description1, description2, url,
		repeaterBand1, repeaterType1, repeaterAddress1, repeaterPort1, band11, band12, band13, reflector1, atStartup1, reconnect1, frequency1, offset1, range1, latitude1, longitude1, agl1, description11, description12, url1,
		repeaterBand2, repeaterType2, repeaterAddress2, repeaterPort2, band21, band22, band23, reflector2, atStartup2, reconnect2, frequency2, offset2, range2, latitude2, longitude2, agl2, description21, description22, url2,
		repeaterBand3, repeaterType3, repeaterAddress3, repeaterPort3, band31, band32, band33, reflector3, atStartup3, reconnect3, frequency3, offset3, range3, latitude3, longitude3, agl3, description31, description32, url3,
		repeaterBand4, repeaterType4, repeaterAddress4, repeaterPort4, band41, band42, band43, reflector4, atStartup4, reconnect4, frequency4, offset4, range4, latitude4, longitude4, agl4, description41, description42, url4,
		ircDDBEnabled, ircDDBHostname, ircDDBUsername, ircDDBPassword, language, infoEnabled, echoEnabled, logEnabled, dratsEnabled, dtmfEnabled,
		aprsEnabled, aprsHostname, aprsPort, dextraEnabled, maxDExtraDongles,
		dplusEnabled, maxDPlusDongles, dplusLogin, dcsEnabled, ccsEnabled, ccsHost,
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

	CIRCDDBGatewayConfigPreferences dialog1(this, -1, gatewayCallsign, gatewayAddress, icomAddress, icomPort, hbAddress, hbPort, latitude, longitude, description1, description2, url,
		repeaterBand1, repeaterType1, repeaterAddress1, repeaterPort1, band11, band12, band13, reflector1, atStartup1, reconnect1, frequency1, offset1, range1, latitude1, longitude1, agl1, description11, description12, url1,
		repeaterBand2, repeaterType2, repeaterAddress2, repeaterPort2, band21, band22, band23, reflector2, atStartup2, reconnect2, frequency2, offset2, range2, latitude2, longitude2, agl2, description21, description22, url2,
		repeaterBand3, repeaterType3, repeaterAddress3, repeaterPort3, band31, band32, band33, reflector3, atStartup3, reconnect3, frequency3, offset3, range3, latitude3, longitude3, agl3, description31, description32, url3,
		repeaterBand4, repeaterType4, repeaterAddress4, repeaterPort4, band41, band42, band43, reflector4, atStartup4, reconnect4, frequency4, offset4, range4, latitude4, longitude4, agl4, description41, description42, url4,
		ircDDBEnabled, ircDDBHostname, ircDDBUsername, ircDDBPassword, language, infoEnabled, echoEnabled, logEnabled, dratsEnabled, dtmfEnabled,
		aprsEnabled, aprsHostname, aprsPort, dextraEnabled, maxDExtraDongles,
		dplusEnabled, maxDPlusDongles, dplusLogin, dcsEnabled, ccsEnabled, ccsHost,
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
	description11    = dialog1.getDescription11();
	description12    = dialog1.getDescription12();
	url1             = dialog1.getURL1();

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
	description21    = dialog1.getDescription21();
	description22    = dialog1.getDescription22();
	url2             = dialog1.getURL2();

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
	description31    = dialog1.getDescription31();
	description32    = dialog1.getDescription32();
	url3             = dialog1.getURL3();

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
	description41    = dialog1.getDescription41();
	description42    = dialog1.getDescription42();
	url4             = dialog1.getURL4();

	ircDDBEnabled    = dialog1.getIrcDDBEnabled();
	ircDDBHostname   = dialog1.getIrcDDBHostname();
	ircDDBUsername   = dialog1.getIrcDDBUsername();
	ircDDBPassword   = dialog1.getIrcDDBPassword();

	aprsEnabled      = dialog1.getAPRSEnabled();
	aprsHostname     = dialog1.getAPRSHostname();
	aprsPort         = dialog1.getAPRSPort();

	dextraEnabled    = dialog1.getDExtraEnabled();
	maxDExtraDongles = dialog1.getMaxDExtraDongles();

	dplusEnabled     = dialog1.getDPlusEnabled();
	maxDPlusDongles  = dialog1.getMaxDPlusDongles();
	dplusLogin       = dialog1.getDPlusLogin();

	dcsEnabled       = dialog1.getDCSEnabled();
	ccsEnabled       = dialog1.getCCSEnabled();
	ccsHost          = dialog1.getCCSHost();

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
	::wxGetApp().setRepeater1(repeaterBand1, repeaterType1, repeaterAddress1, repeaterPort1, band11, band12, band13, reflector1, atStartup1, reconnect1, frequency1, offset1, range1, latitude1, longitude1, agl1, description11, description12, url1);
	::wxGetApp().setRepeater2(repeaterBand2, repeaterType2, repeaterAddress2, repeaterPort2, band21, band22, band23, reflector2, atStartup2, reconnect2, frequency2, offset2, range2, latitude2, longitude2, agl2, description21, description22, url2);
	::wxGetApp().setRepeater3(repeaterBand3, repeaterType3, repeaterAddress3, repeaterPort3, band31, band32, band33, reflector3, atStartup3, reconnect3, frequency3, offset3, range3, latitude3, longitude3, agl3, description31, description32, url3);
	::wxGetApp().setRepeater4(repeaterBand4, repeaterType4, repeaterAddress4, repeaterPort4, band41, band42, band43, reflector4, atStartup4, reconnect4, frequency4, offset4, range4, latitude4, longitude4, agl4, description41, description42, url4);
	::wxGetApp().setIrcDDB(ircDDBEnabled, ircDDBHostname, ircDDBUsername, ircDDBPassword);
	::wxGetApp().setDPRS(aprsEnabled, aprsHostname, aprsPort);
	::wxGetApp().setDExtra(dextraEnabled, maxDExtraDongles);
	::wxGetApp().setDPlus(dplusEnabled, maxDPlusDongles, dplusLogin);
	::wxGetApp().setDCS(dcsEnabled, ccsEnabled, ccsHost);
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

void CIRCDDBGatewayConfigFrame::onAbout(wxCommandEvent& event)
{
	wxAboutDialogInfo info;
	info.AddDeveloper(wxT("Jonathan Naylor, G4KLX"));
	info.SetCopyright(wxT("(C) 2010-2013 using GPL v2 or later"));
	info.SetName(APPLICATION_NAME);
	info.SetVersion(VERSION);
	info.SetDescription(_("This program configures the ircDDB Gateway."));

	::wxAboutBox(info);
}

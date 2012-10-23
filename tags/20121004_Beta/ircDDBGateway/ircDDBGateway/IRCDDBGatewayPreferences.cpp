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
#include "IRCDDBGatewayDefs.h"

const unsigned int BORDER_SIZE = 5U;

CIRCDDBGatewayPreferences::CIRCDDBGatewayPreferences(wxWindow* parent, int id, const wxString& gatewayCallsign, const wxString& gatewayAddress, const wxString& icomAddress, unsigned int icomPort, const wxString& hbAddress, unsigned int hbPort, double latitude, double longitude, const wxString& description1, const wxString& description2, const wxString& url, 
						   const wxString& repeaterBand1, HW_TYPE repeaterType1, const wxString& repeaterAddress1, unsigned int repeaterPort1, unsigned char band11, unsigned char band12, unsigned char band13, const wxString& reflector1, bool atStartup1, RECONNECT reconnect1, double frequency1, double offset1, double range1, double latitude1, double longitude1, double agl1,
						   const wxString& repeaterBand2, HW_TYPE repeaterType2, const wxString& repeaterAddress2, unsigned int repeaterPort2, unsigned char band21, unsigned char band22, unsigned char band23, const wxString& reflector2, bool atStartup2, RECONNECT reconnect2, double frequency2, double offset2, double range2, double latitude2, double longitude2, double agl2,
						   const wxString& repeaterBand3, HW_TYPE repeaterType3, const wxString& repeaterAddress3, unsigned int repeaterPort3, unsigned char band31, unsigned char band32, unsigned char band33, const wxString& reflector3, bool atStartup3, RECONNECT reconnect3, double frequency3, double offset3, double range3, double latitude3, double longitude3, double agl3,
						   const wxString& repeaterBand4, HW_TYPE repeaterType4, const wxString& repeaterAddress4, unsigned int repeaterPort4, unsigned char band41, unsigned char band42, unsigned char band43, const wxString& reflector4, bool atStartup4, RECONNECT reconnect4, double frequency4, double offset4, double range4, double latitude4, double longitude4, double agl4,
						   const wxString& hostname, const wxString& username, const wxString& password, TEXT_LANG language, bool infoEnabled, bool echoEnabled,
						   bool logEnabled, bool dratsEnabled, bool dtmfEnabled, bool aprsEnabled, const wxString& aprsHostname, unsigned int aprsPort, bool dextraEnabled,
						   unsigned int maxDExtraDongles, bool dplusEnabled, unsigned int maxDPlusDongles, const wxString& dplusLogin, bool dcsEnabled,
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
						   const wxString& starNetBand1, const wxString& callsign1, const wxString& logoff1, const wxString& info1, const wxString& permanent1, unsigned int userTimeout1, unsigned int groupTimeout1, STARNET_CALLSIGN_SWITCH callsignSwitch1, bool txMsgSwitch1, const wxString& link1,
						   const wxString& starNetBand2, const wxString& callsign2, const wxString& logoff2, const wxString& info2, const wxString& permanent2, unsigned int userTimeout2, unsigned int groupTimeout2, STARNET_CALLSIGN_SWITCH callsignSwitch2, bool txMsgSwitch2, const wxString& link2,
						   const wxString& starNetBand3, const wxString& callsign3, const wxString& logoff3, const wxString& info3, const wxString& permanent3, unsigned int userTimeout3, unsigned int groupTimeout3, STARNET_CALLSIGN_SWITCH callsignSwitch3, bool txMsgSwitch3, const wxString& link3,
						   const wxString& starNetBand4, const wxString& callsign4, const wxString& logoff4, const wxString& info4, const wxString& permanent4, unsigned int userTimeout4, unsigned int groupTimeout4, STARNET_CALLSIGN_SWITCH callsignSwitch4, bool txMsgSwitch4, const wxString& link4,
						   const wxString& starNetBand5, const wxString& callsign5, const wxString& logoff5, const wxString& info5, const wxString& permanent5, unsigned int userTimeout5, unsigned int groupTimeout5, STARNET_CALLSIGN_SWITCH callsignSwitch5, bool txMsgSwitch5, const wxString& link5,
#else
						   const wxString& starNetBand1, const wxString& callsign1, const wxString& logoff1, const wxString& info1, const wxString& permanent1, unsigned int userTimeout1, unsigned int groupTimeout1, STARNET_CALLSIGN_SWITCH callsignSwitch1, bool txMsgSwitch1,
						   const wxString& starNetBand2, const wxString& callsign2, const wxString& logoff2, const wxString& info2, const wxString& permanent2, unsigned int userTimeout2, unsigned int groupTimeout2, STARNET_CALLSIGN_SWITCH callsignSwitch2, bool txMsgSwitch2,
						   const wxString& starNetBand3, const wxString& callsign3, const wxString& logoff3, const wxString& info3, const wxString& permanent3, unsigned int userTimeout3, unsigned int groupTimeout3, STARNET_CALLSIGN_SWITCH callsignSwitch3, bool txMsgSwitch3,
						   const wxString& starNetBand4, const wxString& callsign4, const wxString& logoff4, const wxString& info4, const wxString& permanent4, unsigned int userTimeout4, unsigned int groupTimeout4, STARNET_CALLSIGN_SWITCH callsignSwitch4, bool txMsgSwitch4,
						   const wxString& starNetBand5, const wxString& callsign5, const wxString& logoff5, const wxString& info5, const wxString& permanent5, unsigned int userTimeout5, unsigned int groupTimeout5, STARNET_CALLSIGN_SWITCH callsignSwitch5, bool txMsgSwitch5,
#endif
						   bool remoteEnabled, const wxString& remotePassword, unsigned int remotePort) :
wxDialog(parent, id, wxString(_("ircDDB Gateway Preferences"))),
m_gateway(NULL),
m_repeater1(NULL),
m_repeater2(NULL),
m_repeater3(NULL),
m_repeater4(NULL),
m_ircDDB(NULL),
m_dprs(NULL),
m_dextra(NULL),
m_dplus(NULL),
m_dcs(NULL),
m_starNet1(NULL),
m_starNet2(NULL),
m_starNet3(NULL),
m_starNet4(NULL),
m_starNet5(NULL),
m_remote(NULL),
m_miscellaneous(NULL)
{
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	wxNotebook* noteBook = new wxNotebook(this, -1);

	m_gateway = new CIRCDDBGatewayGatewaySet(noteBook, -1, APPLICATION_NAME, gatewayCallsign, gatewayAddress, icomAddress, icomPort, hbAddress, hbPort, latitude, longitude, description1, description2, url);
	noteBook->AddPage(m_gateway, _("Gateway"), true);

	m_repeater1 = new CRepeaterSet(noteBook, -1, APPLICATION_NAME, repeaterBand1, repeaterType1, repeaterAddress1, repeaterPort1, band11, band12, band13, dplusEnabled, dextraEnabled, dcsEnabled, reflector1, atStartup1, reconnect1, frequency1, offset1, range1, latitude1, longitude1, agl1);
	noteBook->AddPage(m_repeater1, _("Repeater 1"), false);

	m_repeater2 = new CRepeaterSet(noteBook, -1, APPLICATION_NAME, repeaterBand2, repeaterType2, repeaterAddress2, repeaterPort2, band21, band22, band23, dplusEnabled, dextraEnabled, dcsEnabled, reflector2, atStartup2, reconnect2, frequency2, offset2, range2, latitude2, longitude2, agl2);
	noteBook->AddPage(m_repeater2, _("Repeater 2"), false);

	m_repeater3 = new CRepeaterSet(noteBook, -1, APPLICATION_NAME, repeaterBand3, repeaterType3, repeaterAddress3, repeaterPort3, band31, band32, band33, dplusEnabled, dextraEnabled, dcsEnabled, reflector3, atStartup3, reconnect3, frequency3, offset3, range3, latitude3, longitude3, agl3);
	noteBook->AddPage(m_repeater3, _("Repeater 3"), false);

	m_repeater4 = new CRepeaterSet(noteBook, -1, APPLICATION_NAME, repeaterBand4, repeaterType4, repeaterAddress4, repeaterPort4, band41, band42, band43, dplusEnabled, dextraEnabled, dcsEnabled, reflector4, atStartup4, reconnect4, frequency4, offset4, range4, latitude4, longitude4, agl4);
	noteBook->AddPage(m_repeater4, _("Repeater 4"), false);

	m_ircDDB = new CIrcDDBSet(noteBook, -1, APPLICATION_NAME, hostname, username, password);
	noteBook->AddPage(m_ircDDB, wxT("ircDDB"), false);

	m_dprs = new CDPRSSet(noteBook, -1, APPLICATION_NAME, aprsEnabled, aprsHostname, aprsPort);
	noteBook->AddPage(m_dprs, wxT("D-PRS"), false);

	m_dextra = new CDExtraSet(noteBook, -1, APPLICATION_NAME, dextraEnabled, maxDExtraDongles);
	noteBook->AddPage(m_dextra, wxT("DExtra"), false);

	m_dplus = new CDPlusSet(noteBook, -1, APPLICATION_NAME, dplusEnabled, maxDPlusDongles, dplusLogin);
	noteBook->AddPage(m_dplus, wxT("D-Plus"), false);

	m_dcs = new CDCSSet(noteBook, -1, APPLICATION_NAME, dcsEnabled);
	noteBook->AddPage(m_dcs, wxT("DCS"), false);

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	m_starNet1 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand1, callsign1, logoff1, info1, permanent1, userTimeout1, groupTimeout1, callsignSwitch1, txMsgSwitch1, link1);
	noteBook->AddPage(m_starNet1, wxT("StarNet 1"), false);

	m_starNet2 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand2, callsign2, logoff2, info2, permanent2, userTimeout2, groupTimeout2, callsignSwitch2, txMsgSwitch2, link2);
	noteBook->AddPage(m_starNet2, wxT("StarNet 2"), false);

	m_starNet3 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand3, callsign3, logoff3, info3, permanent3, userTimeout3, groupTimeout3, callsignSwitch3, txMsgSwitch3, link3);
	noteBook->AddPage(m_starNet3, wxT("StarNet 3"), false);

	m_starNet4 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand4, callsign4, logoff4, info4, permanent4, userTimeout4, groupTimeout4, callsignSwitch4, txMsgSwitch4, link4);
	noteBook->AddPage(m_starNet4, wxT("StarNet 4"), false);

	m_starNet5 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand5, callsign5, logoff5, info5, permanent5, userTimeout5, groupTimeout5, callsignSwitch5, txMsgSwitch5, link5);
	noteBook->AddPage(m_starNet5, wxT("StarNet 5"), false);
#else
	m_starNet1 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand1, callsign1, logoff1, info1, permanent1, userTimeout1, groupTimeout1, callsignSwitch1, txMsgSwitch1);
	noteBook->AddPage(m_starNet1, wxT("StarNet 1"), false);

	m_starNet2 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand2, callsign2, logoff2, info2, permanent2, userTimeout2, groupTimeout2, callsignSwitch2, txMsgSwitch2);
	noteBook->AddPage(m_starNet2, wxT("StarNet 2"), false);

	m_starNet3 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand3, callsign3, logoff3, info3, permanent3, userTimeout3, groupTimeout3, callsignSwitch3, txMsgSwitch3);
	noteBook->AddPage(m_starNet3, wxT("StarNet 3"), false);

	m_starNet4 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand4, callsign4, logoff4, info4, permanent4, userTimeout4, groupTimeout4, callsignSwitch4, txMsgSwitch4);
	noteBook->AddPage(m_starNet4, wxT("StarNet 4"), false);

	m_starNet5 = new CStarNetSet(noteBook, -1, APPLICATION_NAME, starNetBand5, callsign5, logoff5, info5, permanent5, userTimeout5, groupTimeout5, callsignSwitch5, txMsgSwitch5);
	noteBook->AddPage(m_starNet5, wxT("StarNet 5"), false);
#endif

	m_remote = new CRemoteSet(noteBook, -1, APPLICATION_NAME, remoteEnabled, remotePassword, remotePort);
	noteBook->AddPage(m_remote, wxT("Remote"), false);

	m_miscellaneous = new CIRCDDBGatewayMiscellaneousSet(noteBook, -1, APPLICATION_NAME, language, infoEnabled, echoEnabled, logEnabled, dratsEnabled, dtmfEnabled);
	noteBook->AddPage(m_miscellaneous, wxT("Misc"), false);

	mainSizer->Add(noteBook, 1, wxALL | wxGROW, BORDER_SIZE);

	mainSizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	SetAutoLayout(true);
	Layout();

	mainSizer->Fit(this);
	mainSizer->SetSizeHints(this);

	SetSizer(mainSizer);
}

CIRCDDBGatewayPreferences::~CIRCDDBGatewayPreferences()
{
}

bool CIRCDDBGatewayPreferences::Validate()
{
	if (!m_gateway->Validate())
		return false;

	if (!m_repeater1->Validate())
		return false;

	if (!m_repeater2->Validate())
		return false;

	if (!m_repeater3->Validate())
		return false;

	if (!m_repeater4->Validate())
		return false;

	if (!m_ircDDB->Validate())
		return false;

	if (!m_dprs->Validate())
		return false;

	if (!m_dplus->Validate())
		return false;

	if (!m_dcs->Validate())
		return false;

	if (!m_starNet1->Validate())
		return false;

	if (!m_starNet2->Validate())
		return false;

	if (!m_starNet3->Validate())
		return false;

	if (!m_starNet4->Validate())
		return false;

	if (!m_starNet5->Validate())
		return false;

	if (!m_remote->Validate())
		return false;

	return m_miscellaneous->Validate();
}

wxString CIRCDDBGatewayPreferences::getGatewayCallsign() const
{
	return m_gateway->getCallsign();
}

wxString CIRCDDBGatewayPreferences::getGatewayAddress() const
{
	return m_gateway->getAddress();
}

wxString CIRCDDBGatewayPreferences::getIcomAddress() const
{
	return m_gateway->getIcomAddress();
}

unsigned int CIRCDDBGatewayPreferences::getIcomPort() const
{
	return m_gateway->getIcomPort();
}

wxString CIRCDDBGatewayPreferences::getHBAddress() const
{
	return m_gateway->getHBAddress();
}

unsigned int CIRCDDBGatewayPreferences::getHBPort() const
{
	return m_gateway->getHBPort();
}

double CIRCDDBGatewayPreferences::getLatitude() const
{
	return m_gateway->getLatitude();
}

double CIRCDDBGatewayPreferences::getLongitude() const
{
	return m_gateway->getLongitude();
}

wxString CIRCDDBGatewayPreferences::getDescription1() const
{
	return m_gateway->getDescription1();
}

wxString CIRCDDBGatewayPreferences::getDescription2() const
{
	return m_gateway->getDescription2();
}

wxString CIRCDDBGatewayPreferences::getURL() const
{
	return m_gateway->getURL();
}

wxString CIRCDDBGatewayPreferences::getRepeaterBand1() const
{
	return m_repeater1->getBand();
}

HW_TYPE CIRCDDBGatewayPreferences::getRepeaterType1() const
{
	return m_repeater1->getType();
}

wxString CIRCDDBGatewayPreferences::getRepeaterAddress1() const
{
	return m_repeater1->getAddress();
}

unsigned int CIRCDDBGatewayPreferences::getRepeaterPort1() const
{
	return m_repeater1->getPort();
}

unsigned char CIRCDDBGatewayPreferences::getBand11() const
{
	return m_repeater1->getBand1();
}

unsigned char CIRCDDBGatewayPreferences::getBand12() const
{
	return m_repeater1->getBand2();
}

unsigned char CIRCDDBGatewayPreferences::getBand13() const
{
	return m_repeater1->getBand3();
}

wxString CIRCDDBGatewayPreferences::getReflector1() const
{
	return m_repeater1->getReflector();
}

bool CIRCDDBGatewayPreferences::atStartup1() const
{
	return m_repeater1->atStartup();
}

RECONNECT CIRCDDBGatewayPreferences::getReconnect1() const
{
	return m_repeater1->getReconnect();
}

double CIRCDDBGatewayPreferences::getFrequency1() const
{
	return m_repeater1->getFrequency();
}

double CIRCDDBGatewayPreferences::getOffset1() const
{
	return m_repeater1->getOffset();
}

double CIRCDDBGatewayPreferences::getRange1() const
{
	return m_repeater1->getRange();
}

double CIRCDDBGatewayPreferences::getLatitude1() const
{
	return m_repeater1->getLatitude();
}

double CIRCDDBGatewayPreferences::getLongitude1() const
{
	return m_repeater1->getLongitude();
}

double CIRCDDBGatewayPreferences::getAGL1() const
{
	return m_repeater1->getAGL();
}

wxString CIRCDDBGatewayPreferences::getRepeaterBand2() const
{
	return m_repeater2->getBand();
}

HW_TYPE CIRCDDBGatewayPreferences::getRepeaterType2() const
{
	return m_repeater2->getType();
}

wxString CIRCDDBGatewayPreferences::getRepeaterAddress2() const
{
	return m_repeater2->getAddress();
}

unsigned int CIRCDDBGatewayPreferences::getRepeaterPort2() const
{
	return m_repeater2->getPort();
}

wxString CIRCDDBGatewayPreferences::getReflector2() const
{
	return m_repeater2->getReflector();
}

unsigned char CIRCDDBGatewayPreferences::getBand21() const
{
	return m_repeater2->getBand1();
}

unsigned char CIRCDDBGatewayPreferences::getBand22() const
{
	return m_repeater2->getBand2();
}

unsigned char CIRCDDBGatewayPreferences::getBand23() const
{
	return m_repeater2->getBand3();
}

bool CIRCDDBGatewayPreferences::atStartup2() const
{
	return m_repeater2->atStartup();
}

RECONNECT CIRCDDBGatewayPreferences::getReconnect2() const
{
	return m_repeater2->getReconnect();
}

double CIRCDDBGatewayPreferences::getFrequency2() const
{
	return m_repeater2->getFrequency();
}

double CIRCDDBGatewayPreferences::getOffset2() const
{
	return m_repeater2->getOffset();
}

double CIRCDDBGatewayPreferences::getRange2() const
{
	return m_repeater2->getRange();
}

double CIRCDDBGatewayPreferences::getLatitude2() const
{
	return m_repeater2->getLatitude();
}

double CIRCDDBGatewayPreferences::getLongitude2() const
{
	return m_repeater2->getLongitude();
}

double CIRCDDBGatewayPreferences::getAGL2() const
{
	return m_repeater2->getAGL();
}

wxString CIRCDDBGatewayPreferences::getRepeaterBand3() const
{
	return m_repeater3->getBand();
}

HW_TYPE CIRCDDBGatewayPreferences::getRepeaterType3() const
{
	return m_repeater3->getType();
}

wxString CIRCDDBGatewayPreferences::getRepeaterAddress3() const
{
	return m_repeater3->getAddress();
}

unsigned int CIRCDDBGatewayPreferences::getRepeaterPort3() const
{
	return m_repeater3->getPort();
}

unsigned char CIRCDDBGatewayPreferences::getBand31() const
{
	return m_repeater3->getBand1();
}

unsigned char CIRCDDBGatewayPreferences::getBand32() const
{
	return m_repeater3->getBand2();
}

unsigned char CIRCDDBGatewayPreferences::getBand33() const
{
	return m_repeater3->getBand3();
}

wxString CIRCDDBGatewayPreferences::getReflector3() const
{
	return m_repeater3->getReflector();
}

bool CIRCDDBGatewayPreferences::atStartup3() const
{
	return m_repeater3->atStartup();
}

RECONNECT CIRCDDBGatewayPreferences::getReconnect3() const
{
	return m_repeater3->getReconnect();
}

double CIRCDDBGatewayPreferences::getFrequency3() const
{
	return m_repeater3->getFrequency();
}

double CIRCDDBGatewayPreferences::getOffset3() const
{
	return m_repeater3->getOffset();
}

double CIRCDDBGatewayPreferences::getRange3() const
{
	return m_repeater3->getRange();
}

double CIRCDDBGatewayPreferences::getLatitude3() const
{
	return m_repeater3->getLatitude();
}

double CIRCDDBGatewayPreferences::getLongitude3() const
{
	return m_repeater3->getLongitude();
}

double CIRCDDBGatewayPreferences::getAGL3() const
{
	return m_repeater3->getAGL();
}

wxString CIRCDDBGatewayPreferences::getRepeaterBand4() const
{
	return m_repeater4->getBand();
}

HW_TYPE CIRCDDBGatewayPreferences::getRepeaterType4() const
{
	return m_repeater4->getType();
}

wxString CIRCDDBGatewayPreferences::getRepeaterAddress4() const
{
	return m_repeater4->getAddress();
}

unsigned int CIRCDDBGatewayPreferences::getRepeaterPort4() const
{
	return m_repeater4->getPort();
}

unsigned char CIRCDDBGatewayPreferences::getBand41() const
{
	return m_repeater4->getBand1();
}

unsigned char CIRCDDBGatewayPreferences::getBand42() const
{
	return m_repeater4->getBand2();
}

unsigned char CIRCDDBGatewayPreferences::getBand43() const
{
	return m_repeater4->getBand3();
}

wxString CIRCDDBGatewayPreferences::getReflector4() const
{
	return m_repeater4->getReflector();
}

bool CIRCDDBGatewayPreferences::atStartup4() const
{
	return m_repeater4->atStartup();
}

RECONNECT CIRCDDBGatewayPreferences::getReconnect4() const
{
	return m_repeater4->getReconnect();
}

double CIRCDDBGatewayPreferences::getFrequency4() const
{
	return m_repeater4->getFrequency();
}

double CIRCDDBGatewayPreferences::getOffset4() const
{
	return m_repeater4->getOffset();
}

double CIRCDDBGatewayPreferences::getRange4() const
{
	return m_repeater4->getRange();
}

double CIRCDDBGatewayPreferences::getLatitude4() const
{
	return m_repeater4->getLatitude();
}

double CIRCDDBGatewayPreferences::getLongitude4() const
{
	return m_repeater4->getLongitude();
}

double CIRCDDBGatewayPreferences::getAGL4() const
{
	return m_repeater4->getAGL();
}

wxString CIRCDDBGatewayPreferences::getHostname() const
{
	return m_ircDDB->getHostname();
}

wxString CIRCDDBGatewayPreferences::getUsername() const
{
	return m_ircDDB->getUsername();
}

wxString CIRCDDBGatewayPreferences::getPassword() const
{
	return m_ircDDB->getPassword();
}

TEXT_LANG CIRCDDBGatewayPreferences::getLanguage() const
{
	return m_miscellaneous->getLanguage();
}

bool CIRCDDBGatewayPreferences::getInfoEnabled() const
{
	return m_miscellaneous->getInfoEnabled();
}

bool CIRCDDBGatewayPreferences::getEchoEnabled() const
{
	return m_miscellaneous->getEchoEnabled();
}

bool CIRCDDBGatewayPreferences::getLogEnabled() const
{
	return m_miscellaneous->getLogEnabled();
}

bool CIRCDDBGatewayPreferences::getDRATSEnabled() const
{
	return m_miscellaneous->getDRATSEnabled();
}

bool CIRCDDBGatewayPreferences::getDTMFEnabled() const
{
	return m_miscellaneous->getDTMFEnabled();
}

bool CIRCDDBGatewayPreferences::getDExtraEnabled() const
{
	return m_dextra->getEnabled();
}

unsigned int CIRCDDBGatewayPreferences::getMaxDExtraDongles() const
{
	return m_dextra->getMaxDongles();
}

bool CIRCDDBGatewayPreferences::getDPlusEnabled() const
{
	return m_dplus->getEnabled();
}

unsigned int CIRCDDBGatewayPreferences::getMaxDPlusDongles() const
{
	return m_dplus->getMaxDongles();
}

wxString CIRCDDBGatewayPreferences::getDPlusLogin() const
{
	return m_dplus->getLogin();
}

bool CIRCDDBGatewayPreferences::getDCSEnabled() const
{
	return m_dcs->getEnabled();
}

bool CIRCDDBGatewayPreferences::getAPRSEnabled() const
{
	return m_dprs->getEnabled();
}

wxString CIRCDDBGatewayPreferences::getAPRSHostname() const
{
	return m_dprs->getHostname();
}

unsigned int CIRCDDBGatewayPreferences::getAPRSPort() const
{
	return m_dprs->getPort();
}

wxString CIRCDDBGatewayPreferences::getStarNetBand1() const
{
	return m_starNet1->getBand();
}

wxString CIRCDDBGatewayPreferences::getStarNetCallsign1() const
{
	return m_starNet1->getCallsign();
}

wxString CIRCDDBGatewayPreferences::getStarNetLogoff1() const
{
	return m_starNet1->getLogoff();
}

wxString CIRCDDBGatewayPreferences::getStarNetInfo1() const
{
	return m_starNet1->getInfo();
}

wxString CIRCDDBGatewayPreferences::getStarNetPermanent1() const
{
	return m_starNet1->getPermanent();
}

unsigned int CIRCDDBGatewayPreferences::getStarNetUserTimeout1() const
{
	return m_starNet1->getUserTimeout();
}

unsigned int CIRCDDBGatewayPreferences::getStarNetGroupTimeout1() const
{
	return m_starNet1->getGroupTimeout();
}

STARNET_CALLSIGN_SWITCH CIRCDDBGatewayPreferences::getStarNetCallsignSwitch1() const
{
	return m_starNet1->getCallsignSwitch();
}

bool CIRCDDBGatewayPreferences::getStarNetTXMsgSwitch1() const
{
	return m_starNet1->getTXMsgSwitch();
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
wxString CIRCDDBGatewayPreferences::getStarNetLink1() const
{
	return m_starNet1->getReflector();
}
#endif

wxString CIRCDDBGatewayPreferences::getStarNetBand2() const
{
	return m_starNet2->getBand();
}

wxString CIRCDDBGatewayPreferences::getStarNetCallsign2() const
{
	return m_starNet2->getCallsign();
}

wxString CIRCDDBGatewayPreferences::getStarNetLogoff2() const
{
	return m_starNet2->getLogoff();
}

wxString CIRCDDBGatewayPreferences::getStarNetInfo2() const
{
	return m_starNet2->getInfo();
}

wxString CIRCDDBGatewayPreferences::getStarNetPermanent2() const
{
	return m_starNet2->getPermanent();
}

unsigned int CIRCDDBGatewayPreferences::getStarNetUserTimeout2() const
{
	return m_starNet2->getUserTimeout();
}

unsigned int CIRCDDBGatewayPreferences::getStarNetGroupTimeout2() const
{
	return m_starNet2->getGroupTimeout();
}

STARNET_CALLSIGN_SWITCH CIRCDDBGatewayPreferences::getStarNetCallsignSwitch2() const
{
	return m_starNet2->getCallsignSwitch();
}

bool CIRCDDBGatewayPreferences::getStarNetTXMsgSwitch2() const
{
	return m_starNet2->getTXMsgSwitch();
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
wxString CIRCDDBGatewayPreferences::getStarNetLink2() const
{
	return m_starNet2->getReflector();
}
#endif

wxString CIRCDDBGatewayPreferences::getStarNetBand3() const
{
	return m_starNet3->getBand();
}

wxString CIRCDDBGatewayPreferences::getStarNetCallsign3() const
{
	return m_starNet3->getCallsign();
}

wxString CIRCDDBGatewayPreferences::getStarNetLogoff3() const
{
	return m_starNet3->getLogoff();
}

wxString CIRCDDBGatewayPreferences::getStarNetInfo3() const
{
	return m_starNet3->getInfo();
}

wxString CIRCDDBGatewayPreferences::getStarNetPermanent3() const
{
	return m_starNet3->getPermanent();
}

unsigned int CIRCDDBGatewayPreferences::getStarNetUserTimeout3() const
{
	return m_starNet3->getUserTimeout();
}

unsigned int CIRCDDBGatewayPreferences::getStarNetGroupTimeout3() const
{
	return m_starNet3->getGroupTimeout();
}

STARNET_CALLSIGN_SWITCH CIRCDDBGatewayPreferences::getStarNetCallsignSwitch3() const
{
	return m_starNet3->getCallsignSwitch();
}

bool CIRCDDBGatewayPreferences::getStarNetTXMsgSwitch3() const
{
	return m_starNet3->getTXMsgSwitch();
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
wxString CIRCDDBGatewayPreferences::getStarNetLink3() const
{
	return m_starNet3->getReflector();
}
#endif

wxString CIRCDDBGatewayPreferences::getStarNetBand4() const
{
	return m_starNet4->getBand();
}

wxString CIRCDDBGatewayPreferences::getStarNetCallsign4() const
{
	return m_starNet4->getCallsign();
}

wxString CIRCDDBGatewayPreferences::getStarNetLogoff4() const
{
	return m_starNet4->getLogoff();
}

wxString CIRCDDBGatewayPreferences::getStarNetInfo4() const
{
	return m_starNet4->getInfo();
}

wxString CIRCDDBGatewayPreferences::getStarNetPermanent4() const
{
	return m_starNet4->getPermanent();
}

unsigned int CIRCDDBGatewayPreferences::getStarNetUserTimeout4() const
{
	return m_starNet4->getUserTimeout();
}

unsigned int CIRCDDBGatewayPreferences::getStarNetGroupTimeout4() const
{
	return m_starNet4->getGroupTimeout();
}

STARNET_CALLSIGN_SWITCH CIRCDDBGatewayPreferences::getStarNetCallsignSwitch4() const
{
	return m_starNet4->getCallsignSwitch();
}

bool CIRCDDBGatewayPreferences::getStarNetTXMsgSwitch4() const
{
	return m_starNet4->getTXMsgSwitch();
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
wxString CIRCDDBGatewayPreferences::getStarNetLink4() const
{
	return m_starNet4->getReflector();
}
#endif

wxString CIRCDDBGatewayPreferences::getStarNetBand5() const
{
	return m_starNet5->getBand();
}

wxString CIRCDDBGatewayPreferences::getStarNetCallsign5() const
{
	return m_starNet5->getCallsign();
}

wxString CIRCDDBGatewayPreferences::getStarNetLogoff5() const
{
	return m_starNet5->getLogoff();
}

wxString CIRCDDBGatewayPreferences::getStarNetInfo5() const
{
	return m_starNet5->getInfo();
}

wxString CIRCDDBGatewayPreferences::getStarNetPermanent5() const
{
	return m_starNet5->getPermanent();
}

unsigned int CIRCDDBGatewayPreferences::getStarNetUserTimeout5() const
{
	return m_starNet5->getUserTimeout();
}

unsigned int CIRCDDBGatewayPreferences::getStarNetGroupTimeout5() const
{
	return m_starNet5->getGroupTimeout();
}

STARNET_CALLSIGN_SWITCH CIRCDDBGatewayPreferences::getStarNetCallsignSwitch5() const
{
	return m_starNet5->getCallsignSwitch();
}

bool CIRCDDBGatewayPreferences::getStarNetTXMsgSwitch5() const
{
	return m_starNet5->getTXMsgSwitch();
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
wxString CIRCDDBGatewayPreferences::getStarNetLink5() const
{
	return m_starNet5->getReflector();
}
#endif

bool CIRCDDBGatewayPreferences::getRemoteEnabled() const
{
	return m_remote->getEnabled();
}

wxString CIRCDDBGatewayPreferences::getRemotePassword() const
{
	return m_remote->getPassword();
}

unsigned int CIRCDDBGatewayPreferences::getRemotePort() const
{
	return m_remote->getPort();
}

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
#include "IRCDDBGatewayConfigDefs.h"

const unsigned int BORDER_SIZE = 5U;

CIRCDDBGatewayConfigPreferences::CIRCDDBGatewayConfigPreferences(wxWindow* parent, int id, const wxString& gatewayCallsign, const wxString& gatewayAddress, const wxString& icomAddress, unsigned int icomPort, const wxString& hbAddress, unsigned int hbPort, double latitude, double longitude, const wxString& description1, const wxString& description2, const wxString& url, 
						   const wxString& repeaterBand1, HW_TYPE repeaterType1, const wxString& repeaterAddress1, unsigned int repeaterPort1, unsigned char band11, unsigned char band12, unsigned char band13, const wxString& reflector1, bool atStartup1, RECONNECT reconnect1, double frequency1, double offset1, double range1, double latitude1, double longitude1, double agl1, const wxString& description11, const wxString& description12, const wxString& url1,
						   const wxString& repeaterBand2, HW_TYPE repeaterType2, const wxString& repeaterAddress2, unsigned int repeaterPort2, unsigned char band21, unsigned char band22, unsigned char band23, const wxString& reflector2, bool atStartup2, RECONNECT reconnect2, double frequency2, double offset2, double range2, double latitude2, double longitude2, double agl2, const wxString& description21, const wxString& description22, const wxString& url2,
						   const wxString& repeaterBand3, HW_TYPE repeaterType3, const wxString& repeaterAddress3, unsigned int repeaterPort3, unsigned char band31, unsigned char band32, unsigned char band33, const wxString& reflector3, bool atStartup3, RECONNECT reconnect3, double frequency3, double offset3, double range3, double latitude3, double longitude3, double agl3, const wxString& description31, const wxString& description32, const wxString& url3,
						   const wxString& repeaterBand4, HW_TYPE repeaterType4, const wxString& repeaterAddress4, unsigned int repeaterPort4, unsigned char band41, unsigned char band42, unsigned char band43, const wxString& reflector4, bool atStartup4, RECONNECT reconnect4, double frequency4, double offset4, double range4, double latitude4, double longitude4, double agl4, const wxString& description41, const wxString& description42, const wxString& url4,
						   bool ircDDBEnabled, const wxString& ircDDBHostname, const wxString& ircDDBUsername, const wxString& ircDDBPassword, TEXT_LANG language, bool infoEnabled, bool echoEnabled,
						   bool logEnabled, bool dratsEnabled, bool dtmfEnabled, bool aprsEnabled, const wxString& aprsHostname, unsigned int aprsPort, bool dextraEnabled,
						   unsigned int maxDExtraDongles, bool dplusEnabled, unsigned int maxDPlusDongles, const wxString& dplusLogin, bool dcsEnabled, bool ccsEnabled, const wxString& ccsHost,
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
m_repeaterData1(NULL),
m_repeaterInfo1(NULL),
m_repeaterData2(NULL),
m_repeaterInfo2(NULL),
m_repeaterData3(NULL),
m_repeaterInfo3(NULL),
m_repeaterData4(NULL),
m_repeaterInfo4(NULL),
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

	m_gateway = new CIRCDDBGatewayConfigGatewaySet(noteBook, -1, APPLICATION_NAME, gatewayCallsign, gatewayAddress, icomAddress, icomPort, hbAddress, hbPort, latitude, longitude, description1, description2, url);
	noteBook->AddPage(m_gateway, _("Gateway"), true);

	m_repeaterData1 = new CRepeaterDataSet(noteBook, -1, APPLICATION_NAME, repeaterBand1, repeaterType1, repeaterAddress1, repeaterPort1, band11, band12, band13, dplusEnabled, dextraEnabled, dcsEnabled, reflector1, atStartup1, reconnect1);
	noteBook->AddPage(m_repeaterData1, _("Repeater 1"), false);

	m_repeaterInfo1 = new CRepeaterInfoSet(noteBook, -1, APPLICATION_NAME, frequency1, offset1, range1, latitude1, longitude1, agl1, description11, description12, url1);
	noteBook->AddPage(m_repeaterInfo1, _("Repeater 1"), false);

	m_repeaterData2 = new CRepeaterDataSet(noteBook, -1, APPLICATION_NAME, repeaterBand2, repeaterType2, repeaterAddress2, repeaterPort2, band21, band22, band23, dplusEnabled, dextraEnabled, dcsEnabled, reflector2, atStartup2, reconnect2);
	noteBook->AddPage(m_repeaterData2, _("Repeater 2"), false);

	m_repeaterInfo2 = new CRepeaterInfoSet(noteBook, -1, APPLICATION_NAME, frequency2, offset2, range2, latitude2, longitude2, agl2, description21, description22, url2);
	noteBook->AddPage(m_repeaterInfo2, _("Repeater 2"), false);

	m_repeaterData3 = new CRepeaterDataSet(noteBook, -1, APPLICATION_NAME, repeaterBand3, repeaterType3, repeaterAddress3, repeaterPort3, band31, band32, band33, dplusEnabled, dextraEnabled, dcsEnabled, reflector3, atStartup3, reconnect3);
	noteBook->AddPage(m_repeaterData3, _("Repeater 3"), false);

	m_repeaterInfo3 = new CRepeaterInfoSet(noteBook, -1, APPLICATION_NAME, frequency3, offset3, range3, latitude3, longitude3, agl3, description31, description32, url3);
	noteBook->AddPage(m_repeaterInfo3, _("Repeater 3"), false);

	m_repeaterData4 = new CRepeaterDataSet(noteBook, -1, APPLICATION_NAME, repeaterBand4, repeaterType4, repeaterAddress4, repeaterPort4, band41, band42, band43, dplusEnabled, dextraEnabled, dcsEnabled, reflector4, atStartup4, reconnect4);
	noteBook->AddPage(m_repeaterData4, _("Repeater 4"), false);

	m_repeaterInfo4 = new CRepeaterInfoSet(noteBook, -1, APPLICATION_NAME, frequency4, offset4, range4, latitude4, longitude4, agl4, description41, description42, url4);
	noteBook->AddPage(m_repeaterInfo4, _("Repeater 4"), false);

	m_ircDDB = new CIRCDDBGatewayConfigIrcDDBSet(noteBook, -1, APPLICATION_NAME, ircDDBEnabled, ircDDBHostname, ircDDBUsername, ircDDBPassword);
	noteBook->AddPage(m_ircDDB, wxT("ircDDB"), false);

	m_dprs = new CDPRSSet(noteBook, -1, APPLICATION_NAME, aprsEnabled, aprsHostname, aprsPort);
	noteBook->AddPage(m_dprs, wxT("D-PRS"), false);

	m_dextra = new CDExtraSet(noteBook, -1, APPLICATION_NAME, dextraEnabled, maxDExtraDongles, MAX_DEXTRA_LINKS);
	noteBook->AddPage(m_dextra, wxT("DExtra"), false);

	m_dplus = new CDPlusSet(noteBook, -1, APPLICATION_NAME, dplusEnabled, maxDPlusDongles, MAX_DPLUS_LINKS, dplusLogin);
	noteBook->AddPage(m_dplus, wxT("D-Plus"), false);

	m_dcs = new CDCSSet(noteBook, -1, APPLICATION_NAME, dcsEnabled, ccsEnabled, ccsHost);
	noteBook->AddPage(m_dcs, _("DCS and CCS"), false);

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

	m_miscellaneous = new CIRCDDBGatewayConfigMiscellaneousSet(noteBook, -1, APPLICATION_NAME, language, infoEnabled, echoEnabled, logEnabled, dratsEnabled, dtmfEnabled);
	noteBook->AddPage(m_miscellaneous, wxT("Misc"), false);

	mainSizer->Add(noteBook, 1, wxALL | wxGROW, BORDER_SIZE);

	mainSizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	SetAutoLayout(true);
	Layout();

	mainSizer->Fit(this);
	mainSizer->SetSizeHints(this);

	SetSizer(mainSizer);
}

CIRCDDBGatewayConfigPreferences::~CIRCDDBGatewayConfigPreferences()
{
}

bool CIRCDDBGatewayConfigPreferences::Validate()
{
	if (!m_gateway->Validate())
		return false;

	if (!m_repeaterData1->Validate())
		return false;

	if (!m_repeaterInfo1->Validate())
		return false;

	if (!m_repeaterData2->Validate())
		return false;

	if (!m_repeaterInfo2->Validate())
		return false;

	if (!m_repeaterData3->Validate())
		return false;

	if (!m_repeaterInfo3->Validate())
		return false;

	if (!m_repeaterData4->Validate())
		return false;

	if (!m_repeaterInfo4->Validate())
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

wxString CIRCDDBGatewayConfigPreferences::getGatewayCallsign() const
{
	return m_gateway->getCallsign();
}

wxString CIRCDDBGatewayConfigPreferences::getGatewayAddress() const
{
	return m_gateway->getAddress();
}

wxString CIRCDDBGatewayConfigPreferences::getIcomAddress() const
{
	return m_gateway->getIcomAddress();
}

unsigned int CIRCDDBGatewayConfigPreferences::getIcomPort() const
{
	return m_gateway->getIcomPort();
}

wxString CIRCDDBGatewayConfigPreferences::getHBAddress() const
{
	return m_gateway->getHBAddress();
}

unsigned int CIRCDDBGatewayConfigPreferences::getHBPort() const
{
	return m_gateway->getHBPort();
}

double CIRCDDBGatewayConfigPreferences::getLatitude() const
{
	return m_gateway->getLatitude();
}

double CIRCDDBGatewayConfigPreferences::getLongitude() const
{
	return m_gateway->getLongitude();
}

wxString CIRCDDBGatewayConfigPreferences::getDescription1() const
{
	return m_gateway->getDescription1();
}

wxString CIRCDDBGatewayConfigPreferences::getDescription2() const
{
	return m_gateway->getDescription2();
}

wxString CIRCDDBGatewayConfigPreferences::getURL() const
{
	return m_gateway->getURL();
}

wxString CIRCDDBGatewayConfigPreferences::getRepeaterBand1() const
{
	return m_repeaterData1->getBand();
}

HW_TYPE CIRCDDBGatewayConfigPreferences::getRepeaterType1() const
{
	return m_repeaterData1->getType();
}

wxString CIRCDDBGatewayConfigPreferences::getRepeaterAddress1() const
{
	return m_repeaterData1->getAddress();
}

unsigned int CIRCDDBGatewayConfigPreferences::getRepeaterPort1() const
{
	return m_repeaterData1->getPort();
}

unsigned char CIRCDDBGatewayConfigPreferences::getBand11() const
{
	return m_repeaterData1->getBand1();
}

unsigned char CIRCDDBGatewayConfigPreferences::getBand12() const
{
	return m_repeaterData1->getBand2();
}

unsigned char CIRCDDBGatewayConfigPreferences::getBand13() const
{
	return m_repeaterData1->getBand3();
}

wxString CIRCDDBGatewayConfigPreferences::getReflector1() const
{
	return m_repeaterData1->getReflector();
}

bool CIRCDDBGatewayConfigPreferences::atStartup1() const
{
	return m_repeaterData1->atStartup();
}

RECONNECT CIRCDDBGatewayConfigPreferences::getReconnect1() const
{
	return m_repeaterData1->getReconnect();
}

double CIRCDDBGatewayConfigPreferences::getFrequency1() const
{
	return m_repeaterInfo1->getFrequency();
}

double CIRCDDBGatewayConfigPreferences::getOffset1() const
{
	return m_repeaterInfo1->getOffset();
}

double CIRCDDBGatewayConfigPreferences::getRange1() const
{
	return m_repeaterInfo1->getRange();
}

double CIRCDDBGatewayConfigPreferences::getLatitude1() const
{
	return m_repeaterInfo1->getLatitude();
}

double CIRCDDBGatewayConfigPreferences::getLongitude1() const
{
	return m_repeaterInfo1->getLongitude();
}

double CIRCDDBGatewayConfigPreferences::getAGL1() const
{
	return m_repeaterInfo1->getAGL();
}

wxString CIRCDDBGatewayConfigPreferences::getDescription11() const
{
	return m_repeaterInfo1->getDescription1();
}

wxString CIRCDDBGatewayConfigPreferences::getDescription12() const
{
	return m_repeaterInfo1->getDescription2();
}

wxString CIRCDDBGatewayConfigPreferences::getURL1() const
{
	return m_repeaterInfo1->getURL();
}

wxString CIRCDDBGatewayConfigPreferences::getRepeaterBand2() const
{
	return m_repeaterData2->getBand();
}

HW_TYPE CIRCDDBGatewayConfigPreferences::getRepeaterType2() const
{
	return m_repeaterData2->getType();
}

wxString CIRCDDBGatewayConfigPreferences::getRepeaterAddress2() const
{
	return m_repeaterData2->getAddress();
}

unsigned int CIRCDDBGatewayConfigPreferences::getRepeaterPort2() const
{
	return m_repeaterData2->getPort();
}

wxString CIRCDDBGatewayConfigPreferences::getReflector2() const
{
	return m_repeaterData2->getReflector();
}

unsigned char CIRCDDBGatewayConfigPreferences::getBand21() const
{
	return m_repeaterData2->getBand1();
}

unsigned char CIRCDDBGatewayConfigPreferences::getBand22() const
{
	return m_repeaterData2->getBand2();
}

unsigned char CIRCDDBGatewayConfigPreferences::getBand23() const
{
	return m_repeaterData2->getBand3();
}

bool CIRCDDBGatewayConfigPreferences::atStartup2() const
{
	return m_repeaterData2->atStartup();
}

RECONNECT CIRCDDBGatewayConfigPreferences::getReconnect2() const
{
	return m_repeaterData2->getReconnect();
}

double CIRCDDBGatewayConfigPreferences::getFrequency2() const
{
	return m_repeaterInfo2->getFrequency();
}

double CIRCDDBGatewayConfigPreferences::getOffset2() const
{
	return m_repeaterInfo2->getOffset();
}

double CIRCDDBGatewayConfigPreferences::getRange2() const
{
	return m_repeaterInfo2->getRange();
}

double CIRCDDBGatewayConfigPreferences::getLatitude2() const
{
	return m_repeaterInfo2->getLatitude();
}

double CIRCDDBGatewayConfigPreferences::getLongitude2() const
{
	return m_repeaterInfo2->getLongitude();
}

double CIRCDDBGatewayConfigPreferences::getAGL2() const
{
	return m_repeaterInfo2->getAGL();
}

wxString CIRCDDBGatewayConfigPreferences::getDescription21() const
{
	return m_repeaterInfo2->getDescription1();
}

wxString CIRCDDBGatewayConfigPreferences::getDescription22() const
{
	return m_repeaterInfo2->getDescription2();
}

wxString CIRCDDBGatewayConfigPreferences::getURL2() const
{
	return m_repeaterInfo2->getURL();
}

wxString CIRCDDBGatewayConfigPreferences::getRepeaterBand3() const
{
	return m_repeaterData3->getBand();
}

HW_TYPE CIRCDDBGatewayConfigPreferences::getRepeaterType3() const
{
	return m_repeaterData3->getType();
}

wxString CIRCDDBGatewayConfigPreferences::getRepeaterAddress3() const
{
	return m_repeaterData3->getAddress();
}

unsigned int CIRCDDBGatewayConfigPreferences::getRepeaterPort3() const
{
	return m_repeaterData3->getPort();
}

unsigned char CIRCDDBGatewayConfigPreferences::getBand31() const
{
	return m_repeaterData3->getBand1();
}

unsigned char CIRCDDBGatewayConfigPreferences::getBand32() const
{
	return m_repeaterData3->getBand2();
}

unsigned char CIRCDDBGatewayConfigPreferences::getBand33() const
{
	return m_repeaterData3->getBand3();
}

wxString CIRCDDBGatewayConfigPreferences::getReflector3() const
{
	return m_repeaterData3->getReflector();
}

bool CIRCDDBGatewayConfigPreferences::atStartup3() const
{
	return m_repeaterData3->atStartup();
}

RECONNECT CIRCDDBGatewayConfigPreferences::getReconnect3() const
{
	return m_repeaterData3->getReconnect();
}

double CIRCDDBGatewayConfigPreferences::getFrequency3() const
{
	return m_repeaterInfo3->getFrequency();
}

double CIRCDDBGatewayConfigPreferences::getOffset3() const
{
	return m_repeaterInfo3->getOffset();
}

double CIRCDDBGatewayConfigPreferences::getRange3() const
{
	return m_repeaterInfo3->getRange();
}

double CIRCDDBGatewayConfigPreferences::getLatitude3() const
{
	return m_repeaterInfo3->getLatitude();
}

double CIRCDDBGatewayConfigPreferences::getLongitude3() const
{
	return m_repeaterInfo3->getLongitude();
}

double CIRCDDBGatewayConfigPreferences::getAGL3() const
{
	return m_repeaterInfo3->getAGL();
}

wxString CIRCDDBGatewayConfigPreferences::getDescription31() const
{
	return m_repeaterInfo3->getDescription1();
}

wxString CIRCDDBGatewayConfigPreferences::getDescription32() const
{
	return m_repeaterInfo3->getDescription2();
}

wxString CIRCDDBGatewayConfigPreferences::getURL3() const
{
	return m_repeaterInfo3->getURL();
}

wxString CIRCDDBGatewayConfigPreferences::getRepeaterBand4() const
{
	return m_repeaterData4->getBand();
}

HW_TYPE CIRCDDBGatewayConfigPreferences::getRepeaterType4() const
{
	return m_repeaterData4->getType();
}

wxString CIRCDDBGatewayConfigPreferences::getRepeaterAddress4() const
{
	return m_repeaterData4->getAddress();
}

unsigned int CIRCDDBGatewayConfigPreferences::getRepeaterPort4() const
{
	return m_repeaterData4->getPort();
}

unsigned char CIRCDDBGatewayConfigPreferences::getBand41() const
{
	return m_repeaterData4->getBand1();
}

unsigned char CIRCDDBGatewayConfigPreferences::getBand42() const
{
	return m_repeaterData4->getBand2();
}

unsigned char CIRCDDBGatewayConfigPreferences::getBand43() const
{
	return m_repeaterData4->getBand3();
}

wxString CIRCDDBGatewayConfigPreferences::getReflector4() const
{
	return m_repeaterData4->getReflector();
}

bool CIRCDDBGatewayConfigPreferences::atStartup4() const
{
	return m_repeaterData4->atStartup();
}

RECONNECT CIRCDDBGatewayConfigPreferences::getReconnect4() const
{
	return m_repeaterData4->getReconnect();
}

double CIRCDDBGatewayConfigPreferences::getFrequency4() const
{
	return m_repeaterInfo4->getFrequency();
}

double CIRCDDBGatewayConfigPreferences::getOffset4() const
{
	return m_repeaterInfo4->getOffset();
}

double CIRCDDBGatewayConfigPreferences::getRange4() const
{
	return m_repeaterInfo4->getRange();
}

double CIRCDDBGatewayConfigPreferences::getLatitude4() const
{
	return m_repeaterInfo4->getLatitude();
}

double CIRCDDBGatewayConfigPreferences::getLongitude4() const
{
	return m_repeaterInfo4->getLongitude();
}

double CIRCDDBGatewayConfigPreferences::getAGL4() const
{
	return m_repeaterInfo4->getAGL();
}

wxString CIRCDDBGatewayConfigPreferences::getDescription41() const
{
	return m_repeaterInfo4->getDescription1();
}

wxString CIRCDDBGatewayConfigPreferences::getDescription42() const
{
	return m_repeaterInfo4->getDescription2();
}

wxString CIRCDDBGatewayConfigPreferences::getURL4() const
{
	return m_repeaterInfo4->getURL();
}

bool CIRCDDBGatewayConfigPreferences::getIrcDDBEnabled() const
{
	return m_ircDDB->getEnabled();
}

wxString CIRCDDBGatewayConfigPreferences::getIrcDDBHostname() const
{
	return m_ircDDB->getHostname();
}

wxString CIRCDDBGatewayConfigPreferences::getIrcDDBUsername() const
{
	return m_ircDDB->getUsername();
}

wxString CIRCDDBGatewayConfigPreferences::getIrcDDBPassword() const
{
	return m_ircDDB->getPassword();
}

TEXT_LANG CIRCDDBGatewayConfigPreferences::getLanguage() const
{
	return m_miscellaneous->getLanguage();
}

bool CIRCDDBGatewayConfigPreferences::getInfoEnabled() const
{
	return m_miscellaneous->getInfoEnabled();
}

bool CIRCDDBGatewayConfigPreferences::getEchoEnabled() const
{
	return m_miscellaneous->getEchoEnabled();
}

bool CIRCDDBGatewayConfigPreferences::getLogEnabled() const
{
	return m_miscellaneous->getLogEnabled();
}

bool CIRCDDBGatewayConfigPreferences::getDRATSEnabled() const
{
	return m_miscellaneous->getDRATSEnabled();
}

bool CIRCDDBGatewayConfigPreferences::getDTMFEnabled() const
{
	return m_miscellaneous->getDTMFEnabled();
}

bool CIRCDDBGatewayConfigPreferences::getDExtraEnabled() const
{
	return m_dextra->getEnabled();
}

unsigned int CIRCDDBGatewayConfigPreferences::getMaxDExtraDongles() const
{
	return m_dextra->getMaxDongles();
}

bool CIRCDDBGatewayConfigPreferences::getDPlusEnabled() const
{
	return m_dplus->getEnabled();
}

unsigned int CIRCDDBGatewayConfigPreferences::getMaxDPlusDongles() const
{
	return m_dplus->getMaxDongles();
}

wxString CIRCDDBGatewayConfigPreferences::getDPlusLogin() const
{
	return m_dplus->getLogin();
}

bool CIRCDDBGatewayConfigPreferences::getDCSEnabled() const
{
	return m_dcs->getDCSEnabled();
}

bool CIRCDDBGatewayConfigPreferences::getCCSEnabled() const
{
	return m_dcs->getCCSEnabled();
}

wxString CIRCDDBGatewayConfigPreferences::getCCSHost() const
{
	return m_dcs->getCCSHost();
}

bool CIRCDDBGatewayConfigPreferences::getAPRSEnabled() const
{
	return m_dprs->getEnabled();
}

wxString CIRCDDBGatewayConfigPreferences::getAPRSHostname() const
{
	return m_dprs->getHostname();
}

unsigned int CIRCDDBGatewayConfigPreferences::getAPRSPort() const
{
	return m_dprs->getPort();
}

wxString CIRCDDBGatewayConfigPreferences::getStarNetBand1() const
{
	return m_starNet1->getBand();
}

wxString CIRCDDBGatewayConfigPreferences::getStarNetCallsign1() const
{
	return m_starNet1->getCallsign();
}

wxString CIRCDDBGatewayConfigPreferences::getStarNetLogoff1() const
{
	return m_starNet1->getLogoff();
}

wxString CIRCDDBGatewayConfigPreferences::getStarNetInfo1() const
{
	return m_starNet1->getInfo();
}

wxString CIRCDDBGatewayConfigPreferences::getStarNetPermanent1() const
{
	return m_starNet1->getPermanent();
}

unsigned int CIRCDDBGatewayConfigPreferences::getStarNetUserTimeout1() const
{
	return m_starNet1->getUserTimeout();
}

unsigned int CIRCDDBGatewayConfigPreferences::getStarNetGroupTimeout1() const
{
	return m_starNet1->getGroupTimeout();
}

STARNET_CALLSIGN_SWITCH CIRCDDBGatewayConfigPreferences::getStarNetCallsignSwitch1() const
{
	return m_starNet1->getCallsignSwitch();
}

bool CIRCDDBGatewayConfigPreferences::getStarNetTXMsgSwitch1() const
{
	return m_starNet1->getTXMsgSwitch();
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
wxString CIRCDDBGatewayConfigPreferences::getStarNetLink1() const
{
	return m_starNet1->getReflector();
}
#endif

wxString CIRCDDBGatewayConfigPreferences::getStarNetBand2() const
{
	return m_starNet2->getBand();
}

wxString CIRCDDBGatewayConfigPreferences::getStarNetCallsign2() const
{
	return m_starNet2->getCallsign();
}

wxString CIRCDDBGatewayConfigPreferences::getStarNetLogoff2() const
{
	return m_starNet2->getLogoff();
}

wxString CIRCDDBGatewayConfigPreferences::getStarNetInfo2() const
{
	return m_starNet2->getInfo();
}

wxString CIRCDDBGatewayConfigPreferences::getStarNetPermanent2() const
{
	return m_starNet2->getPermanent();
}

unsigned int CIRCDDBGatewayConfigPreferences::getStarNetUserTimeout2() const
{
	return m_starNet2->getUserTimeout();
}

unsigned int CIRCDDBGatewayConfigPreferences::getStarNetGroupTimeout2() const
{
	return m_starNet2->getGroupTimeout();
}

STARNET_CALLSIGN_SWITCH CIRCDDBGatewayConfigPreferences::getStarNetCallsignSwitch2() const
{
	return m_starNet2->getCallsignSwitch();
}

bool CIRCDDBGatewayConfigPreferences::getStarNetTXMsgSwitch2() const
{
	return m_starNet2->getTXMsgSwitch();
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
wxString CIRCDDBGatewayConfigPreferences::getStarNetLink2() const
{
	return m_starNet2->getReflector();
}
#endif

wxString CIRCDDBGatewayConfigPreferences::getStarNetBand3() const
{
	return m_starNet3->getBand();
}

wxString CIRCDDBGatewayConfigPreferences::getStarNetCallsign3() const
{
	return m_starNet3->getCallsign();
}

wxString CIRCDDBGatewayConfigPreferences::getStarNetLogoff3() const
{
	return m_starNet3->getLogoff();
}

wxString CIRCDDBGatewayConfigPreferences::getStarNetInfo3() const
{
	return m_starNet3->getInfo();
}

wxString CIRCDDBGatewayConfigPreferences::getStarNetPermanent3() const
{
	return m_starNet3->getPermanent();
}

unsigned int CIRCDDBGatewayConfigPreferences::getStarNetUserTimeout3() const
{
	return m_starNet3->getUserTimeout();
}

unsigned int CIRCDDBGatewayConfigPreferences::getStarNetGroupTimeout3() const
{
	return m_starNet3->getGroupTimeout();
}

STARNET_CALLSIGN_SWITCH CIRCDDBGatewayConfigPreferences::getStarNetCallsignSwitch3() const
{
	return m_starNet3->getCallsignSwitch();
}

bool CIRCDDBGatewayConfigPreferences::getStarNetTXMsgSwitch3() const
{
	return m_starNet3->getTXMsgSwitch();
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
wxString CIRCDDBGatewayConfigPreferences::getStarNetLink3() const
{
	return m_starNet3->getReflector();
}
#endif

wxString CIRCDDBGatewayConfigPreferences::getStarNetBand4() const
{
	return m_starNet4->getBand();
}

wxString CIRCDDBGatewayConfigPreferences::getStarNetCallsign4() const
{
	return m_starNet4->getCallsign();
}

wxString CIRCDDBGatewayConfigPreferences::getStarNetLogoff4() const
{
	return m_starNet4->getLogoff();
}

wxString CIRCDDBGatewayConfigPreferences::getStarNetInfo4() const
{
	return m_starNet4->getInfo();
}

wxString CIRCDDBGatewayConfigPreferences::getStarNetPermanent4() const
{
	return m_starNet4->getPermanent();
}

unsigned int CIRCDDBGatewayConfigPreferences::getStarNetUserTimeout4() const
{
	return m_starNet4->getUserTimeout();
}

unsigned int CIRCDDBGatewayConfigPreferences::getStarNetGroupTimeout4() const
{
	return m_starNet4->getGroupTimeout();
}

STARNET_CALLSIGN_SWITCH CIRCDDBGatewayConfigPreferences::getStarNetCallsignSwitch4() const
{
	return m_starNet4->getCallsignSwitch();
}

bool CIRCDDBGatewayConfigPreferences::getStarNetTXMsgSwitch4() const
{
	return m_starNet4->getTXMsgSwitch();
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
wxString CIRCDDBGatewayConfigPreferences::getStarNetLink4() const
{
	return m_starNet4->getReflector();
}
#endif

wxString CIRCDDBGatewayConfigPreferences::getStarNetBand5() const
{
	return m_starNet5->getBand();
}

wxString CIRCDDBGatewayConfigPreferences::getStarNetCallsign5() const
{
	return m_starNet5->getCallsign();
}

wxString CIRCDDBGatewayConfigPreferences::getStarNetLogoff5() const
{
	return m_starNet5->getLogoff();
}

wxString CIRCDDBGatewayConfigPreferences::getStarNetInfo5() const
{
	return m_starNet5->getInfo();
}

wxString CIRCDDBGatewayConfigPreferences::getStarNetPermanent5() const
{
	return m_starNet5->getPermanent();
}

unsigned int CIRCDDBGatewayConfigPreferences::getStarNetUserTimeout5() const
{
	return m_starNet5->getUserTimeout();
}

unsigned int CIRCDDBGatewayConfigPreferences::getStarNetGroupTimeout5() const
{
	return m_starNet5->getGroupTimeout();
}

STARNET_CALLSIGN_SWITCH CIRCDDBGatewayConfigPreferences::getStarNetCallsignSwitch5() const
{
	return m_starNet5->getCallsignSwitch();
}

bool CIRCDDBGatewayConfigPreferences::getStarNetTXMsgSwitch5() const
{
	return m_starNet5->getTXMsgSwitch();
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
wxString CIRCDDBGatewayConfigPreferences::getStarNetLink5() const
{
	return m_starNet5->getReflector();
}
#endif

bool CIRCDDBGatewayConfigPreferences::getRemoteEnabled() const
{
	return m_remote->getEnabled();
}

wxString CIRCDDBGatewayConfigPreferences::getRemotePassword() const
{
	return m_remote->getPassword();
}

unsigned int CIRCDDBGatewayConfigPreferences::getRemotePort() const
{
	return m_remote->getPort();
}

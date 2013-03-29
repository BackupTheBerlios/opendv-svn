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

#include "IRCDDBGatewayConfigDefs.h"
#include "IRCDDBGatewayConfigApp.h"
#include "Version.h"
#include "Utils.h"

#include <wx/config.h>
#include <wx/cmdline.h>
#include <wx/filename.h>

IMPLEMENT_APP(CIRCDDBGatewayConfigApp)

const wxChar*       NAME_PARAM = wxT("Gateway Name");
const wxChar*   CONFDIR_OPTION = wxT("confdir");

CIRCDDBGatewayConfigApp::CIRCDDBGatewayConfigApp() :
wxApp(),
m_name(),
m_confDir(),
m_frame(NULL),
m_config(NULL)
{
}

CIRCDDBGatewayConfigApp::~CIRCDDBGatewayConfigApp()
{
}

bool CIRCDDBGatewayConfigApp::OnInit()
{
	SetVendorName(VENDOR_NAME);

	if (!wxApp::OnInit())
		return false;

#if defined(__WINDOWS__)
	m_config = new CIRCDDBGatewayConfig(new wxConfig(APPLICATION_NAME), m_name);
#else
	if (m_confDir.IsEmpty())
		m_confDir = CONF_DIR;

	m_config = new CIRCDDBGatewayConfig(m_confDir, m_name);
#endif

	wxString frameName = APPLICATION_NAME + wxT(" - ");
	if (!m_name.IsEmpty()) {
		frameName.Append(m_name);
		frameName.Append(wxT(" - "));
	}
	frameName.Append(VERSION);

	m_frame = new CIRCDDBGatewayConfigFrame(frameName);
	m_frame->Show();

	SetTopWindow(m_frame);

	return true;
}

int CIRCDDBGatewayConfigApp::OnExit()
{
	delete m_config;

	return 0;
}

void CIRCDDBGatewayConfigApp::OnInitCmdLine(wxCmdLineParser& parser)
{
	parser.AddOption(CONFDIR_OPTION,   wxEmptyString, wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddParam(NAME_PARAM, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);

	wxApp::OnInitCmdLine(parser);
}

bool CIRCDDBGatewayConfigApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
	if (!wxApp::OnCmdLineParsed(parser))
		return false;

	wxString confDir;
	bool found = parser.Found(CONFDIR_OPTION, &confDir);
	if (found)
		m_confDir = confDir;

	if (parser.GetParamCount() > 0U)
		m_name = parser.GetParam(0U);

	return true;
}

void CIRCDDBGatewayConfigApp::getGateway(wxString& callsign, wxString& address, wxString& icomAddress, unsigned int& icomPort, wxString& hbAddress, unsigned int& hbPort, double& latitude, double& longitude, wxString& description1, wxString& description2, wxString& url) const
{
	m_config->getGateway(callsign, address, icomAddress, icomPort, hbAddress, hbPort, latitude, longitude, description1, description2, url);
}

void CIRCDDBGatewayConfigApp::setGateway(const wxString& callsign, const wxString& address, const wxString& icomAddress, unsigned int icomPort, const wxString& hbAddress, unsigned int hbPort, double latitude, double longitude, const wxString& description1, const wxString& description2, const wxString& url)
{
	m_config->setGateway(callsign, address, icomAddress, icomPort, hbAddress, hbPort, latitude, longitude, description1, description2, url);
}

void CIRCDDBGatewayConfigApp::getRepeater1(wxString& callsign, wxString& band, HW_TYPE& type, wxString& address, unsigned int& port, unsigned char& band1, unsigned char& band2, unsigned char& band3, wxString& reflector, bool& atStartup, RECONNECT& reconnect, double& frequency, double& offset, double& range, double& latitude, double& longitude, double& agl, wxString& description1, wxString& description2, wxString& url) const
{
	m_config->getRepeater1(callsign, band, type, address, port, band1, band2, band3, reflector, atStartup, reconnect, frequency, offset, range, latitude, longitude, agl, description1, description2, url);
}

void CIRCDDBGatewayConfigApp::setRepeater1(const wxString& band, HW_TYPE type, const wxString& address, unsigned int port, unsigned char band1, unsigned char band2, unsigned char band3, const wxString& reflector, bool atStartup, RECONNECT reconnect, double frequency, double offset, double range, double latitude, double longitude, double agl, const wxString& description1, const wxString& description2, const wxString& url)
{
	m_config->setRepeater1(band, type, address, port, band1, band2, band3, reflector, atStartup, reconnect, frequency, offset, range, latitude, longitude, agl, description1, description2, url);
}

void CIRCDDBGatewayConfigApp::getRepeater2(wxString& callsign, wxString& band, HW_TYPE& type, wxString& address, unsigned int& port, unsigned char& band1, unsigned char& band2, unsigned char& band3, wxString& reflector, bool& atStartup, RECONNECT& reconnect, double& frequency, double& offset, double& range, double& latitude, double& longitude, double& agl, wxString& description1, wxString& description2, wxString& url) const
{
	m_config->getRepeater2(callsign, band, type, address, port, band1, band2, band3, reflector, atStartup, reconnect, frequency, offset, range, latitude, longitude, agl, description1, description2, url);
}

void CIRCDDBGatewayConfigApp::setRepeater2(const wxString& band, HW_TYPE type, const wxString& address, unsigned int port, unsigned char band1, unsigned char band2, unsigned char band3, const wxString& reflector, bool atStartup, RECONNECT reconnect, double frequency, double offset, double range, double latitude, double longitude, double agl, const wxString& description1, const wxString& description2, const wxString& url)
{
	m_config->setRepeater2(band, type, address, port, band1, band2, band3, reflector, atStartup, reconnect, frequency, offset, range, latitude, longitude, agl, description1, description2, url);
}

void CIRCDDBGatewayConfigApp::getRepeater3(wxString& callsign, wxString& band, HW_TYPE& type, wxString& address, unsigned int& port, unsigned char& band1, unsigned char& band2, unsigned char& band3, wxString& reflector, bool& atStartup, RECONNECT& reconnect, double& frequency, double& offset, double& range, double& latitude, double& longitude, double& agl, wxString& description1, wxString& description2, wxString& url) const
{
	m_config->getRepeater3(callsign, band, type, address, port, band1, band2, band3, reflector, atStartup, reconnect, frequency, offset, range, latitude, longitude, agl, description1, description2, url);
}

void CIRCDDBGatewayConfigApp::setRepeater3(const wxString& band, HW_TYPE type, const wxString& address, unsigned int port, unsigned char band1, unsigned char band2, unsigned char band3, const wxString& reflector, bool atStartup, RECONNECT reconnect, double frequency, double offset, double range, double latitude, double longitude, double agl, const wxString& description1, const wxString& description2, const wxString& url)
{
	m_config->setRepeater3(band, type, address, port, band1, band2, band3, reflector, atStartup, reconnect, frequency, offset, range, latitude, longitude, agl, description1, description2, url);
}

void CIRCDDBGatewayConfigApp::getRepeater4(wxString& callsign, wxString& band, HW_TYPE& type, wxString& address, unsigned int& port, unsigned char& band1, unsigned char& band2, unsigned char& band3, wxString& reflector, bool& atStartup, RECONNECT& reconnect, double& frequency, double& offset, double& range, double& latitude, double& longitude, double& agl, wxString& description1, wxString& description2, wxString& url) const
{
	m_config->getRepeater4(callsign, band, type, address, port, band1, band2, band3, reflector, atStartup, reconnect, frequency, offset, range, latitude, longitude, agl, description1, description2, url);
}

void CIRCDDBGatewayConfigApp::setRepeater4(const wxString& band, HW_TYPE type, const wxString& address, unsigned int port, unsigned char band1, unsigned char band2, unsigned char band3, const wxString& reflector, bool atStartup, RECONNECT reconnect, double frequency, double offset, double range, double latitude, double longitude, double agl, const wxString& description1, const wxString& description2, const wxString& url)
{
	m_config->setRepeater4(band, type, address, port, band1, band2, band3, reflector, atStartup, reconnect, frequency, offset, range, latitude, longitude, agl, description1, description2, url);
}

void CIRCDDBGatewayConfigApp::getIrcDDB(bool& enabled, wxString& hostname, wxString& username, wxString& password) const
{
	m_config->getIrcDDB(enabled, hostname, username, password);
}

void CIRCDDBGatewayConfigApp::setIrcDDB(bool enabled, const wxString& hostname, const wxString& username, const wxString& password)
{
	m_config->setIrcDDB(enabled, hostname, username, password);
}

void CIRCDDBGatewayConfigApp::getDPRS(bool& enabled, wxString& hostname, unsigned int& port) const
{
	m_config->getDPRS(enabled, hostname, port);
}

void CIRCDDBGatewayConfigApp::setDPRS(bool enabled, const wxString& hostname, unsigned int port)
{
	m_config->setDPRS(enabled, hostname, port);
}

void CIRCDDBGatewayConfigApp::getDExtra(bool& enabled, unsigned int& maxDongles) const
{
	m_config->getDExtra(enabled, maxDongles);
}

void CIRCDDBGatewayConfigApp::setDExtra(bool enabled, unsigned int maxDongles)
{
	m_config->setDExtra(enabled, maxDongles);
}

void CIRCDDBGatewayConfigApp::getDPlus(bool& enabled, unsigned int& maxDongles, wxString& login) const
{
	m_config->getDPlus(enabled, maxDongles, login);
}

void CIRCDDBGatewayConfigApp::setDPlus(bool enabled, unsigned int maxDongles, const wxString& login)
{
	m_config->setDPlus(enabled, maxDongles, login);
}

void CIRCDDBGatewayConfigApp::getDCS(bool& dcsEnabled, bool& ccsEnabled, wxString& ccsHost) const
{
	m_config->getDCS(dcsEnabled, ccsEnabled, ccsHost);
}

void CIRCDDBGatewayConfigApp::setDCS(bool dcsEnabled, bool ccsEnabled, const wxString& ccsHost)
{
	m_config->setDCS(dcsEnabled, ccsEnabled, ccsHost);
}

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
void CIRCDDBGatewayConfigApp::getStarNet1(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch, wxString& reflector) const
{
	m_config->getStarNet1(band, callsign, logoff, info, permanent, userTimeout, groupTimeout, callsignSwitch, txMsgSwitch, reflector);
}

void CIRCDDBGatewayConfigApp::setStarNet1(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector)
{
	m_config->setStarNet1(band, callsign, logoff, info, permanent, userTimeout, groupTimeout, callsignSwitch, txMsgSwitch, reflector);
}

void CIRCDDBGatewayConfigApp::getStarNet2(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch, wxString& reflector) const
{
	m_config->getStarNet2(band, callsign, logoff, info, permanent, userTimeout, groupTimeout, callsignSwitch, txMsgSwitch, reflector);
}

void CIRCDDBGatewayConfigApp::setStarNet2(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector)
{
	m_config->setStarNet2(band, callsign, logoff, info, permanent, userTimeout, groupTimeout, callsignSwitch, txMsgSwitch, reflector);
}

void CIRCDDBGatewayConfigApp::getStarNet3(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch, wxString& reflector) const
{
	m_config->getStarNet3(band, callsign, logoff, info, permanent, userTimeout, groupTimeout, callsignSwitch, txMsgSwitch, reflector);
}

void CIRCDDBGatewayConfigApp::setStarNet3(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector)
{
	m_config->setStarNet3(band, callsign, logoff, info, permanent, userTimeout, groupTimeout, callsignSwitch, txMsgSwitch, reflector);
}

void CIRCDDBGatewayConfigApp::getStarNet4(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch, wxString& reflector) const
{
	m_config->getStarNet4(band, callsign, logoff, info, permanent, userTimeout, groupTimeout, callsignSwitch, txMsgSwitch, reflector);
}

void CIRCDDBGatewayConfigApp::setStarNet4(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector)
{
	m_config->setStarNet4(band, callsign, logoff, info, permanent, userTimeout, groupTimeout, callsignSwitch, txMsgSwitch, reflector);
}

void CIRCDDBGatewayConfigApp::getStarNet5(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch, wxString& reflector) const
{
	m_config->getStarNet5(band, callsign, logoff, info, permanent, userTimeout, groupTimeout, callsignSwitch, txMsgSwitch, reflector);
}

void CIRCDDBGatewayConfigApp::setStarNet5(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector)
{
	m_config->setStarNet5(band, callsign, logoff, info, permanent, userTimeout, groupTimeout, callsignSwitch, txMsgSwitch, reflector);
}
#else
void CIRCDDBGatewayConfigApp::getStarNet1(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch) const
{
	m_config->getStarNet1(band, callsign, logoff, info, permanent, userTimeout, groupTimeout, callsignSwitch, txMsgSwitch);
}

void CIRCDDBGatewayConfigApp::setStarNet1(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch)
{
	m_config->setStarNet1(band, callsign, logoff, info, permanent, userTimeout, groupTimeout, callsignSwitch, txMsgSwitch);
}

void CIRCDDBGatewayConfigApp::getStarNet2(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch) const
{
	m_config->getStarNet2(band, callsign, logoff, info, permanent, userTimeout, groupTimeout, callsignSwitch, txMsgSwitch);
}

void CIRCDDBGatewayConfigApp::setStarNet2(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch)
{
	m_config->setStarNet2(band, callsign, logoff, info, permanent, userTimeout, groupTimeout, callsignSwitch, txMsgSwitch);
}

void CIRCDDBGatewayConfigApp::getStarNet3(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch) const
{
	m_config->getStarNet3(band, callsign, logoff, info, permanent, userTimeout, groupTimeout, callsignSwitch, txMsgSwitch);
}

void CIRCDDBGatewayConfigApp::setStarNet3(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch)
{
	m_config->setStarNet3(band, callsign, logoff, info, permanent, userTimeout, groupTimeout, callsignSwitch, txMsgSwitch);
}

void CIRCDDBGatewayConfigApp::getStarNet4(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch) const
{
	m_config->getStarNet4(band, callsign, logoff, info, permanent, userTimeout, groupTimeout, callsignSwitch, txMsgSwitch);
}

void CIRCDDBGatewayConfigApp::setStarNet4(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch)
{
	m_config->setStarNet4(band, callsign, logoff, info, permanent, userTimeout, groupTimeout, callsignSwitch, txMsgSwitch);
}

void CIRCDDBGatewayConfigApp::getStarNet5(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch) const
{
	m_config->getStarNet5(band, callsign, logoff, info, permanent, userTimeout, groupTimeout, callsignSwitch, txMsgSwitch);
}

void CIRCDDBGatewayConfigApp::setStarNet5(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch)
{
	m_config->setStarNet5(band, callsign, logoff, info, permanent, userTimeout, groupTimeout, callsignSwitch, txMsgSwitch);
}
#endif

void CIRCDDBGatewayConfigApp::getRemote(bool& enabled, wxString& password, unsigned int& port) const
{
	m_config->getRemote(enabled, password, port);
}

void CIRCDDBGatewayConfigApp::setRemote(bool enabled, const wxString& password, unsigned int port)
{
	m_config->setRemote(enabled, password, port);
}

void CIRCDDBGatewayConfigApp::getMiscellaneous(TEXT_LANG& language, bool& infoEnabled, bool& echoEnabled, bool& logEnabled, bool& dratsEnabled, bool& dtmfEnabled) const
{
	m_config->getMiscellaneous(language, infoEnabled, echoEnabled, logEnabled, dratsEnabled, dtmfEnabled);
}

void CIRCDDBGatewayConfigApp::setMiscellaneous(TEXT_LANG language, bool infoEnabled, bool echoEnabled, bool logEnabled, bool dratsEnabled, bool dtmfEnabled)
{
	m_config->setMiscellaneous(language, infoEnabled, echoEnabled, logEnabled, dratsEnabled, dtmfEnabled);
}

bool CIRCDDBGatewayConfigApp::writeConfig()
{
	return m_config->write();
}

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

#ifndef	IRCDDBGatewayApp_H
#define	IRCDDBGatewayApp_H

#include "IRCDDBGatewayThreadHelper.h"
#include "IRCDDBGatewayStatusData.h"
#include "IRCDDBGatewayConfig.h"
#include "IRCDDBGatewayFrame.h"
#include "Defs.h"

#include <wx/wx.h>
#include <wx/snglinst.h>

class CIRCDDBGatewayApp : public wxApp {

public:
	CIRCDDBGatewayApp();
	virtual ~CIRCDDBGatewayApp();

	virtual bool OnInit();
	virtual int  OnExit();
	
	virtual void OnInitCmdLine(wxCmdLineParser& parser);
	virtual bool OnCmdLineParsed(wxCmdLineParser& parser);

	// This is overridden because dialog boxes from threads are bad news
#if defined(__WXDEBUG__)
	virtual void OnAssertFailure(const wxChar* file, int line, const wxChar* func, const wxChar* cond, const wxChar* msg);
#endif

	virtual void showLog(const wxString& text);

	virtual CIRCDDBGatewayStatusData* getStatus() const;

	virtual void getGateway(wxString& callsign, wxString& address, wxString& icomAddress, unsigned int& icomPort, wxString& hbAddress, unsigned int& hbPort, double& latitude, double& longitude, wxString& description1, wxString& description2, wxString& url) const;
	virtual void setGateway(const wxString& callsign, const wxString& address, const wxString& icomAddress, unsigned int icomPort, const wxString& hbAddress, unsigned int hbPort, double latitude, double longitude, const wxString& description1, const wxString& description2, const wxString& url);

	virtual void getRepeater1(wxString& callsign, wxString& band, HW_TYPE& type, wxString& address, unsigned int& port, unsigned char& band1, unsigned char& band2, unsigned char& band3, wxString& reflector, bool& atStartup, RECONNECT& reconnect, double& frequency, double& offset, double& range, double& latitude, double& longitude, double& agl, wxString& description1, wxString& description2, wxString& url) const;
	virtual void setRepeater1(const wxString& band, HW_TYPE type, const wxString& address, unsigned int port, unsigned char band1, unsigned char band2, unsigned char band3, const wxString& reflector, bool atStartup, RECONNECT reconnect, double frequency, double offset, double range, double latitude, double longitude, double agl, const wxString& description1, const wxString& description2, const wxString& url);

	virtual void getRepeater2(wxString& callsign, wxString& band, HW_TYPE& type, wxString& address, unsigned int& port, unsigned char& band1, unsigned char& band2, unsigned char& band3, wxString& reflector, bool& atStartup, RECONNECT& reconnect, double& frequency, double& offset, double& range, double& latitude, double& longitude, double& agl, wxString& description1, wxString& description2, wxString& url) const;
	virtual void setRepeater2(const wxString& band, HW_TYPE type, const wxString& address, unsigned int port, unsigned char band1, unsigned char band2, unsigned char band3, const wxString& reflector, bool atStartup, RECONNECT reconnect, double frequency, double offset, double range, double latitude, double longitude, double agl, const wxString& description1, const wxString& description2, const wxString& url);

	virtual void getRepeater3(wxString& callsign, wxString& band, HW_TYPE& type, wxString& address, unsigned int& port, unsigned char& band1, unsigned char& band2, unsigned char& band3, wxString& reflector, bool& atStartup, RECONNECT& reconnect, double& frequency, double& offset, double& range, double& latitude, double& longitude, double& agl, wxString& description1, wxString& description2, wxString& url) const;
	virtual void setRepeater3(const wxString& band, HW_TYPE type, const wxString& address, unsigned int port, unsigned char band1, unsigned char band2, unsigned char band3, const wxString& reflector, bool atStartup, RECONNECT reconnect, double frequency, double offset, double range, double latitude, double longitude, double agl, const wxString& description1, const wxString& description2, const wxString& url);

	virtual void getRepeater4(wxString& callsign, wxString& band, HW_TYPE& type, wxString& address, unsigned int& port, unsigned char& band1, unsigned char& band2, unsigned char& band3, wxString& reflector, bool& atStartup, RECONNECT& reconnect, double& frequency, double& offset, double& range, double& latitude, double& longitude, double& agl, wxString& description1, wxString& description2, wxString& url) const;
	virtual void setRepeater4(const wxString& band, HW_TYPE type, const wxString& address, unsigned int port, unsigned char band1, unsigned char band2, unsigned char band3, const wxString& reflector, bool atStartup, RECONNECT reconnect, double frequency, double offset, double range, double latitude, double longitude, double agl, const wxString& description1, const wxString& description2, const wxString& url);

	virtual void getIrcDDB(bool& enabled, wxString& hostname, wxString& username, wxString& password) const;
	virtual void setIrcDDB(bool enabled, const wxString& hostname, const wxString& username, const wxString& password);

	virtual void getDPRS(bool& enabled, wxString& hostname, unsigned int& port) const;
	virtual void setDPRS(bool enabled, const wxString& hostname, unsigned int port);

	virtual void getDExtra(bool& enabled, unsigned int& maxDongles) const;
	virtual void setDExtra(bool enabled, unsigned int maxDongles);

	virtual void getDPlus(bool& dplusEnabled, unsigned int& maxDongles, wxString& dplusLogin) const;
	virtual void setDPlus(bool dplusEnabled, unsigned int maxDongles, const wxString& dplusLogin);

	virtual void getDCS(bool& dcsEnabled, bool& ccsEnabled, wxString& ccsHost) const;
	virtual void setDCS(bool dcsEnabled, bool ccsEnabled, const wxString& ccsHost);

#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	virtual void getStarNet1(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch, wxString& reflector) const;
	virtual void setStarNet1(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector);

	virtual void getStarNet2(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch, wxString& reflector) const;
	virtual void setStarNet2(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector);

	virtual void getStarNet3(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch, wxString& reflector) const;
	virtual void setStarNet3(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector);

	virtual void getStarNet4(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch, wxString& reflector) const;
	virtual void setStarNet4(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector);

	virtual void getStarNet5(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch, wxString& reflector) const;
	virtual void setStarNet5(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector);
#else
	virtual void getStarNet1(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch) const;
	virtual void setStarNet1(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch);

	virtual void getStarNet2(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch) const;
	virtual void setStarNet2(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch);

	virtual void getStarNet3(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch) const;
	virtual void setStarNet3(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch);

	virtual void getStarNet4(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch) const;
	virtual void setStarNet4(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch);

	virtual void getStarNet5(wxString& band, wxString& callsign, wxString& logoff, wxString& info, wxString& permanent, unsigned int& userTimeout, unsigned int& groupTimeout, STARNET_CALLSIGN_SWITCH& callsignSwitch, bool& txMsgSwitch) const;
	virtual void setStarNet5(const wxString& band, const wxString& callsign, const wxString& logoff, const wxString& info, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch);
#endif

	virtual void getRemote(bool& enabled, wxString& password, unsigned int& port) const;
	virtual void setRemote(bool enabled, const wxString& password, unsigned int port);

	virtual void getMiscellaneous(TEXT_LANG& language, bool& infoEnabled, bool& echoEnabled, bool& logEnabled, bool& dratsEnabled, bool& dtmfEnabled) const;
	virtual void setMiscellaneous(TEXT_LANG language, bool infoEnabled, bool echoEnabled, bool logEnabled, bool dratsEnabled, bool dtmfEnabled);

	virtual void getPosition(int& x, int& y) const;
	virtual void setPosition(int x, int y);

	virtual bool writeConfig();

private:
	wxString                    m_name;
	bool                        m_nolog;
	bool                        m_gui;
	wxString                    m_logDir;
	wxString                    m_confDir;
	CIRCDDBGatewayFrame*        m_frame;
	CIRCDDBGatewayThreadHelper* m_thread;
	CIRCDDBGatewayConfig*       m_config;
	wxSingleInstanceChecker*    m_checker;
	wxLogChain*                 m_logChain;

	void createThread();
};

DECLARE_APP(CIRCDDBGatewayApp)

#endif

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

#ifndef	IRCDDBGatewayThread_H
#define	IRCDDBGatewayThread_H

#include "IcomRepeaterProtocolHandler.h"
#include "HBRepeaterProtocolHandler.h"
#include "RepeaterProtocolHandler.h"
#include "IRCDDBGatewayStatusData.h"
#include "DExtraProtocolHandler.h"
#include "DPlusProtocolHandler.h"
#include "DCSProtocolHandler.h"
#include "G2ProtocolHandler.h"
#include "RemoteHandler.h"
#include "CacheManager.h"
#include "APRSWriter.h"
#include "Timer.h"
#include "Defs.h"
#include "IRC.h"

#include <wx/wx.h>

class CIRCDDBGatewayThread {
public:
	CIRCDDBGatewayThread(const wxString& logDir, const wxString& name);
	virtual ~CIRCDDBGatewayThread();

	virtual void setGateway(const wxString& callsign, const wxString& address, double latitude, double longitude, const wxString& description1, const wxString& description2, const wxString& url);
	virtual void addRepeater(const wxString& callsign, const wxString& band, const wxString& address, unsigned int port, HW_TYPE hwType, const wxString& reflector, bool atStartup, RECONNECT reconnect, bool dratsEnabled, double frequency, double offset, double range, double agl, IRepeaterProtocolHandler* handler, unsigned char band1 = 0x00U, unsigned char band2 = 0x00U, unsigned char band3 = 0x00U);
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	virtual void addStarNet(const wxString& callsign, const wxString& logoff, const wxString& repeater, const wxString& infoText, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch, const wxString& reflector);
#else
	virtual void addStarNet(const wxString& callsign, const wxString& logoff, const wxString& repeater, const wxString& infoText, const wxString& permanent, unsigned int userTimeout, unsigned int groupTimeout, STARNET_CALLSIGN_SWITCH callsignSwitch, bool txMsgSwitch);
#endif
	virtual void setIcomRepeaterHandler(CIcomRepeaterProtocolHandler* handler);
	virtual void setHBRepeaterHandler(CHBRepeaterProtocolHandler* handler);
	virtual void setIRC(IIRC* irc);
	virtual void setLanguage(TEXT_LANG language);
	virtual void setDExtra(bool enabled, unsigned int maxDongles);
	virtual void setDPlus(bool enabled, unsigned int maxDongles, const wxString& login);
	virtual void setDCS(bool enabled);
	virtual void setLog(bool enabled);
	virtual void setAPRSWriter(CAPRSWriter* writer);
	virtual void setInfoEnabled(bool enabled);
	virtual void setEchoEnabled(bool enabled);
	virtual void setDTMFEnabled(bool enabled);
	virtual void setDDModeEnabled(bool enabled);
	virtual void setRemote(bool enabled, const wxString& password, unsigned int port);

	virtual CIRCDDBGatewayStatusData* getStatus() const;

	virtual void run();
	virtual void kill();

private:
	wxString                  m_logDir;
	wxString                  m_name;
	bool                      m_killed;
	bool                      m_stopped;
	wxString                  m_gatewayCallsign;
	wxString                  m_gatewayAddress;
	double                    m_latitude;
	double                    m_longitude;
	wxString                  m_description1;
	wxString                  m_description2;
	wxString                  m_url;
	CIcomRepeaterProtocolHandler* m_icomRepeaterHandler;
	CHBRepeaterProtocolHandler*   m_hbRepeaterHandler;
	CDExtraProtocolHandler*   m_dextraHandler;
	CDPlusProtocolHandler*    m_dplusHandler;
	CDCSProtocolHandler*      m_dcsHandler;
	CG2ProtocolHandler*       m_g2Handler;
	CAPRSWriter*              m_aprsWriter;
	IIRC*                     m_irc;
	CCacheManager             m_cache;
	TEXT_LANG                 m_language;
	bool                      m_dextraEnabled;
	unsigned int              m_dextraPort;
	unsigned int              m_dextraMaxDongles;
	bool                      m_dplusEnabled;
	unsigned int              m_dplusPort;
	unsigned int              m_dplusMaxDongles;
	wxString                  m_dplusLogin;
	bool                      m_dcsEnabled;
	unsigned int              m_dcsPort;
	bool                      m_infoEnabled;
	bool                      m_echoEnabled;
	bool                      m_dtmfEnabled;
	bool                      m_logEnabled;
	bool                      m_ddModeEnabled;
	IRCDDB_STATUS             m_lastStatus;
	CTimer                    m_statusTimer1;
	CTimer                    m_statusTimer2;
	bool                      m_remoteEnabled;
	wxString                  m_remotePassword;
	unsigned int              m_remotePort;
	CRemoteHandler*           m_remote;
	CTimer                    m_statusFileTimer;
	wxString                  m_status1;
	wxString                  m_status2;
	wxString                  m_status3;
	wxString                  m_status4;
	wxString                  m_status5;

	void processIrcDDB();
	void processRepeater(IRepeaterProtocolHandler* handler);
	void processDExtra();
	void processDPlus();
	void processDCS();
	void processG2();
	void processDD();

	void loadReflectors();
	void loadDExtraReflectors();
	void loadDPlusReflectors();
	void loadDCSReflectors();

	void writeStatus();

	void readStatusFiles();
	void readStatusFile(const wxString& filename, unsigned int n, wxString& var);
};

#endif

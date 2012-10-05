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

#ifndef	IRCDDBGatewayPreferences_H
#define	IRCDDBGatewayPreferences_H

#include <wx/wx.h>
#include <wx/notebook.h>

#include "IRCDDBGatewayMiscellaneousSet.h"
#include "IRCDDBGatewayGatewaySet.h"
#include "RepeaterInfoSet.h"
#include "RepeaterDataSet.h"
#include "StarNetSet.h"
#include "RemoteSet.h"
#include "IrcDDBSet.h"
#include "DExtraSet.h"
#include "DPlusSet.h"
#include "DPRSSet.h"
#include "DCSSet.h"

class CIRCDDBGatewayPreferences : public wxDialog {
public:
	CIRCDDBGatewayPreferences(wxWindow* parent, int id, const wxString& gatewayCallsign, const wxString& gatewayAddress, const wxString& icomAddress,
				 unsigned int icomPort, const wxString& hbAddress, unsigned int hbPort, double latitude, double longitude,
				 const wxString& description1, const wxString& description2, const wxString& url,
				 const wxString& repeaterBand1, HW_TYPE repeaterType1, const wxString& repeaterAddress1, unsigned int repeaterPort1, unsigned char band11, unsigned char band12, unsigned char band13, const wxString& reflector1, bool atStartup1, RECONNECT reconnect1, double frequency1, double offset1, double range1, double latitude1, double longitude1, double agl1, const wxString& description11, const wxString& description12, const wxString& url1,
				 const wxString& repeaterBand2, HW_TYPE repeaterType2, const wxString& repeaterAddress2, unsigned int repeaterPort2, unsigned char band21, unsigned char band22, unsigned char band23, const wxString& reflector2, bool atStartup2, RECONNECT reconnect2, double frequency2, double offset2, double range2, double latitude2, double longitude2, double agl2, const wxString& description21, const wxString& description22, const wxString& url2,
				 const wxString& repeaterBand3, HW_TYPE repeaterType3, const wxString& repeaterAddress3, unsigned int repeaterPort3, unsigned char band31, unsigned char band32, unsigned char band33, const wxString& reflector3, bool atStartup3, RECONNECT reconnect3, double frequency3, double offset3, double range3, double latitude3, double longitude3, double agl3, const wxString& description31, const wxString& description32, const wxString& url3,
				 const wxString& repeaterBand4, HW_TYPE repeaterType4, const wxString& repeaterAddress4, unsigned int repeaterPort4, unsigned char band41, unsigned char band42, unsigned char band43, const wxString& reflector4, bool atStartup4, RECONNECT reconnect4, double frequency4, double offset4, double range4, double latitude4, double longitude4, double agl4, const wxString& description41, const wxString& description42, const wxString& url4,
				 const wxString& hostname, const wxString& username, const wxString& password, TEXT_LANG language, bool infoEnabled, bool echoEnabled, bool logEnabled,
				 bool dratsEnabled, bool dtmfEnabled, bool aprsEnabled, const wxString& aprsHostname,
				 unsigned int aprsPort, bool dextraEnabled, unsigned int maxDExtraDongles,
				 bool dplusEnabled, unsigned int maxDPlusDongles, const wxString& dplusLogin,
				 bool dcsEnabled,
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
				 bool remoteEnabled, const wxString& remotePassword, unsigned int remotePort);
	virtual ~CIRCDDBGatewayPreferences();

	virtual bool Validate();

	virtual wxString     getGatewayCallsign() const;
	virtual wxString     getGatewayAddress() const;
	virtual wxString     getIcomAddress() const;
	virtual unsigned int getIcomPort() const;
	virtual wxString     getHBAddress() const;
	virtual unsigned int getHBPort() const;
	virtual double       getLatitude() const;
	virtual double       getLongitude() const;
	virtual wxString     getDescription1() const;
	virtual wxString     getDescription2() const;
	virtual wxString     getURL() const;

	virtual wxString     getRepeaterBand1() const;
	virtual HW_TYPE      getRepeaterType1() const;
	virtual wxString     getRepeaterAddress1() const;
	virtual unsigned int getRepeaterPort1() const;
	virtual unsigned char getBand11() const;
	virtual unsigned char getBand12() const;
	virtual unsigned char getBand13() const;
	virtual wxString     getReflector1() const;
	virtual bool         atStartup1() const;
	virtual RECONNECT    getReconnect1() const;

	virtual double       getFrequency1() const;
	virtual double       getOffset1() const;
	virtual double       getRange1() const;
	virtual double       getLatitude1() const;
	virtual double       getLongitude1() const;
	virtual double       getAGL1() const;
	virtual wxString     getDescription11() const;
	virtual wxString     getDescription12() const;
	virtual wxString     getURL1() const;

	virtual wxString     getRepeaterBand2() const;
	virtual HW_TYPE      getRepeaterType2() const;
	virtual wxString     getRepeaterAddress2() const;
	virtual unsigned int getRepeaterPort2() const;
	virtual unsigned char getBand21() const;
	virtual unsigned char getBand22() const;
	virtual unsigned char getBand23() const;
	virtual wxString     getReflector2() const;
	virtual bool         atStartup2() const;
	virtual RECONNECT    getReconnect2() const;

	virtual double       getFrequency2() const;
	virtual double       getOffset2() const;
	virtual double       getRange2() const;
	virtual double       getLatitude2() const;
	virtual double       getLongitude2() const;
	virtual double       getAGL2() const;
	virtual wxString     getDescription21() const;
	virtual wxString     getDescription22() const;
	virtual wxString     getURL2() const;

	virtual wxString     getRepeaterBand3() const;
	virtual HW_TYPE      getRepeaterType3() const;
	virtual wxString     getRepeaterAddress3() const;
	virtual unsigned int getRepeaterPort3() const;
	virtual unsigned char getBand31() const;
	virtual unsigned char getBand32() const;
	virtual unsigned char getBand33() const;
	virtual wxString     getReflector3() const;
	virtual bool         atStartup3() const;
	virtual RECONNECT    getReconnect3() const;

	virtual double       getFrequency3() const;
	virtual double       getOffset3() const;
	virtual double       getRange3() const;
	virtual double       getLatitude3() const;
	virtual double       getLongitude3() const;
	virtual double       getAGL3() const;
	virtual wxString     getDescription31() const;
	virtual wxString     getDescription32() const;
	virtual wxString     getURL3() const;

	virtual wxString     getRepeaterBand4() const;
	virtual HW_TYPE      getRepeaterType4() const;
	virtual wxString     getRepeaterAddress4() const;
	virtual unsigned int getRepeaterPort4() const;
	virtual unsigned char getBand41() const;
	virtual unsigned char getBand42() const;
	virtual unsigned char getBand43() const;
	virtual wxString     getReflector4() const;
	virtual bool         atStartup4() const;
	virtual RECONNECT    getReconnect4() const;

	virtual double       getFrequency4() const;
	virtual double       getOffset4() const;
	virtual double       getRange4() const;
	virtual double       getLatitude4() const;
	virtual double       getLongitude4() const;
	virtual double       getAGL4() const;
	virtual wxString     getDescription41() const;
	virtual wxString     getDescription42() const;
	virtual wxString     getURL4() const;

	virtual wxString     getHostname() const;
	virtual wxString     getUsername() const;
	virtual wxString     getPassword() const;

	virtual TEXT_LANG    getLanguage() const;
	virtual bool         getInfoEnabled() const;
	virtual bool         getEchoEnabled() const;
	virtual bool         getLogEnabled() const;
	virtual bool         getDRATSEnabled() const;
	virtual bool         getDTMFEnabled() const;

	virtual bool         getDExtraEnabled() const;
	virtual unsigned int getMaxDExtraDongles() const;

	virtual bool         getDPlusEnabled() const;
	virtual unsigned int getMaxDPlusDongles() const;
	virtual wxString     getDPlusLogin() const;

	virtual bool         getDCSEnabled() const;

	virtual bool         getAPRSEnabled() const;
	virtual wxString     getAPRSHostname() const;
	virtual unsigned int getAPRSPort() const;

	virtual wxString     getStarNetBand1() const;
	virtual wxString     getStarNetCallsign1() const;
	virtual wxString     getStarNetLogoff1() const;
	virtual wxString     getStarNetInfo1() const;
	virtual wxString     getStarNetPermanent1() const;
	virtual unsigned int getStarNetUserTimeout1() const;
	virtual unsigned int getStarNetGroupTimeout1() const;
	virtual STARNET_CALLSIGN_SWITCH getStarNetCallsignSwitch1() const;
	virtual bool         getStarNetTXMsgSwitch1() const;
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	virtual wxString     getStarNetLink1() const;
#endif

	virtual wxString     getStarNetBand2() const;
	virtual wxString     getStarNetCallsign2() const;
	virtual wxString     getStarNetLogoff2() const;
	virtual wxString     getStarNetInfo2() const;
	virtual wxString     getStarNetPermanent2() const;
	virtual unsigned int getStarNetUserTimeout2() const;
	virtual unsigned int getStarNetGroupTimeout2() const;
	virtual STARNET_CALLSIGN_SWITCH getStarNetCallsignSwitch2() const;
	virtual bool         getStarNetTXMsgSwitch2() const;
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	virtual wxString     getStarNetLink2() const;
#endif

	virtual wxString     getStarNetBand3() const;
	virtual wxString     getStarNetCallsign3() const;
	virtual wxString     getStarNetLogoff3() const;
	virtual wxString     getStarNetInfo3() const;
	virtual wxString     getStarNetPermanent3() const;
	virtual unsigned int getStarNetUserTimeout3() const;
	virtual unsigned int getStarNetGroupTimeout3() const;
	virtual STARNET_CALLSIGN_SWITCH getStarNetCallsignSwitch3() const;
	virtual bool         getStarNetTXMsgSwitch3() const;
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	virtual wxString     getStarNetLink3() const;
#endif

	virtual wxString     getStarNetBand4() const;
	virtual wxString     getStarNetCallsign4() const;
	virtual wxString     getStarNetLogoff4() const;
	virtual wxString     getStarNetInfo4() const;
	virtual wxString     getStarNetPermanent4() const;
	virtual unsigned int getStarNetUserTimeout4() const;
	virtual unsigned int getStarNetGroupTimeout4() const;
	virtual STARNET_CALLSIGN_SWITCH getStarNetCallsignSwitch4() const;
	virtual bool         getStarNetTXMsgSwitch4() const;
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	virtual wxString     getStarNetLink4() const;
#endif

	virtual wxString     getStarNetBand5() const;
	virtual wxString     getStarNetCallsign5() const;
	virtual wxString     getStarNetLogoff5() const;
	virtual wxString     getStarNetInfo5() const;
	virtual wxString     getStarNetPermanent5() const;
	virtual unsigned int getStarNetUserTimeout5() const;
	virtual unsigned int getStarNetGroupTimeout5() const;
	virtual STARNET_CALLSIGN_SWITCH getStarNetCallsignSwitch5() const;
	virtual bool         getStarNetTXMsgSwitch5() const;
#if defined(DEXTRA_LINK) || defined(DCS_LINK)
	virtual wxString     getStarNetLink5() const;
#endif

	virtual bool         getRemoteEnabled() const;
	virtual wxString     getRemotePassword() const;
	virtual unsigned int getRemotePort() const;

private:
	CIRCDDBGatewayGatewaySet*       m_gateway;
	CRepeaterDataSet*               m_repeaterData1;
	CRepeaterInfoSet*               m_repeaterInfo1;
	CRepeaterDataSet*               m_repeaterData2;
	CRepeaterInfoSet*               m_repeaterInfo2;
	CRepeaterDataSet*               m_repeaterData3;
	CRepeaterInfoSet*               m_repeaterInfo3;
	CRepeaterDataSet*               m_repeaterData4;
	CRepeaterInfoSet*               m_repeaterInfo4;
	CIrcDDBSet*                     m_ircDDB;
	CDPRSSet*                       m_dprs;
	CDExtraSet*                     m_dextra;
	CDPlusSet*                      m_dplus;
	CDCSSet*                        m_dcs;
	CStarNetSet*                    m_starNet1;
	CStarNetSet*                    m_starNet2;
	CStarNetSet*                    m_starNet3;
	CStarNetSet*                    m_starNet4;
	CStarNetSet*                    m_starNet5;
	CRemoteSet*                     m_remote;
	CIRCDDBGatewayMiscellaneousSet* m_miscellaneous;
};

#endif

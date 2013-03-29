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

#ifndef	DStarRepeaterConfigApp_H
#define	DStarRepeaterConfigApp_H

#include "DStarRepeaterConfigFrame.h"
#include "DStarRepeaterConfigDefs.h"
#include "DStarRepeaterConfig.h"

#include <wx/wx.h>

class CDStarRepeaterConfigApp : public wxApp {

public:
	CDStarRepeaterConfigApp();
	virtual ~CDStarRepeaterConfigApp();

	virtual bool OnInit();
	virtual int  OnExit();
	
	virtual void OnInitCmdLine(wxCmdLineParser& parser);
	virtual bool OnCmdLineParsed(wxCmdLineParser& parser);

	virtual void getCallsign(wxString& callsign, wxString& gateway, DSTAR_MODE& mode, ACK_TYPE& ack, bool& restriction, bool& rpt1Validation, bool& dtmfBlanking) const;
	virtual void setCallsign(const wxString& callsign, const wxString& gateway, DSTAR_MODE mode, ACK_TYPE ack, bool restriction, bool rpt1Validation, bool dtmfBlanking);

	virtual void getNetwork(wxString& gatewayAddress, unsigned int& gatewayPort, wxString& localAddress, unsigned int& localPort) const;
	virtual void setNetwork(const wxString& gatewayAddress, unsigned int gatewayPort, const wxString& localAddress, unsigned int localPort);

	virtual void getModem(wxString& name) const;
	virtual void setModem(const wxString& name);

	virtual void getTimes(unsigned int& timeout, unsigned int& ackTime) const;
	virtual void setTimes(unsigned int timeout, unsigned int ackTime);

	virtual void getBeacon(unsigned int& time, wxString& text, bool& voice, TEXT_LANG& language) const;
	virtual void setBeacon(unsigned int time, const wxString& text, bool voice, TEXT_LANG language);

	virtual void getAnnouncement(bool& enabled, unsigned int& time, wxString& recordRPT1, wxString& recordRPT2, wxString& deleteRPT1, wxString& deleteRPT2) const;
	virtual void setAnnouncement(bool enabled, unsigned int time, const wxString& recordRPT1, const wxString& recordRPT2, const wxString& deleteRPT1, const wxString& deleteRPT2);

	virtual void getControl(bool& enabled, wxString& rpt1Callsign, wxString& rpt2Callsign, wxString& shutdown, wxString& startup, wxString& status1, wxString& status2, wxString& status3, wxString& status4, wxString& status5, wxString& command1, wxString& command1Line, wxString& command2, wxString& command2Line, wxString& command3, wxString& command3Line, wxString& command4, wxString& command4Line, wxString& output1, wxString& output2, wxString& output3, wxString& output4) const;
	virtual void setControl(bool enabled, const wxString& rpt1Callsign, const wxString& rpt2Callsign, const wxString& shutdown, const wxString& startup, const wxString& status1, const wxString& status2, const wxString& status3, const wxString& status4, const wxString& status5, const wxString& command1, const wxString& command1Line, const wxString& command2, const wxString& command2Line, const wxString& command3, const wxString& command3Line, const wxString& command4, const wxString& command4Line, const wxString& output1, const wxString& output2, const wxString& output3, const wxString& output4);

	virtual void getController(wxString& type, unsigned int& activeHangTime) const;
	virtual void setController(const wxString& type, unsigned int activeHangTime);

	virtual bool writeConfig();

private:
	wxString                   m_name;
	wxString                   m_confDir;
	CDStarRepeaterConfigFrame* m_frame;
	CDStarRepeaterConfig*      m_config;
};

DECLARE_APP(CDStarRepeaterConfigApp)

#endif

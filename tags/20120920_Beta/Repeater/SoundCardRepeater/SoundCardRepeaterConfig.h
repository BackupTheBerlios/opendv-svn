/*
 *   Copyright (C) 2009-2012 by Jonathan Naylor G4KLX
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

#ifndef	SoundCardRepeaterConfig_H
#define	SoundCardRepeaterConfig_H

#include "SoundCardRepeaterDefs.h"
#include "DStarDefines.h"

#include <wx/wx.h>
#include <wx/config.h>
#include <wx/filename.h>

class CSoundCardRepeaterConfig {
public:
#if defined(__WINDOWS__)
	CSoundCardRepeaterConfig(wxConfigBase* config, const wxString& name);
#else
	CSoundCardRepeaterConfig(const wxString& dir, const wxString& name);
#endif
	~CSoundCardRepeaterConfig();

	void getCallsign(wxString& callsign, wxString& gateway, DSTAR_MODE& mode, ACK_TYPE& ack, bool& restriction, bool& rpt1Validation) const;
	void setCallsign(const wxString& callsign, const wxString& gateway, DSTAR_MODE mode, ACK_TYPE ack, bool restriction, bool rpt1Validation);

	void getNetwork(wxString& gatewayAddress, unsigned int& gatewayPort, wxString& localAddress, unsigned int& localPort) const;
	void setNetwork(const wxString& gatewayAddress, unsigned int gatewayPort, const wxString& localAddress, unsigned int localPort);

	void getRadio(wxString& readDevice, wxString& writeDevice, wxFloat32& rxLevel, wxFloat32& txLevel, SQUELCH_MODE& squelchMode, wxFloat32& squelchLevel, bool& rxInvert, bool& txInvert) const;
	void setRadio(const wxString& readDevice, const wxString& writeDevice, wxFloat32 rxLevel, wxFloat32 txLevel, SQUELCH_MODE squelchMode, wxFloat32 squelchLevel, bool rxInvert, bool txInvert);

	void getController(wxString& type, unsigned int& config, int& pttDelay, bool& pttInvert) const;
	void setController(const wxString& type, unsigned int config, int pttDelay, bool pttInvert);

	void getTimes(unsigned int& timeout, unsigned int& ackTime, unsigned int& hangTime) const;
	void setTimes(unsigned int timeout, unsigned int ackTime, unsigned int hangTimer);

	void getBeacon(unsigned int& time, wxString& text, bool& voice, TEXT_LANG& language) const;
	void setBeacon(unsigned int time, const wxString& text, bool voice, TEXT_LANG language);

	void getControl(bool& enabled, wxString& rpt1Callsign, wxString& rpt2Callsign, wxString& shutdown, wxString& startup, wxString& status1, wxString& status2, wxString& status3, wxString& status4, wxString& status5, wxString& command1, wxString& command1Line, wxString& command2, wxString& command2Line, wxString& command3, wxString& command3Line, wxString& command4, wxString& command4Line, wxString& output1, wxString& output2, wxString& output3, wxString& output4) const;
	void setControl(bool enabled, const wxString& rpt1Callsign, const wxString& rpt2Callsign, const wxString& shutdown, const wxString& startup, const wxString& status1, const wxString& status2, const wxString& status3, const wxString& status4, const wxString& status5, const wxString& command1, const wxString& command1Line, const wxString& command2, const wxString& command2Line, const wxString& command3, const wxString& command3Line, const wxString& command4, const wxString& command4Line, const wxString& output1, const wxString& output2, const wxString& output3, const wxString& output4);

	void getActiveHang(unsigned int& time) const;
	void setActiveHang(unsigned int time);

	void getOutputs(bool& out1, bool& out2, bool& out3, bool& out4) const;
	void setOutputs(bool out1, bool out2, bool out3, bool out4);

	void getLogging(bool& logging) const;
	void setLogging(bool logging);

	void getPosition(int& x, int& y) const;
	void setPosition(int x, int y);

	bool write();

private:
#if defined(__WINDOWS__)
	wxConfigBase* m_config;
	wxString      m_name;
#else
	wxFileName    m_fileName;
#endif
	wxString      m_callsign;
	wxString      m_gateway;
	DSTAR_MODE    m_mode;
	ACK_TYPE      m_ack;
	bool          m_restriction;
	bool          m_rpt1Validation;
	wxString      m_gatewayAddress;
	unsigned int  m_gatewayPort;
	wxString      m_localAddress;
	unsigned int  m_localPort;
	wxString      m_readDevice;
	wxString      m_writeDevice;
	wxFloat32     m_rxLevel;
	wxFloat32     m_txLevel;
	SQUELCH_MODE  m_squelchMode;
	wxFloat32     m_squelchLevel;
	bool          m_rxInvert;
	bool          m_txInvert;
	wxString      m_interfaceType;
	unsigned int  m_interfaceConfig;
	int           m_pttDelay;
	bool          m_pttInvert;
	unsigned int  m_timeout;
	unsigned int  m_ackTime;
	unsigned int  m_hangTime;
	unsigned int  m_beaconTime;
	wxString      m_beaconText;
	bool          m_beaconVoice;
	TEXT_LANG     m_language;
	bool          m_controlEnabled;
	wxString      m_controlRpt1Callsign;
	wxString      m_controlRpt2Callsign;
	wxString      m_controlShutdown;
	wxString      m_controlStartup;
	wxString      m_controlStatus1;
	wxString      m_controlStatus2;
	wxString      m_controlStatus3;
	wxString      m_controlStatus4;
	wxString      m_controlStatus5;
	wxString      m_controlCommand1;
	wxString      m_controlCommand1Line;
	wxString      m_controlCommand2;
	wxString      m_controlCommand2Line;
	wxString      m_controlCommand3;
	wxString      m_controlCommand3Line;
	wxString      m_controlCommand4;
	wxString      m_controlCommand4Line;
	wxString      m_controlOutput1;
	wxString      m_controlOutput2;
	wxString      m_controlOutput3;
	wxString      m_controlOutput4;
	unsigned int  m_activeHangTime;
	bool          m_output1;
	bool          m_output2;
	bool          m_output3;
	bool          m_output4;
	bool          m_logging;
	int           m_x;
	int           m_y;
};

#endif

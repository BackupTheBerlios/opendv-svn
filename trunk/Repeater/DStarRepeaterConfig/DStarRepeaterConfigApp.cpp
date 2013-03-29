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

#include "DStarRepeaterConfigApp.h"
#include "DStarDefines.h"
#include "Version.h"

#include <wx/cmdline.h>
#include <wx/filename.h>

IMPLEMENT_APP(CDStarRepeaterConfigApp)

const wxChar*       NAME_PARAM = wxT("Repeater Name");
const wxChar*   CONFDIR_OPTION = wxT("confdir");


CDStarRepeaterConfigApp::CDStarRepeaterConfigApp() :
wxApp(),
m_name(),
m_confDir(),
m_frame(NULL),
m_config(NULL)
{
}

CDStarRepeaterConfigApp::~CDStarRepeaterConfigApp()
{
}

bool CDStarRepeaterConfigApp::OnInit()
{
	SetVendorName(VENDOR_NAME);

	if (!wxApp::OnInit())
		return false;

#if defined(__WINDOWS__)
	m_config = new CDStarRepeaterConfig(new wxConfig(APPLICATION_NAME), m_name);
#else
	if (m_confDir.IsEmpty())
		m_confDir = CONF_DIR;

	m_config = new CDStarRepeaterConfig(m_confDir, CONFIG_FILE_NAME, m_name);
#endif

	wxString frameName = APPLICATION_NAME + wxT(" - ");
	if (!m_name.IsEmpty()) {
		frameName.Append(m_name);
		frameName.Append(wxT(" - "));
	}
	frameName.Append(VERSION);

	m_frame = new CDStarRepeaterConfigFrame(frameName);
	m_frame->Show();

	SetTopWindow(m_frame);

	return true;
}

int CDStarRepeaterConfigApp::OnExit()
{
	delete m_config;

	return 0;
}

void CDStarRepeaterConfigApp::OnInitCmdLine(wxCmdLineParser& parser)
{
	parser.AddOption(CONFDIR_OPTION, wxEmptyString, wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddParam(NAME_PARAM, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);

	wxApp::OnInitCmdLine(parser);
}

bool CDStarRepeaterConfigApp::OnCmdLineParsed(wxCmdLineParser& parser)
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

void CDStarRepeaterConfigApp::getCallsign(wxString& callsign, wxString& gateway, DSTAR_MODE& mode, ACK_TYPE& ack, bool& restriction, bool& rpt1Validation, bool& dtmfBlanking) const
{
	m_config->getCallsign(callsign, gateway, mode, ack, restriction, rpt1Validation, dtmfBlanking);
}

void CDStarRepeaterConfigApp::setCallsign(const wxString& callsign, const wxString& gateway, DSTAR_MODE mode, ACK_TYPE ack, bool restriction, bool rpt1Validation, bool dtmfBlanking)
{
	m_config->setCallsign(callsign, gateway, mode, ack, restriction, rpt1Validation, dtmfBlanking);
}

void CDStarRepeaterConfigApp::getNetwork(wxString& gatewayAddress, unsigned int& gatewayPort, wxString& localAddress, unsigned int& localPort) const
{
	m_config->getNetwork(gatewayAddress, gatewayPort, localAddress, localPort);
}

void CDStarRepeaterConfigApp::setNetwork(const wxString& gatewayAddress, unsigned int gatewayPort, const wxString& localAddress, unsigned int localPort)
{
	m_config->setNetwork(gatewayAddress, gatewayPort, localAddress, localPort);
}

void CDStarRepeaterConfigApp::getModem(wxString& name) const
{
	m_config->getModem(name);
}

void CDStarRepeaterConfigApp::setModem(const wxString& name)
{
	m_config->setModem(name);
}

void CDStarRepeaterConfigApp::getTimes(unsigned int& timeout, unsigned int& ackTime) const
{
	m_config->getTimes(timeout, ackTime);
}

void CDStarRepeaterConfigApp::setTimes(unsigned int timeout, unsigned int ackTime)
{
	m_config->setTimes(timeout, ackTime);
}

void CDStarRepeaterConfigApp::getBeacon(unsigned int& time, wxString& text, bool& voice, TEXT_LANG& language) const
{
	m_config->getBeacon(time, text, voice, language);
}

void CDStarRepeaterConfigApp::setBeacon(unsigned int time, const wxString& text, bool voice, TEXT_LANG language)
{
	m_config->setBeacon(time, text, voice, language);
}

void CDStarRepeaterConfigApp::getAnnouncement(bool& enabled, unsigned int& time, wxString& recordRPT1, wxString& recordRPT2, wxString& deleteRPT1, wxString& deleteRPT2) const
{
	m_config->getAnnouncement(enabled, time, recordRPT1, recordRPT2, deleteRPT1, deleteRPT2);
}

void CDStarRepeaterConfigApp::setAnnouncement(bool enabled, unsigned int time, const wxString& recordRPT1, const wxString& recordRPT2, const wxString& deleteRPT1, const wxString& deleteRPT2)
{
	m_config->setAnnouncement(enabled, time, recordRPT1, recordRPT2, deleteRPT1, deleteRPT2);
}

void CDStarRepeaterConfigApp::getControl(bool& enabled, wxString& rpt1Callsign, wxString& rpt2Callsign, wxString& shutdown, wxString& startup, wxString& status1, wxString& status2, wxString& status3, wxString& status4, wxString& status5, wxString& command1, wxString& command1Line, wxString& command2, wxString& command2Line, wxString& command3, wxString& command3Line, wxString& command4, wxString& command4Line, wxString& output1, wxString& output2, wxString& output3, wxString& output4) const
{
	m_config->getControl(enabled, rpt1Callsign, rpt2Callsign, shutdown, startup, status1, status2, status3, status4, status5, command1, command1Line, command2, command2Line, command3, command3Line, command4, command4Line, output1, output2, output3, output4);
}

void CDStarRepeaterConfigApp::setControl(bool enabled, const wxString& rpt1Callsign, const wxString& rpt2Callsign, const wxString& shutdown, const wxString& startup, const wxString& status1, const wxString& status2, const wxString& status3, const wxString& status4, const wxString& status5, const wxString& command1, const wxString& command1Line, const wxString& command2, const wxString& command2Line, const wxString& command3, const wxString& command3Line, const wxString& command4, const wxString& command4Line, const wxString& output1, const wxString& output2, const wxString& output3, const wxString& output4)
{
	m_config->setControl(enabled, rpt1Callsign, rpt2Callsign, shutdown, startup, status1, status2, status3, status4, status5, command1, command1Line, command2, command2Line, command3, command3Line, command4, command4Line, output1, output2, output3, output4);
}

void CDStarRepeaterConfigApp::getController(wxString& type, unsigned int& activeHangTime) const
{
	m_config->getController(type, activeHangTime);
}

void CDStarRepeaterConfigApp::setController(const wxString& type, unsigned int activeHangTime)
{
	m_config->setController(type, activeHangTime);
}

bool CDStarRepeaterConfigApp::writeConfig()
{
	return m_config->write();
}

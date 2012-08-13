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

#include "DStarRepeaterPreferences.h"
#include "DStarDefines.h"

const unsigned int BORDER_SIZE   = 5U;

#include <wx/gbsizer.h>
#include <wx/notebook.h>

CDStarRepeaterPreferences::CDStarRepeaterPreferences(wxWindow* parent, int id, const wxString& callsign,
	const wxString& gateway, DSTAR_MODE mode, ACK_TYPE ack, bool restriction, bool rpt1Validation,
	const wxString& gatewayAddress, unsigned int gatewayPort, const wxString& localAddress,
	unsigned int localPort, unsigned int timeout, unsigned int ackTime, unsigned int hangTime,
	unsigned int beaconTime, const wxString& beaconText, bool beaconVoice, TEXT_LANG language,
	const wxString& readDevice, const wxString& writeDevice, bool rxInvert, bool txInvert, wxFloat32 rxLevel,
	wxFloat32 txLevel, SQUELCH_MODE squelchMode, wxFloat32 squelchLevel, const wxString& interfaceType,
	unsigned int interfaceConfig, int pttDelay, bool pttInvert, bool enabled, const wxString& rpt1Callsign,
	const wxString& rpt2Callsign, const wxString& shutdown, const wxString& startup, const wxString& status1,
	const wxString& status2, const wxString& status3, const wxString& status4, const wxString& status5,
	const wxString& command1, const wxString& command1Line, const wxString& command2,
	const wxString& command2Line, const wxString& command3, const wxString& command3Line,
	const wxString& command4, const wxString& command4Line, const wxString& output1, const wxString& output2,
	const wxString& output3, const wxString& output4, unsigned int activeHangTime) :
wxDialog(parent, id, wxString(_("D-Star Repeater Preferences"))),
m_callsign(NULL),
m_network(NULL),
m_times(NULL),
m_beacon(NULL),
m_radio(NULL),
m_controller(NULL),
m_control1(NULL),
m_control2(NULL),
m_active(NULL)
{
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	wxNotebook* noteBook = new wxNotebook(this, -1);

	m_callsign = new CDStarRepeaterCallsignSet(noteBook, -1, APPLICATION_NAME, callsign, gateway, mode, ack, restriction, rpt1Validation);
	noteBook->AddPage(m_callsign, _("Callsign"), true);

	m_network = new CNetworkSet(noteBook, -1, APPLICATION_NAME, gatewayAddress, gatewayPort, localAddress, localPort);
	noteBook->AddPage(m_network, _("Network"), false);

	m_times = new CDStarRepeaterTimesSet(noteBook, -1, APPLICATION_NAME, timeout, ackTime, hangTime);
	noteBook->AddPage(m_times, _("Timers"), false);

	m_beacon = new CBeaconSet(noteBook, -1, APPLICATION_NAME, beaconTime, beaconText, beaconVoice, language);
	noteBook->AddPage(m_beacon, _("Beacon"), false);

	m_radio = new CDStarRepeaterRadioSet(noteBook, -1, APPLICATION_NAME, readDevice, writeDevice, rxLevel, txLevel, squelchMode, squelchLevel, rxInvert, txInvert);
	noteBook->AddPage(m_radio, _("Radio"), false);

	m_controller = new CDStarRepeaterControllerSet(noteBook, -1, APPLICATION_NAME, interfaceType, interfaceConfig, pttDelay, pttInvert);
	noteBook->AddPage(m_controller, _("Controller"), false);

	m_control1 = new CControl1Set(noteBook, -1, APPLICATION_NAME, enabled, rpt1Callsign, rpt2Callsign, shutdown, startup, status1, status2, status3, status4, status5, output1, output2, output3, output4);
	noteBook->AddPage(m_control1, _("Control 1"), false);

	m_control2 = new CControl2Set(noteBook, -1, APPLICATION_NAME, command1, command1Line, command2, command2Line, command3, command3Line, command4, command4Line);
	noteBook->AddPage(m_control2, _("Control 2"), false);

	m_active = new CActiveHangSet(noteBook, -1, APPLICATION_NAME, activeHangTime);
	noteBook->AddPage(m_active, _("Active Output"), false);

	mainSizer->Add(noteBook, 1, wxALL | wxGROW, BORDER_SIZE);

	mainSizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	SetAutoLayout(true);
	Layout();

	mainSizer->Fit(this);
	mainSizer->SetSizeHints(this);

	SetSizer(mainSizer);
}

CDStarRepeaterPreferences::~CDStarRepeaterPreferences()
{
}

bool CDStarRepeaterPreferences::Validate()
{
	if (!m_callsign->Validate() || !m_network->Validate() || !m_times->Validate() || !m_beacon->Validate() || !m_radio->Validate() || !m_controller->Validate() || !m_control1->Validate() || !m_control2->Validate())
		return false;

	return true;	
}

wxString CDStarRepeaterPreferences::getCallsign() const
{
	return m_callsign->getCallsign();
}

wxString CDStarRepeaterPreferences::getGateway() const
{
	return m_callsign->getGateway();
}

DSTAR_MODE CDStarRepeaterPreferences::getMode() const
{
	return m_callsign->getMode();
}

ACK_TYPE CDStarRepeaterPreferences::getAck() const
{
	return m_callsign->getAck();
}

bool CDStarRepeaterPreferences::getRestriction() const
{
	return m_callsign->getRestriction();
}

bool CDStarRepeaterPreferences::getRPT1Validation() const
{
	return m_callsign->getRPT1Validation();
}

wxString CDStarRepeaterPreferences::getGatewayAddress() const
{
	return m_network->getGatewayAddress();
}

unsigned int CDStarRepeaterPreferences::getGatewayPort() const
{
	return m_network->getGatewayPort();
}

wxString CDStarRepeaterPreferences::getLocalAddress() const
{
	return m_network->getLocalAddress();
}

unsigned int CDStarRepeaterPreferences::getLocalPort() const
{
	return m_network->getLocalPort();
}

unsigned int CDStarRepeaterPreferences::getTimeout() const
{
	return m_times->getTimeout();
}

unsigned int CDStarRepeaterPreferences::getAckTime() const
{
	return m_times->getAckTime();
}

unsigned int CDStarRepeaterPreferences::getHangTime() const
{
	return m_times->getHangTime();
}

unsigned int CDStarRepeaterPreferences::getBeaconTime() const
{
	return m_beacon->getTime();
}

wxString CDStarRepeaterPreferences::getBeaconText() const
{
	return m_beacon->getText();
}

bool CDStarRepeaterPreferences::getBeaconVoice() const
{
	return m_beacon->getVoice();
}

TEXT_LANG CDStarRepeaterPreferences::getLanguage() const
{
	return m_beacon->getLanguage();
}

wxString CDStarRepeaterPreferences::getReadDevice() const
{
	return m_radio->getReadDevice();
}

wxString CDStarRepeaterPreferences::getWriteDevice() const
{
	return m_radio->getWriteDevice();
}

wxFloat32 CDStarRepeaterPreferences::getRXLevel() const
{
	return m_radio->getRXLevel();
}

wxFloat32 CDStarRepeaterPreferences::getTXLevel() const
{
	return m_radio->getTXLevel();
}

SQUELCH_MODE CDStarRepeaterPreferences::getSquelchMode() const
{
	return m_radio->getSquelchMode();
}

wxFloat32 CDStarRepeaterPreferences::getSquelchLevel() const
{
	return m_radio->getSquelchLevel();
}

bool CDStarRepeaterPreferences::getRXInvert() const
{
	return m_radio->getRXInvert();
}

bool CDStarRepeaterPreferences::getTXInvert() const
{
	return m_radio->getTXInvert();
}

wxString CDStarRepeaterPreferences::getType() const
{
	return m_controller->getType();
}

unsigned int CDStarRepeaterPreferences::getConfig() const
{
	return m_controller->getConfig();
}

int CDStarRepeaterPreferences::getPTTDelay() const
{
	return m_controller->getPTTDelay();
}

bool CDStarRepeaterPreferences::getPTTInvert() const
{
	return m_controller->getPTTInvert();
}

bool CDStarRepeaterPreferences::getEnabled() const
{
	return m_control1->getEnabled();
}

wxString CDStarRepeaterPreferences::getRPT1Callsign() const
{
	return m_control1->getRPT1Callsign();
}

wxString CDStarRepeaterPreferences::getRPT2Callsign() const
{
	return m_control1->getRPT2Callsign();
}

wxString CDStarRepeaterPreferences::getShutdown() const
{
	return m_control1->getShutdown();
}

wxString CDStarRepeaterPreferences::getStartup() const
{
	return m_control1->getStartup();
}

wxString CDStarRepeaterPreferences::getStatus1() const
{
	return m_control1->getStatus1();
}

wxString CDStarRepeaterPreferences::getStatus2() const
{
	return m_control1->getStatus2();
}

wxString CDStarRepeaterPreferences::getStatus3() const
{
	return m_control1->getStatus3();
}

wxString CDStarRepeaterPreferences::getStatus4() const
{
	return m_control1->getStatus4();
}

wxString CDStarRepeaterPreferences::getStatus5() const
{
	return m_control1->getStatus5();
}

wxString CDStarRepeaterPreferences::getOutput1() const
{
	return m_control1->getOutput1();
}

wxString CDStarRepeaterPreferences::getOutput2() const
{
	return m_control1->getOutput2();
}

wxString CDStarRepeaterPreferences::getOutput3() const
{
	return m_control1->getOutput3();
}

wxString CDStarRepeaterPreferences::getOutput4() const
{
	return m_control1->getOutput4();
}

wxString CDStarRepeaterPreferences::getCommand1() const
{
	return m_control2->getCommand1();
}

wxString CDStarRepeaterPreferences::getCommand1Line() const
{
	return m_control2->getCommand1Line();
}

wxString CDStarRepeaterPreferences::getCommand2() const
{
	return m_control2->getCommand2();
}

wxString CDStarRepeaterPreferences::getCommand2Line() const
{
	return m_control2->getCommand2Line();
}

wxString CDStarRepeaterPreferences::getCommand3() const
{
	return m_control2->getCommand3();
}

wxString CDStarRepeaterPreferences::getCommand3Line() const
{
	return m_control2->getCommand3Line();
}

wxString CDStarRepeaterPreferences::getCommand4() const
{
	return m_control2->getCommand4();
}

wxString CDStarRepeaterPreferences::getCommand4Line() const
{
	return m_control2->getCommand4Line();
}

unsigned int CDStarRepeaterPreferences::getActiveHangTime() const
{
	return m_active->getTime();
}


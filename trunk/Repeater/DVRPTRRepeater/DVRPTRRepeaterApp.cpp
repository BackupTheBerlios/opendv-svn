/*
 *   Copyright (C) 2011,2012 by Jonathan Naylor G4KLX
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

#include "DVRPTRRepeaterTXRXThread.h"
#include "RepeaterProtocolHandler.h"
#include "DVRPTRRepeaterTRXThread.h"
#include "DVRPTRRepeaterTXThread.h"
#include "DVRPTRRepeaterRXThread.h"
#include "DVRPTRRepeaterLogger.h"
#include "DVRPTRControllerV2.h"
#include "DVRPTRControllerV1.h"
#include "DVRPTRRepeaterApp.h"
#include "DVRPTRController.h"
#include "K8055Controller.h"
#include "DummyController.h"
#include "DStarDefines.h"
#include "Version.h"
#include "Logger.h"

#include <wx/cmdline.h>
#include <wx/filename.h>

IMPLEMENT_APP(CDVRPTRRepeaterApp)

const wxChar*       NAME_PARAM = wxT("Repeater Name");
const wxChar* NOLOGGING_SWITCH = wxT("nolog");
const wxChar*       GUI_SWITCH = wxT("gui");
const wxChar*    LOGDIR_OPTION = wxT("logdir");
const wxChar*   CONFDIR_OPTION = wxT("confdir");


CDVRPTRRepeaterApp::CDVRPTRRepeaterApp() :
wxApp(),
m_name(),
m_nolog(false),
m_gui(false),
m_logDir(),
m_confDir(),
m_frame(NULL),
m_thread(NULL),
m_config(NULL),
m_logChain(NULL)
{
}

CDVRPTRRepeaterApp::~CDVRPTRRepeaterApp()
{
}

bool CDVRPTRRepeaterApp::OnInit()
{
	SetVendorName(VENDOR_NAME);

	if (!wxApp::OnInit())
		return false;

	if (!m_nolog) {
		wxString logBaseName = LOG_BASE_NAME;
		if (!m_name.IsEmpty()) {
			logBaseName.Append(wxT("_"));
			logBaseName.Append(m_name);
		}

#if defined(__WINDOWS__)
		if (m_logDir.IsEmpty())
			m_logDir = wxFileName::GetHomeDir();
#else
		if (m_logDir.IsEmpty())
			m_logDir = LOG_DIR;
#endif

		wxLog* log = new CLogger(m_logDir, logBaseName);
		wxLog::SetActiveTarget(log);
	} else {
		new wxLogNull;
	}

	m_logChain = new wxLogChain(new CDVRPTRRepeaterLogger);

#if defined(__WINDOWS__)
	m_config = new CDVRPTRRepeaterConfig(new wxConfig(APPLICATION_NAME), m_name);
#else
	if (m_confDir.IsEmpty())
		m_confDir = CONF_DIR;

	m_config = new CDVRPTRRepeaterConfig(m_confDir, m_name);
#endif

	wxString frameName = APPLICATION_NAME + wxT(" - ");
	if (!m_name.IsEmpty()) {
		frameName.Append(m_name);
		frameName.Append(wxT(" - "));
	}
	frameName.Append(VERSION);

	wxPoint position = wxDefaultPosition;

	int x, y;
	getPosition(x, y);
	if (x >= 0 && y >= 0)
		position = wxPoint(x, y);

	m_frame = new CDVRPTRRepeaterFrame(frameName, position, m_gui);
	m_frame->Show();

	SetTopWindow(m_frame);

	wxLogInfo(wxT("Starting ") + APPLICATION_NAME + wxT(" - ") + VERSION);

	// Log the version of wxWidgets and the Operating System
	wxLogInfo(wxT("Using wxWidgets %d.%d.%d on %s"), wxMAJOR_VERSION, wxMINOR_VERSION, wxRELEASE_NUMBER, ::wxGetOsDescription().c_str());

	createThread();

	return true;
}

int CDVRPTRRepeaterApp::OnExit()
{
	m_logChain->SetLog(NULL);

	wxLogInfo(APPLICATION_NAME + wxT(" is exiting"));

	m_thread->kill();

	m_thread->wait();

	delete m_config;

	return 0;
}

void CDVRPTRRepeaterApp::OnInitCmdLine(wxCmdLineParser& parser)
{
	parser.AddSwitch(NOLOGGING_SWITCH, wxEmptyString, wxEmptyString, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddSwitch(GUI_SWITCH,       wxEmptyString, wxEmptyString, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddOption(LOGDIR_OPTION,    wxEmptyString, wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddOption(CONFDIR_OPTION,   wxEmptyString, wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddParam(NAME_PARAM, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);

	wxApp::OnInitCmdLine(parser);
}

bool CDVRPTRRepeaterApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
	if (!wxApp::OnCmdLineParsed(parser))
		return false;

	m_nolog = parser.Found(NOLOGGING_SWITCH);
	m_gui   = parser.Found(GUI_SWITCH);

	wxString logDir;
	bool found = parser.Found(LOGDIR_OPTION, &logDir);
	if (found)
		m_logDir = logDir;

	wxString confDir;
	found = parser.Found(CONFDIR_OPTION, &confDir);
	if (found)
		m_confDir = confDir;

	if (parser.GetParamCount() > 0U)
		m_name = parser.GetParam(0U);

	return true;
}

#if defined(__WXDEBUG__)
void CDVRPTRRepeaterApp::OnAssertFailure(const wxChar* file, int line, const wxChar* func, const wxChar* cond, const wxChar* msg)
{
	wxLogFatalError(wxT("Assertion failed on line %d in file %s and function %s: %s %s"), line, file, func, cond, msg);
}
#endif

CDVRPTRRepeaterStatusData* CDVRPTRRepeaterApp::getStatus() const
{
	return m_thread->getStatus();
}

void CDVRPTRRepeaterApp::showLog(const wxString& text)
{
	m_frame->showLog(text);
}

void CDVRPTRRepeaterApp::getCallsign(wxString& callsign, wxString& gateway, DSTAR_MODE& mode, ACK_TYPE& ack, bool& restriction, bool& rpt1Validation) const
{
	m_config->getCallsign(callsign, gateway, mode, ack, restriction, rpt1Validation);
}

void CDVRPTRRepeaterApp::setCallsign(const wxString& callsign, const wxString& gateway, DSTAR_MODE mode, ACK_TYPE ack, bool restriction, bool rpt1Validation)
{
	m_config->setCallsign(callsign, gateway, mode, ack, restriction, rpt1Validation);
}

void CDVRPTRRepeaterApp::getNetwork(wxString& gatewayAddress, unsigned int& gatewayPort, wxString& localAddress, unsigned int& localPort) const
{
	m_config->getNetwork(gatewayAddress, gatewayPort, localAddress, localPort);
}

void CDVRPTRRepeaterApp::setNetwork(const wxString& gatewayAddress, unsigned int gatewayPort, const wxString& localAddress, unsigned int localPort)
{
	m_config->setNetwork(gatewayAddress, gatewayPort, localAddress, localPort);
}

void CDVRPTRRepeaterApp::getModem(DVRPTR_VERSION& version, wxString& port, bool& rxInvert, bool& txInvert, bool& channel, unsigned int& modLevel, unsigned int& txDelay) const
{
	m_config->getModem(version, port, rxInvert, txInvert, channel, modLevel, txDelay);
}

void CDVRPTRRepeaterApp::setModem(DVRPTR_VERSION version, const wxString& port, bool rxInvert, bool txInvert, bool channel, unsigned int modLevel, unsigned int txDelay)
{
	m_config->setModem(version, port, rxInvert, txInvert, channel, modLevel, txDelay);
}

void CDVRPTRRepeaterApp::getModem(wxString& path) const
{
	m_config->getModem(path);
}

void CDVRPTRRepeaterApp::setModem(const wxString& path)
{
	m_config->setModem(path);
}

void CDVRPTRRepeaterApp::getTimes(unsigned int& timeout, unsigned int& ackTime) const
{
	m_config->getTimes(timeout, ackTime);
}

void CDVRPTRRepeaterApp::setTimes(unsigned int timeout, unsigned int ackTime)
{
	m_config->setTimes(timeout, ackTime);
}

void CDVRPTRRepeaterApp::getBeacon(unsigned int& time, wxString& text, bool& voice, TEXT_LANG& language) const
{
	m_config->getBeacon(time, text, voice, language);
}

void CDVRPTRRepeaterApp::setBeacon(unsigned int time, const wxString& text, bool voice, TEXT_LANG language)
{
	m_config->setBeacon(time, text, voice, language);
}

void CDVRPTRRepeaterApp::getControl(bool& enabled, wxString& rpt1Callsign, wxString& rpt2Callsign, wxString& shutdown, wxString& startup, wxString& status1, wxString& status2, wxString& status3, wxString& status4, wxString& status5, wxString& command1, wxString& command1Line, wxString& command2, wxString& command2Line, wxString& command3, wxString& command3Line, wxString& command4, wxString& command4Line, wxString& output1, wxString& output2, wxString& output3, wxString& output4) const
{
	m_config->getControl(enabled, rpt1Callsign, rpt2Callsign, shutdown, startup, status1, status2, status3, status4, status5, command1, command1Line, command2, command2Line, command3, command3Line, command4, command4Line, output1, output2, output3, output4);
}

void CDVRPTRRepeaterApp::setControl(bool enabled, const wxString& rpt1Callsign, const wxString& rpt2Callsign, const wxString& shutdown, const wxString& startup, const wxString& status1, const wxString& status2, const wxString& status3, const wxString& status4, const wxString& status5, const wxString& command1, const wxString& command1Line, const wxString& command2, const wxString& command2Line, const wxString& command3, const wxString& command3Line, const wxString& command4, const wxString& command4Line, const wxString& output1, const wxString& output2, const wxString& output3, const wxString& output4)
{
	m_config->setControl(enabled, rpt1Callsign, rpt2Callsign, shutdown, startup, status1, status2, status3, status4, status5, command1, command1Line, command2, command2Line, command3, command3Line, command4, command4Line, output1, output2, output3, output4);
}

void CDVRPTRRepeaterApp::getController(wxString& type, unsigned int& activeHangTime) const
{
	m_config->getController(type, activeHangTime);
}

void CDVRPTRRepeaterApp::setController(const wxString& type, unsigned int activeHangTime)
{
	m_config->setController(type, activeHangTime);
}

void CDVRPTRRepeaterApp::getOutputs(bool& out1, bool& out2, bool& out3, bool& out4) const
{
	m_config->getOutputs(out1, out2, out3, out4);
}

void CDVRPTRRepeaterApp::setOutputs(bool out1, bool out2, bool out3, bool out4)
{
	m_config->setOutputs(out1, out2, out3, out4);
	m_thread->setOutputs(out1, out2, out3, out4);

	wxLogInfo(wxT("Output 1 = %d, output 2 = %d, output 3 = %d, output 4 = %d"), out1, out2, out3, out4);
}

void CDVRPTRRepeaterApp::getLogging(bool& logging) const
{
	m_config->getLogging(logging);
}

void CDVRPTRRepeaterApp::setLogging(bool logging)
{
	wxLogInfo(wxT("Frame logging set to %d, in %s"), int(logging), ::wxGetHomeDir().c_str());

	m_config->setLogging(logging);
	m_thread->setLogging(logging, ::wxGetHomeDir());
}

void CDVRPTRRepeaterApp::getPosition(int& x, int& y) const
{
	m_config->getPosition(x, y);
}

void CDVRPTRRepeaterApp::setPosition(int x, int y)
{
	m_config->setPosition(x, y);
}

bool CDVRPTRRepeaterApp::writeConfig()
{
	return m_config->write();
}

void CDVRPTRRepeaterApp::shutdown()
{
	m_thread->shutdown();
}

void CDVRPTRRepeaterApp::startup()
{
	m_thread->startup();
}

void CDVRPTRRepeaterApp::command1()
{
	m_thread->command1();
}

void CDVRPTRRepeaterApp::command2()
{
	m_thread->command2();
}

void CDVRPTRRepeaterApp::command3()
{
	m_thread->command3();
}

void CDVRPTRRepeaterApp::command4()
{
	m_thread->command4();
}

void CDVRPTRRepeaterApp::createThread()
{
	wxString callsign, gateway;
	DSTAR_MODE mode;
	ACK_TYPE ack;
	bool restriction, rpt1Validation;
	getCallsign(callsign, gateway, mode, ack, restriction, rpt1Validation);

	switch (mode) {
		case MODE_RXONLY:
			m_thread = new CDVRPTRRepeaterRXThread;
			break;
		case MODE_TXONLY:
			m_thread = new CDVRPTRRepeaterTXThread;
			break;
		case MODE_TXANDRX:
			m_thread = new CDVRPTRRepeaterTXRXThread;
			break;
		default:
			m_thread = new CDVRPTRRepeaterTRXThread;
			break;
	}

	m_thread->setCallsign(callsign, gateway, mode, ack, restriction, rpt1Validation);
	wxLogInfo(wxT("Callsign set to \"%s\", gateway set to \"%s\", mode: %d, ack: %d, restriction: %d, RPT1 validation: %d"), callsign.c_str(), gateway.c_str(), int(mode), int(ack), restriction, rpt1Validation);

	wxString gatewayAddress, localAddress;
	unsigned int gatewayPort, localPort;
	getNetwork(gatewayAddress, gatewayPort, localAddress, localPort);
	wxLogInfo(wxT("Gateway set to %s:%u, local set to %s:%u"), gatewayAddress.c_str(), gatewayPort, localAddress.c_str(), localPort);

	if (!gatewayAddress.IsEmpty()) {
		CRepeaterProtocolHandler* handler = new CRepeaterProtocolHandler(gatewayAddress, gatewayPort, localAddress, localPort);

		bool res = handler->open();
		if (!res)
			wxLogError(wxT("Cannot open the protocol handler"));
		else
			m_thread->setProtocolHandler(handler);
	}

	unsigned int timeout, ackTime;
	getTimes(timeout, ackTime);
	m_thread->setTimes(timeout, ackTime);
	wxLogInfo(wxT("Timeout set to %u secs, ack time set to %u ms"), timeout, ackTime);

	unsigned int beaconTime;
	wxString beaconText;
	bool beaconVoice;
	TEXT_LANG language;
	getBeacon(beaconTime, beaconText, beaconVoice, language);
	m_thread->setBeacon(beaconTime, beaconText, beaconVoice, language);
	wxLogInfo(wxT("Beacon set to %u mins, text set to \"%s\", voice set to %d, language set to %d"), beaconTime / 60U, beaconText.c_str(), int(beaconVoice), int(language));

	DVRPTR_VERSION modemVersion;
	wxString modemPort, modemPath;
	bool rxInvert, txInvert, channel;
	unsigned int modLevel, txDelay;
	getModem(modemVersion, modemPort, rxInvert, txInvert, channel, modLevel, txDelay);
	getModem(modemPath);
	wxLogInfo(wxT("DV-RPTR modem: version: %d, port: %s, RX invert: %d, TX invert: %d, channel: %s, mod level: %u%%, TX delay: %u ms"), int(modemVersion), modemPort.c_str(), int(rxInvert), int(txInvert), channel ? wxT("B") : wxT("A"), modLevel, txDelay);

	if (!modemPort.IsEmpty()) {
		if (!modemPath.IsEmpty())
			wxLogInfo(wxT("DV-RPTR modem: path: %s"), modemPath.c_str());

		IDVRPTRController* controller = NULL;
		switch (modemVersion) {
			case DVRPTR_V1:
				controller = new CDVRPTRControllerV1(modemPort, modemPath, rxInvert, txInvert, channel, modLevel, txDelay);
				break;
			case DVRPTR_V2:
				controller = new CDVRPTRControllerV2(modemPort, modemPath, rxInvert, txInvert, channel, modLevel, txDelay);
				break;
			default:
				wxLogError(wxT("Unknown DV-RPTR modem version - %d"), int(modemVersion));
				break;
		}

		if (controller != NULL) {
			bool res = controller->open();
			if (!res) {
				wxLogError(wxT("Cannot open the DV-RPTR modem"));
			} else {
				m_thread->setModem(controller);
				setModem(controller->getPath());
			}
		}
	}

	wxString controllerType;
	unsigned int activeHangTime;
	getController(controllerType, activeHangTime);
	wxLogInfo(wxT("Controller set to %s, active hang time: %u ms"), controllerType.c_str(), activeHangTime);

	CExternalController* controller = NULL;

	wxString port;
	if (controllerType.StartsWith(wxT("Velleman K8055 - "), &port)) {
		unsigned long num;
		port.ToULong(&num);
		controller = new CExternalController(new CK8055Controller(num), false, false);
	} else {
		controller = new CExternalController(new CDummyController, false, false);
	}

	bool res = controller->open();
	if (!res)
		wxLogError(wxT("Cannot open the hardware interface - %s"), controllerType.c_str());
	else
		m_thread->setController(controller, activeHangTime);

	bool out1, out2, out3, out4;
	getOutputs(out1, out2, out3, out4);
	m_thread->setOutputs(out1, out2, out3, out4);
	m_frame->setOutputs(out1, out2, out3, out4);
	wxLogInfo(wxT("Output 1 = %d, output 2 = %d, output 3 = %d, output 4 = %d"), out1, out2, out3, out4);

	bool enabled;
	wxString rpt1Callsign, rpt2Callsign;
	wxString shutdown, startup;
	wxString status1, status2, status3, status4, status5;
	wxString command1, command1Line, command2, command2Line;
	wxString command3, command3Line, command4, command4Line;
	wxString output1, output2, output3, output4;
	getControl(enabled, rpt1Callsign, rpt2Callsign, shutdown, startup, status1, status2, status3, status4, status5, command1, command1Line, command2, command2Line, command3, command3Line, command4, command4Line, output1, output2, output3, output4);
	m_thread->setControl(enabled, rpt1Callsign, rpt2Callsign, shutdown, startup, status1, status2, status3, status4, status5, command1, command1Line, command2, command2Line, command3, command3Line, command4, command4Line, output1, output2, output3, output4);
	wxLogInfo(wxT("Control: enabled: %d, RPT1: %s, RPT2: %s, shutdown: %s, startup: %s, status1: %s, status2: %s, status3: %s, status4: %s, status5: %s, command1: %s = %s, command2: %s = %s, command3: %s = %s, command4: %s = %s, output1: %s, output2: %s, output3: %s, output4: %s"), enabled, rpt1Callsign.c_str(), rpt2Callsign.c_str(), shutdown.c_str(), startup.c_str(), status1.c_str(), status2.c_str(), status3.c_str(), status4.c_str(), status5.c_str(), command1.c_str(), command1Line.c_str(), command2.c_str(), command2Line.c_str(), command3.c_str(), command3Line.c_str(), command4.c_str(), command4Line.c_str(), output1.c_str(), output2.c_str(), output3.c_str(), output4.c_str());

	bool logging;
	getLogging(logging);
	m_thread->setLogging(logging, ::wxGetHomeDir());
	m_frame->setLogging(logging);
	wxLogInfo(wxT("Frame logging set to %d, in %s"), int(logging), ::wxGetHomeDir().c_str());

	wxFileName wlFilename(wxFileName::GetHomeDir(), WHITELIST_FILE_NAME);
	bool exists = wlFilename.FileExists();
	if (exists) {
		CCallsignList* list = new CCallsignList(wlFilename.GetFullPath());
		bool res = list->load();
		if (!res) {
			wxLogError(wxT("Unable to open white list file - %s"), wlFilename.GetFullPath().c_str());
			delete list;
		} else {
			wxLogInfo(wxT("%u callsigns loaded into the white list"), list->getCount());
			m_thread->setWhiteList(list);
		}
	}

	wxFileName blFilename(wxFileName::GetHomeDir(), BLACKLIST_FILE_NAME);
	exists = blFilename.FileExists();
	if (exists) {
		CCallsignList* list = new CCallsignList(blFilename.GetFullPath());
		bool res = list->load();
		if (!res) {
			wxLogError(wxT("Unable to open black list file - %s"), blFilename.GetFullPath().c_str());
			delete list;
		} else {
			wxLogInfo(wxT("%u callsigns loaded into the black list"), list->getCount());
			m_thread->setBlackList(list);
		}
	}

	m_thread->start();
}

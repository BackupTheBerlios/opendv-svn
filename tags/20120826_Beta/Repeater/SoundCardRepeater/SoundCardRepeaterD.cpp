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

#include "SoundCardRepeaterTXRXThread.h"
#include "SoundCardRepeaterTRXThread.h"
#include "SoundCardRepeaterTXThread.h"
#include "SoundCardRepeaterRXThread.h"
#include "SoundCardRepeaterConfig.h"
#include "RepeaterProtocolHandler.h"
#include "SoundCardRepeaterD.h"
#include "SerialController.h"
#include "URIUSBController.h"
#include "K8055Controller.h"
#include "DummyController.h"
#include "DStarDefines.h"
#include "CallsignList.h"
#include "Version.h"
#include "Logger.h"

#include <wx/cmdline.h>
#include <wx/wfstream.h>
#include <wx/fileconf.h>
#include <wx/filename.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

const wxChar*       NAME_PARAM = wxT("Repeater Name");
const wxChar* NOLOGGING_SWITCH = wxT("nolog");
const wxChar*    LOGDIR_OPTION = wxT("logdir");
const wxChar*   CONFDIR_OPTION = wxT("confdir");
const wxChar*    DAEMON_SWITCH = wxT("daemon");


int main(int argc, char** argv)
{
	bool res = ::wxInitialize();
	if (!res) {
		::fprintf(stderr, "soundcardrepeaterd: failed to initialise the wxWidgets library, exiting\n");
		return -1;
	}

	wxCmdLineParser parser(argc, argv);
	parser.AddSwitch(NOLOGGING_SWITCH, wxEmptyString, wxEmptyString, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddSwitch(DAEMON_SWITCH,    wxEmptyString, wxEmptyString, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddOption(LOGDIR_OPTION,    wxEmptyString, wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddOption(CONFDIR_OPTION,   wxEmptyString, wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddParam(NAME_PARAM, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);

	int cmd = parser.Parse();
	if (cmd != 0) {
		::wxUninitialize();
		return 0;
	}

	bool  nolog = parser.Found(NOLOGGING_SWITCH);
	bool daemon = parser.Found(DAEMON_SWITCH);

	wxString logDir;
	bool found = parser.Found(LOGDIR_OPTION, &logDir);
	if (!found)
		logDir.Clear();

	wxString confDir;
	found = parser.Found(CONFDIR_OPTION, &confDir);
	if (!found)
		confDir = CONF_DIR;

	wxString name;
	if (parser.GetParamCount() > 0U)
		name = parser.GetParam(0U);

	if (daemon) {
		pid_t pid = ::fork();

		if (pid < 0) {
			::fprintf(stderr, "soundcardrepeaterd: error in fork(), exiting\n");
			::wxUninitialize();
			return 1;
		}

		// If this is the parent, exit
		if (pid > 0)
			return 0;

		// We are the child from here onwards
		::setsid();

		::chdir("/");

		::umask(0);
	}

	CSoundCardRepeaterD repeater(nolog, logDir, confDir, name);

	if (!repeater.init()) {
		::wxUninitialize();
		return 1;
	}

	repeater.run();

	::wxUninitialize();
	return 0;
}

CSoundCardRepeaterD::CSoundCardRepeaterD(bool nolog, const wxString& logDir, const wxString& confDir, const wxString& name) :
m_name(name),
m_nolog(nolog),
m_logDir(logDir),
m_confDir(confDir),
m_thread(NULL)
{
}

CSoundCardRepeaterD::~CSoundCardRepeaterD()
{
}

bool CSoundCardRepeaterD::init()
{
	if (!m_nolog) {
		wxString logBaseName = LOG_BASE_NAME;
		if (!m_name.IsEmpty()) {
			logBaseName.Append(wxT("_"));
			logBaseName.Append(m_name);
		}

		if (m_logDir.IsEmpty())
			m_logDir = LOG_DIR;

		wxLog* log = new CLogger(m_logDir, logBaseName);
		wxLog::SetActiveTarget(log);
	} else {
		new wxLogNull;
	}

	wxLogInfo(wxT("Starting ") + APPLICATION_NAME + wxT(" - ") + VERSION);

	// Log the version of wxWidgets and the Operating System
	wxLogInfo(wxT("Using wxWidgets %d.%d.%d on %s"), wxMAJOR_VERSION, wxMINOR_VERSION, wxRELEASE_NUMBER, ::wxGetOsDescription().c_str());

	return createThread();
}

void CSoundCardRepeaterD::run()
{
	m_thread->start();

	m_thread->wait();

	wxLogInfo(APPLICATION_NAME + wxT(" is exiting"));
}

bool CSoundCardRepeaterD::createThread()
{
	CSoundCardRepeaterConfig config(m_confDir, m_name);

	wxString callsign, gateway;
	DSTAR_MODE mode;
	ACK_TYPE ack;
	bool restriction, rpt1Validation;
	config.getCallsign(callsign, gateway, mode, ack, restriction, rpt1Validation);

	switch (mode) {
		case MODE_RXONLY:
			m_thread = new CSoundCardRepeaterRXThread;
			break;
		case MODE_TXONLY:
			m_thread = new CSoundCardRepeaterTXThread;
			break;
		case MODE_TXANDRX:
			m_thread = new CSoundCardRepeaterTXRXThread;
			break;
		default:
			m_thread = new CSoundCardRepeaterTRXThread;
			break;
	}

	m_thread->setCallsign(callsign, gateway, mode, ack, restriction, rpt1Validation);
	wxLogInfo(wxT("Callsign set to \"%s\", gateway set to \"%s\", mode: %d, ack: %d, restriction: %d, RPT1 validation: %d"), callsign.c_str(), gateway.c_str(), int(mode), int(ack), restriction, rpt1Validation);

	wxString gatewayAddress, localAddress;
	unsigned int gatewayPort, localPort;
	config.getNetwork(gatewayAddress, gatewayPort, localAddress, localPort);
	wxLogInfo(wxT("Gateway set to %s:%u, local set to %s:%u"), gatewayAddress.c_str(), gatewayPort, localAddress.c_str(), localPort);

	if (!gatewayAddress.IsEmpty()) {
		CRepeaterProtocolHandler* handler = new CRepeaterProtocolHandler(gatewayAddress, gatewayPort, localAddress, localPort);

		bool res = handler->open();
		if (!res) {
			wxLogError(wxT("Cannot open the protocol handler"));
			return false;
		}

		m_thread->setProtocolHandler(handler);
	}

	unsigned int timeout, ackTime, hangTime;
	config.getTimes(timeout, ackTime, hangTime);
	m_thread->setTimes(timeout, ackTime, hangTime);
	wxLogInfo(wxT("Timeout set to %u secs, Ack time set to %u ms, Hang time set to %u ms"), timeout, ackTime, hangTime);

	unsigned int beaconTime;
	wxString beaconText;
	bool beaconVoice;
	TEXT_LANG language;
	config.getBeacon(beaconTime, beaconText, beaconVoice, language);
	m_thread->setBeacon(beaconTime, beaconText, beaconVoice, language);
	wxLogInfo(wxT("Beacon set to %u mins, text set to \"%s\", voice set to %d, language set to %d"), beaconTime / 60U, beaconText.c_str(), int(beaconVoice), int(language));

	wxString readDevice, writeDevice;
	bool rxInvert, txInvert;
	wxFloat32 rxLevel, txLevel, squelchLevel;
	SQUELCH_MODE squelchMode;
	config.getRadio(readDevice, writeDevice, rxLevel, txLevel, squelchMode, squelchLevel, rxInvert, txInvert);
	wxLogInfo(wxT("Soundcard set to %s:%s, levels: %.2f:%.2f, GMSK Inversion set to %d:%d, squelch: mode: %d level: %.2f"), readDevice.c_str(), writeDevice.c_str(), rxLevel, txLevel, rxInvert, txInvert, int(squelchMode), squelchLevel);

	if (!readDevice.IsEmpty() && !writeDevice.IsEmpty()) {
#if defined(__WINDOWS__)
		CSoundCardReaderWriter* soundcard = new CSoundCardReaderWriter(readDevice, writeDevice, DSTAR_RADIO_SAMPLE_RATE, DSTAR_RADIO_BLOCK_SIZE);
#else
		CSoundCardReaderWriter* soundcard = new CSoundCardReaderWriter(readDevice, writeDevice, DSTAR_RADIO_SAMPLE_RATE, 64U);
#endif
		soundcard->setCallback(m_thread, 0U);

		bool res = soundcard->open();
		if (!res) {
			wxLogError(wxT("Cannot open the sound card"));
			return false;
		}

		m_thread->setSoundCard(soundcard, rxLevel, txLevel, squelchMode, squelchLevel, rxInvert, txInvert);
	}

	wxString type;
	unsigned int cfg;
	int pttDelay;
	bool pttInvert;
	config.getController(type, cfg, pttDelay, pttInvert);
	wxLogInfo(wxT("Controller set to %s, config: %u, ptt delay: %d ms, PTT Inversion set to %d"), type.c_str(), cfg, pttDelay * 20, pttInvert);

	CExternalController* controller = NULL;

	wxString port;
	if (type.StartsWith(wxT("Velleman K8055 - "), &port)) {
		unsigned long num;
		port.ToULong(&num);
		controller = new CExternalController(new CK8055Controller(num), pttInvert, false);
	} else if (type.StartsWith(wxT("URI USB - "), &port)) {
		unsigned long num;
		port.ToULong(&num);
		controller = new CExternalController(new CURIUSBController(num, false), pttInvert, false);
	} else if (type.StartsWith(wxT("Serial - "), &port)) {
		controller = new CExternalController(new CSerialController(port, cfg), pttInvert, false);
	} else {
		controller = new CExternalController(new CDummyController, pttInvert, false);
	}

	bool res = controller->open();
	if (!res) {
		wxLogError(wxT("Cannot open the hardware interface - %s"), type.c_str());
		return false;
	}

	m_thread->setController(controller, pttDelay);

	bool out1, out2, out3, out4;
	config.getOutputs(out1, out2, out3, out4);
	m_thread->setOutputs(out1, out2, out3, out4);
	wxLogInfo(wxT("Output 1 = %d, output 2 = %d, output 3 = %d, output 4 = %d"), out1, out2, out3, out4);

	bool enabled;
	wxString rpt1Callsign, rpt2Callsign;
	wxString shutdown, startup;
	wxString status1, status2, status3, status4, status5;
	wxString command1, command1Line, command2, command2Line;
	wxString command3, command3Line, command4, command4Line;
	wxString output1, output2, output3, output4;
	config.getControl(enabled, rpt1Callsign, rpt2Callsign, shutdown, startup, status1, status2, status3, status4, status5, command1, command1Line, command2, command2Line, command3, command3Line, command4, command4Line, output1, output2, output3, output4);
	m_thread->setControl(enabled, rpt1Callsign, rpt2Callsign, shutdown, startup, status1, status2, status3, status4, status5, command1, command1Line, command2, command2Line, command3, command3Line, command4, command4Line, output1, output2, output3, output4);
	wxLogInfo(wxT("Control: enabled: %d, RPT1: %s, RPT2: %s, shutdown: %s, startup: %s, status1: %s, status2: %s, status3: %s, status4: %s, status5: %s, command1: %s = %s, command2: %s = %s, command3: %s = %s, command4: %s = %s, output1: %s, output2: %s, output3: %s, output4: %s"), enabled, rpt1Callsign.c_str(), rpt2Callsign.c_str(), shutdown.c_str(), startup.c_str(), status1.c_str(), status2.c_str(), status3.c_str(), status4.c_str(), status5.c_str(), command1.c_str(), command1Line.c_str(), command2.c_str(), command2Line.c_str(), command1.c_str(), command1Line.c_str(), command2.c_str(), command2Line.c_str(), command3.c_str(), command3Line.c_str(), command4.c_str(), command4Line.c_str(), output1.c_str(), output2.c_str(), output3.c_str(), output4.c_str());

	unsigned int activeHangTime;
	config.getActiveHang(activeHangTime);
	m_thread->setActiveHang(activeHangTime);
	wxLogInfo(wxT("Active Hang: time: %u"), activeHangTime);

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

	return true;
}

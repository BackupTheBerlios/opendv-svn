/*
 *   Copyright (C) 2013 by Jonathan Naylor G4KLX
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

#include "DStarModemDVAPController.h"
#include "ModemProtocolServer.h"
#include "DStarModemThread.h"
#include "DStarModemConfig.h"
#include "DStarModemNull.h"
#include "DStarDefines.h"
#include "DStarModemD.h"
#include "Version.h"
#include "Logger.h"

#include <wx/cmdline.h>
#include <wx/wfstream.h>
#include <wx/fileconf.h>
#include <wx/filename.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>

const wxChar*       NAME_PARAM = wxT("Modem Name");
const wxChar* NOLOGGING_SWITCH = wxT("nolog");
const wxChar*    LOGDIR_OPTION = wxT("logdir");
const wxChar*   CONFDIR_OPTION = wxT("confdir");
const wxChar*    DAEMON_SWITCH = wxT("daemon");

static CDStarModemD* m_modem = NULL;

static void handler(int signum)
{
	m_modem->kill();
}

int main(int argc, char** argv)
{
	bool res = ::wxInitialize();
	if (!res) {
		::fprintf(stderr, "dstarmodemd: failed to initialise the wxWidgets library, exiting\n");
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

	if (parser.GetParamCount() == 0U) {
			::fprintf(stderr, "dstarmodemd: missing modem name, exiting\n");
			::wxUninitialize();
		return 1;
	}

	wxString name = parser.GetParam(0U);
	name.Prepend(wxT("GMSK_"));

	if (daemon) {
		pid_t pid = ::fork();

		if (pid < 0) {
			::fprintf(stderr, "dstarmodemd: error in fork(), exiting\n");
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

	wxString pidFileName;
	pidFileName.Printf(wxT("/var/run/dstarmodem_%s.pid"), name.c_str());

	char fileName[128U];
	::memset(fileName, 0x00U, 128U);
	for (unsigned int i = 0U; i < pidFileName.Len(); i++)
		fileName[i] = pidFileName.GetChar(i);

	FILE* fp = ::fopen(fileName, "wt");
	if (fp != NULL) {
		::fprintf(fp, "%u\n", ::getpid());
		::fclose(fp);
	}

	::signal(SIGUSR1, handler);

	m_modem = new CDStarModemD(nolog, logDir, confDir, name);

	if (!m_modem->init()) {
		::wxUninitialize();
		return 1;
	}

	m_modem->run();

	delete m_modem;

	::unlink(fileName);

	::wxUninitialize();

	return 0;
}

CDStarModemD::CDStarModemD(bool nolog, const wxString& logDir, const wxString& confDir, const wxString& name) :
m_name(name),
m_nolog(nolog),
m_logDir(logDir),
m_confDir(confDir),
m_thread(NULL),
m_checker(NULL)
{
}

CDStarModemD::~CDStarModemD()
{
}

bool CDStarModemD::init()
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

	wxString appName;
	appName = APPLICATION_NAME + wxT(" ") + m_name;
	appName.Replace(wxT(" "), wxT("_"));

	m_checker = new wxSingleInstanceChecker(appName, wxT("/tmp"));
	bool ret = m_checker->IsAnotherRunning();
	if (ret) {
		wxLogError(wxT("Another copy of the D-Star Modem is running, exiting"));
		return false;
	}

	wxLogInfo(wxT("Starting ") + APPLICATION_NAME + wxT(" - ") + VERSION);

	// Log the SVN revsion and the version of wxWidgets and the Operating System
	wxLogInfo(SVNREV);
	wxLogInfo(wxT("Using wxWidgets %d.%d.%d on %s"), wxMAJOR_VERSION, wxMINOR_VERSION, wxRELEASE_NUMBER, ::wxGetOsDescription().c_str());

	return createThread();
}

void CDStarModemD::run()
{
	m_thread->run();

	delete m_checker;

	wxLogInfo(APPLICATION_NAME + wxT(" is exiting"));
}

bool CDStarModemD::createThread()
{
	if (m_confDir.IsEmpty())
		m_confDir = CONF_DIR;

	CDStarModemConfig config(m_confDir, CONFIG_FILE_NAME, m_name);

	m_thread = new CDStarModemThread;

	CModemProtocolServer* server = new CModemProtocolServer(m_name);
	int ret = server->open();
	if (ret)
		m_thread->setServer(server);

	wxLogInfo(wxT("Listening on UNIX socket \"%s\""), m_name.c_str());

	MODEM_TYPE type;
	config.getType(type);

	IDStarModem* modem = NULL;
	if (type == MT_NONE) {
		wxLogInfo(wxT("Null:"));
		modem = new CDStarModemNull;
	} else if (type == MT_DVAP) {
		wxString port;
		unsigned int frequency;
		int power, squelch;
		config.getDVAP(port, frequency, power, squelch);
		wxLogInfo(wxT("DVAP: port: %s, frequency: %u Hz, power: %d dBm, squelch: %d dBm"), port.c_str(), frequency, power, squelch);
		modem = new CDStarModemDVAPController(port, frequency, power, squelch);
	} else {
		wxLogError(wxT("Unknown modem type: %d"), int(type));
	}

	if (modem != NULL) {
		bool res = modem->open();
		if (!res)
			wxLogError(wxT("Cannot open the D-Star modem"));
		else
			m_thread->setModem(modem);
	}

	return true;
}

void CDStarModemD::kill()
{
	wxASSERT(m_thread != NULL);

	m_thread->kill();
}

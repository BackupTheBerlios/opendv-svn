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
#include "DStarModemNull.h"
#include "DStarModemApp.h"
#include "DStarDefines.h"
#include "Version.h"
#include "Logger.h"

#include <wx/cmdline.h>

const wxChar*       NAME_PARAM = wxT("Modem Name");
const wxChar* NOLOGGING_SWITCH = wxT("nolog");
const wxChar*   INSTALL_SWITCH = wxT("install");
const wxChar* UNINSTALL_SWITCH = wxT("uninstall");
const wxChar*       RUN_SWITCH = wxT("run");
const wxChar*    LOGDIR_OPTION = wxT("logdir");
const wxChar*   CONFDIR_OPTION = wxT("confdir");

const unsigned int FILENAME_LEN = 255U;

const wxChar* SERVICE_NAME = wxT("D-Star Modem");

IMPLEMENT_APP_NO_MAIN(CDStarModemApp)

void WINAPI svcStart(DWORD argc, LPTSTR* argv);
static SERVICE_TABLE_ENTRY g_dispatchTable[] = {
	{wxT("D-Star Modem"), svcStart},
	{NULL,                NULL}
};

static SERVICE_STATUS g_svcStatus = {
	SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS,
	SERVICE_STOPPED,
	SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN,
	NO_ERROR,
	0UL,
	0UL,
	100UL
};

static SERVICE_STATUS_HANDLE g_svcStatusHnd;

static HINSTANCE g_hInstance;
static HINSTANCE g_hPrevInstance;
static wxCmdLineArgType g_lpCmdLine;
static int g_nCmdShow;

extern "C" int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, wxCmdLineArgType lpCmdLine, int nCmdShow)
{
		g_hInstance     = hInstance;
		g_hPrevInstance = hPrevInstance;
		g_lpCmdLine     = lpCmdLine;
		g_nCmdShow      = nCmdShow;
		return wxEntry(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	if (strstr(lpCmdLine, "-run") == NULL) {
		return wxEntry(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	} else {
		g_hInstance     = hInstance;
		g_hPrevInstance = hPrevInstance;
		g_lpCmdLine     = lpCmdLine;
		g_nCmdShow      = nCmdShow;
		::StartServiceCtrlDispatcher(g_dispatchTable);
		return 0;
	}
}

CDStarModemApp::CDStarModemApp() :
wxApp(),
m_thread(NULL),
m_config(NULL),
m_checker(NULL)
{
}

CDStarModemApp::~CDStarModemApp()
{
}

bool CDStarModemApp::OnInit()
{
	wxCmdLineParser parser(argc, argv);
	parser.AddSwitch(NOLOGGING_SWITCH, wxEmptyString, wxEmptyString, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddSwitch(INSTALL_SWITCH,   wxEmptyString, wxEmptyString, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddSwitch(UNINSTALL_SWITCH, wxEmptyString, wxEmptyString, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddSwitch(RUN_SWITCH,       wxEmptyString, wxEmptyString, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddOption(LOGDIR_OPTION,    wxEmptyString, wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddOption(CONFDIR_OPTION,   wxEmptyString, wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
	parser.AddParam(NAME_PARAM, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);

	int ret = parser.Parse();
	if (ret != 0)
		return false;

	wxString logDir = wxEmptyString;
	bool found = parser.Found(LOGDIR_OPTION, &logDir);

	wxString confDir = wxEmptyString;
	found = parser.Found(CONFDIR_OPTION, &confDir);

	if (parser.GetParamCount() == 0U) {
		wxMessageDialog(NULL, wxT("The name of the modem must be specified"), SERVICE_NAME, wxOK).ShowModal();
		return false;		
	}

	wxString name = parser.GetParam(0U);
#if defined(__WINDOWS__)
	name.Prepend(wxT("GMSK "));
#else
	name.Prepend(wxT("GMSK_"));
#endif

	if (parser.Found(RUN_SWITCH)) {
		SetVendorName(VENDOR_NAME);

		if (!parser.Found(NOLOGGING_SWITCH)) {
			wxString logBaseName = LOG_BASE_NAME;
			if (!name.IsEmpty()) {
				logBaseName.Append(wxT("_"));
				logBaseName.Append(name);
			}

#if defined(__WINDOWS__)
			if (logDir.IsEmpty())
				logDir = wxFileName::GetHomeDir();
#else
			if (logDir.IsEmpty())
				logDir = LOG_DIR;
#endif

			wxLog* log = new CLogger(logDir, logBaseName);
			wxLog::SetActiveTarget(log);
		} else {
			new wxLogNull;
		}

		wxString appName;
		if (!name.IsEmpty())
			appName = APPLICATION_NAME + wxT(" ") + name;
		else
			appName = APPLICATION_NAME;

#if !defined(__WINDOWS__)
		appName.Replace(wxT(" "), wxT("_"));
		m_checker = new wxSingleInstanceChecker(appName, wxT("/tmp"));
#else
		m_checker = new wxSingleInstanceChecker(appName);
#endif

		bool ret = m_checker->IsAnotherRunning();
		if (ret) {
			wxLogError(wxT("Another copy of the D-Star Modem is running, exiting"));
			return false;
		}

#if defined(__WINDOWS__)
		m_config = new CDStarModemConfig(new wxConfig(APPLICATION_NAME), name);
#else
		if (confDir.IsEmpty())
			confDir = CONF_DIR;

		m_config = new CDStarModemConfig(m_confDir, CONFIG_FILE_NAME, name);
#endif

		wxLogInfo(wxT("Starting ") + APPLICATION_NAME + wxT(" - ") + VERSION);

		// Log the SVN revsion and the version of wxWidgets and the Operating System
		wxLogInfo(SVNREV);
		wxLogInfo(wxT("Using wxWidgets %d.%d.%d on %s"), wxMAJOR_VERSION, wxMINOR_VERSION, wxRELEASE_NUMBER, ::wxGetOsDescription().c_str());

		createThread(name);

		return true;
	} else if (parser.Found(INSTALL_SWITCH)) {
		if (isInstalled()) {
			wxMessageDialog(NULL, wxT("D-Star Modem already installed"), SERVICE_NAME, wxOK).ShowModal();
		} else {
			install();
		}
	} else if (parser.Found(UNINSTALL_SWITCH)) {
		if (!isInstalled()) {
			wxMessageDialog(NULL, wxT("D-Star Modem was not installed."), SERVICE_NAME, wxOK).ShowModal();
		} else {
			uninstall();
		}
	} else {
		wxMessageDialog(NULL, wxT("Must have one of -run, -install or -uninstall"), SERVICE_NAME, wxOK).ShowModal();
	}

	return false;
}

int CDStarModemApp::OnExit()
{
	wxLogInfo(APPLICATION_NAME + wxT(" is exiting"));

	m_thread->kill();

	delete m_config;

	delete m_checker;

	return 0;
}

#if defined(__WXDEBUG__)
void CDStarModemApp::OnAssertFailure(const wxChar* file, int line, const wxChar* func, const wxChar* cond, const wxChar* msg)
{
	wxLogFatalError(wxT("Assertion failed on line %d in file %s and function %s: %s %s"), line, file, func, cond, msg);
}
#endif

bool CDStarModemApp::isInstalled()
{
	bool found = false;

	SC_HANDLE scm = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (scm == NULL) {
		wxMessageDialog(NULL, wxString::Format(wxT("OpenSCManager error %d"), GetLastError()), SERVICE_NAME, wxOK).ShowModal();
		return false;
	}

	SC_HANDLE svc = ::OpenService(scm, SERVICE_NAME, SERVICE_ALL_ACCESS);
	if (svc != NULL) {
		found = true;
		::CloseServiceHandle(svc);
	}

	::CloseServiceHandle(scm);

	return found;
}

void CDStarModemApp::install()
{
	SC_HANDLE scm = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (scm == NULL) {
		wxMessageDialog(NULL, wxString::Format(wxT("OpenSCManager error %d"), GetLastError()), SERVICE_NAME, wxOK).ShowModal();
		return;
	}

	// Get the args but strip out the -install command
	wxString args = wxString((char*)g_lpCmdLine, wxConvLocal);
	args.Replace(wxT("-install"), wxEmptyString);
	args.Prepend(wxT("-run "));
	args.Trim();

	TCHAR fileName[FILENAME_LEN];
	::GetModuleFileName(NULL, fileName, FILENAME_LEN);

	wxString commandLine;
	commandLine.Printf(wxT("%s %s"), fileName, args);

	SC_HANDLE schService = ::CreateService(scm, SERVICE_NAME, SERVICE_NAME, SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS, SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL, commandLine, NULL, NULL, NULL, NULL, NULL);
	if (schService == NULL) {
		wxMessageDialog(NULL, wxString::Format(wxT("CreateService error %d"), GetLastError()), SERVICE_NAME, wxOK).ShowModal();
		::CloseServiceHandle(scm);
		return;
	}

	wxMessageDialog(NULL, wxT("D-Star Modem installed."), SERVICE_NAME, wxOK).ShowModal();

	::CloseServiceHandle(schService);
	::CloseServiceHandle(scm);
}

void CDStarModemApp::uninstall()
{
	SC_HANDLE scm = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (scm == NULL) {
		wxMessageDialog(NULL, wxString::Format(wxT("OpenSCManager error %d"), GetLastError()), SERVICE_NAME, wxOK).ShowModal();
		return;
	}

	SC_HANDLE svc = ::OpenService(scm, wxT("D-Star Modem"), SERVICE_ALL_ACCESS);
	if (svc == NULL) {
		wxMessageDialog(NULL, wxString::Format(wxT("OpenService error %d"), GetLastError()), SERVICE_NAME, wxOK).ShowModal();
		::CloseServiceHandle(scm);
		return;
	}

	SERVICE_STATUS status;
	::ControlService(svc, SERVICE_CONTROL_STOP, &status);

	if (!::DeleteService(svc))
		wxMessageDialog(NULL, wxString::Format(wxT("DeleteService error %d"), GetLastError()), SERVICE_NAME, wxOK).ShowModal();
	else
		wxMessageDialog(NULL, wxT("D-Star Modem uninstalled."), SERVICE_NAME, wxOK).ShowModal();

	::CloseServiceHandle(svc);
	::CloseServiceHandle(scm);
}

void WINAPI svcHandler(DWORD opcode)
{
	if (g_svcStatusHnd == NULL)
		return;

	switch (opcode) {
		case SERVICE_CONTROL_SHUTDOWN:
		case SERVICE_CONTROL_STOP:
			g_svcStatus.dwCurrentState = SERVICE_STOPPED;
			break;

		case SERVICE_CONTROL_INTERROGATE:
			break;
	}

	if (!::SetServiceStatus(g_svcStatusHnd, &g_svcStatus))
		MessageBox(NULL, wxString::Format(wxT("SetServiceStatus handler error %d"), GetLastError()), SERVICE_NAME, MB_OK);
}

void WINAPI svcStart(DWORD argc, LPTSTR* argv)
{
	g_svcStatusHnd = ::RegisterServiceCtrlHandler(SERVICE_NAME, &svcHandler);
	if (g_svcStatusHnd == NULL) {
		MessageBox(NULL, wxString::Format(wxT("RegisterServiceCtrlHandler error %d"), GetLastError()), SERVICE_NAME, MB_OK);
		return;
	}

	g_svcStatus.dwCurrentState = SERVICE_RUNNING;

	if (!::SetServiceStatus(g_svcStatusHnd, &g_svcStatus)) {
		MessageBox(NULL, wxString::Format(wxT("SetServiceStatus Run error %d"), GetLastError()), SERVICE_NAME, MB_OK);
		return;
	}

	wxEntry(g_hInstance, g_hPrevInstance, g_lpCmdLine, g_nCmdShow);
}

void CDStarModemApp::createThread(const wxString& name)
{
	wxASSERT(m_config != NULL);

	CDStarModemThread* thread = new CDStarModemThread;

	CModemProtocolServer* server = new CModemProtocolServer(name);
	int ret = server->open();
	if (ret)
		thread->setServer(server);

	wxLogInfo(wxT("Listening on Named Pipe \"%s\""), name.c_str());

	MODEM_TYPE type;
	m_config->getType(type);

	IDStarModem* modem = NULL;
	if (type == MT_NONE) {
		wxLogInfo(wxT("Null:"));
		modem = new CDStarModemNull;
	} else if (type == MT_DVAP) {
		wxString port;
		unsigned int frequency;
		int power, squelch;
		m_config->getDVAP(port, frequency, power, squelch);
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
			thread->setModem(modem);
	}

	// Convert the worker class into a thread
	m_thread = new CDStarModemThreadHelper(thread);
	m_thread->start();
}

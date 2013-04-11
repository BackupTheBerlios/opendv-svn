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

#include "DStarModemService.h"

const unsigned int FILENAME_LEN = 255U;

const wxChar* SERVICE_NAME = wxT("D-Star Modem");

IMPLEMENT_APP_NO_MAIN(CDStarModemService)

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
	if (strstr(lpCmdLine, "-run") == NULL) {
		return wxEntry(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	} else {
		g_hInstance     = hInstance;
		g_hPrevInstance = hPrevInstance;
		g_lpCmdLine     = lpCmdLine;
		g_nCmdShow      = nCmdShow;

		BOOL ret = ::StartServiceCtrlDispatcher(g_dispatchTable);
		if (!ret)
			wxMessageDialog(NULL, wxString::Format(wxT("StartServiceCtrlDispatcher error %d"), GetLastError()), SERVICE_NAME, wxOK).ShowModal();

		return 0;
	}
}

CDStarModemService::CDStarModemService()
{
}

CDStarModemService::~CDStarModemService()
{
}

bool CDStarModemService::OnInit()
{
	const wxString usage = wxT("-install\n-uninstall");

	if (argc != 2) {
		wxMessageDialog(NULL, usage, SERVICE_NAME, wxOK).ShowModal();
		return false;
	}

	wxString command = wxString(argv[1]);

	if (command.IsSameAs(wxT("-run"))) {
		// INIT THE REAL SERVICE HERE AND LET THE MAIN LOOP RUN
		return true;
	} else if (command.IsSameAs(wxT("-install"))) {
		if (isInstalled()) {
			wxMessageDialog(NULL, wxT("D-Star Modem already installed"), SERVICE_NAME, wxOK).ShowModal();
		} else {
			install();
		}
	} else if (command.IsSameAs(wxT("-uninstall"))) {
		if (!isInstalled()) {
			wxMessageDialog(NULL, wxT("D-Star Modem was not installed."), SERVICE_NAME, wxOK).ShowModal();
		} else {
			uninstall();
		}
	} else {
		wxMessageDialog(NULL, usage, SERVICE_NAME, wxOK).ShowModal();
	}

	return FALSE;
}

bool CDStarModemService::isInstalled()
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

void CDStarModemService::install()
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

void CDStarModemService::uninstall()
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

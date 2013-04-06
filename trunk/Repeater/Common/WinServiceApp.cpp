////////////////////////////////////////////////////////////////////////////////
// SDG!                                                                       //
//                                                                            //
// Name:        WinServiceApp.cpp                                             //
// Purpose:                                                                   //
// Author:      Jan Knepper                                                   //
// Created:     2007/06/10                                                    //
// Copyright:   (c) 2007 Jan Knepper                                          //
// Licence:     wxWidgets licence                                             //
////////////////////////////////////////////////////////////////////////////////

#include "WinServiceDef.h"
#include "WinServiceEvent.h"
#include "WinServiceApp.h"

#include <wx/wx.h>

const DWORD THREAD_ALIVE = 15000UL;

////////////////////////////////////////////////////////////////////////////////
// CWinServiceStatus                                                         //
////////////////////////////////////////////////////////////////////////////////

const Type   CWinServiceStatus::SERVICETYPE = Type_WIN32_OWN_PROCESS;
const Accept CWinServiceStatus::ACCEPTED    = static_cast<Accept>(Accept_STOP | Accept_PAUSE_CONTINUE | Accept_SHUTDOWN | Accept_PARAMCHANGE);

CWinServiceStatus::CWinServiceStatus() :
SERVICE_STATUS()
{
	dwServiceType              = SERVICETYPE;

	dwCurrentState             = State_STOPPED;
	dwControlsAccepted         = Accept_NULL;

	dwWin32ExitCode            = NO_ERROR;
	dwServiceSpecificExitCode  = 0;
	dwCheckPoint               = 0;
	dwWaitHint                 = 100;
}

CWinServiceStatus::CWinServiceStatus(const State&  _dwCurrentState, Accept _dwControlsAccepted) :
SERVICE_STATUS()
{
	dwServiceType              = SERVICETYPE;

	dwCurrentState             = _dwCurrentState;
	dwControlsAccepted         = _dwControlsAccepted;

	dwWin32ExitCode            = NO_ERROR;
	dwServiceSpecificExitCode  = 0;
	dwCheckPoint               = 0;
	dwWaitHint                 = 100;
}

const wxChar* CWinServiceStatus::FormatState() const
{
	return FormatState(static_cast<State>(dwCurrentState));
}

const wxChar* CWinServiceStatus::FormatState(State state)
{
	static const wxChar* STATES[] = {
		wxT("Unknown"         ),
		wxT("Stopped"         ),
		wxT("Start Pending"   ),
		wxT("Stop Pending"    ),
		wxT("Running"         ),
		wxT("Continue Pending"),
		wxT("Pause Pending"   ),
		wxT("Paused"          )
	};

	if (state >= State_STOPPED || state <= State_PAUSED)
		return STATES[state];

	return STATES[0];
}

////////////////////////////////////////////////////////////////////////////////
// CServiceThread                                                            //
////////////////////////////////////////////////////////////////////////////////

CWinServiceThread::CWinServiceThread() :
wxThread(wxTHREAD_JOINABLE)
{
}

////////////////////////////////////////////////////////////////////////////////
// Static functions to map Win API to wxWidgets objects                       //
////////////////////////////////////////////////////////////////////////////////

void WINAPI CWinServiceApp::Main(DWORD /*argc*/, LPTSTR* /*argv*/)
{
	CWinServiceApp* wsa = wxDynamicCast(wxApp::GetInstance(), CWinServiceApp);
	if (wsa != NULL)
		wsa->Start();
}

DWORD WINAPI CWinServiceApp::Handler(DWORD control, DWORD eventtype, LPVOID eventdata, LPVOID _wsa /*context*/)
{
	wxASSERT(_wsa != NULL);

	CWinServiceApp* wsa = wxDynamicCast( _wsa, CWinServiceApp);
	if (wsa != NULL) {
		CWinServiceEvent wse(static_cast<Control>(control), eventtype, eventdata);
      
		if (wsa->ProcessEvent(wse))
			return wse.GetError();
	}
 
	return ERROR_CALL_NOT_IMPLEMENTED;
}

VOID WINAPI CWinServiceApp::Exit(PVOID _wsa /*data*/, BOOLEAN flag)
{
	wxASSERT(_wsa != NULL);

	CWinServiceApp* wsa = wxDynamicCast(_wsa, CWinServiceApp);
	if (wsa != NULL)
		wsa->Exit(flag != FALSE);
}

////////////////////////////////////////////////////////////////////////////////
// CWinServiceApp                                                            //
////////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(CWinServiceApp, wxAppConsole)
	EVT_WINSERVICE(CWinServiceApp::OnWinService)
END_EVENT_TABLE()

int CWinServiceApp::OnRun()
{
	const static SERVICE_TABLE_ENTRY ste[] = {
		{wxT(""), Main},
		{0, 0}
	};

	if (::StartServiceCtrlDispatcher(ste) != FALSE)
		return 0;

	return -1;
}

void CWinServiceApp::OnWinService(CWinServiceEvent& wse)
{

	switch (wse.GetControl()) {
		case Control_CONTINUE:
			if (m_thread != NULL && m_thread->IsPaused())
				m_thread->Resume();

			wxLogMessage(wxT("CWinServiceEvent :: Control_CONTINUE"));
			SetStatus(CWinServiceStatus(State_RUNNING));
			break;

		case Control_INTERROGATE:
			wxLogMessage(wxT("CWinServiceEvent :: Control_INTEROGATE"));
			break;               

		case Control_PARAMCHANGE:
			wxLogMessage(wxT("CWinServiceEvent :: Control_PARAMCHANGE"));
			break;                

		case Control_PAUSE:
			if (m_thread != NULL && m_thread->IsRunning())
				m_thread->Pause();

			wxLogMessage(wxT("CWinServiceEvent :: Control_PAUSE"));
			SetStatus(CWinServiceStatus(State_PAUSED));
			break;              

		case Control_SHUTDOWN:  // System Shutdown.
			wxLogMessage(wxT("CWinServiceEvent :: Control_SHUTDOWN"));
			SetStatus(CWinServiceStatus(State_STOP_PENDING, Accept_SHUTDOWN));
			Stop();
			break;                

		case Control_STOP:
			wxLogMessage (wxT("CWinServiceEvent :: Control_STOP"));
			SetStatus(CWinServiceStatus(State_STOP_PENDING, Accept_SHUTDOWN));
			Stop();
			break;                

		default:
			wxLogMessage(wxT("%s UNKNOWN: %lX %lX %LX"), __FUNCTION__, wse.GetControl(), wse.GetType(), wse.GetData());
			wse.SetError(Error_CALL_NOT_IMPLEMENTED );
			wse.Skip();                 // Act as it was not handled!
			break;
	}
}

void CWinServiceApp::Install(bool create)
{
	TCHAR name[FILENAME_MAX];
	DWORD NAME = ::GetModuleFileName(0, name, FILENAME_MAX);
	if (NAME == 0)
		return;

	name[NAME] = _T('\0');

	wxLogMessage(wxT("%s %s"), create ? wxT("Installing") : wxT("Uninstalling"), name);
         
	SC_HANDLE scm = ::OpenSCManager(0, 0, SC_MANAGER_ALL_ACCESS);
	if (scm == 0) {
		wxLogError(wxT("Could not open Service Control Manager"));
		return;
	}

	if (create) {
		SC_HANDLE handle = ::CreateService(scm, m_serviceName.c_str(), m_displayName.c_str(), SC_MANAGER_CREATE_SERVICE, CWinServiceStatus :: SERVICETYPE, SERVICE_AUTO_START, SERVICE_ERROR_NORMAL, name, 0, 0, 0, 0, 0 );
		if (handle != 0) {
			wxLogMessage(wxT("Installed '%s' (%s)"), m_serviceName.c_str(), name);
         
			::CloseServiceHandle(handle);
		} else {
			wxLogError(wxT( "Could not install %s (%s)" ), m_serviceName.c_str(), name);
		}
	} else {
		SC_HANDLE handle = ::OpenService(scm, m_serviceName.c_str(), SC_MANAGER_ALL_ACCESS);
		if (handle != 0) {
			wxLogMessage(wxT("Found Service %s"), m_serviceName.c_str());
         
			if (::DeleteService(handle))
				wxLogMessage(wxT("Deleted '%s'"), m_serviceName.c_str());
			else
				wxLogError(wxT("Could not delete '%s'"), m_serviceName.c_str());
            
			::CloseServiceHandle(handle);
		} else {
			wxLogError(wxT("Could not open Service '%s'"), m_serviceName.c_str());
		}
	}

	::CloseServiceHandle(scm);
}

bool CWinServiceApp::SetStatus(const CWinServiceStatus& ss)
{
	m_wss = ss;

	if (m_ssh != NULL)
		return ::SetServiceStatus(m_ssh, m_wss) != FALSE;

	// Presume debugging/development mode, i.e. running as a console application!
	wxLogMessage(m_wss.FormatState());

	return true;
}

void CWinServiceApp::Stop()
{
	// Signal 'event' so the wait-thread releases!
	::SetEvent(m_event);              // Release the wait-thread.
}

bool CWinServiceApp::Start()
{
	m_ssh = ::RegisterServiceCtrlHandlerEx(m_serviceName.c_str(), Handler, this);
	if (m_ssh == 0)
		return false;

	if (!SetStatus(CWinServiceStatus(State_RUNNING)))
		return false;

	m_thread = StartThread();

	m_event = ::CreateEvent(0, TRUE, FALSE, 0 /*servicename*/);

   ::RegisterWaitForSingleObject(&m_wait, m_event, CWinServiceApp::Exit, this, THREAD_ALIVE, WT_EXECUTEDEFAULT);

   return true;
}

// Exit is used for 2 purposes:
// 1. To normally stop (Exit) the service and terminate (Delete) the 
//    CWinServiceThread. This is done by calling Stop from the Handler/
//    OnWinService event handler which signals 'event'.
// 2. To stop (Exit) the service and terminate (Delete) the CWinServiceThread
//    in case it has died. This is done every THREAD_ALIVE milliseconds.
//    if 'flag' is true this THREAD_ALIVE timeout is running.

void CWinServiceApp::Exit(bool flag)
{
	if (m_thread == NULL)
		return;
      
	if (flag) {								// Time out has occurred.
		// Check if the thread is still alive.
		if (m_thread != NULL && m_thread->IsAlive())
			return;                          // If it is, return.
	}   

	// For RegisterWaitForSingleObject bug/stupidity.
	// It calls the WAITORTIMERCALLBACK (this function) several times when 'event'
	// is being signaled.

	if (m_mutex.TryLock () == wxMUTEX_BUSY)
		return;
   
   ::CloseHandle(m_event);

   ::UnregisterWait(m_wait);

   if (m_thread != NULL) {
		m_thread->Delete();
		m_thread->Wait();

		delete  m_thread;
		m_thread   = NULL;
	}

	SetStatus(CWinServiceStatus(State_STOPPED, Accept_SHUTDOWN));
}

int CWinServiceApp::Debug(int argc, wxChar** argv)
{
	m_thread = StartThread();

	m_event  = ::CreateEvent(0, TRUE, FALSE, 0 /*servicename*/);

	::RegisterWaitForSingleObject(&m_wait, m_event, CWinServiceApp::Exit, this, THREAD_ALIVE, WT_EXECUTEDEFAULT);
   
	for (;;) {
		wxLogMessage(wxT("[S]top"   ));
		wxLogMessage(wxT("[P]ause"  ));
		wxLogMessage(wxT("[R]esume" ));
		wxLogMessage(wxT(""         ));
		wxLogMessage(wxT("Command: "));

		switch (getchar()) {
			case 'S':
			case 's':
				Handler(SERVICE_CONTROL_STOP, 0, 0, this);
				::WaitForSingleObject(m_wait, INFINITE);
            
				while (m_wss.GetState () != State_STOPPED) {
					wxLogMessage(wxT("Waiting for SetServiceStatus ( SERVICE_STOPPED )"));
					wxSleep(5);
				}

				return 0;

			case 'P':
			case 'p':
				Handler(SERVICE_CONTROL_PAUSE, 0, 0, this);
				break;

			case 'R':
			case 'r':
				Handler(SERVICE_CONTROL_CONTINUE, 0, 0, this);
				break;
		}
	}
}

CWinServiceApp::CWinServiceApp(const wxString& _servicename, const wxString& _displayname) :
wxAppConsole(),
m_thread(NULL),
m_serviceName(_servicename),
m_displayName(_displayname),
m_mutex(),
m_ssh(0),
m_wait(INVALID_HANDLE_VALUE),
m_event(INVALID_HANDLE_VALUE),
m_wss()
{
}

CWinServiceApp :: ~CWinServiceApp ()
{
}

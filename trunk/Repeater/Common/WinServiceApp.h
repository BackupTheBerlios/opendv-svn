////////////////////////////////////////////////////////////////////////////////
// SDG!                                                                       //
//                                                                            //
// Name:        WinServiceApp.hpp                                             //
// Purpose:                                                                   //
// Author:      Jan Knepper                                                   //
// Created:     2007/06/10                                                    //
// Copyright:   (c) 2007 Jan Knepper                                          //
// Licence:     wxWidgets licence                                             //
////////////////////////////////////////////////////////////////////////////////

#ifndef WINSERVICEAPP_HPP
#define WINSERVICEAPP_HPP

#include <wx/app.h>

#include "WinServiceDef.h"
#include "WinServiceEvent.h"
#include "WinServiceApp.h"

enum Type {
	Type_KERNEL_DRIVER       = SERVICE_KERNEL_DRIVER,
	Type_FILE_SYSTEM_DRIVER  = SERVICE_FILE_SYSTEM_DRIVER,
	Type_ADAPTER             = SERVICE_ADAPTER,
	Type_RECOGNIZER_DRIVER   = SERVICE_RECOGNIZER_DRIVER,
	Type_DRIVER              = SERVICE_DRIVER,
	Type_WIN32_OWN_PROCESS   = SERVICE_WIN32_OWN_PROCESS,
	Type_WIN32_SHARE_PROCESS = SERVICE_WIN32_SHARE_PROCESS,
	Type_WIN32               = SERVICE_WIN32,
	Type_TYPE_ALL            = SERVICE_TYPE_ALL
};

enum  State {
	State_STOPPED          = SERVICE_STOPPED,
	State_START_PENDING    = SERVICE_START_PENDING,
	State_STOP_PENDING     = SERVICE_STOP_PENDING,
	State_RUNNING          = SERVICE_RUNNING,
	State_CONTINUE_PENDING = SERVICE_CONTINUE_PENDING,
	State_PAUSE_PENDING    = SERVICE_PAUSE_PENDING,
	State_PAUSED           = SERVICE_PAUSED
};

enum  Accept {
	Accept_NULL                  = 0,

	Accept_STOP                  = SERVICE_ACCEPT_STOP,
	Accept_PAUSE_CONTINUE        = SERVICE_ACCEPT_PAUSE_CONTINUE,
	Accept_SHUTDOWN              = SERVICE_ACCEPT_SHUTDOWN,
	Accept_PARAMCHANGE           = SERVICE_ACCEPT_PARAMCHANGE,
	Accept_NETBINDCHANGE         = SERVICE_ACCEPT_NETBINDCHANGE,
	Accept_HARDWAREPROFILECHANGE = SERVICE_ACCEPT_HARDWAREPROFILECHANGE,
	Accept_POWEREVENT            = SERVICE_ACCEPT_POWEREVENT,
	Accept_SESSIONCHANGE         = SERVICE_ACCEPT_SESSIONCHANGE
};

class WXDLLIMPEXP_WINSERVICE CWinServiceStatus : protected SERVICE_STATUS
{
public:
	CWinServiceStatus(const State& state, Accept = ACCEPTED);
	CWinServiceStatus();
	~CWinServiceStatus();

	State GetState() const
	{
		return static_cast<State>(dwCurrentState);
	}

	const wxChar* FormatState() const;

	static const wxChar* FormatState(State state);
	static const Type    SERVICETYPE;
	static const Accept  ACCEPTED;

	operator SERVICE_STATUS*()
	{
		return this;
	}

private:
};

class WXDLLIMPEXP_WINSERVICE CWinServiceThread : public wxThread
{
public:

protected :
	CWinServiceThread();

private:
};

class WXDLLIMPEXP_WINSERVICE CWinServiceApp : public wxAppConsole
{
public:
	~CWinServiceApp();

	int OnRun();

protected:
	CWinServiceThread* m_thread;

	void Install(bool create);
	bool SetStatus(const CWinServiceStatus& status);

	void Stop();

	virtual CWinServiceThread *StartThread() = 0;

	int Debug(int, wxChar**);
	CWinServiceApp(const wxString& serviceName, const wxString& displayName);

private:
	static void WINAPI  Main(DWORD, LPTSTR*);
	static DWORD WINAPI Handler(DWORD, DWORD, LPVOID, LPVOID);
	static VOID WINAPI  Exit(PVOID, BOOLEAN);

	wxString m_serviceName;
	wxString m_displayName;
	wxMutex  m_mutex;            // Purely for RegisterWaitForSingleObject bugs/stupidity.

	SERVICE_STATUS_HANDLE m_ssh;
	HANDLE                m_wait;
	HANDLE                m_event;
	CWinServiceStatus     m_wss;              // Last set status.

	void OnWinService(CWinServiceEvent& event);
	bool Start();
	void Exit(bool flag);

	DECLARE_EVENT_TABLE()
};

#endif

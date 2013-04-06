////////////////////////////////////////////////////////////////////////////////
// SDG!                                                                       //
//                                                                            //
// Name:        WinServiceEvent.hpp                                           //
// Purpose:                                                                   //
// Author:      Jan Knepper                                                   //
// Created:     2007/06/10                                                    //
// Copyright:   (c) 2007 Jan Knepper                                          //
// Licence:     wxWidgets licence                                             //
////////////////////////////////////////////////////////////////////////////////

#ifndef WINSERVICEEVENT_HPP
#define WINSERVICEEVENT_HPP

#include <wx/event.h>

#include "WinServiceDef.h"
#include "WinServiceApp.h"
#include "WinServiceEvent.h"

DECLARE_EVENT_TYPE(wxEVT_WINSERVICE, 0)

typedef void (wxEvtHandler::*CWinServiceEventFunction) (CWinServiceEvent& event);

#define CWinServiceEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(CWinServiceEventFunction,&func)

#define EVT_WINSERVICE(func) \
	wx__DECLARE_EVT0(wxEVT_WINSERVICE,CWinServiceEventHandler(func))

enum Control {
	Control_STOP                  = SERVICE_CONTROL_STOP,
	Control_PAUSE                 = SERVICE_CONTROL_PAUSE,
	Control_CONTINUE              = SERVICE_CONTROL_CONTINUE,
	Control_INTERROGATE           = SERVICE_CONTROL_INTERROGATE,
	Control_SHUTDOWN              = SERVICE_CONTROL_SHUTDOWN,
	Control_PARAMCHANGE           = SERVICE_CONTROL_PARAMCHANGE,
	Control_NETBINDADD            = SERVICE_CONTROL_NETBINDADD,
	Control_NETBINDREMOVE         = SERVICE_CONTROL_NETBINDREMOVE,
	Control_NETBINDENABLE         = SERVICE_CONTROL_NETBINDENABLE,
	Control_NETBINDDISABLE        = SERVICE_CONTROL_NETBINDDISABLE,
	Control_DEVICEEVENT           = SERVICE_CONTROL_DEVICEEVENT,
	Control_HARDWAREPROFILECHANGE = SERVICE_CONTROL_HARDWAREPROFILECHANGE,
	Control_POWEREVENT            = SERVICE_CONTROL_POWEREVENT,
	Control_SESSIONCHANGE         = SERVICE_CONTROL_SESSIONCHANGE
};

enum Error {
	Error_NONE                 = NO_ERROR,
	Error_CALL_NOT_IMPLEMENTED = ERROR_CALL_NOT_IMPLEMENTED
};

class WXDLLIMPEXP_WINSERVICE CWinServiceEvent : public wxEvent
{
public:
	CWinServiceEvent(Control control, wxUint32 type, void* data);
	CWinServiceEvent(CWinServiceApp* app, Control control, wxUint32 type, void* data);
	~CWinServiceEvent();

	Control  GetControl() const
	{
		return m_control;
	}

	wxUint32 GetType() const
	{
		return m_type;
	}

	void* GetData() const
	{
		return m_data;
	}

	Error GetError() const
	{
		return m_error;
	}

	void SetError(Error error)
	{
		m_error = error;
	}

	wxEvent* Clone() const;

protected :
	CWinServiceEvent(const CWinServiceEvent& event);

private:
	Control  m_control;
	wxUint32 m_type;
	void*    m_data;
	Error    m_error;
};

#endif

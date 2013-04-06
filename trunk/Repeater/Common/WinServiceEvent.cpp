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
#include "WinServiceApp.h"
#include "WinServiceEvent.h"

#include <wx/wx.h>

DEFINE_EVENT_TYPE(wxEVT_WINSERVICE)

wxEvent* CWinServiceEvent::Clone() const
{
	return new CWinServiceEvent(*this);
}

CWinServiceEvent::CWinServiceEvent(const CWinServiceEvent& event) :
wxEvent(event),
m_control(event.m_control),
m_type(event.m_type),
m_data(event.m_data),
m_error(event.m_error)
{
}

CWinServiceEvent::CWinServiceEvent(Control _control, wxUint32 _type, void* _data) :
wxEvent(0, wxEVT_WINSERVICE),
m_control(_control),
m_type(_type),
m_data(_data),
m_error(Error_NONE)
{
}

CWinServiceEvent::CWinServiceEvent(CWinServiceApp* _object, Control _control, wxUint32 _type, void* _data) :
wxEvent(0, wxEVT_WINSERVICE),
m_control(_control),
m_type(_type),
m_data(_data),
m_error(Error_NONE)
{
	m_eventObject  = _object;
}

CWinServiceEvent::~CWinServiceEvent()
{
}

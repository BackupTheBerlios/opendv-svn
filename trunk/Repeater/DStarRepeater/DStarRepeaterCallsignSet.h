/*
 *   Copyright (C) 2009,2010,2011 by Jonathan Naylor G4KLX
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

#ifndef	DStarRepeaterCallsignSet_H
#define	DStarRepeaterCallsignSet_H

#include "CallsignTextCtrl.h"
#include "DStarDefines.h"

#include <wx/wx.h>

class CDStarRepeaterCallsignSet : public wxPanel {
public:
	CDStarRepeaterCallsignSet(wxWindow* parent, int id, const wxString& title, const wxString& callsign, const wxString& gateway, DSTAR_MODE mode, ACK_TYPE ack, bool restriction, bool rpt1Validation);
	virtual ~CDStarRepeaterCallsignSet();

	virtual bool Validate();

	virtual wxString   getCallsign() const;
	virtual wxString   getGateway() const;
	virtual DSTAR_MODE getMode() const;
	virtual ACK_TYPE   getAck() const;
	virtual bool       getRestriction() const;
	virtual bool       getRPT1Validation() const;

	virtual void onMode(wxCommandEvent& event);

private:
	wxString           m_title;
	CCallsignTextCtrl* m_callsign;
	CCallsignTextCtrl* m_gateway;
	wxChoice*          m_suffix;
	wxChoice*          m_mode;
	wxChoice*          m_ack;
	wxChoice*          m_restriction;
	wxChoice*          m_rpt1Validation;

	DECLARE_EVENT_TABLE()
};

#endif

/*
 *   Copyright (C) 2011,2012,2013 by Jonathan Naylor G4KLX
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

#ifndef	DStarModemConfigFrame_H
#define	DStarModemConfigFrame_H

#include "DStarModemConfigDefs.h"

#include <wx/wx.h>

class CDStarModemConfigFrame : public wxFrame {
public:
#if defined(__WINDOWS__)
	CDStarModemConfigFrame(const wxString& title);
#else
	CDStarModemConfigFrame(const wxString& title, const wxString& confDir);
#endif
	virtual ~CDStarModemConfigFrame();

	virtual void onQuit(wxCommandEvent& event);
	virtual void onAbout(wxCommandEvent& event);
	virtual void onClose(wxCloseEvent& event);

	virtual void onConfigure(wxCommandEvent& event);

private:
#if !defined(__WINDOWS__)
	wxString  m_confDir;
#endif
	wxChoice* m_name;
	wxChoice* m_type;

	DECLARE_EVENT_TABLE()

	wxMenuBar* createMenuBar();
};

#endif

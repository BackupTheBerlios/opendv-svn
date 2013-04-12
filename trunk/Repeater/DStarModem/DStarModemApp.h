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

#ifndef DStarModemApp_H
#define	DStarModemApp_H

#include "DStarModemThreadHelper.h"
#include "DStarModemApp.h"
#include "DStarModemConfig.h"
#include "DStarModemDefs.h"

#include <wx/wx.h>
#include <wx/snglinst.h>

class CDStarModemApp: public wxApp
{
public:
	CDStarModemApp();
	virtual ~CDStarModemApp();

	virtual bool OnInit();
	virtual int  OnExit();

	// This is overridden because dialog boxes from threads are bad news
#if defined(__WXDEBUG__)
	virtual void OnAssertFailure(const wxChar* file, int line, const wxChar* func, const wxChar* cond, const wxChar* msg);
#endif

private:
	CDStarModemThreadHelper* m_thread;
	CDStarModemConfig*       m_config;
	wxSingleInstanceChecker* m_checker;

	void createThread(const wxString& name);
	bool isInstalled();
	void install();
	void uninstall();
};

#endif

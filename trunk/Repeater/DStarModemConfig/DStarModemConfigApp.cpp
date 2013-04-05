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

#include "DStarModemConfigApp.h"
#include "DStarDefines.h"
#include "Version.h"

#include <wx/cmdline.h>
#include <wx/filename.h>

IMPLEMENT_APP(CDStarModemConfigApp)

const wxChar* CONFDIR_OPTION = wxT("confdir");


CDStarModemConfigApp::CDStarModemConfigApp() :
wxApp(),
m_confDir(),
m_frame(NULL)
{
}

CDStarModemConfigApp::~CDStarModemConfigApp()
{
}

bool CDStarModemConfigApp::OnInit()
{
	SetVendorName(VENDOR_NAME);

	if (!wxApp::OnInit())
		return false;

	wxString frameName = APPLICATION_NAME + wxT(" - ") + VERSION;
#if defined(__WINDOWS__)
	m_frame = new CDStarModemConfigFrame(frameName);
#else
	m_frame = new CDStarModemConfigFrame(frameName, m_confDir);
#endif
	m_frame->Show();

	SetTopWindow(m_frame);

	return true;
}

int CDStarModemConfigApp::OnExit()
{
	return 0;
}

void CDStarModemConfigApp::OnInitCmdLine(wxCmdLineParser& parser)
{
	parser.AddOption(CONFDIR_OPTION, wxEmptyString, wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);

	wxApp::OnInitCmdLine(parser);
}

bool CDStarModemConfigApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
	if (!wxApp::OnCmdLineParsed(parser))
		return false;

	wxString confDir;
	bool found = parser.Found(CONFDIR_OPTION, &confDir);
	if (found)
		m_confDir = confDir;

	return true;
}

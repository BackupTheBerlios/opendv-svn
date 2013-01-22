/*
 *   Copyright (C) 2011,2012 by Jonathan Naylor G4KLX
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

#ifndef	TimerControlConfig_H
#define	TimerControlConfig_H

#include <wx/wx.h>
#include <wx/config.h>

class CTimerControlConfig {

public:
	CTimerControlConfig(wxConfigBase* config, const wxString& name);
	~CTimerControlConfig();

	void getGateway(wxString& address, unsigned int& port, wxString& password) const;
	void setGateway(const wxString& address, unsigned int port, const wxString& password) const;

	void getDelay(bool& enabled) const;
	void setDelay(bool enabled) const;

	void getPosition(int& x, int& y) const;
	void setPosition(int x, int y);

private:
	wxConfigBase* m_config;
	wxString      m_name;
};

#endif

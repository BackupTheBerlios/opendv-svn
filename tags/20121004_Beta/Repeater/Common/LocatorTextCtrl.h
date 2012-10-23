/*
 *   Copyright (C) 2002,2003,2012 by Jonathan Naylor G4KLX
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

#ifndef LocatorTextCtrl_H
#define	LocatorTextCtrl_H

#include <wx/wx.h>

#include "RestrictedTextCtrl.h"

const wxString LOCATOR_CHARS  = wxT("ABCDEFGHIJKLMNOPQRSTUVWX0123456789");

class CLocatorTextCtrl : public CRestrictedTextCtrl {

public:
	CLocatorTextCtrl(wxWindow* parent, int id, const wxString& value, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0L);
	virtual ~CLocatorTextCtrl();

    private:
};

#endif

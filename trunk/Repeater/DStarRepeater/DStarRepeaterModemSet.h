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

#ifndef	DStarRepeaterModemSet_H
#define	DStarRepeaterModemSet_H

#include "DStarRepeaterDefs.h"
#include "DStarDefines.h"

#include <wx/wx.h>

class CDStarRepeaterModemSet : public wxPanel {
public:
	CDStarRepeaterModemSet(wxWindow* parent, int id, const wxString& title, const wxString& name);
	virtual ~CDStarRepeaterModemSet();

	virtual bool Validate();

	virtual wxString getName() const;

private:
	wxString  m_title;
	wxChoice* m_name;
};

#endif

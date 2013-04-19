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

#ifndef	DStarRepeaterConfigRaspberrySet_H
#define	DStarRepeaterConfigRaspberrySet_H

#include "DStarRepeaterConfigDefs.h"
#include "DStarDefines.h"

#include <wx/wx.h>

class CDStarRepeaterConfigRaspberrySet : public wxDialog {
public:
	CDStarRepeaterConfigRaspberrySet(wxWindow* parent, int id, bool rxInvert, bool txInvert, unsigned int txDelay);
	virtual ~CDStarRepeaterConfigRaspberrySet();

	virtual bool Validate();

	virtual bool         getRXInvert() const;
	virtual bool         getTXInvert() const;
	virtual unsigned int getTXDelay() const;

private:
	wxChoice* m_txInvert;
	wxChoice* m_rxInvert;
	wxSlider* m_txDelay;
};

#endif

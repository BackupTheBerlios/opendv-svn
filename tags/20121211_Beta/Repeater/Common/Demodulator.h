/*
 *	Copyright (C) 2009,2011 by Jonathan Naylor, G4KLX
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; version 2 of the License.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 */

#ifndef	Demodulator_H
#define	Demodulator_H

#include <wx/wx.h>

#include "Utils.h"

class IDemodulator {
public:
	virtual TRISTATE decode(wxFloat32 val, bool listening) = 0;

	virtual void reset() = 0;

private:
};

#endif

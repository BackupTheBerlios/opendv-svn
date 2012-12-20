/*
 *	Copyright (C) 2009 by Jonathan Naylor, G4KLX
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

#ifndef	Modulator_H
#define	Modulator_H

#include <wx/wx.h>

class IModulator {
public:
	virtual unsigned int code(bool bit, wxFloat32* buffer, unsigned int length) = 0;

private:
};

#endif

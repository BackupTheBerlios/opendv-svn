/*
 *	Copyright (C) 2010,2011,2012 by Jonathan Naylor, G4KLX
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

#ifndef	Version_H
#define	Version_H

#include <wx/wx.h>

const wxString VENDOR_NAME = wxT("G4KLX");

#if defined(__WXDEBUG__)
const wxString VERSION = wxT("20120901 - DEBUG");
#else
const wxString VERSION = wxT("20120901");
#endif

#endif

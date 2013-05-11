/*
 *   Copyright (C) 2010-2013 by Jonathan Naylor G4KLX
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

#ifndef	GMSKModem_H
#define	GMSKModem_H

#include "HeaderData.h"
#include "Utils.h"

#include <wx/wx.h>

class IGMSKModem {
public:
	virtual bool open() = 0;

	virtual CHeaderData* readHeader(bool& error) = 0;							// Old
	virtual bool readHeader(unsigned char* header, unsigned int length) = 0;	// News
	virtual int  readData(unsigned char* data, unsigned int length, bool& end) = 0;

	virtual TRISTATE getPTT() = 0;
	virtual void     setPTT(bool on) = 0;

	virtual TRISTATE hasSpace() = 0;											// Old
	virtual int      getSpace() = 0;											// New

	virtual void writeHeader(const CHeaderData& header) = 0;					// Old
	virtual void writeHeader(unsigned char* data, unsigned int length) = 0;		// New
	virtual int  writeData(unsigned char* data, unsigned int length) = 0;

	virtual void close() = 0;

private:
};

#endif


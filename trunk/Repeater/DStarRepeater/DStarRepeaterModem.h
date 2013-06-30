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

#ifndef	DStarRepeaterModem_H
#define	DStarRepeaterModem_H

#include "HeaderData.h"

#include <wx/wx.h>

enum DSMT_TYPE {
	DSMTT_NONE,
	DSMTT_HEADER,
	DSMTT_DATA,
	DSMTT_EOT,
	DSMTT_LOST
};

class IDStarRepeaterModem {
public:
	virtual bool start() = 0;

	virtual bool writeHeader(const CHeaderData& header) = 0;
	virtual bool writeData(const unsigned char* data, unsigned int length, bool end) = 0;

	virtual unsigned int getSpace() = 0;
	virtual bool getTX() = 0;

	virtual DSMT_TYPE read() = 0;
	virtual CHeaderData* readHeader() = 0;
	virtual unsigned int readData(unsigned char* data, unsigned int length) = 0;

	virtual void stop() = 0;

private:
};

#endif

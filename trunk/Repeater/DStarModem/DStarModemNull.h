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

#ifndef	DStarModemNull_H
#define	DStarModemNull_H

#include "DStarModem.h"

#include <wx/wx.h>

class CDStarModemNull : public IDStarModem {
public:
	CDStarModemNull();
	virtual ~CDStarModemNull();

	virtual bool open();

	virtual bool writeHeader(const CHeaderData& header);
	virtual bool writeData(const unsigned char* data, unsigned int length, bool end);

	virtual bool setTX(bool on);

	virtual unsigned int getSpace();
	virtual bool         getTX();

	virtual DSMT_TYPE read();
	virtual CHeaderData* readHeader();
	virtual unsigned int readData(unsigned char* data, unsigned int length, bool& end);

	virtual void heartbeat();

	virtual void close();

private:
};

#endif

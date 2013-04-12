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

#include "DStarModemNull.h"

CDStarModemNull::CDStarModemNull()
{
}

CDStarModemNull::~CDStarModemNull()
{
}

bool CDStarModemNull::open()
{
	wxLogMessage(wxT("Null D-Star Modem opened successfully"));

	return true;
}

bool CDStarModemNull::writeHeader(const CHeaderData& header)
{
	return true;
}

bool CDStarModemNull::writeData(const unsigned char* data, unsigned int length, bool end)
{
	return true;
}

bool CDStarModemNull::setTX(bool on)
{
	return true;
}

unsigned int CDStarModemNull::getSpace()
{
	return 0;
}

bool CDStarModemNull::getTX()
{
	return false;
}

DSMT_TYPE CDStarModemNull::read()
{
	return DSMTT_NONE;
}

CHeaderData* CDStarModemNull::readHeader()
{
	return NULL;
}

unsigned int CDStarModemNull::readData(unsigned char* data, unsigned int length, bool& end)
{
	return 0U;
}

void CDStarModemNull::heartbeat()
{
}

void CDStarModemNull::close()
{
	wxLogMessage(wxT("Null D-Star Modem closed successfully"));
}

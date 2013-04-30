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

#include "DStarRepeaterModemNull.h"

CDStarRepeaterModemNull::CDStarRepeaterModemNull() :
m_tx(false)
{
}

CDStarRepeaterModemNull::~CDStarRepeaterModemNull()
{
}

bool CDStarRepeaterModemNull::start()
{
	wxLogMessage(wxT("Null D-Star Modem started successfully"));

	return true;
}

bool CDStarRepeaterModemNull::writeHeader(const CHeaderData& header)
{
	m_tx = true;

	return true;
}

bool CDStarRepeaterModemNull::writeData(const unsigned char* data, unsigned int length, bool end)
{
	if (end)
		m_tx = false;

	return true;
}

unsigned int CDStarRepeaterModemNull::getSpace()
{
	return 0;
}

bool CDStarRepeaterModemNull::getTX()
{
	return m_tx;
}

DSMT_TYPE CDStarRepeaterModemNull::read()
{
	return DSMTT_NONE;
}

CHeaderData* CDStarRepeaterModemNull::readHeader()
{
	return NULL;
}

unsigned int CDStarRepeaterModemNull::readData(unsigned char* data, unsigned int length, bool& end)
{
	return 0U;
}

void CDStarRepeaterModemNull::stop()
{
	wxLogMessage(wxT("Null D-Star Modem stopped successfully"));
}

/*
 *   Copyright (C) 2010 by Jonathan Naylor G4KLX
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

#include <wx/wx.h>

const unsigned int GMSK_MODEM_DATA_LENGTH = 8U;


class CGMSKModem {
public:
	CGMSKModem(unsigned int address);
	~CGMSKModem();

	bool open();

	CHeaderData* readHeader();
	int readData(unsigned char* data, unsigned int length, bool& end);

	void setPTT(bool on);

	void writeHeader(const CHeaderData& header);
	void writeData(const unsigned char* data, unsigned int length);

	void close();

private:
	unsigned int m_address;
};

#endif

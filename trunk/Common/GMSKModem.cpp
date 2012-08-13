/*
 *   Copyright (C) 2010,2011 by Jonathan Naylor G4KLX
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

#include "DStarDefines.h"
#include "GMSKModem.h"
#include "node.h"

const int VENDOR_ID = 0x04D8;


CGMSKModem::CGMSKModem(unsigned int address) :
m_address(address)
{
}


CGMSKModem::~CGMSKModem()
{
}

bool CGMSKModem::open()
{
	::node_setvid(VENDOR_ID);

	int ret = ::node_init(1, m_address);
	if (ret != 0) {
		wxLogError(wxT("Cannot find the GMSK Modem with product id: 0x%04X"), m_address);
		return false;
	}

	wxLogMessage(wxT("Found the GMSK Modem with product id: 0x%04X"), m_address);

	char version[80];
	ret = ::node_version(version, 80);
	if (ret < 0) {
		wxLogError(wxT("Unable to read the GMSK Modem version"));
		::node_close();
		return false;
	}

	wxString text(version, wxConvLocal);
	wxLogMessage(wxT("Firmware version: %s"), text.c_str());

	return true;
}

CHeaderData* CGMSKModem::readHeader()
{
	int ret = ::node_status();
	if (ret < 0) {
		wxLogError(wxT("node_status() returned %d"), ret);
		return NULL;
	}

	if ((ret & HeaderDecodeDone) == 0x00)
		return NULL;

	unsigned char header[RADIO_HEADER_LENGTH_BYTES];

	int len = ::node_gethdr(header, RADIO_HEADER_LENGTH_BYTES);
	if (len <= 0) {
		wxLogMessage(wxT("node_gethdr() returned %d"), len);
		return NULL;
	}

	ret = ::node_status();
	if (ret < 0)
		wxLogError(wxT("node_status() returned %d"), ret);

	if ((ret & CRC_ERROR) == CRC_ERROR) {
		wxLogMessage(wxT("Invalid CRC on header"));
		return NULL;
	}

	return new CHeaderData(header, RADIO_HEADER_LENGTH_BYTES, false);
}

int CGMSKModem::readData(unsigned char* data, unsigned int length, bool& end)
{
	if (length != GMSK_MODEM_DATA_LENGTH)
		return -1;

	end = false;

	int ret = ::node_getdta(data, GMSK_MODEM_DATA_LENGTH);
	if (ret < 0) {
		return -1;
	} else if (ret == 0) {
		ret = ::node_status();
		if (ret > 0 && (ret & LastFrameRead) == LastFrameRead)
			end = true;
	}

	return ret;
}

void CGMSKModem::writeHeader(const CHeaderData& header)
{
	unsigned char buffer[RADIO_HEADER_LENGTH_BYTES];
	::memset(buffer, ' ', RADIO_HEADER_LENGTH_BYTES);

	buffer[0] = header.getFlag1();
	buffer[1] = header.getFlag2();
	buffer[2] = header.getFlag3();

	wxString rpt2 = header.getRptCall1();
	for (unsigned int i = 0U; i < rpt2.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer[i + 3U]  = rpt2.GetChar(i);

	wxString rpt1 = header.getRptCall2();
	for (unsigned int i = 0U; i < rpt1.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer[i + 11U] = rpt1.GetChar(i);

	wxString your = header.getYourCall();
	for (unsigned int i = 0U; i < your.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer[i + 19U] = your.GetChar(i);

	wxString my1 = header.getMyCall1();
	for (unsigned int i = 0U; i < my1.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer[i + 27U] = my1.GetChar(i);

	wxString my2 = header.getMyCall2();
	for (unsigned int i = 0U; i < my2.Len() && i < SHORT_CALLSIGN_LENGTH; i++)
		buffer[i + 35U] = my2.GetChar(i);

	buffer[39] = 0x00;		// Empty CRC for calculation
	buffer[40] = 0x00;

	::node_puthdr(buffer, RADIO_HEADER_LENGTH_BYTES);
}

void CGMSKModem::setPTT(bool on)
{
	::node_setptt(on ? 1 : 0);
}

void CGMSKModem::writeData(const unsigned char* data, unsigned int length)
{
	wxASSERT(data != NULL);
	wxASSERT(length > 0U);

	if (length > 50) {
		wxLogError(wxT("%u bytes send to writeData"), length);
		return;
	}

	// Bodge to retain the const-ness of the data
	unsigned char buffer[50];
	::memcpy(buffer, data, length);

	::node_putdta(buffer, length);
}

void CGMSKModem::close()
{
	::node_close();
}

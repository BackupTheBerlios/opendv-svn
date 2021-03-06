/*
 *   Copyright (C) 2011-2014 by Jonathan Naylor G4KLX
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

#ifndef	DVRPTRV3Controller_H
#define	DVRPTRV3Controller_H

#include "SerialDataController.h"
#include "TCPReaderWriter.h"
#include "DStarDefines.h"
#include "RingBuffer.h"
#include "Modem.h"
#include "Utils.h"

#include <wx/wx.h>

enum RESP_TYPE_V3 {
	RT3_TIMEOUT,
	RT3_ERROR,
	RT3_UNKNOWN,
	RT3_SPACE,
	RT3_QUERY,
	RT3_CONFIG,
	RT3_HEADER,
	RT3_DATA
};

class CDVRPTRV3Controller : public CModem {
public:
	CDVRPTRV3Controller(const wxString& port, const wxString& path, bool txInvert, unsigned int modLevel, bool duplex, const wxString& callsign, unsigned int txDelay);
	CDVRPTRV3Controller(const wxString& address, unsigned int port, bool txInvert, unsigned int modLevel, bool duplex, const wxString& callsign, unsigned int txDelay);
	virtual ~CDVRPTRV3Controller();

	virtual void* Entry();

	virtual bool start();

	virtual unsigned int getSpace();

	virtual bool writeHeader(const CHeaderData& header);
	virtual bool writeData(const unsigned char* data, unsigned int length, bool end);

	virtual wxString getPath() const;

	static wxArrayString getDevices();

private:
	CONNECTION_TYPE            m_connection;
	wxString                   m_usbPort;
	wxString                   m_usbPath;
	wxString                   m_address;
	unsigned int               m_port;
	bool                       m_txInvert;
	unsigned int               m_modLevel;
	bool                       m_duplex;
	wxString                   m_callsign;
	unsigned int               m_txDelay;
	CSerialDataController*     m_usb;
	CTCPReaderWriter*          m_network;
	unsigned char*             m_buffer;
	CRingBuffer<unsigned char> m_txData;
	bool                       m_rx;

	bool readSerial();
	bool setConfig();
	bool readSpace();

	RESP_TYPE_V3 getResponse(unsigned char* buffer, unsigned int& length);

	bool findPort();
	bool findPath();

	bool findModem();
	bool openModem();

	int readModem(unsigned char* buffer, unsigned int length);
	bool writeModem(const unsigned char* buffer, unsigned int length);
	void closeModem();
};

#endif

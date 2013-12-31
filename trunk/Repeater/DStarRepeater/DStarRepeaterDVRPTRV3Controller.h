/*
 *   Copyright (C) 2011,2012,2013 by Jonathan Naylor G4KLX
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

#ifndef	DStarRepeaterDVRPTRV3Controller_H
#define	DStarRepeaterDVRPTRV3Controller_H

#include "SerialDataController.h"
#include "DStarRepeaterModem.h"
#include "TCPReaderWriter.h"
#include "DStarDefines.h"
#include "RingBuffer.h"
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

class CDStarRepeaterDVRPTRV3Controller : public wxThread, public IDStarRepeaterModem {
public:
	CDStarRepeaterDVRPTRV3Controller(const wxString& port, const wxString& path, bool txInvert, unsigned int modLevel, bool duplex, const wxString& callsign, unsigned int txDelay);
	CDStarRepeaterDVRPTRV3Controller(const wxString& address, unsigned int port, bool txInvert, unsigned int modLevel, bool duplex, const wxString& callsign, unsigned int txDelay);
	virtual ~CDStarRepeaterDVRPTRV3Controller();

	virtual void* Entry();

	virtual bool start();

	virtual unsigned int getSpace();
	virtual bool getTX();

	virtual DSMT_TYPE    read();
	virtual CHeaderData* readHeader();
	virtual unsigned int readData(unsigned char* data, unsigned int length);

	virtual bool writeHeader(const CHeaderData& header);
	virtual bool writeData(const unsigned char* data, unsigned int length, bool end);

	virtual void stop();

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
	CRingBuffer<unsigned char> m_rxData;
	CRingBuffer<unsigned char> m_txData;
	bool                       m_tx;
	bool                       m_rx;
	unsigned int               m_space;
	bool                       m_stopped;
	wxMutex                    m_mutex;
	DSMT_TYPE                  m_readType;
	unsigned int               m_readLength;
	unsigned char*             m_readBuffer;

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

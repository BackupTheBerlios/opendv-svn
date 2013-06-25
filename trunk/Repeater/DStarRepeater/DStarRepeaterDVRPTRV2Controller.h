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

#ifndef	DStarRepeaterDVRPTRV2Controller_H
#define	DStarRepeaterDVRPTRV2Controller_H

#include "SerialDataController.h"
#include "DStarRepeaterModem.h"
#include "TCPReaderWriter.h"
#include "DStarDefines.h"
#include "RingBuffer.h"
#include "Utils.h"

#include <wx/wx.h>

enum RESP_TYPE_V2 {
	RT2_TIMEOUT,
	RT2_ERROR,
	RT2_UNKNOWN,
	RT2_SPACE,
	RT2_QUERY,
	RT2_CONFIG,
	RT2_HEADER,
	RT2_DATA
};

class CDStarRepeaterDVRPTRV2Controller : public wxThread, public IDStarRepeaterModem {
public:
	CDStarRepeaterDVRPTRV2Controller(const wxString& port, const wxString& path, bool txInvert, unsigned int modLevel, bool duplex, const wxString& callsign);
	CDStarRepeaterDVRPTRV2Controller(const wxString& address, unsigned int port, bool txInvert, unsigned int modLevel, bool duplex, const wxString& callsign);
	virtual ~CDStarRepeaterDVRPTRV2Controller();

	virtual void* Entry();

	virtual bool start();

	virtual unsigned int getSpace();
	virtual bool getTX();

	virtual DSMT_TYPE    read();
	virtual CHeaderData* readHeader();
	virtual unsigned int readData(unsigned char* data, unsigned int length, bool& end);

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
	unsigned char              m_readType;
	unsigned int               m_readLength;
	unsigned char*             m_readBuffer;

	bool readSerial();
	bool setConfig();
	bool readSpace();

	RESP_TYPE_V2 getResponse(unsigned char* buffer, unsigned int& length);

	bool findPort();
	bool findPath();

	bool findModem();
	bool openModem();

	int readModem(unsigned char* buffer, unsigned int length);
	bool writeModem(const unsigned char* buffer, unsigned int length);
	void closeModem();
};

#endif
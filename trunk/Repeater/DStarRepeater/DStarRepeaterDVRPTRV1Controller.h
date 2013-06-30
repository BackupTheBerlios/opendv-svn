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

#ifndef	DStarRepeaterDVRPTRV1Controller_H
#define	DStarRepeaterDVRPTRV1Controller_H

#include "SerialDataController.h"
#include "DStarRepeaterModem.h"
#include "RingBuffer.h"
#include "Utils.h"

#include <wx/wx.h>

enum RESP_TYPE_V1 {
	RT1_TIMEOUT,
	RT1_ERROR,
	RT1_UNKNOWN,
	RT1_GET_STATUS,
	RT1_GET_VERSION,
	RT1_GET_SERIAL,
	RT1_GET_CONFIG,
	RT1_SET_CONFIG,
	RT1_RXPREAMBLE,
	RT1_START,
	RT1_HEADER,
	RT1_RXSYNC,
	RT1_DATA,
	RT1_EOT,
	RT1_RXLOST,
	RT1_SET_TESTMDE,
	RT1_DEBUG
};

class CDStarRepeaterDVRPTRV1Controller : public wxThread, public IDStarRepeaterModem {
public:
	CDStarRepeaterDVRPTRV1Controller(const wxString& port, const wxString& path, bool delay, bool rxInvert, bool txInvert, bool channel, unsigned int modLevel, unsigned int txDelay);
	virtual ~CDStarRepeaterDVRPTRV1Controller();

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
	wxString                   m_port;
	wxString                   m_path;
	bool                       m_delay;
	bool                       m_rxInvert;
	bool                       m_txInvert;
	bool                       m_channel;
	unsigned int               m_modLevel;
	unsigned int               m_txDelay;
	CSerialDataController      m_serial;
	unsigned char*             m_buffer;
	CRingBuffer<unsigned char> m_rxData;
	CRingBuffer<unsigned char> m_txData;
	unsigned char              m_txCounter;
	unsigned char              m_pktCounter;
	bool                       m_tx;
	bool                       m_rx;
	unsigned int               m_txSpace;
	bool                       m_txEnabled;
	bool                       m_checksum;
	unsigned int               m_space;
	bool                       m_stopped;
	wxMutex                    m_mutex;
	DSMT_TYPE                  m_readType;
	unsigned int               m_readLength;
	unsigned char*             m_readBuffer;

	bool readVersion();
	bool readStatus();
	bool setConfig();
	bool setEnabled(bool enable);

	RESP_TYPE_V1 getResponse(unsigned char* buffer, unsigned int& length);

	bool findPort();
	bool findPath();

	bool findModem();
	bool openModem();
};

#endif


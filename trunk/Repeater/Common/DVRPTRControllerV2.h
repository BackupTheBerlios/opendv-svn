/*
 *   Copyright (C) 2011,2012 by Jonathan Naylor G4KLX
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

#ifndef	DVRPTRControllerV2_H
#define	DVRPTRControllerV2_H

#include "SerialDataController.h"
#include "DVRPTRController.h"
#include "RingBuffer.h"
#include "Utils.h"

#include <wx/wx.h>

enum RESP_TYPE_V2 {
	RT2_TIMEOUT,
	RT2_ERROR,
	RT2_UNKNOWN,
	RT2_GET_STATUS,
	RT2_GET_VERSION,
	RT2_GET_SERIAL,
	RT2_GET_CONFIG,
	RT2_SET_CONFIG,
	RT2_RXPREAMBLE,
	RT2_START,
	RT2_HEADER,
	RT2_RXSYNC,
	RT2_DATA,
	RT2_EOT,
	RT2_RXLOST,
	RT2_SET_TESTMDE
};

class CDVRPTRControllerV2 : public IDVRPTRController, public wxThread {
public:
	CDVRPTRControllerV2(const wxString& port, const wxString& path, bool rxInvert, bool txInvert, bool channel, unsigned int modLevel, unsigned int txDelay);
	virtual ~CDVRPTRControllerV2();

	virtual void* Entry();

	virtual bool open();

	virtual bool getPTT() const;

	virtual bool hasSpace();

	virtual void purgeRX();
	virtual void purgeTX();

	virtual DATA_QUEUE_TYPE readQueue(unsigned char* data, unsigned int& length);

	virtual bool writeStart();
	virtual bool writeHeader(const CHeaderData& header);
	virtual bool writeData(const unsigned char* data, unsigned int length);
	virtual bool writeEnd();

	virtual void close();

	virtual wxString getPath() const;

	static wxArrayString getDevices();

private:
	wxString                   m_port;
	wxString                   m_path;
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
	bool                       m_ptt;
	bool                       m_rx;
	unsigned int               m_txSpace;
	bool                       m_txEnabled;
	bool                       m_checksum;
	unsigned int               m_space;
	bool                       m_stopped;
	wxMutex                    m_mutex;

	bool getVersion();
	bool getStatus();
	bool setConfig();
	bool setEnabled(bool enable);

	RESP_TYPE_V2 getResponse(unsigned char* buffer, unsigned int& length);

	bool findPort();
	bool findPath();

	bool findModem();
	bool openModem();
};

#endif


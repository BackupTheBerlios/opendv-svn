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

#ifndef	DVRPTRController_H
#define	DVRPTRController_H

#include "SerialDataController.h"
#include "RingBuffer.h"
#include "HeaderData.h"
#include "Utils.h"

#include <wx/wx.h>

enum RESP_TYPE {
	RT_TIMEOUT,
	RT_ERROR,
	RT_UNKNOWN,
	RT_GET_STATUS,
	RT_GET_VERSION,
	RT_GET_SERIAL,
	RT_GET_CONFIG,
	RT_SET_CONFIG,
	RT_RXPREAMBLE,
	RT_START,
	RT_HEADER,
	RT_RXSYNC,
	RT_DATA,
	RT_EOT,
	RT_RXLOST,
	RT_SET_TESTMDE
};

enum DATA_QUEUE_TYPE {
	DQT_NONE,
	DQT_HEADER,
	DQT_DATA,
	DQT_EOT,
	DQT_LOST,
	DQT_START
};

class CDVRPTRController : public wxThread {
public:
	CDVRPTRController(const wxString& port, bool rxInvert, bool txInvert, bool channel, unsigned int modLevel, unsigned int txDelay);
	virtual ~CDVRPTRController();

	virtual void* Entry();

	virtual bool open();

	virtual bool getPTT() const;

	virtual bool hasSpace();

	virtual DATA_QUEUE_TYPE readQueue(unsigned char* data, unsigned int& length);

	virtual bool writeStart();
	virtual bool writeHeader(const CHeaderData& header);
	virtual bool writeData(const unsigned char* data, unsigned int length);
	virtual bool writeEnd();

	virtual void close();

	static wxArrayString getDevices();

private:
	wxString                   m_port;
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
	unsigned int               m_txSpace;
	bool                       m_txEnabled;
	bool                       m_checksum;
	unsigned int               m_space;
	bool                       m_stopped;
	wxMutex                    m_mutex;

	bool getVersion();
	void getStatus();
	bool setConfig();
	bool setEnabled(bool enable);

	RESP_TYPE getResponse(unsigned char* buffer, unsigned int& length);
};

#endif


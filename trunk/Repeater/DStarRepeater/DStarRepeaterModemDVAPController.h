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

#ifndef	DStarRepeaterModemDVAPController_H
#define	DStarRepeaterModemDVAPController_H

#include "SerialDataController.h"
#include "DStarRepeaterModem.h"
#include "RingBuffer.h"
#include "HeaderData.h"
#include "Utils.h"

#include <wx/wx.h>

enum RESP_TYPE {
	RT_TIMEOUT,
	RT_ERROR,
	RT_UNKNOWN,
	RT_NAME,
	RT_SERIAL,
	RT_FIRMWARE,
	RT_START,
	RT_STOP,
	RT_MODULATION,
	RT_MODE,
	RT_SQUELCH,
	RT_POWER,
	RT_FREQUENCY,
	RT_FREQLIMITS,
	RT_STATE,
	RT_PTT,
	RT_ACK,
	RT_HEADER,
	RT_HEADER_ACK,
	RT_GMSK_DATA,
	RT_FM_DATA
};

class CDStarRepeaterModemDVAPController : public wxThread, public IDStarRepeaterModem {
public:
	CDStarRepeaterModemDVAPController(const wxString& port, unsigned int frequency, int power, int squelch);
	virtual ~CDStarRepeaterModemDVAPController();

	virtual void* Entry();

	virtual bool start();

	virtual DSMT_TYPE    read();
	virtual CHeaderData* readHeader();
	virtual unsigned int readData(unsigned char* data, unsigned int length, bool& end);

	virtual unsigned int getSpace();

	virtual bool writeHeader(const CHeaderData& header);
	virtual bool writeData(const unsigned char* data, unsigned int length, bool end);

	virtual void stop();

private:
	CSerialDataController      m_serial;
	wxUint32                   m_frequency;
	wxInt16                    m_power;
	wxInt8                     m_squelch;
	unsigned int               m_space;
	bool                       m_rx;
	bool                       m_tx;
	unsigned char*             m_buffer;
	wxUint16                   m_streamId;
	wxUint8                    m_framePos;
	wxUint8                    m_seq;
	CRingBuffer<unsigned char> m_rxData;
	CRingBuffer<unsigned char> m_txData;
	bool                       m_stopped;
	wxMutex                    m_mutex;
	unsigned int               m_count;
	bool                       m_accepted;
	unsigned char              m_readType;
	unsigned int               m_readLength;
	unsigned char*             m_readBuffer;

	bool getName();
	bool getFirmware();
	bool getSerial();

	bool setModulation();
	bool setMode();

	bool setSquelch();
	bool setPower();
	bool setFrequency();

	bool startDVAP();
	bool stopDVAP();

	void resync();

	RESP_TYPE getResponse(unsigned char* buffer, unsigned int& length);
};

#endif

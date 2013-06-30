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

#ifndef	DStarRepeaterGMSKController_H
#define	DStarRepeaterGMSKController_H

#include "DStarRepeaterModem.h"
#include "DStarDefines.h"
#include "RingBuffer.h"
#include "GMSKModem.h"
#include "Utils.h"

#include <wx/wx.h>

class CDStarRepeaterGMSKController : public wxThread, public IDStarRepeaterModem {
public:
	CDStarRepeaterGMSKController(USB_INTERFACE iface, unsigned int address);
	virtual ~CDStarRepeaterGMSKController();

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

private:
	IGMSKModem*                m_modem;
	unsigned char*             m_buffer;
	CRingBuffer<unsigned char> m_rxData;
	CRingBuffer<unsigned char> m_txData;
	bool                       m_tx;
	unsigned int               m_space;
	bool                       m_stopped;
	wxMutex                    m_mutex;
	DSMT_TYPE                  m_readType;
	unsigned int               m_readLength;
	unsigned char*             m_readBuffer;
};

#endif


/*
 *   Copyright (C) 2010-2013 by Jonathan Naylor G4KLX
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

#ifndef	DongleHandler_H
#define	DongleHandler_H

#include "DongleProtocolHandler.h"
#include "ReflectorCallback.h"
#include "DStarDefines.h"
#include "HeaderLogger.h"
#include "HeaderData.h"
#include "AMBEData.h"
#include "PollData.h"
#include "Timer.h"
#include "Defs.h"

#if defined(__WINDOWS__)
#include "Inaddr.h"
#else
#include <netinet/in.h>
#endif

#include <wx/wx.h>
#include <wx/ffile.h>

class CDongleHandler {
public:
	static void initialise(unsigned int maxReflectors);

	static void setCallsign(const wxString& callsign);
	static void setDongleProtocolHandler(CDongleProtocolHandler* handler);
	static void setHeaderLogger(CHeaderLogger* logger);
	static void setMaxDongles(unsigned int maxDongles);

	static void writeHeader(IReflectorCallback* handler, CHeaderData& header);
	static void writeAMBE(IReflectorCallback* handler, CAMBEData& data);

	static void process(CHeaderData& header);
	static void process(CAMBEData& data);
	static void process(const CPollData& poll);

	static void clock(unsigned int ms);

	static bool stateChange();
	static void writeStatus(wxFFile& file);

	static void finalise();

	static void getInfo(IReflectorCallback* handler, CRemoteRepeaterData& data);

	static wxString getDongles();

protected:
	CDongleHandler(const wxString& reflector, const in_addr& address, unsigned int port);
	~CDongleHandler();

	void processInt(CHeaderData& header);
	void processInt(CAMBEData& data);

	void writeHeaderInt(IReflectorCallback* handler, CHeaderData& header);
	void writeAMBEInt(IReflectorCallback* handler, CAMBEData& data);

	bool clockInt(unsigned int ms);

private:
	static unsigned int            m_maxReflectors;
	static unsigned int            m_maxDongles;
	static CDongleHandler**        m_reflectors;

	static wxString                m_callsign;
	static CDongleProtocolHandler* m_handler;

	static bool                    m_stateChange;

	static CHeaderLogger*          m_headerLogger;

	wxString            m_reflector;
	in_addr             m_yourAddress;
	unsigned int        m_yourPort;
	IReflectorCallback* m_destination;
	time_t              m_time;
	CTimer              m_pollTimer;
	CTimer              m_pollInactivityTimer;
	unsigned int        m_dExtraId;
	unsigned int        m_dExtraSeq;
	CTimer              m_inactivityTimer;
	CHeaderData*        m_header;
};

#endif

/*
 *   Copyright (C) 2013 by Jonathan Naylor G4KLX
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

#ifndef	CCSHandler_H
#define	CCSHandler_H

#include "CCSProtocolHandler.h"
#include "ReflectorCallback.h"
#include "DStarDefines.h"
#include "HeaderLogger.h"
#include "ConnectData.h"
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

class CCCSHandler {
public:
	static void initialise();

	static void setCCSProtocolHandler(CCCSProtocolHandler* handler);
	static void setHeaderLogger(CHeaderLogger* logger);
	static void setCallsign(const wxString& callsign);

	static bool link();
	static void unlink();

	static void writeHeader(IReflectorCallback* handler, CHeaderData& header, DIRECTION direction);
	static void writeAMBE(IReflectorCallback* handler, CAMBEData& data, DIRECTION direction);

	static void process(CAMBEData& header);
	static void process(CPollData& data);
	static void process(CConnectData& connect);

	static void clock(unsigned int ms);

	static void finalise();

	static CCS_STATUS getState();

private:
	static CCS_STATUS               m_state;

	static wxString                 m_callsign;

	static CCCSProtocolHandler*     m_handler;

	static in_addr                  m_address;

	static CHeaderLogger*           m_headerLogger;

	static CTimer                   m_pollInactivityTimer;
	static CTimer                   m_pollTimer;
	static CTimer                   m_tryTimer;
	static unsigned int             m_tryCount;

	static unsigned int calcBackoff();
};

#endif

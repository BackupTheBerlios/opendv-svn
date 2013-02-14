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
#include "RepeaterCallback.h"
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
	CCCSHandler(IRepeaterCallback* handler, const wxString& callsign, double latitude, double longitude, unsigned int localPort);
	~CCCSHandler();

	bool connect();

	void writeHeard(const CHeaderData& data);
	void writeEnd();
	void writeHeader(CHeaderData& header);
	void writeAMBE(CAMBEData& data, const wxString& dtmf = wxEmptyString);

	void setReflector(const wxString& callsign = wxEmptyString);

	static void disconnect();

	static void initialise(unsigned int count);

	static void process();

	static void clock(unsigned int ms);

	static void setHeaderLogger(CHeaderLogger* logger);

	static void setLocalAddress(const wxString& address);

	static void finalise();

protected:
	void clockInt(unsigned int ms);

	void processInt();

	void disconnectInt();

private:
	static CCCSHandler**  m_handlers;
	static unsigned int   m_count;

	static wxString       m_localAddress;
	static CHeaderLogger* m_headerLogger;

	IRepeaterCallback*  m_handler;
	wxString            m_callsign;
	wxString            m_reflector;
	wxString            m_locator;
	in_addr             m_ccsAddress;
	CCCSProtocolHandler m_protocol;
	CCS_STATUS          m_state;
	wxString            m_local;
	CTimer              m_inactivityTimer;
	CTimer              m_pollInactivityTimer;
	CTimer              m_pollTimer;
	CTimer              m_tryTimer;
	unsigned int        m_tryCount;
	unsigned int        m_id;
	unsigned int        m_seqNo;
	wxString            m_yourCall;
	wxString            m_myCall1;
	wxString            m_myCall2;

	void process(CAMBEData& header);
	void process(CPollData& data);
	void process(CConnectData& connect);
	void process(CCCSData& data);

	unsigned int calcBackoff();
};

#endif

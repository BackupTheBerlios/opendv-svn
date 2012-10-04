/*
 *   Copyright (C) 2012 by Jonathan Naylor G4KLX
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

#ifndef	DCSHander_H
#define	DCSHander_H

#include "DCSProtocolHandler.h"
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

enum DCS_STATE {
	DCS_LINKING,
	DCS_LINKED,
	DCS_UNLINKING
};

class CDCSHandler {
public:
	static void initialise(unsigned int maxReflectors);

	static void setCallsign(const wxString& callsign);
	static void setDCSProtocolHandler(CDCSProtocolHandler* handler);
	static void setHeaderLogger(CHeaderLogger* logger);

	static void link(IReflectorCallback* handler, const wxString& repeater, const wxString& reflector, const in_addr& address);
	static void unlink(IReflectorCallback* handler, const wxString& exclude = wxEmptyString);
	static void unlink();

	static void writeHeader(const wxString& callsign, CHeaderData& header, DIRECTION direction);
	static void writeAMBE(CAMBEData& data, DIRECTION direction);

	static void process(CAMBEData& header);
	static void process(CPollData& data);
	static void process(CConnectData& connect);

	static void gatewayUpdate(const wxString& reflector, const wxString& address);
	static void clock(unsigned int ms);

	static bool stateChange();
	static void writeStatus(wxFFile& file);

	static void finalise();

	static void getInfo(IReflectorCallback* handler, CRemoteRepeaterData& data);

	static wxString getIncoming(const wxString& callsign);

protected:
	CDCSHandler(IReflectorCallback* handler, const wxString& reflector, const wxString& repeater, const in_addr& address, unsigned int port, DIRECTION direction);
	CDCSHandler(const wxString& reflector, const in_addr& address, unsigned int port, DIRECTION direction);
	~CDCSHandler();

	void processInt(CAMBEData& data);
	bool processInt(CConnectData& connect, CD_TYPE type);

	void writeHeaderInt(const wxString& callsign, CHeaderData& header, DIRECTION direction);
	void writeAMBEInt(CAMBEData& data, DIRECTION direction);

	bool clockInt(unsigned int ms);

private:
	static unsigned int         m_maxReflectors;
	static CDCSHandler**        m_reflectors;

	static wxString             m_callsign;
	static CDCSProtocolHandler* m_handler;

	static bool                 m_stateChange;

	static CHeaderLogger*       m_headerLogger;

	wxString            m_reflector;
	wxString            m_repeater;
	in_addr             m_address;
	unsigned int        m_port;
	DIRECTION           m_direction;
	DCS_STATE           m_linkState;
	IReflectorCallback* m_destination;
	time_t              m_time;
	CTimer              m_pollTimer;
	CTimer              m_pollInactivityTimer;
	CTimer              m_tryTimer;
	unsigned int        m_dcsId;
	unsigned int        m_dcsSeq;
	unsigned int        m_rptrId;
	unsigned int        m_seqNo;
	CTimer              m_inactivityTimer;

	// Header data
	wxString            m_yourCall;
	wxString            m_myCall1;
	wxString            m_myCall2;
	wxString            m_rptCall1;
	wxString            m_rptCall2;
};

#endif

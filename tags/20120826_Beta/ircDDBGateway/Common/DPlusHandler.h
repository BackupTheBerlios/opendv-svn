/*
 *   Copyright (C) 2010,2011,2012 by Jonathan Naylor G4KLX
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

#ifndef	DPlusHandler_H
#define	DPlusHandler_H

#include "DPlusProtocolHandler.h"
#include "DPlusAuthenticator.h"
#include "ReflectorCallback.h"
#include "CacheManager.h"
#include "DStarDefines.h"
#include "HeaderLogger.h"
#include "ConnectData.h"
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

enum DPLUS_STATE {
	DPLUS_LINKING,
	DPLUS_LINKED,
	DPLUS_UNLINKING
};

class CDPlusHandler {
public:
	static void initialise(unsigned int maxReflectors);

	static void setCallsign(const wxString& callsign);
	static void setDPlusProtocolHandler(CDPlusProtocolHandler* handler);
	static void setDPlusLogin(const wxString& dplusLogin);
	static void setHeaderLogger(CHeaderLogger* logger);
	static void setMaxDongles(unsigned int maxDongles);

	static void startAuthenticator(const wxString& address, CCacheManager* cache);

	static void link(IReflectorCallback* handler, const wxString& repeater, const wxString& reflector, const in_addr& address);
	static void relink(IReflectorCallback* handler, const wxString& reflector);
	static void unlink(IReflectorCallback* handler, const wxString& exclude = wxEmptyString);
	static void unlink();

	static void writeHeader(IReflectorCallback* handler, CHeaderData& header, DIRECTION direction);
	static void writeAMBE(CAMBEData& data, DIRECTION direction);

	static void process(CHeaderData& header);
	static void process(CAMBEData& header);
	static void process(const CPollData& header);
	static void process(CConnectData& process);

	static void gatewayUpdate(const wxString& gateway, const wxString& address);
	static void clock(unsigned int ms);

	static bool stateChange();
	static void writeStatus(wxFFile& file);

	static void finalise();

	static void getInfo(IReflectorCallback* handler, CRemoteRepeaterData& data);

	static wxString getDongles();

protected:
	CDPlusHandler(IReflectorCallback* handler, const wxString& repeater, const wxString& reflector, const in_addr& address);
	CDPlusHandler(const in_addr& address, unsigned int port);
	~CDPlusHandler();

	void processInt(CHeaderData& header);
	void processInt(CAMBEData& data);
	bool processInt(CConnectData& connect, CD_TYPE type);

	void writeHeaderInt(IReflectorCallback* handler, CHeaderData& header, DIRECTION direction);
	void writeAMBEInt(CAMBEData& data, DIRECTION direction);

	bool clockInt(unsigned int ms);

private:
	static unsigned int           m_maxReflectors;
	static unsigned int           m_maxDongles;
	static CDPlusHandler**        m_reflectors;

	static wxString               m_gatewayCallsign;
	static wxString               m_dplusLogin;
	static CDPlusProtocolHandler* m_handler;

	static bool                   m_stateChange;

	static CHeaderLogger*         m_headerLogger;
	static CDPlusAuthenticator*   m_authenticator;

	wxString            m_repeater;
	wxString            m_callsign;
	wxString            m_reflector;
	in_addr             m_address;
	unsigned int        m_port;
	DIRECTION           m_direction;
	DPLUS_STATE         m_linkState;
	IReflectorCallback* m_destination;
	time_t              m_time;
	CTimer              m_pollTimer;
	CTimer              m_pollInactivityTimer;
	unsigned int        m_dPlusId;
	unsigned int        m_dPlusSeq;
	unsigned int        m_rptrId;
	CTimer              m_inactivityTimer;
	CHeaderData*        m_header;
};

#endif

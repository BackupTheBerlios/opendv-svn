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

#ifndef	SplitRepeaterThread_H
#define	SplitRepeaterThread_H

#include "SplitRepeaterProtocolHandler.h"
#include "SplitRepeaterStatusData.h"
#include "SplitRepeaterHeaderData.h"
#include "SplitRepeaterAMBEData.h"
#include "SlowDataEncoder.h"
#include "BeaconCallback.h"
#include "DStarDefines.h"
#include "CallsignList.h"
#include "BeaconUnit.h"
#include "Timer.h"
#include "Utils.h"

#include <wx/wx.h>
#include <wx/regex.h>

class CSplitRepeaterThread : public IBeaconCallback, public wxThread {
public:
	CSplitRepeaterThread();
	virtual ~CSplitRepeaterThread();

	virtual void setCallsign(const wxString& callsign, const wxString& gateway, DSTAR_MODE mode, ACK_TYPE ack, bool restriction, bool rpt1Validation);
	virtual void setProtocolHandler(CSplitRepeaterProtocolHandler* hander);
	virtual bool setGateway(const wxString& address, unsigned int port);
	virtual bool setReceiver1(const wxString& address, unsigned int port);
	virtual bool setReceiver2(const wxString& address, unsigned int port);
	virtual bool setTransmitter1(const wxString& address, unsigned int port);
	virtual bool setTransmitter2(const wxString& address, unsigned int port);
	virtual void setTimes(unsigned int timeout, unsigned int ackTime);
	virtual void setBeacon(unsigned int time, const wxString& text, bool voice, TEXT_LANG language);
	virtual void setControl(bool enabled, const wxString& rpt1Callsign, const wxString& rpt2Callsign, const wxString& shutdown, const wxString& startup, const wxString& status1, const wxString& status2, const wxString& status3, const wxString& status4, const wxString& status5, const wxString& command1, const wxString& command1Line, const wxString& command2, const wxString& command2Line, const wxString& command3, const wxString& command3Line, const wxString& command4, const wxString& command4Line);
	virtual void setWhiteList(CCallsignList* list);
	virtual void setBlackList(CCallsignList* list);

	virtual void shutdown();
	virtual void startup();

	virtual void command1();
	virtual void command2();
	virtual void command3();
	virtual void command4();

	virtual CSplitRepeaterStatusData* getStatus();

	virtual void  start();
	virtual void* Entry();
	virtual void  kill();
	virtual void  wait();

	virtual void transmitBeaconHeader();
	virtual void transmitBeaconData(const unsigned char* data, unsigned int length, bool end);

private:
	CSplitRepeaterProtocolHandler* m_protocolHandler;
	in_addr                        m_gatewayAddress;
	unsigned int                   m_gatewayPort;
	in_addr                    m_receiver1Address;
	unsigned int               m_receiver1Port;
	in_addr                    m_receiver2Address;
	unsigned int               m_receiver2Port;
	in_addr                    m_transmitter1Address;
	unsigned int               m_transmitter1Port;
	in_addr                    m_transmitter2Address;
	unsigned int               m_transmitter2Port;
	unsigned int               m_radioId;
	unsigned int               m_networkId;
	unsigned int               m_receiver1Id;
	unsigned int               m_receiver2Id;
	CSplitRepeaterHeaderData*  m_receiver1Header;
	CSplitRepeaterHeaderData*  m_receiver2Header;
	CSplitRepeaterAMBEData**   m_receiver1Data;
	CSplitRepeaterAMBEData**   m_receiver2Data;
	bool                       m_stopped;
	wxString                   m_rptCallsign;
	wxString                   m_gwyCallsign;
	wxString                   m_dmyCallsign;
	CBeaconUnit*               m_beacon;
	unsigned int               m_beaconId;
	unsigned int               m_beaconSeq;
	CSplitRepeaterHeaderData*  m_rxHeader;
	CSplitRepeaterHeaderData*  m_txHeader;
	unsigned char              m_radioSeqNo;
	CTimer                     m_timeoutTimer;
	CTimer                     m_radio1WatchdogTimer;
	CTimer                     m_radio2WatchdogTimer;
	CTimer                     m_networkWatchdogTimer;
	CTimer                     m_pollTimer;
	CTimer                     m_ackTimer;
	CTimer                     m_status1Timer;
	CTimer                     m_status2Timer;
	CTimer                     m_status3Timer;
	CTimer                     m_status4Timer;
	CTimer                     m_status5Timer;
	CTimer                     m_beaconTimer;
	DSTAR_RPT_STATE            m_state;
	CSlowDataEncoder           m_ackEncoder;
	CSlowDataEncoder           m_linkEncoder;
	CSlowDataEncoder           m_status1Encoder;
	CSlowDataEncoder           m_status2Encoder;
	CSlowDataEncoder           m_status3Encoder;
	CSlowDataEncoder           m_status4Encoder;
	CSlowDataEncoder           m_status5Encoder;
	bool                       m_killed;
	DSTAR_MODE                 m_mode;
	ACK_TYPE                   m_ack;
	bool                       m_restriction;
	bool                       m_rpt1Validation;
	bool                       m_controlEnabled;
	wxString                   m_controlRPT1;
	wxString                   m_controlRPT2;
	wxString                   m_controlShutdown;
	wxString                   m_controlStartup;
	wxString                   m_controlStatus1;
	wxString                   m_controlStatus2;
	wxString                   m_controlStatus3;
	wxString                   m_controlStatus4;
	wxString                   m_controlStatus5;
	wxString                   m_controlCommand1;
	wxString                   m_controlCommand1Line;
	wxString                   m_controlCommand2;
	wxString                   m_controlCommand2Line;
	wxString                   m_controlCommand3;
	wxString                   m_controlCommand3Line;
	wxString                   m_controlCommand4;
	wxString                   m_controlCommand4Line;
	bool                       m_shutdown;
	unsigned int               m_ambeFrames;
	unsigned int               m_ambeSilence;
	unsigned int               m_ambeBits;
	unsigned int               m_ambeErrors;
	unsigned int               m_lastAMBEBits;
	unsigned int               m_lastAMBEErrors;
	wxString                   m_ackText;
	LINK_STATUS                m_linkStatus;
	wxString                   m_reflector;
	wxString                   m_status1Text;
	wxString                   m_status2Text;
	wxString                   m_status3Text;
	wxString                   m_status4Text;
	wxString                   m_status5Text;
	wxRegEx                    m_regEx;
	CCallsignList*             m_whiteList;
	CCallsignList*             m_blackList;
	bool                       m_busyData;
	wxStopWatch                m_packetTime;
	unsigned int               m_totalPackets;
	unsigned int               m_receiver1Packets;
	unsigned int               m_receiver2Packets;
	unsigned int               m_receiver1Choice;
	unsigned int               m_receiver2Choice;
	unsigned int               m_receiver1Rejected;
	unsigned int               m_receiver2Rejected;

	void processNetworkHeader(CSplitRepeaterHeaderData* header);
	void processNetworkFrame(CSplitRepeaterAMBEData* data);

	void processReceiver1Frame(CSplitRepeaterAMBEData* data);
	void processReceiver2Frame(CSplitRepeaterAMBEData* data);

	void processReceiver1Header(CSplitRepeaterHeaderData* header);
	void processReceiver2Header(CSplitRepeaterHeaderData* header);

	bool processRadioHeader(CSplitRepeaterHeaderData* header);
	void processRadioHeader();

	void processRadioFrame(CSplitRepeaterAMBEData* data);

	void chooseReceiver();

	void transmitHeader(CSplitRepeaterHeaderData* header);
	void transmitFrame(CSplitRepeaterAMBEData* data);
	void transmitStatus();
	void transmitErrorStatus();
	void transmitUserStatus(unsigned int n);

	void repeaterStateMachine();
	void receiveNetwork();
	void endOfRadioData();
	bool setRepeaterState(DSTAR_RPT_STATE state);
	bool checkControl(const CSplitRepeaterHeaderData& header);
	TRISTATE checkHeader(CSplitRepeaterHeaderData& header);
	void clock(unsigned long ms);
	void clearQueue(unsigned int n);
};

#endif

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

#ifndef	DVRPTRRepeaterTRXThread_H
#define	DVRPTRRepeaterTRXThread_H

#include "DVRPTRRepeaterStatusData.h"
#include "RepeaterProtocolHandler.h"
#include "DVRPTRRepeaterThread.h"
#include "DVRPTRRepeaterDefs.h"
#include "DVRPTRController.h"
#include "DVTOOLFileWriter.h"
#include "SlowDataDecoder.h"
#include "SlowDataEncoder.h"
#include "BeaconCallback.h"
#include "CallsignList.h"
#include "OutputQueue.h"
#include "BeaconUnit.h"
#include "HeaderData.h"
#include "AMBEFEC.h"
#include "Timer.h"

#include <wx/wx.h>
#include <wx/regex.h>

class CDVRPTRRepeaterTRXThread : public IDVRPTRRepeaterThread, public IBeaconCallback, public wxThread {
public:
	CDVRPTRRepeaterTRXThread();
	virtual ~CDVRPTRRepeaterTRXThread();

	virtual void setCallsign(const wxString& callsign, const wxString& gateway, DSTAR_MODE mode, ACK_TYPE ack, bool restriction, bool rpt1Validation);
	virtual void setProtocolHandler(CRepeaterProtocolHandler* handler);
	virtual void setModem(CDVRPTRController* modem);
	virtual void setController(CExternalController* controller, unsigned int activeHangTime);
	virtual void setTimes(unsigned int timeout, unsigned int ackTime);
	virtual void setBeacon(unsigned int time, const wxString& text, bool voice, TEXT_LANG language);
	virtual void setControl(bool enabled, const wxString& rpt1Callsign, const wxString& rpt2Callsign, const wxString& shutdown, const wxString& startup, const wxString& status1, const wxString& status2, const wxString& status3, const wxString& status4, const wxString& status5, const wxString& command1, const wxString& command1Line, const wxString& command2, const wxString& command2Line, const wxString& command3, const wxString& command3Line, const wxString& command4, const wxString& command4Line, const wxString& output1, const wxString& output2, const wxString& output3, const wxString& output4);
	virtual void setOutputs(bool out1, bool out2, bool out3, bool out4);
	virtual void setLogging(bool logging, const wxString& dir);
	virtual void setWhiteList(CCallsignList* list);
	virtual void setBlackList(CCallsignList* list);

	virtual void shutdown();
	virtual void startup();

	virtual void command1();
	virtual void command2();
	virtual void command3();
	virtual void command4();

	virtual CDVRPTRRepeaterStatusData* getStatus();

	virtual void  start();
	virtual void* Entry();
	virtual void  kill();
	virtual void  wait();

	virtual void transmitBeaconHeader();
	virtual void transmitBeaconData(const unsigned char* data, unsigned int length, bool end);

private:
	CDVRPTRController*         m_dvrptr;
	CRepeaterProtocolHandler*  m_protocolHandler;
	CExternalController*       m_controller;
	bool                       m_stopped;
	wxString                   m_rptCallsign;
	wxString                   m_gwyCallsign;
	wxString                   m_dmyCallsign;
	CBeaconUnit*               m_beacon;
	CHeaderData*               m_rxHeader;
	COutputQueue               m_localQueue;
	COutputQueue               m_radioQueue;
	COutputQueue**             m_networkQueue;
	unsigned int               m_writeNum;
	unsigned int               m_readNum;
	unsigned char              m_radioSeqNo;
	unsigned char              m_networkSeqNo;
	CTimer                     m_timeoutTimer;
	CTimer                     m_watchdogTimer;
	CTimer                     m_pollTimer;
	CTimer                     m_ackTimer;
	CTimer                     m_status1Timer;
	CTimer                     m_status2Timer;
	CTimer                     m_status3Timer;
	CTimer                     m_status4Timer;
	CTimer                     m_status5Timer;
	CTimer                     m_beaconTimer;
	DSTAR_RPT_STATE            m_rptState;
	DSTAR_RX_STATE             m_rxState;
	CSlowDataDecoder           m_slowDataDecoder;
	CSlowDataEncoder           m_ackEncoder;
	CSlowDataEncoder           m_linkEncoder;
	CSlowDataEncoder           m_headerEncoder;
	CSlowDataEncoder           m_status1Encoder;
	CSlowDataEncoder           m_status2Encoder;
	CSlowDataEncoder           m_status3Encoder;
	CSlowDataEncoder           m_status4Encoder;
	CSlowDataEncoder           m_status5Encoder;
	bool                       m_tx;
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
	wxString                   m_controlOutput1;
	wxString                   m_controlOutput2;
	wxString                   m_controlOutput3;
	wxString                   m_controlOutput4;
	bool                       m_output1;
	bool                       m_output2;
	bool                       m_output3;
	bool                       m_output4;
	CTimer                     m_activeHangTimer;
	bool                       m_shutdown;
	bool                       m_disable;
	CDVTOOLFileWriter*         m_logging;
	unsigned char*             m_lastData;
	CAMBEFEC                   m_ambe;
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
	wxStopWatch                m_headerTime;
	wxStopWatch                m_packetTime;
	unsigned int               m_packetCount;
	unsigned int               m_packetSilence;
	CCallsignList*             m_whiteList;
	CCallsignList*             m_blackList;
	bool                       m_busyData;

	void receiveHeader(unsigned char* data, unsigned int length);
	void receiveRadioData(unsigned char* data, unsigned int length);
	void receiveSlowData(unsigned char* data, unsigned int length);
	void transmitRadioHeader(CHeaderData* header);
	void transmitLocalHeader(CHeaderData* header);
	void transmitNetworkHeader(CHeaderData* header);
	void transmitStatus();
	void transmitErrorStatus();
	void transmitUserStatus(unsigned int n);
	void transmitLocalData();
	void transmitRadioData();
	void transmitNetworkData();
	void transmitLocalHeader();
	void transmitRadioHeader();
	void transmitNetworkHeader();

	void repeaterStateMachine();
	void receiveRadio();
	void receiveNetwork();
	bool processRadioHeader(CHeaderData* header);
	void processNetworkHeader(CHeaderData* header);
	void processRadioFrame(unsigned char* data, FRAME_TYPE type);
	unsigned int processNetworkFrame(unsigned char* data, unsigned int length, unsigned char seqNo);
	void endOfRadioData();
	void endOfNetworkData();
	void setRadioState(DSTAR_RX_STATE state);
	bool setRepeaterState(DSTAR_RPT_STATE state);
	bool checkControl(const CHeaderData& header);
	TRISTATE checkHeader(CHeaderData& header);
	unsigned int countBits(unsigned char byte);
	void clock(unsigned int ms);
	void blankDTMF(unsigned char* data);
};

#endif

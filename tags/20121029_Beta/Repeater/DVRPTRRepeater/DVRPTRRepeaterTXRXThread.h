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

#ifndef	DVRPTRRepeaterTXRXThread_H
#define	DVRPTRRepeaterTXRXThread_H

#include "DVRPTRRepeaterStatusData.h"
#include "RepeaterProtocolHandler.h"
#include "DVRPTRRepeaterThread.h"
#include "DVRPTRRepeaterDefs.h"
#include "DVRPTRController.h"
#include "DVTOOLFileWriter.h"
#include "SlowDataDecoder.h"
#include "CallsignList.h"
#include "OutputQueue.h"
#include "HeaderData.h"
#include "AMBEFEC.h"
#include "Timer.h"

#include <wx/wx.h>

class CDVRPTRRepeaterTXRXThread : public IDVRPTRRepeaterThread {
public:
	CDVRPTRRepeaterTXRXThread();
	virtual ~CDVRPTRRepeaterTXRXThread();

	virtual void setCallsign(const wxString& callsign, const wxString& gateway, DSTAR_MODE mode, ACK_TYPE ack, bool restriction, bool rpt1Validation);
	virtual void setProtocolHandler(CRepeaterProtocolHandler* handler);
	virtual void setModem(IDVRPTRController* modem);
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

	virtual void run();
	virtual void kill();

private:
	IDVRPTRController*         m_dvrptr;
	CRepeaterProtocolHandler*  m_protocolHandler;
	CExternalController*       m_controller;
	wxString                   m_rptCallsign;
	CHeaderData*               m_rxHeader;
	CHeaderData*               m_txHeader;
	COutputQueue**             m_networkQueue;
	unsigned int               m_writeNum;
	unsigned int               m_readNum;
	unsigned char              m_radioSeqNo;
	unsigned char              m_networkSeqNo;
	CTimer                     m_watchdogTimer;
	CTimer                     m_pollTimer;
	DSTAR_RPT_STATE            m_rptState;
	DSTAR_RX_STATE             m_rxState;
	CSlowDataDecoder           m_slowDataDecoder;
	bool                       m_tx;
	bool                       m_killed;
	CTimer                     m_activeHangTimer;
	bool                       m_disable;
	unsigned char*             m_lastData;
	CAMBEFEC                   m_ambe;
	unsigned int               m_ambeFrames;
	unsigned int               m_ambeSilence;
	unsigned int               m_ambeBits;
	unsigned int               m_ambeErrors;
	unsigned int               m_lastAMBEBits;
	unsigned int               m_lastAMBEErrors;
	wxStopWatch                m_headerTime;
	wxStopWatch                m_packetTime;
	unsigned int               m_packetCount;
	unsigned int               m_packetSilence;

	void receiveHeader(unsigned char* data, unsigned int length);
	void receiveRadioData(unsigned char* data, unsigned int length);
	void receiveSlowData(unsigned char* data, unsigned int length);
	void transmitNetworkHeader(const CHeaderData& header);
	void transmitNetworkData();
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
	unsigned int countBits(unsigned char byte);
	void clock(unsigned int ms);
	void blankDTMF(unsigned char* data);
};

#endif

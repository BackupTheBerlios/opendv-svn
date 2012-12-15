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

#ifndef	DummyRepeaterThread_H
#define	DummyRepeaterThread_H

#include "DummyRepeaterSlowDataEncoder.h"
#include "DummyRepeaterSlowDataDecoder.h"
#include "DummyRepeaterDVDongleThread.h"
#include "RepeaterProtocolHandler.h"
#include "SoundCardReaderWriter.h"
#include "DVDongleController.h"
#include "ExternalController.h"
#include "DummyRepeaterDefs.h"
#include "AudioCallback.h"
#include "RingBuffer.h"
#include "HeaderData.h"
#include "Timer.h"

#include <wx/wx.h>

enum CLIENT_STATE {
	CLIENT_RECEIVE,
	CLIENT_TRANSMIT,
	CLIENT_WANT_RECEIVE
};

class CDummyRepeaterThread : public wxThread, public IAudioCallback {
public:
	CDummyRepeaterThread();
	virtual ~CDummyRepeaterThread();

	virtual void callback(const wxFloat32* input, wxFloat32* output, unsigned int nSamples, int id);

	virtual void setCallsign(const wxString& callsign1, const wxString& callsign2);
	virtual void setSoundCard(CSoundCardReaderWriter* soundcard);
	virtual void setDVDongle(CDVDongleController* dongle);
	virtual void setProtocol(CRepeaterProtocolHandler* protocol);
	virtual void setController(CExternalController* controller);
	virtual void setMessage(const wxString& text);
	virtual void setBleep(bool on);

	virtual void setYour(const wxString& your);
	virtual void setRpt1(const wxString& rpt1);
	virtual void setRpt2(const wxString& rpt2);

	virtual bool setTransmit(bool transmit);

	virtual void* Entry();
	virtual void  kill();

private:
	CSoundCardReaderWriter*       m_soundcard;
	CDummyRepeaterDVDongleThread* m_dongle;
	CExternalController*          m_controller;
	CDummyRepeaterSlowDataEncoder m_slowDataEncoder;
	CDummyRepeaterSlowDataDecoder m_slowDataDecoder;
	CRepeaterProtocolHandler*     m_protocol;
	bool                          m_stopped;
	CRingBuffer<wxFloat32>        m_decodeAudio;
	CRingBuffer<unsigned char>    m_decodeData;
	CRingBuffer<wxFloat32>        m_encodeAudio;
	CRingBuffer<unsigned char>    m_encodeData;
	CLIENT_STATE                  m_transmit;
	wxString                      m_callsign1;
	wxString                      m_callsign2;
	wxString                      m_your;
	wxString                      m_rpt1;
	wxString                      m_rpt2;
	wxString                      m_message;
	unsigned int                  m_frameCount;
	unsigned int                  m_networkSeqNo;
	bool                          m_killed;
	bool                          m_started;
	CTimer                        m_watchdog;
	CTimer                        m_poll;
	unsigned int                  m_clockCount;
	bool                          m_busy;
	bool                          m_localTX;
	bool                          m_externalTX;

	void receive();
	void transmit();

	void resetReceiver();
	void processHeader(CHeaderData* header);
	bool processFrame(const unsigned char* buffer, unsigned char seqNo);

	void serviceNetwork();

	void checkController();
};

#endif

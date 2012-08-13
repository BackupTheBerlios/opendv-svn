/*
 *   Copyright (C) 2009,2010 by Jonathan Naylor G4KLX
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

#ifndef	DStarClientPreferences_H
#define	DStarClientPreferences_H

#include <wx/wx.h>
#include <wx/notebook.h>

#include "DStarClientControllerSet.h"
#include "DStarClientCallsignSet.h"
#include "SoundcardSet.h"
#include "InversionSet.h"
#include "DVDongleSet.h"
#include "MessageSet.h"
#include "BleepSet.h"

class CDStarClientPreferences : public wxDialog {
public:
	CDStarClientPreferences(wxWindow* parent, int id, const wxString& callsign1, const wxString& callsign2,
								   const wxString& audioReadDevice, const wxString& audioWriteDevice,
								   const wxString& radioReadDevice, const wxString& radioWriteDevice,
								   bool rxInvert, bool txInvert, const wxString& ifaceType, bool ptt,
								   unsigned int delay, bool invert, const wxString& dvdDevice,
								   const wxString& message, bool bleep);
	virtual ~CDStarClientPreferences();

	virtual bool Validate();

	virtual wxString getCallsign1() const;
	virtual wxString getCallsign2() const;

	virtual wxString getAudioSoundcardReadDevice() const;
	virtual wxString getAudioSoundcardWriteDevice() const;

	virtual wxString getRadioSoundcardReadDevice() const;
	virtual wxString getRadioSoundcardWriteDevice() const;

	virtual bool     getRXGMSKInvert() const;
	virtual bool     getTXGMSKInvert() const;

	virtual wxString getDVDDevice() const;

	virtual wxString     getIfaceType() const;
	virtual bool         getPTTEnable() const;
	virtual unsigned int getTXDelay() const;
	virtual bool         getTXInvert() const;

	virtual wxString getMessage() const;

	virtual bool     getBleep() const;

private:
	CDStarClientCallsignSet*   m_callsign;
	CSoundcardSet*             m_audioSoundcard;
	CSoundcardSet*             m_radioSoundcard;
	CInversionSet*             m_inversion;
	CDStarClientControllerSet* m_controller;
	CDVDongleSet*              m_dongle;
	CMessageSet*               m_message;
	CBleepSet*                 m_bleep;
};

#endif

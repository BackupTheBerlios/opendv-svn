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

#include "DStarClientPreferences.h"

#include "DStarClientDefs.h"

CDStarClientPreferences::CDStarClientPreferences(wxWindow* parent, int id, const wxString& callsign1,
								   const wxString& callsign2, const wxString& audioReadDevice,
								   const wxString& audioWriteDevice, const wxString& radioReadDevice,
								   const wxString& radioWriteDevice, bool rxInvert, bool txInvert,
								   const wxString& ifaceType, bool ptt, unsigned int delay, bool invert,
								   const wxString& dvdDevice, const wxString& message, bool bleep) :
wxDialog(parent, id, wxString(_("D-Star Client Preferences")), wxDefaultPosition, wxDefaultSize, wxRESIZE_BORDER | wxDEFAULT_DIALOG_STYLE),
m_callsign(NULL),
m_audioSoundcard(NULL),
m_radioSoundcard(NULL),
m_inversion(NULL),
m_controller(NULL),
m_dongle(NULL),
m_message(NULL),
m_bleep(NULL)
{
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	wxNotebook* noteBook = new wxNotebook(this, -1);

	m_callsign = new CDStarClientCallsignSet(noteBook, -1, APPLICATION_NAME, callsign1, callsign2);
	noteBook->AddPage(m_callsign, _("Callsign"), true);

	m_audioSoundcard = new CSoundcardSet(noteBook, -1, APPLICATION_NAME, audioReadDevice, audioWriteDevice);
	noteBook->AddPage(m_audioSoundcard, _("Audio Sound Card"), false);

	m_radioSoundcard = new CSoundcardSet(noteBook, -1, APPLICATION_NAME, radioReadDevice, radioWriteDevice);
	noteBook->AddPage(m_radioSoundcard, _("Radio Sound Card"), false);

	m_inversion = new CInversionSet(noteBook, -1, APPLICATION_NAME, rxInvert, txInvert);
	noteBook->AddPage(m_inversion, _("GMSK Inversion"), false);

	m_controller = new CDStarClientControllerSet(noteBook, -1, APPLICATION_NAME, ifaceType, ptt, delay, invert);
	noteBook->AddPage(m_controller, _("Controller"), false);

	m_dongle = new CDVDongleSet(noteBook, -1, APPLICATION_NAME, dvdDevice);
	noteBook->AddPage(m_dongle, _("DV-Dongle"), false);

	m_message = new CMessageSet(noteBook, -1, APPLICATION_NAME, message);
	noteBook->AddPage(m_message, _("Message"), false);

	m_bleep = new CBleepSet(noteBook, -1, APPLICATION_NAME, bleep);
	noteBook->AddPage(m_bleep, _("Bleep"), false);

	mainSizer->Add(noteBook, 1, wxALL | wxGROW, BORDER_SIZE);

	mainSizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALL | wxALIGN_RIGHT, BORDER_SIZE);

	SetAutoLayout(true);
	Layout();

	mainSizer->Fit(this);
	mainSizer->SetSizeHints(this);

	SetSizer(mainSizer);
}

CDStarClientPreferences::~CDStarClientPreferences()
{
}

bool CDStarClientPreferences::Validate()
{
	if (!m_callsign->Validate())
		return false;

	if (!m_audioSoundcard->Validate())
		return false;

	if (!m_radioSoundcard->Validate())
		return false;

	if (!m_inversion->Validate())
		return false;

	if (!m_controller->Validate())
		return false;

	if (!m_dongle->Validate())
		return false;

	if (!m_message->Validate())
		return false;

	return m_bleep->Validate();
}

wxString CDStarClientPreferences::getCallsign1() const
{
	return m_callsign->getCallsign1();
}

wxString CDStarClientPreferences::getCallsign2() const
{
	return m_callsign->getCallsign2();
}

wxString CDStarClientPreferences::getAudioSoundcardReadDevice() const
{
	return m_audioSoundcard->getReadDevice();
}

wxString CDStarClientPreferences::getAudioSoundcardWriteDevice() const
{
	return m_audioSoundcard->getWriteDevice();
}

wxString CDStarClientPreferences::getRadioSoundcardReadDevice() const
{
	return m_radioSoundcard->getReadDevice();
}

wxString CDStarClientPreferences::getRadioSoundcardWriteDevice() const
{
	return m_radioSoundcard->getWriteDevice();
}

bool CDStarClientPreferences::getRXGMSKInvert() const
{
	return m_inversion->getRXInvert();
}

bool CDStarClientPreferences::getTXGMSKInvert() const
{
	return m_inversion->getTXInvert();
}

wxString CDStarClientPreferences::getDVDDevice() const
{
	return m_dongle->getDevice();
}

wxString CDStarClientPreferences::getIfaceType() const
{
	return m_controller->getType();
}

bool CDStarClientPreferences::getPTTEnable() const
{
	return m_controller->getPTT();
}

unsigned int CDStarClientPreferences::getTXDelay() const
{
	return m_controller->getDelay();
}

bool CDStarClientPreferences::getTXInvert() const
{
	return m_controller->getInvert();
}

wxString CDStarClientPreferences::getMessage() const
{
	return m_message->getMessage();
}

bool CDStarClientPreferences::getBleep() const
{
	return m_bleep->getBleep();
}

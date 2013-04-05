/*
 *   Copyright (C) 2011,2012,2013 by Jonathan Naylor G4KLX
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

#ifndef	DStarModemConfig_H
#define	DStarModemConfig_H

#include "DStarDefines.h"

#include <wx/wx.h>
#include <wx/config.h>
#include <wx/filename.h>

class CDStarModemConfig {
public:
#if defined(__WINDOWS__)
	CDStarModemConfig(wxConfigBase* config, const wxString& name);
#else
	CDStarModemConfig(const wxString& dir, const wxString& configName, const wxString& name);
#endif
	~CDStarModemConfig();

	void getType(MODEM_TYPE& type) const;
	void setType(MODEM_TYPE type);

	void getDVAP(wxString& port, unsigned int& frequency, int& power, int& squelch) const;
	void setDVAP(const wxString& port, unsigned int frequency, int power, int squelch);

	void getGMSK(USB_INTERFACE& type, unsigned int& address) const;
	void setGMSK(USB_INTERFACE type, unsigned int address);

	void getDVRPTR1(wxString& port, bool& rxInvert, bool& txInvert, bool& channel, unsigned int& modLevel, unsigned int& txDelay) const;
	void setDVRPTR1(const wxString& port, bool rxInvert, bool txInvert, bool channel, unsigned int modLevel, unsigned int txDelay);

	void getDVRPTR2(CONNECTION_TYPE& connectionType, wxString& usbPort, wxString& address, unsigned int& port, bool& txInvert, unsigned int& modLevel) const;
	void setDVRPTR2(CONNECTION_TYPE connectionType, const wxString& usbPort, const wxString& address, unsigned int port, bool txInvert, unsigned int modLevel);

	bool write();

private:
#if defined(__WINDOWS__)
	wxConfigBase* m_config;
	wxString      m_name;
#else
	wxFileName    m_fileName;
#endif
	wxString      m_modemName;
	MODEM_TYPE    m_modemType;

	// DVAP
	wxString      m_dvapPort;
	unsigned int  m_dvapFrequency;
	int           m_dvapPower;
	int           m_dvapSquelch;

	// GMSK
	USB_INTERFACE m_gmskInterface;
	unsigned int  m_gmskAddress;

	// DV-RPTR 1
	wxString      m_dvrptr1Port;
	bool          m_dvrptr1RXInvert;
	bool          m_dvrptr1TXInvert;
	bool          m_dvrptr1Channel;
	unsigned int  m_dvrptr1ModLevel;
	unsigned int  m_dvrptr1TXDelay;

	// DV-RPTR 2
	CONNECTION_TYPE m_dvrptr2Connection;
	wxString      m_dvrptr2USBPort;
	wxString      m_dvrptr2Address;
	unsigned int  m_dvrptr2Port;
	bool          m_dvrptr2TXInvert;
	unsigned int  m_dvrptr2ModLevel;

	// Raspberry PI
};

#endif

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

#ifndef	RepeaterSet_H
#define	RepeaterSet_H

#include "AddressTextCtrl.h"
#include "PortTextCtrl.h"
#include "Defs.h"

#include <wx/wx.h>

class CRepeaterSet : public wxPanel {
public:
	CRepeaterSet(wxWindow* parent, int id, const wxString& title, const wxString& band, HW_TYPE type, const wxString& address, unsigned int port, unsigned char band1, unsigned char band2, unsigned char band3, bool dplusEnabled, bool dExtraEnabled, bool dcsEnabled, const wxString& reflector, bool atStartup, RECONNECT reconnect, double frequency, double offset, double range, double latitude, double longitude, double agl, const wxString& description1, const wxString& description2, const wxString& url);
	virtual ~CRepeaterSet();

	virtual bool Validate();

	virtual wxString     getBand() const;

	virtual HW_TYPE      getType() const;
	virtual wxString     getAddress() const;
	virtual unsigned int getPort() const;

	virtual unsigned char getBand1() const;
	virtual unsigned char getBand2() const;
	virtual unsigned char getBand3() const;

	virtual wxString     getReflector() const;
	virtual bool         atStartup() const;
	virtual RECONNECT    getReconnect() const;

	virtual double       getFrequency() const;
	virtual double       getOffset() const;
	virtual double       getRange() const;

	virtual double       getLatitude() const;
	virtual double       getLongitude() const;
	virtual double       getAGL() const;

	virtual wxString     getDescription1() const;
	virtual wxString     getDescription2() const;
	virtual wxString     getURL() const;

	virtual void onBand(wxCommandEvent& event);
	virtual void onType(wxCommandEvent& event);

private:
	wxString           m_title;
	wxChoice*          m_band;
	wxChoice*          m_type;
	CAddressTextCtrl*  m_address;
	CPortTextCtrl*     m_port;
	CPortTextCtrl*     m_band1;
	CPortTextCtrl*     m_band2;
	CPortTextCtrl*     m_band3;
	wxChoice*          m_reflector;
	wxChoice*          m_channel;
	wxChoice*          m_startup;
	wxChoice*          m_reconnect;
	wxTextCtrl*        m_frequency;
	wxTextCtrl*        m_offset;
	wxTextCtrl*        m_range;
	wxTextCtrl*        m_latitude;
	wxTextCtrl*        m_longitude;
	wxTextCtrl*        m_agl;
	wxTextCtrl*        m_description1;
	wxTextCtrl*        m_description2;
	wxTextCtrl*        m_url;

	bool isDDMode() const;

	DECLARE_EVENT_TABLE()
};

#endif

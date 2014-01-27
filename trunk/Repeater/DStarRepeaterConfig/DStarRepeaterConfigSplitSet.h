/*
 *   Copyright (C) 2014 by Jonathan Naylor G4KLX
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

#ifndef	DStarRepeaterConfigSplitSet_H
#define	DStarRepeaterConfigSplitSet_H

#include "DStarRepeaterConfigDefs.h"
#include "AddressTextCtrl.h"
#include "PortTextCtrl.h"

#include <wx/wx.h>

class CDStarRepeaterConfigSplitSet : public wxDialog {
public:
	CDStarRepeaterConfigSplitSet(wxWindow* parent, int id, const wxString& transmitter1Address, unsigned int transmitter1Port, const wxString& transmitter2Address, unsigned int transmitter2Port, const wxString& receiver1Address, unsigned int receiver1Port, const wxString& receiver2Address, unsigned int receiver2Port, unsigned int timeout);
	virtual ~CDStarRepeaterConfigSplitSet();

	virtual bool Validate();

	virtual wxString     getTransmitter1Address() const;
	virtual unsigned int getTransmitter1Port() const;
	virtual wxString     getTransmitter2Address() const;
	virtual unsigned int getTransmitter2Port() const;

	virtual wxString     getReceiver1Address() const;
	virtual unsigned int getReceiver1Port() const;
	virtual wxString     getReceiver2Address() const;
	virtual unsigned int getReceiver2Port() const;

	virtual unsigned int getTimeout() const;

private:
	CAddressTextCtrl* m_transmitter1Address;
	CPortTextCtrl*    m_transmitter1Port;
	CAddressTextCtrl* m_transmitter2Address;
	CPortTextCtrl*    m_transmitter2Port;
	CAddressTextCtrl* m_receiver1Address;
	CPortTextCtrl*    m_receiver1Port;
	CAddressTextCtrl* m_receiver2Address;
	CPortTextCtrl*    m_receiver2Port;
	wxSlider*         m_timeout;
};

#endif

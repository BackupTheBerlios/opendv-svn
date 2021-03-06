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

#ifndef	GMSKRepeaterStatusData_H
#define	GMSKRepeaterStatusData_H

#include "DStarDefines.h"

#include <wx/wx.h>

class CGMSKRepeaterStatusData {
public:
	CGMSKRepeaterStatusData(const wxString& myCall1, const wxString& myCall2, const wxString& yourCall,
							 const wxString& rptCall1, const wxString& rptCall2, unsigned char flag1,
							 unsigned char flag2, unsigned char flag3, bool tx, DSTAR_RPT_STATE state,
							 unsigned int timeoutTimer, unsigned int timeoutExpiry, unsigned int beaconTimer,
							 unsigned int beaconExpiry, float percent, const wxString& text,
							 const wxString& status1, const wxString& status2, const wxString& status3,
							 const wxString& status4, const wxString& status5);
	~CGMSKRepeaterStatusData();

	wxString      getMyCall1() const;
	wxString      getMyCall2() const;
	wxString      getYourCall() const;
	wxString      getRptCall1() const;
	wxString      getRptCall2() const;
	unsigned char getFlag1() const;
	unsigned char getFlag2() const;
	unsigned char getFlag3() const;

	bool          getTX() const;

	DSTAR_RPT_STATE getState() const;

	unsigned int  getTimeoutTimer() const;
	unsigned int  getTimeoutExpiry() const;

	unsigned int  getBeaconTimer() const;
	unsigned int  getBeaconExpiry() const;

	float         getPercent() const;

	wxString      getText() const;
	wxString      getStatus1() const;
	wxString      getStatus2() const;
	wxString      getStatus3() const;
	wxString      getStatus4() const;
	wxString      getStatus5() const;

private:
	wxString        m_myCall1;
	wxString        m_myCall2;
	wxString        m_yourCall;
	wxString        m_rptCall1;
	wxString        m_rptCall2;
	unsigned char   m_flag1;
	unsigned char   m_flag2;
	unsigned char   m_flag3;
	bool            m_tx;
	DSTAR_RPT_STATE m_state;
	unsigned int    m_timeoutTimer;
	unsigned int    m_timeoutExpiry;
	unsigned int    m_beaconTimer;
	unsigned int    m_beaconExpiry;
	float           m_percent;
	wxString        m_text;
	wxString        m_status1;
	wxString        m_status2;
	wxString        m_status3;
	wxString        m_status4;
	wxString        m_status5;
};

#endif

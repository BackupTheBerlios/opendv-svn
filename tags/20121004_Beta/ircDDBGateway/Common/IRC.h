/*
 *   Copyright (C) 2012 by Jonathan Naylor G4KLX
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

#ifndef	IRC_H
#define	IRC_H

#include "DStarDefines.h"

#include <wx/wx.h>

enum IRCDDB_RESPONSE_TYPE {
	IDRT_NONE,
	IDRT_USER,
	IDRT_GATEWAY,
	IDRT_REPEATER
};

class IIRC {
public:
	virtual bool open() = 0;

	virtual void rptrQTH(double latitude, double longitude, const wxString& desc1, const wxString& desc2, const wxString& infoURL) = 0;

	virtual void rptrQRG(const wxString& module, double txFrequency, double duplexShift, double range, double agl) = 0;

	virtual void kickWatchdog(const wxString& wdInfo) = 0;

	virtual int getConnectionState() = 0;

	virtual bool sendHeard(const wxString& myCall, const wxString& myCallExt, const wxString& yourCall, const wxString& rpt1, const wxString& rpt2, unsigned char flag1, unsigned char flag2, unsigned char flag3) = 0;

	virtual bool sendHeardWithTXMsg(const wxString& myCall, const wxString& myCallExt, const wxString& yourCall, const wxString& rpt1, const wxString& rpt2, unsigned char flag1, unsigned char flag2, unsigned char flag3, const wxString& network_destination, const wxString& tx_message) = 0;

	virtual bool sendHeardWithTXStats(const wxString& myCall, const wxString& myCallExt, const wxString& yourCall, const wxString& rpt1, const wxString& rpt2, unsigned char flag1, unsigned char flag2, unsigned char flag3, int num_dv_frames, int num_dv_silent_frames, int num_bit_errors) = 0;

	virtual bool findGateway(const wxString& gatewayCallsign) = 0;

	virtual bool findRepeater(const wxString& repeaterCallsign) = 0;

	virtual bool findUser(const wxString& userCallsign) = 0;

	virtual IRCDDB_RESPONSE_TYPE getMessageType() = 0;

	virtual bool receiveRepeater(wxString& repeaterCallsign, wxString& gatewayCallsign, wxString& address, DSTAR_PROTOCOL& protocol) = 0;

	virtual bool receiveGateway(wxString& gatewayCallsign, wxString& address, DSTAR_PROTOCOL& protocol) = 0;

	virtual bool receiveUser(wxString& userCallsign, wxString& repeaterCallsign, wxString& gatewayCallsign, wxString& address) = 0;

	virtual bool receiveUser(wxString& userCallsign, wxString& repeaterCallsign, wxString& gatewayCallsign, wxString& address, wxString& timeStamp) = 0;

	virtual void close() = 0;
};

#endif

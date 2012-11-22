/*
 *   Copyright (C) 2010,2012 by Jonathan Naylor G4KLX
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

#ifndef	PollData_H
#define	PollData_H

#include <wx/wx.h>

#if defined(__WINDOWS__)
#include "Inaddr.h"
#else
#include <netinet/in.h>
#endif

class CPollData {
public:
	CPollData(const wxString& data, const in_addr& address, unsigned int port);
	CPollData(const in_addr& address, unsigned int port);
	CPollData();
	~CPollData();

	bool setDExtraData(const unsigned char* data, unsigned int length, const in_addr& address, unsigned int port);
	bool setDPlusData(const unsigned char* data, unsigned int length, const in_addr& address, unsigned int port);
	bool setDCSData(const unsigned char* data, unsigned int length, const in_addr& address, unsigned int port);

	unsigned int getDExtraData(unsigned char* data, unsigned int length) const;
	unsigned int getDPlusData(unsigned char* data, unsigned int length) const;
	unsigned int getDCSData(unsigned char* data, unsigned int length) const;

	wxString     getData1() const;
	void         setData1(const wxString& data);

	wxString     getData2() const;
	void         setData2(const wxString& data);

	bool         isDongle() const;

	in_addr      getAddress() const;
	unsigned int getPort() const;

	unsigned int getLength() const;

private:
	wxString     m_data1;
	wxString     m_data2;
	bool         m_dongle;
	unsigned int m_length;
	in_addr      m_address;
	unsigned int m_port;
};

#endif

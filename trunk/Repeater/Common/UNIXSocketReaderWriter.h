/*
 *   Copyright (C) 2013 by Jonathan Naylor G4KLX
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

#ifndef UNIXSocketReaderWriter_H
#define UNIXSocketReaderWriter_H

#include <wx/wx.h>

#include <sys/un.h>

class CUNIXSocketReaderWriter {
public:
	CUNIXSocketReaderWriter(const wxString& server, const wxString& client);
	~CUNIXSocketReaderWriter();

	bool open();

	int  read(unsigned char* buffer, unsigned int length);
	bool write(const unsigned char* buffer, unsigned int length);

	void close();

private:
	wxString           m_server;
	wxString           m_client;
	int                m_sockServer;
	int                m_sockClient;
	struct sockaddr_un m_dest;
};

#endif
/*
 *	Copyright (C) 2013 by Jonathan Naylor, G4KLX
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; version 2 of the License.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 */

#include "UNIXSocketReaderWriter.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <cerrno>

CUNIXSocketReaderWriter::CUNIXSocketReaderWriter(const wxString& server, const wxString& client) :
m_server(server),
m_client(client),
m_sockServer(-1),
m_sockClient(-1),
m_dest()
{
	wxASSERT(!server.IsEmpty());
	wxASSERT(!client.IsEmpty());
}

CUNIXSocketReaderWriter::~CUNIXSocketReaderWriter()
{
}

bool CUNIXSocketReaderWriter::open()
{
	m_sockServer = ::socket(PF_UNIX, SOCK_DGRAM, 0);
	if (m_sockServer < 0) {
		wxLogError(wxT("Socket creation for the UNIX server failed: err=%d"), errno);
		return false;
	}

	m_sockClient = ::socket(PF_UNIX, SOCK_DGRAM, 0);
	if (m_sockClient < 0) {
		wxLogError(wxT("Socket creation for the UNIX client failed: err=%d"), errno);
		close();
		return false;
	}

	::memset(&m_dest, 0x00U, sizeof(struct sockaddr_un));
	m_dest.sun_family = AF_UNIX;
	for (unsigned int i = 0U; i < m_client.Len(); i++)
		m_dest.sun_path[i] = m_client.GetChar(i);

	struct sockaddr_un serv;
	::memset(&serv, 0x00U, sizeof(struct sockaddr_un));
	serv.sun_family = AF_UNIX;
	for (unsigned int i = 0U; i < m_server.Len(); i++)
		serv.sun_path[i] = m_server.GetChar(i);

	int ret = ::bind(m_sockServer, (struct sockaddr*)&serv, sizeof(struct sockaddr_un));
	if (ret < 0) {
		wxLogError(wxT("Binding to the UNIX server socket failed: err=%d"), errno);
		close();
		return false;
	}

	return true;
}

bool CUNIXSocketReaderWriter::write(const unsigned char* buffer, unsigned int length)
{
	wxASSERT(m_sockClient != -1);
	wxASSERT(buffer != NULL);

	int ret = ::sendto(m_sockClient, buffer, length, 0, (struct sockaddr*)&m_dest, sizeof(struct sockaddr_un));
	if (ret < 0) {
		wxLogError(wxT("Error from sendto to the client socket, err=%d"), errno);
		return false;
	}

	return ret == int(length);
}

int CUNIXSocketReaderWriter::read(unsigned char* buffer, unsigned int length)
{
	wxASSERT(m_sockServer != -1);
	wxASSERT(buffer != NULL);

	struct timeval tv;
	tv.tv_sec  = 0;
	tv.tv_usec = 0;

	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(m_sockServer, &fds);

	int n = ::select(m_sockServer + 1, &fds, NULL, NULL, &tv);
	if (n < 0) {
		wxLogError(wxT("Error from select on the server socket, err=%d"), errno);
		return -1;
	}

	if (n == 0)
		return 0;

	n = ::read(m_sockServer, buffer, length);
	if (n == 0)
		return -2;
	if (n < 0) {
		wxLogError(wxT("Error from read on the server socket, err=%d"), errno);
		return -1;
	}

	return int(n);
}

void CUNIXSocketReaderWriter::close()
{
	if (m_sockServer != -1) {
		::close(m_sockServer);
		m_sockServer = -1;
	}

	if (m_sockClient != -1) {
		::close(m_sockClient);
		m_sockClient = -1;
	}
}

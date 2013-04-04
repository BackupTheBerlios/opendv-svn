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

#include "UNIXSocketClient.h"

#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cerrno>

CUNIXSocketClient::CUNIXSocketClient(const wxString& name) :
m_name(name),
m_sock(-1)
{
	wxASSERT(!name.IsEmpty());
}

CUNIXSocketClient::CUNIXSocketClient(int fd) :
m_name(),
m_sock(fd)
{
	wxASSERT(fd >= 0);
}

CUNIXSocketClient::~CUNIXSocketClient()
{
}

bool CUNIXSocketClient::open()
{
	m_sock = ::socket(PF_UNIX, SOCK_STREAM, 0);
	if (m_sock < 0) {
		wxLogError(wxT("Socket creation for the UNIX socket failed: err=%d"), errno);
		return false;
	}

	struct sockaddr_un serv;
	::memset(&serv, 0x00U, sizeof(struct sockaddr_un));
	serv.sun_family = AF_UNIX;
	for (unsigned int i = 0U; i < m_name.Len(); i++)
		serv.sun_path[i] = m_name.GetChar(i);

	int ret = ::connect(m_sock, (struct sockaddr*)&serv, sizeof(struct sockaddr_un));
	if (ret < 0) {
		wxLogError(wxT("Connection to the UNIX socket (%s) failed: err=%d"), m_name.c_str(), errno);
		::close(m_sock);
		m_sock = -1;
		return false;
	}

	wxLogMessage(wxT("Connected on UNIX socket \"%s\""), m_name.c_str());

	return true;
}

bool CUNIXSocketClient::write(const unsigned char* buffer, unsigned int length)
{
	wxASSERT(m_sock != -1);
	wxASSERT(buffer != NULL);

	int ret = ::write(m_sock, buffer, length);
	if (ret < 0) {
		wxLogError(wxT("Error from write to the UNIX socket, err=%d"), errno);
		return false;
	}

	return ret == int(length);
}

int CUNIXSocketClient::read(unsigned char* buffer, unsigned int length)
{
	wxASSERT(m_sock != -1);
	wxASSERT(buffer != NULL);

	struct timeval tv;
	tv.tv_sec  = 0;
	tv.tv_usec = 0;

	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(m_sock, &fds);

	int n = ::select(m_sock + 1, &fds, NULL, NULL, &tv);
	if (n < 0) {
		wxLogError(wxT("Error from select on the UNIX socket, err=%d"), errno);
		return -1;
	}

	if (n == 0)
		return 0;

	n = ::read(m_sock, buffer, length);
	if (n == 0)
		return -2;
	if (n < 0) {
		wxLogError(wxT("Error from read on the UNIX socket, err=%d"), errno);
		return -1;
	}

	return int(n);
}

void CUNIXSocketClient::close()
{
	if (m_sock != -1) {
		::close(m_sock);
		m_sock = -1;
	}
}

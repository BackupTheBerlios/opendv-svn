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

#include "UNIXSocketServer.h"

#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cerrno>

CUNIXSocketServer::CUNIXSocketServer(const wxString& name) :
m_name(name),
m_sock(-1)
{
	wxASSERT(!name.IsEmpty());
}

CUNIXSocketServer::~CUNIXSocketServer()
{
}

bool CUNIXSocketServer::start()
{
	m_sock = ::socket(PF_UNIX, SOCK_STREAM, 0);
	if (m_sock < 0) {
		wxLogError(wxT("Socket creation for the UNIX server failed: err=%d"), errno);
		return false;
	}

	struct sockaddr_un serv;
	::memset(&serv, 0x00U, sizeof(struct sockaddr_un));
	serv.sun_family = AF_UNIX;
	for (unsigned int i = 0U; i < m_name.Len(); i++)
		serv.sun_path[i] = m_name.GetChar(i);

	socklen_t len = ::strlen(serv.sun_path) + sizeof(serv.sun_family);

	::unlink(serv.sun_path);

	int ret = ::bind(m_sock, (struct sockaddr*)&serv, len);
	if (ret < 0) {
		wxLogError(wxT("Binding to the UNIX server socket (%s) failed: err=%d"), m_name.c_str(), errno);
		close();
		return false;
	}

	::listen(m_sock, 1);

	wxLogMessage(wxT("Listening on UNIX socket \"%s\""), m_name.c_str());

	return true;
}

bool CUNIXSocketServer::isConnected()
{
	if (m_client != NULL)
		return true;

	struct timeval tv;
	tv.tv_sec  = 0;
	tv.tv_usec = 0;

	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(m_sock, &fds);

	int n = ::select(m_sock + 1, &fds, NULL, NULL, &tv);
	if (n < 0) {
		wxLogError(wxT("Selecting to the UNIX server socket failed: err=%d"), errno);
		return false;
	}

	if (n == 0)
		return false;

	struct sockaddr_un addr;
	socklen_t len = sizeof(struct sockaddr_un);
	int fd = ::accept(m_sock, (struct sockaddr*)&addr, &len);
	if (fd < 0) {
		wxLogError(wxT("Accepting to the UNIX server socket failed: err=%d"), errno);
		return false;
	}

	::close(m_sock);
	m_sock = -1;

	wxLogMessage(wxT("Accepted incoming UNIX socket connection"));

	m_client = new CUNIXSocketClient(fd);

	return true;
}

bool CUNIXSocketServer::write(const unsigned char* buffer, unsigned int length)
{
	wxASSERT(m_client != NULL);
	wxASSERT(buffer != NULL);

	return m_client->write(buffer, length);
}

int CUNIXSocketServer::read(unsigned char* buffer, unsigned int length)
{
	wxASSERT(m_client != NULL);
	wxASSERT(buffer != NULL);

	return m_client->read(buffer, length);
}

void CUNIXSocketServer::close()
{
	wxASSERT(m_client != NULL);

	m_client->close();
	delete m_client;

	m_client = NULL;

	start();
}

void CUNIXSocketServer::stop()
{
	if (m_client != NULL) {
		m_client->close();
		delete m_client;
		m_client = NULL;
	}

	if (m_sock != -1) {
		::close(m_sock);
		m_sock = -1;
	}
}

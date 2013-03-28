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

#include "NamedPipeServer.h"

CNamedPipeServer::CNamedPipeServer(const wxString& name) :
m_name(name),
m_handle(INVALID_HANDLE_VALUE),
m_overlappedRead(),
m_overlappedWrite(),
m_overlappedConnect()
{
	wxASSERT(!name.IsEmpty());

	::memset(&m_overlappedRead, 0x00U, sizeof(OVERLAPPED));
	::memset(&m_overlappedWrite, 0x00U, sizeof(OVERLAPPED));
	::memset(&m_overlappedConnect, 0x00U, sizeof(OVERLAPPED));
}

CNamedPipeServer::~CNamedPipeServer()
{
}

bool CNamedPipeServer::open()
{
	wxString fileName;
	fileName.Printf(wxT("\\\\.\\pipe\\%s"), m_name.c_str());

	m_handle = ::CreateNamedPipe(fileName.c_str(), PIPE_ACCESS_DUPLEX | FILE_FLAG_FIRST_PIPE_INSTANCE | FILE_FLAG_OVERLAPPED, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE, 1, 200, 200, 10, NULL);
	if (m_handle == INVALID_HANDLE_VALUE) {
		wxLogError(wxT("CreateNamedPipe for the Named Pipe, %s, failed: err=%lu"), fileName.c_str(), ::GetLastError());
		return false;
	}

	return true;
}

bool CNamedPipeServer::hasClient()
{
	wxASSERT(m_handle != INVALID_HANDLE_VALUE);

	BOOL res = ::ConnectNamedPipe(m_handle, &m_overlappedConnect);
	if (!res) {
		DWORD err = ::GetLastError();
		if (err == ERROR_IO_PENDING)
			return true;

		wxLogError(wxT("Error from Named Pipe ConnectNamedPipe: err=%lu"), err);
		return false;
	}

	return true;
}

bool CNamedPipeServer::write(const unsigned char* buffer, unsigned int length)
{
	wxASSERT(m_handle != INVALID_HANDLE_VALUE);
	wxASSERT(buffer != NULL);

	DWORD bytes = 0UL;
	BOOL res = ::WriteFile(m_handle, buffer, length, &bytes, &m_overlappedWrite);
	if (!res) {
		DWORD err = ::GetLastError();
		if (err == ERROR_IO_PENDING)
			return true;

		wxLogError(wxT("Error from Named Pipe WriteFile: err=%lu"), err);
		return false;
	}

	return true;
}

int CNamedPipeServer::read(unsigned char* buffer, unsigned int length)
{
	wxASSERT(m_handle != INVALID_HANDLE_VALUE);
	wxASSERT(buffer != NULL);

	DWORD bytes = 0UL;
	BOOL res = ::ReadFile(m_handle, buffer, length, &bytes, &m_overlappedRead);
	if (!res) {
		DWORD err = ::GetLastError();
		if (err == ERROR_IO_PENDING)
			return 0;

		wxLogError(wxT("Error from Named Pipe ReadFile: err=%lu"), err);
		return -1;
	}

	return int(bytes);
}

void CNamedPipeServer::close()
{
	if (m_handle != INVALID_HANDLE_VALUE) {
		::CloseHandle(m_handle);
		m_handle = INVALID_HANDLE_VALUE;
	}
}

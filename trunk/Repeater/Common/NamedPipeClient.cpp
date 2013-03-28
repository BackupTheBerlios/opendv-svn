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

#include "NamedPipeClient.h"

CNamedPipeClient::CNamedPipeClient(const wxString& name) :
m_name(name),
m_handle(INVALID_HANDLE_VALUE)
{
	wxASSERT(!name.IsEmpty());
}

CNamedPipeClient::~CNamedPipeClient()
{
}

bool CNamedPipeClient::open()
{
	wxString fileName;
	fileName.Printf(wxT("\\\\.\\pipe\\%s"), m_name.c_str());

	m_handle = ::CreateFile(fileName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (m_handle == INVALID_HANDLE_VALUE) {
		wxLogError(wxT("CreateFile for the Named Pipe, %s, failed: err=%lu"), fileName.c_str(), ::GetLastError());
		return false;
	}

	return true;
}

bool CNamedPipeClient::write(const unsigned char* buffer, unsigned int length)
{
	wxASSERT(m_handle != INVALID_HANDLE_VALUE);
	wxASSERT(buffer != NULL);

	DWORD bytes = 0UL;
	BOOL res = ::WriteFile(m_handle, buffer, length, &bytes, NULL);
	if (!res || bytes != length) {
		wxLogError(wxT("Error from Named Pipe WriteFile: err=%lu, written=%u"), ::GetLastError(), bytes);
		return false;
	}

	return true;
}

int CNamedPipeClient::read(unsigned char* buffer, unsigned int length)
{
	wxASSERT(m_handle != INVALID_HANDLE_VALUE);
	wxASSERT(buffer != NULL);

	DWORD bytes = 0UL;
	BOOL res = ::ReadFile(m_handle, buffer, length, &bytes, NULL);
	if (!res)
		return -1;

	return int(bytes);
}

void CNamedPipeClient::close()
{
	if (m_handle != INVALID_HANDLE_VALUE) {
		::CloseHandle(m_handle);
		m_handle = INVALID_HANDLE_VALUE;
	}
}

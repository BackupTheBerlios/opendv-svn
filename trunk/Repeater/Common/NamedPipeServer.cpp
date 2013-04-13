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

const unsigned int READ_BUFFER_LENGTH = 500U;

CNamedPipeServer::CNamedPipeServer(const wxString& name) :
m_name(name),
m_handle(INVALID_HANDLE_VALUE),
m_waitConnect(false),
m_connected(false),
m_readPending(false),
m_readBuffer(NULL),
m_overlappedRead(),
m_overlappedWrite(),
m_overlappedConnect()
{
	wxASSERT(!name.IsEmpty());

	m_readBuffer = new unsigned char[READ_BUFFER_LENGTH];
}

CNamedPipeServer::~CNamedPipeServer()
{
	delete[] m_readBuffer;
}

bool CNamedPipeServer::start()
{
	wxString fileName;
	fileName.Printf(wxT("\\\\.\\pipe\\%s"), m_name.c_str());

	m_handle = ::CreateNamedPipe(fileName.c_str(), PIPE_ACCESS_DUPLEX | FILE_FLAG_FIRST_PIPE_INSTANCE | FILE_FLAG_OVERLAPPED, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE, 1, 200, 200, 10, NULL);
	if (m_handle == INVALID_HANDLE_VALUE) {
		wxLogError(wxT("CreateNamedPipe for the Named Pipe, %s, failed: err=%lu"), fileName.c_str(), ::GetLastError());
		return false;
	}

	::memset(&m_overlappedRead, 0x00U, sizeof(OVERLAPPED));
	m_overlappedRead.hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);

	::memset(&m_overlappedWrite, 0x00U, sizeof(OVERLAPPED));
	m_overlappedWrite.hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);

	::memset(&m_overlappedConnect, 0x00U, sizeof(OVERLAPPED));
	m_overlappedConnect.hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);

	m_readPending = false;
	m_waitConnect = false;
	m_connected = false;

	wxLogMessage(wxT("Listening on Named Pipe \"%s\""), fileName.c_str());

	return true;
}

bool CNamedPipeServer::isConnected()
{
	wxASSERT(m_handle != INVALID_HANDLE_VALUE);

	if (m_connected)
		return true;

	if (!m_waitConnect) {
		::ConnectNamedPipe(m_handle, &m_overlappedConnect);
		DWORD err = ::GetLastError();
		if (err == ERROR_PIPE_CONNECTED) {
			wxLogMessage(wxT("Accepted incoming Named Pipe connection"));
			m_connected = true;
			return true;
		}

		if (err != ERROR_IO_PENDING) {
			wxLogError(wxT("Error from Named Pipe ConnectNamedPipe: err=%lu"), err);
			return false;
		}

		m_waitConnect = true;
	}

	DWORD ret = ::WaitForSingleObject(m_overlappedConnect.hEvent, 20UL);
	if (ret == WAIT_OBJECT_0) {
		wxLogMessage(wxT("Accepted incoming Named Pipe connection"));
		m_waitConnect = false;
		m_connected = true;
		return true;
	}

	if (ret == WAIT_FAILED) {
		DWORD err = ::GetLastError();
		wxLogError(wxT("Error from wait on Named Pipe ConnectNamedPipe: err=%lu"), err);
		m_waitConnect = false;
		return false;
	}

	return false;
}

bool CNamedPipeServer::write(const unsigned char* buffer, unsigned int length)
{
	wxASSERT(m_handle != INVALID_HANDLE_VALUE);
	wxASSERT(buffer != NULL);

	BOOL res = ::WriteFile(m_handle, buffer, length, NULL, &m_overlappedWrite);
	if (res)
		return true;

	DWORD err = ::GetLastError();
	if (err != ERROR_IO_PENDING) {
		wxLogError(wxT("Error from Named Pipe WriteFile: err=%lu"), err);
		return false;
	}

	DWORD ret = ::WaitForSingleObject(m_overlappedWrite.hEvent, INFINITE);
	if (ret == WAIT_OBJECT_0)
		return true;

	wxLogError(wxT("Error from wait on Named Pipe WriteFile: err=%lu"), ::GetLastError());
	return false;
}

int CNamedPipeServer::read(unsigned char* buffer, unsigned int length)
{
	wxASSERT(m_handle != INVALID_HANDLE_VALUE);
	wxASSERT(buffer != NULL);

	if (!m_readPending) {
		if (length > READ_BUFFER_LENGTH)
			length = READ_BUFFER_LENGTH;

		BOOL res = ::ReadFile(m_handle, m_readBuffer, length, NULL, &m_overlappedRead);
		if (res) {
			::memcpy(buffer, m_readBuffer, length);
			return length;
		}

		DWORD err = ::GetLastError();
		if (err != ERROR_IO_PENDING) {
			wxLogError(wxT("Error from Named Pipe ReadFile: err=%lu"), err);
			return -1;
		}

		m_readPending = true;
	}

	BOOL ret = HasOverlappedIoCompleted(&m_overlappedRead);
	if (!ret)
		return 0;

	m_readPending = false;

	DWORD bytes = 0UL;
	ret = ::GetOverlappedResult(m_handle, &m_overlappedRead, &bytes, TRUE);
	if (!ret) {
		wxLogError(wxT("Error from GetOverlappedResult: err=%lu"), ::GetLastError());
		return -1;
	}

	::memcpy(buffer, m_readBuffer, bytes);

	return int(bytes);
}

void CNamedPipeServer::stop()
{
	if (m_handle != INVALID_HANDLE_VALUE) {
		::CloseHandle(m_overlappedRead.hEvent);
		::CloseHandle(m_overlappedWrite.hEvent);
		::CloseHandle(m_overlappedConnect.hEvent);
		::CloseHandle(m_handle);
		m_handle = INVALID_HANDLE_VALUE;
	}
}

/*
 *   Copyright (C) 2002-2004,2007-2011 by Jonathan Naylor G4KLX
 *   Copyright (C) 1999-2001 by Thomas Sailor HB9JNX
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

#include "SerialController.h"

#include <sys/types.h>

#if defined(__APPLE__) && defined(__MACH__)
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOTypes.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/serial/IOSerialKeys.h>
#include <IOKit/IOBSD.h>
#include <sys/param.h>
#elif defined(__WINDOWS__)
#include <setupapi.h>
#include <winioctl.h>
#else
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <cerrno>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#endif


wxArrayString CSerialController::getDevices()
{
	wxArrayString devices;

	devices.Alloc(10);

#if defined(__APPLE__) && defined(__MACH__)
	mach_port_t masterPort;
	kern_return_t ret = ::IOMasterPort(MACH_PORT_NULL, &masterPort);
	if (ret != KERN_SUCCESS)
		return devices;

	CFMutableDictionaryRef match = ::IOServiceMatching(kIOSerialBSDServiceValue);
	if (match == NULL)
		wxLogWarning(wxT("IOServiceMatching() returned NULL"));
	else
		::CFDictionarySetValue(match, CFSTR(kIOSerialBSDTypeKey), CFSTR(kIOSerialBSDRS232Type));

	io_iterator_t services;
	ret = ::IOServiceGetMatchingServices(masterPort, match, &services);
	if (ret != KERN_SUCCESS)
		return devices;

	io_object_t modem;
	while ((modem = ::IOIteratorNext(services))) {
		CFTypeRef filePath = ::IORegistryEntryCreateCFProperty(modem, CFSTR(kIOCalloutDeviceKey), kCFAllocatorDefault, 0);
		if (filePath != NULL) {
			char port[MAXPATHLEN];
			Boolean result = ::CFStringGetCString((const __CFString*)filePath, port, MAXPATHLEN, kCFStringEncodingASCII);
			::CFRelease(filePath);

			if (result)
				devices.Add(port);

			::IOObjectRelease(modem);
		}
	}

	::IOObjectRelease(services);
#elif defined(__WINDOWS__)
	devices.Add(wxT("\\\\.\\COM1"));
	devices.Add(wxT("\\\\.\\COM2"));
	devices.Add(wxT("\\\\.\\COM3"));
	devices.Add(wxT("\\\\.\\COM4"));
	devices.Add(wxT("\\\\.\\COM5"));
	devices.Add(wxT("\\\\.\\COM6"));
	devices.Add(wxT("\\\\.\\COM7"));
	devices.Add(wxT("\\\\.\\COM8"));
	devices.Add(wxT("\\\\.\\COM9"));
	devices.Add(wxT("\\\\.\\COM10"));
	devices.Add(wxT("\\\\.\\COM11"));
	devices.Add(wxT("\\\\.\\COM12"));
	devices.Add(wxT("\\\\.\\COM13"));
	devices.Add(wxT("\\\\.\\COM14"));
	devices.Add(wxT("\\\\.\\COM15"));
	devices.Add(wxT("\\\\.\\COM16"));
	devices.Add(wxT("\\\\.\\COM17"));
	devices.Add(wxT("\\\\.\\COM18"));
	devices.Add(wxT("\\\\.\\COM19"));
#else
	devices.Add(wxT("/dev/ttyS0"));
	devices.Add(wxT("/dev/ttyS1"));
	devices.Add(wxT("/dev/ttyS2"));
	devices.Add(wxT("/dev/ttyS3"));
	devices.Add(wxT("/dev/ttyS4"));
	devices.Add(wxT("/dev/ttyUSB0"));
	devices.Add(wxT("/dev/ttyUSB1"));
	devices.Add(wxT("/dev/ttyUSB2"));
	devices.Add(wxT("/dev/ttyUSB3"));
	devices.Add(wxT("/dev/ttyUSB4"));
#endif

	return devices;
}

#if defined(__WINDOWS__)

CSerialController::CSerialController(const wxString& device, unsigned int config, SERIAL_SPEED speed, SERIAL_FORMAT format, SERIAL_CONTROL control) :
m_device(device),
m_config(config),
m_speed(speed),
m_format(format),
m_control(control),
m_rts(false),
m_dtr(false),
m_handle(INVALID_HANDLE_VALUE)
{
	wxASSERT(!device.IsEmpty());
	wxASSERT(config == 1U || config == 2U || config == 3U);
}

CSerialController::~CSerialController()
{
}

bool CSerialController::open()
{
	wxASSERT(m_handle == INVALID_HANDLE_VALUE);

	DWORD errCode;

	m_handle = ::CreateFile(m_device.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (m_handle == INVALID_HANDLE_VALUE) {
		wxLogError(wxT("Cannot open device - %s"), m_device.c_str());
		return false;
	}

	DCB dcb;
	if (::GetCommState(m_handle, &dcb) == 0) {
		wxLogError(wxT("Cannot get the attributes for %s"), m_device.c_str());
		::ClearCommError(m_handle, &errCode, NULL);
		::CloseHandle(m_handle);
		return false;
	}

	dcb.BaudRate = DWORD(m_speed);

	switch (m_format) {
		case SERIAL_8N1:
			dcb.ByteSize = 8;
			dcb.Parity   = NOPARITY;
			dcb.fParity  = FALSE;
			dcb.StopBits = ONESTOPBIT;
			break;
	}

	switch (m_control) {
		case SERIAL_HWFC:
			dcb.fOutxCtsFlow = TRUE;
			dcb.fOutxDsrFlow = TRUE;
			dcb.fDtrControl  = DTR_CONTROL_HANDSHAKE;
			dcb.fRtsControl  = RTS_CONTROL_HANDSHAKE;
			break;
		case SERIAL_NOFC:
			dcb.fOutxCtsFlow = FALSE;
			dcb.fOutxDsrFlow = FALSE;
			dcb.fDtrControl  = DTR_CONTROL_DISABLE;
			dcb.fRtsControl  = RTS_CONTROL_DISABLE;
			break;
	}

	if (::SetCommState(m_handle, &dcb) == 0) {
		wxLogError(wxT("Cannot set the attributes for %s"), m_device.c_str());
		::ClearCommError(m_handle, &errCode, NULL);
		::CloseHandle(m_handle);
		return false;
	}

	if (::EscapeCommFunction(m_handle, CLRDTR) == 0) {
		wxLogError(wxT("Cannot clear DTR for %s"), m_device.c_str());
		::ClearCommError(m_handle, &errCode, NULL);
		::CloseHandle(m_handle);
		return false;
	}

	if (::EscapeCommFunction(m_handle, CLRRTS) == 0) {
		wxLogError(wxT("Cannot clear RTS for %s"), m_device.c_str());
		::ClearCommError(m_handle, &errCode, NULL);
		::CloseHandle(m_handle);
		return false;
	}

	::ClearCommError(m_handle, &errCode, NULL);

	return true;
}

int CSerialController::read(unsigned char* buffer, unsigned int length)
{
	wxASSERT(m_handle != INVALID_HANDLE_VALUE);
	wxASSERT(buffer != NULL);

	if (length == 0U)
		return 0;

	DWORD bytes = 0UL;
	BOOL res = ::ReadFile(m_handle, buffer, length, &bytes, NULL);
	if (!res)
		return -1;

	return int(bytes);
}

int CSerialController::write(const unsigned char* buffer, unsigned int length)
{
	wxASSERT(m_handle != INVALID_HANDLE_VALUE);
	wxASSERT(buffer != NULL);

	if (length == 0U)
		return 0;

	DWORD bytes = 0UL;
	BOOL res = ::WriteFile(m_handle, buffer, length, &bytes, NULL);
	if (!res)
		return -1;

	return int(bytes);
}

bool CSerialController::getCD() const
{
	wxASSERT(m_handle != INVALID_HANDLE_VALUE);

	DWORD status;
	DWORD errCode;
	if (::GetCommModemStatus(m_handle, &status) == 0) {
		::ClearCommError(m_handle, &errCode, NULL);
		return false;
	}

	return (status & MS_RLSD_ON) == MS_RLSD_ON;
}

bool CSerialController::getCTS() const
{
	wxASSERT(m_handle != INVALID_HANDLE_VALUE);

	DWORD status;
	DWORD errCode;
	if (::GetCommModemStatus(m_handle, &status) == 0) {
		::ClearCommError(m_handle, &errCode, NULL);
		return false;
	}

	return (status & MS_CTS_ON) == MS_CTS_ON;
}

bool CSerialController::getDSR() const
{
	wxASSERT(m_handle != INVALID_HANDLE_VALUE);

	DWORD status;
	DWORD errCode;
	if (::GetCommModemStatus(m_handle, &status) == 0) {
		::ClearCommError(m_handle, &errCode, NULL);
		return false;
	}

	return (status & MS_DSR_ON) == MS_DSR_ON;
}

bool CSerialController::setRTS(bool set)
{
	wxASSERT(m_handle != INVALID_HANDLE_VALUE);

	if (set == m_rts)
		return true;

	DWORD rts = (set) ? SETRTS : CLRRTS;
	DWORD errCode;

	if (::EscapeCommFunction(m_handle, rts) == 0) {
		::ClearCommError(m_handle, &errCode, NULL);
		return false;
	}

	m_rts = set;

	return true;
}

bool CSerialController::setDTR(bool set)
{
	wxASSERT(m_handle != INVALID_HANDLE_VALUE);

	if (set == m_dtr)
		return true;

	DWORD dtr = (set) ? SETDTR : CLRDTR;
	DWORD errCode;

	if (::EscapeCommFunction(m_handle, dtr) == 0) {
		::ClearCommError(m_handle, &errCode, NULL);
		return false;
	}

	m_dtr = set;

	return true;
}

void CSerialController::getDigitalInputs(bool& inp1, bool& inp2, bool& inp3, bool& inp4, bool& inp5)
{
	wxASSERT(m_handle != INVALID_HANDLE_VALUE);

	inp1 = inp2 = inp3 = inp4 = inp5 = false;

	DWORD status;
	DWORD errCode;
	if (::GetCommModemStatus(m_handle, &status) == 0) {
		::ClearCommError(m_handle, &errCode, NULL);
		return;
	}

	switch (m_config) {
		case 1U:
			inp1 = (status & MS_DSR_ON) == MS_DSR_ON;
			inp2 = (status & MS_CTS_ON) == MS_CTS_ON;
			break;
		case 2U:
			inp1 = (status & MS_RLSD_ON) == MS_RLSD_ON;
			inp2 = (status & MS_RLSD_ON) == MS_RLSD_ON;
			break;
		case 3U:
			inp1 = (status & MS_RLSD_ON) == MS_RLSD_ON;
			break;
		default:
			wxLogError(wxT("Unknown serial config - %u"), m_config);
			break;
	}
}

void CSerialController::setDigitalOutputs(bool outp1, bool outp2, bool outp3, bool outp4, bool outp5, bool outp6, bool outp7, bool outp8)
{
	wxASSERT(m_handle != INVALID_HANDLE_VALUE);

	switch (m_config) {
		case 1U:
			if (outp1 != m_dtr) {
				DWORD dtr = (outp1) ? SETDTR : CLRDTR;
				DWORD errCode;

				if (::EscapeCommFunction(m_handle, dtr) == 0) {
					::ClearCommError(m_handle, &errCode, NULL);
					return;
				}

				m_dtr = outp1;
			}
			if (outp3 != m_rts) {
				DWORD rts = (outp3) ? SETRTS : CLRRTS;
				DWORD errCode;

				if (::EscapeCommFunction(m_handle, rts) == 0) {
					::ClearCommError(m_handle, &errCode, NULL);
					return;
				}

				m_rts = outp3;
			}
			break;

		case 2U:
		case 3U:
			if (outp1 != m_rts) {
				DWORD rts = (outp1) ? SETRTS : CLRRTS;
				DWORD errCode;

				if (::EscapeCommFunction(m_handle, rts) == 0) {
					::ClearCommError(m_handle, &errCode, NULL);
					return;
				}

				m_rts = outp1;
			}
			if (outp3 != m_dtr) {
				DWORD dtr = (outp3) ? SETDTR : CLRDTR;
				DWORD errCode;

				if (::EscapeCommFunction(m_handle, dtr) == 0) {
					::ClearCommError(m_handle, &errCode, NULL);
					return;
				}

				m_dtr = outp3;
			}
			break;

		default:
			wxLogError(wxT("Unknown serial config - %u"), m_config);
			break;
	}
}

void CSerialController::close()
{
	wxASSERT(m_handle != INVALID_HANDLE_VALUE);

	::CloseHandle(m_handle);
	m_handle = INVALID_HANDLE_VALUE;
}

#else

CSerialController::CSerialController(const wxString& device, unsigned int config, SERIAL_SPEED speed, SERIAL_FORMAT format, SERIAL_CONTROL control) :
m_device(device),
m_config(config),
m_speed(speed),
m_format(format),
m_control(control),
m_rts(false),
m_dtr(false),
m_fd(-1)
{
	wxASSERT(!device.IsEmpty());
	wxASSERT(config == 1U || config == 2U || config == 3U);
}

CSerialController::~CSerialController()
{
}

bool CSerialController::open()
{
	wxASSERT(m_fd == -1);

	m_fd = ::open(m_device.mb_str(), O_RDWR | O_NOCTTY | O_NDELAY, 0);
	if (m_fd < 0) {
		wxLogError(wxT("Cannot open device - %s"), m_device.c_str());
		return false;
	}

	if (::isatty(m_fd) == 0) {
		wxLogError(wxT("%s is not a TTY device"), m_device.c_str());
		::close(m_fd);
		return false;
	}

	termios termios;
	if (::tcgetattr(m_fd, &termios) < 0) {
		wxLogError(wxT("Cannot get the attributes for %s"), m_device.c_str());
		::close(m_fd);
		return false;
	}

	// FIXME XXX unfinished
	termios.c_cflag |= (CLOCAL | CREAD);
	termios.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	termios.c_oflag &= ~OPOST;
	termios.c_cc[VMIN]  = 0;
	termios.c_cc[VTIME] = 10;

	switch (m_format) {
		case SERIAL_8N1:
			termios.c_cflag &= ~(CSIZE | PARENB | CSTOPB);
			termios.c_cflag |= CS8;
			break;
	}

	switch (m_control) {
		case SERIAL_HWFC:
			termios.c_cflag |= CRTSCTS;
			break;
		case SERIAL_NOFC:
			termios.c_cflag &= ~CRTSCTS;
			break;
	}

	switch (m_speed) {
		case SERIAL_1200:
			::cfsetospeed(&termios, B1200);
			::cfsetispeed(&termios, B1200);
			break;
		case SERIAL_2400:
			::cfsetospeed(&termios, B2400);
			::cfsetispeed(&termios, B2400);
			break;
		case SERIAL_4800:
			::cfsetospeed(&termios, B4800);
			::cfsetispeed(&termios, B4800);
			break;
		case SERIAL_9600:
			::cfsetospeed(&termios, B9600);
			::cfsetispeed(&termios, B9600);
			break;
		case SERIAL_19200:
			::cfsetospeed(&termios, B19200);
			::cfsetispeed(&termios, B19200);
			break;
		case SERIAL_38400:
			::cfsetospeed(&termios, B230400);
			::cfsetispeed(&termios, B230400);
			break;
		case SERIAL_76800:
			::cfsetospeed(&termios, B230400);
			::cfsetispeed(&termios, B230400);
			break;
		case SERIAL_230400:
			::cfsetospeed(&termios, B230400);
			::cfsetispeed(&termios, B230400);
			break;
		default:
			wxLogError(wxT("Unsupported serial port speed - %d"), int(m_speed));
			::close(m_fd);
			return false;
	}

	if (::tcsetattr(m_fd, TCSANOW, &termios) < 0) {
		wxLogError(wxT("Cannot set the attributes for %s"), m_device.c_str());
		::close(m_fd);
		return false;
	}

	unsigned int y;
	if (::ioctl(m_fd, TIOCMGET, &y) < 0) {
		wxLogError(wxT("Cannot get the modem status bits for %s"), m_device.c_str());
		::close(m_fd);
		return false;
	}

	y &= ~TIOCM_DTR;
	y &= ~TIOCM_RTS;

	if (::ioctl(m_fd, TIOCMSET, &y) < 0) {
		wxLogError(wxT("Cannot set the modem status bits for %s"), m_device.c_str());
		::close(m_fd);
		return false;
	}

	return true;
}

int CSerialController::read(unsigned char* buffer, unsigned int length)
{
	wxASSERT(m_fd != -1);
	wxASSERT(buffer != NULL);

	if (length == 0U)
		return 0;

	ssize_t n = 0;
	while (n <= 0) {
		n = ::read(m_fd, buffer, length);
		if (n < 0 && errno != EAGAIN) {
			wxLogError(wxT("Error returned from read(), errno=%d"), errno);
			return -1;
		}
		if (errno == EAGAIN || n == 0)
			::wxMilliSleep(5UL);
	}

	return n;
}

int CSerialController::write(const unsigned char* buffer, unsigned int length)
{
	wxASSERT(m_fd != -1);
	wxASSERT(buffer != NULL);

	if (length == 0U)
		return 0;

	ssize_t n = 0;
	while (n <= 0) {
		n = ::write(m_fd, buffer, length);
		if (n < 0 && errno != EAGAIN) {
			wxLogError(wxT("Error returned from write(), errno=%d"), errno);
			return -1;
		}
		if (errno == EAGAIN || n == 0)
			::wxMilliSleep(5UL);
	}

	return n;
}

bool CSerialController::getCD() const
{
	wxASSERT(m_fd != -1);

	unsigned int y;
	if (::ioctl(m_fd, TIOCMGET, &y) < 0)
		return false;

	return (y & TIOCM_CD) == TIOCM_CD;
}

bool CSerialController::getCTS() const
{
	wxASSERT(m_fd != -1);

	unsigned int y;
	if (::ioctl(m_fd, TIOCMGET, &y) < 0)
		return false;

	return (y & TIOCM_CTS) == TIOCM_CTS;
}

bool CSerialController::getDSR() const
{
	wxASSERT(m_fd != -1);

	unsigned int y;
	if (::ioctl(m_fd, TIOCMGET, &y) < 0)
		return false;

	return (y & TIOCM_DSR) == TIOCM_DSR;
}

bool CSerialController::setRTS(bool set)
{
	wxASSERT(m_fd != -1);

	if (set == m_rts)
		return true;

	unsigned int y;
	if (::ioctl(m_fd, TIOCMGET, &y) < 0)
		return false;

	if (set)
		y |= TIOCM_RTS;
	else
		y &= ~TIOCM_RTS;

	if (::ioctl(m_fd, TIOCMSET, &y) < 0)
		return false;

	m_rts = set;

	return true;
}

bool CSerialController::setDTR(bool set)
{
	wxASSERT(m_fd != -1);

	if (set == m_dtr)
		return true;

	unsigned int y;
	if (::ioctl(m_fd, TIOCMGET, &y) < 0)
		return false;

	if (set)
		y |= TIOCM_DTR;
	else
		y &= ~TIOCM_DTR;

	if (::ioctl(m_fd, TIOCMSET, &y) < 0)
		return false;

	m_dtr = set;

	return true;
}

void CSerialController::getDigitalInputs(bool& inp1, bool& inp2, bool& inp3, bool& inp4, bool& inp5)
{
	wxASSERT(m_fd != -1);

	inp1 = inp2 = inp3 = inp4 = inp5 = false;

	unsigned int y;
	if (::ioctl(m_fd, TIOCMGET, &y) < 0)
		return;

	switch (m_config) {
		case 1U:
			inp1 = (y & TIOCM_DSR) == TIOCM_DSR;
			inp2 = (y & TIOCM_CTS) == TIOCM_CTS;
			break;
		case 2U:
			inp1 = (y & TIOCM_CD) == TIOCM_CD;
			inp2 = (y & TIOCM_CD) == TIOCM_CD;
			break;
		case 3U:
			inp1 = (y & TIOCM_CD) == TIOCM_CD;
			break;
		default:
			wxLogError(wxT("Unknown serial config - %u"), m_config);
			break;
	}
}

void CSerialController::setDigitalOutputs(bool outp1, bool outp2, bool outp3, bool outp4, bool outp5, bool outp6, bool outp7, bool outp8)
{
	wxASSERT(m_fd != -1);

	if (m_config == 1U) {
		if (outp1 == m_dtr && outp3 == m_rts)
			return;

		unsigned int y;
		if (::ioctl(m_fd, TIOCMGET, &y) < 0)
			return;

		if (outp1)
			y |= TIOCM_DTR;
		else
			y &= ~TIOCM_DTR;

		if (outp3)
			y |= TIOCM_RTS;
		else
			y &= ~TIOCM_RTS;

		if (::ioctl(m_fd, TIOCMSET, &y) < 0)
			return;

		m_dtr = outp1;
		m_rts = outp3;
	} else if (m_config == 2U || m_config == 3U) {
		if (outp1 == m_rts && outp3 == m_dtr)
			return;

		unsigned int y;
		if (::ioctl(m_fd, TIOCMGET, &y) < 0)
			return;

		if (outp1)
			y |= TIOCM_RTS;
		else
			y &= ~TIOCM_RTS;

		if (outp3)
			y |= TIOCM_DTR;
		else
			y &= ~TIOCM_DTR;

		if (::ioctl(m_fd, TIOCMSET, &y) < 0)
			return;

		m_rts = outp1;
		m_dtr = outp3;
	} else {
		wxLogError(wxT("Unknown serial config - %u"), m_config);
	}
}

void CSerialController::close()
{
	wxASSERT(m_fd != -1);

	::close(m_fd);
	m_fd = -1;
}

#endif

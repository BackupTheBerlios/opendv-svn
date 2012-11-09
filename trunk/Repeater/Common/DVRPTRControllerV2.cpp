/*
 *   Copyright (C) 2011,2012 by Jonathan Naylor G4KLX
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

// TODO Handle flags 102 + 103

#include "DVRPTRControllerV2.h"
#include "DStarDefines.h"
#include "Timer.h"

#if defined(__WINDOWS__)
#include <setupapi.h>
#else
#include <wx/dir.h>
#endif

const unsigned int MAX_RESPONSES = 30U;

wxArrayString CDVRPTRControllerV2::getDevices()
{
#if defined(__WINDOWS__)
	return CSerialDataController::getDevices();
#else
	wxArrayString devices;
	devices.Alloc(5U);

	devices.Add(wxT("/dev/ttyACM0"));
	devices.Add(wxT("/dev/ttyACM1"));
	devices.Add(wxT("/dev/ttyACM2"));
	devices.Add(wxT("/dev/ttyACM3"));
	devices.Add(wxT("/dev/ttyACM4"));
	devices.Add(wxT("/dev/ttyACM5"));
	devices.Add(wxT("/dev/ttyACM6"));
	devices.Add(wxT("/dev/ttyACM7"));
	devices.Add(wxT("/dev/ttyACM8"));
	devices.Add(wxT("/dev/ttyACM9"));

	return devices;
#endif
}

const unsigned int BUFFER_LENGTH = 200U;

CDVRPTRControllerV2::CDVRPTRControllerV2(const wxString& port, const wxString& path, bool txInvert, unsigned int modLevel, bool duplex, const wxString& callsign) :
wxThread(wxTHREAD_JOINABLE),
m_port(port),
m_path(path),
m_txInvert(txInvert),
m_modLevel(modLevel),
m_duplex(duplex),
m_callsign(callsign),
m_serial(port, SERIAL_115200),
m_buffer(NULL),
m_rxData(1000U),
m_txData(1000U),
m_counter(0U),
m_ptt(false),
m_rx(false),
m_space(0U),
m_stopped(false),
m_mutex()
{
	wxASSERT(!port.IsEmpty());

	m_buffer = new unsigned char[BUFFER_LENGTH];
}

CDVRPTRControllerV2::~CDVRPTRControllerV2()
{
	delete[] m_buffer;
}

bool CDVRPTRControllerV2::open()
{
	findPort();

	bool ret = openModem();
	if (!ret)
		return false;

	findPath();

	Create();
	SetPriority(100U);
	Run();

	return true;
}

void* CDVRPTRControllerV2::Entry()
{
	wxLogMessage(wxT("Starting DV-RPTR2 Modem Controller thread"));

	// Clock every 5ms-ish
	CTimer pollTimer(200U, 0U, 250U);

	pollTimer.start();

	while (!m_stopped) {
		// Poll the modem status every 250ms
		if (pollTimer.hasExpired()) {
			bool ret = getSpace();
			if (!ret) {
				ret = findModem();
				if (!ret) {
					wxLogMessage(wxT("Stopping DV-RPTR2 Modem Controller thread"));
					return NULL;
				}
			}

			pollTimer.reset();
		}

		unsigned int length;
		RESP_TYPE_V2 type = getResponse(m_buffer, length);

		switch (type) {
			case RT2_TIMEOUT:
				break;

			case RT2_ERROR: {
					bool ret = findModem();
					if (!ret) {
						wxLogMessage(wxT("Stopping DV-RPTR2 Modem Controller thread"));
						return NULL;
					}
				}
				break;

			case RT2_HEADER: {
					// CUtils::dump(wxT("RT2_HEADER"), m_buffer, length);
					m_mutex.Lock();

					unsigned int space = m_rxData.freeSpace();
					if (space < 57U) {
						wxLogMessage(wxT("Out of space in the DV-RPTR RX queue"));
					} else {
						unsigned char data[2U];
						data[0U] = DQT_HEADER;
						data[1U] = RADIO_HEADER_LENGTH_BYTES;
						m_rxData.addData(data, 2U);
						m_rxData.addData(m_buffer + 9U, RADIO_HEADER_LENGTH_BYTES - 2U);

						// Dummy checksum
						data[0U] = 0xFFU;
						data[1U] = 0xFFU;
						m_rxData.addData(data, 2U);

						data[0U] = DQT_DATA;
						data[1U] = DV_FRAME_LENGTH_BYTES;
						m_rxData.addData(data, 2U);
						m_rxData.addData(m_buffer + 51U, DV_FRAME_LENGTH_BYTES);

						m_rx = true;
					}

					m_mutex.Unlock();
				}
				break;

			case RT2_DATA_LONG: {
					// CUtils::dump(wxT("RT2_DATA_LONG"), m_buffer, length);
					m_mutex.Lock();

					unsigned int space = m_rxData.freeSpace();
					if (space < 16U) {
						wxLogMessage(wxT("Out of space in the DV-RPTR RX queue"));
					} else {
						unsigned char data[2U];
						data[0U] = DQT_DATA;
						data[1U] = DV_FRAME_LENGTH_BYTES;
						m_rxData.addData(data, 2U);
						m_rxData.addData(m_buffer + 51U, DV_FRAME_LENGTH_BYTES);

						m_rx = true;

						// End of transmission?
						if ((m_buffer[50U] & 0x40U) == 0x40U) {
							data[0U] = DQT_EOT;
							data[1U] = 0U;
							m_rxData.addData(data, 2U);

							m_rx = false;
						}
					}

					m_mutex.Unlock();
				}
				break;

			case RT2_DATA_SHORT: {
					// CUtils::dump(wxT("RT2_DATA_SHORT"), m_buffer, length);
					m_mutex.Lock();

					unsigned int space = m_rxData.freeSpace();
					if (space < 16U) {
						wxLogMessage(wxT("Out of space in the DV-RPTR RX queue"));
					} else {
						unsigned char data[2U];
						data[0U] = DQT_DATA;
						data[1U] = DV_FRAME_LENGTH_BYTES;
						m_rxData.addData(data, 2U);
						m_rxData.addData(m_buffer + 5U, DV_FRAME_LENGTH_BYTES);

						m_rx = true;

						// End of transmission?
						if ((m_buffer[19U] & 0x40U) == 0x40U) {
							data[0U] = DQT_EOT;
							data[1U] = 0U;
							m_rxData.addData(data, 2U);

							m_rx = false;
						}
					}

					m_mutex.Unlock();
				}
				break;

			case RT2_SPACE:
				m_space = m_buffer[9U];
				// CUtils::dump(wxT("RT2_SPACE"), m_buffer, length);
				break;

			// These should not be received in this loop, but don't complain if we do
			case RT2_QUERY:
			case RT2_CONFIG:
				break;

			default:
				wxLogMessage(wxT("Unknown DV-RPTR2 message, type"));
				CUtils::dump(wxT("Buffer dump"), m_buffer, length);
				break;
		}

		if (m_space > 0U) {
			if (!m_txData.isEmpty()) {
				m_mutex.Lock();

				unsigned char len = 0U;
				m_txData.getData(&len, 1U);

				unsigned char data[200U];
				m_txData.getData(data, len);

				m_mutex.Unlock();

				// CUtils::dump(wxT("Write"), data, len);

				int ret = m_serial.write(data, len);
				if (ret == -1) {
					bool ret = findModem();
					if (!ret) {
						wxLogMessage(wxT("Stopping DV-RPTR2 Modem Controller thread"));
						return NULL;
					}
				} else {
					m_space--;
				}
			}
		}

		Sleep(5UL);

		pollTimer.clock();
	}

	wxLogMessage(wxT("Stopping DV-RPTR2 Modem Controller thread"));

	m_serial.close();

	return NULL;
}

void CDVRPTRControllerV2::purgeRX()
{
	wxMutexLocker locker(m_mutex);

	m_rxData.clear();
}

void CDVRPTRControllerV2::purgeTX()
{
	wxMutexLocker locker(m_mutex);

	m_txData.clear();
}

DATA_QUEUE_TYPE CDVRPTRControllerV2::readQueue(unsigned char* data, unsigned int& length)
{
	wxMutexLocker locker(m_mutex);

	if (m_rxData.isEmpty())
		return DQT_NONE;

	unsigned char header[2U];
	m_rxData.getData(header, 2U);

	DATA_QUEUE_TYPE type = DATA_QUEUE_TYPE(header[0U]);

	length = header[1U];

	if (length > 0U)
		m_rxData.getData(data, length);

	return type;
}

bool CDVRPTRControllerV2::writeHeader(const CHeaderData& header)
{
	unsigned char buffer[105U];

	::memset(buffer, 0x00U, 105U);

	buffer[0U] = 'H';
	buffer[1U] = 'E';
	buffer[2U] = 'A';
	buffer[3U] = 'D';
	buffer[4U] = 'X';
	buffer[5U] = '0';
	buffer[6U] = '0';
	buffer[7U] = '0';
	buffer[8U] = '2';

	::memset(buffer + 9U, ' ', RADIO_HEADER_LENGTH_BYTES);

	buffer[9U]  = header.getFlag1();
	buffer[10U] = header.getFlag2();
	buffer[11U] = header.getFlag3();

	wxString rpt2 = header.getRptCall2();
	for (unsigned int i = 0U; i < rpt2.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer[i + 12U]  = rpt2.GetChar(i);

	wxString rpt1 = header.getRptCall1();
	for (unsigned int i = 0U; i < rpt1.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer[i + 20U] = rpt1.GetChar(i);

	wxString your = header.getYourCall();
	for (unsigned int i = 0U; i < your.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer[i + 28U] = your.GetChar(i);

	wxString my1 = header.getMyCall1();
	for (unsigned int i = 0U; i < my1.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer[i + 36U] = my1.GetChar(i);

	wxString my2 = header.getMyCall2();
	for (unsigned int i = 0U; i < my2.Len() && i < SHORT_CALLSIGN_LENGTH; i++)
		buffer[i + 44U] = my2.GetChar(i);

	wxMutexLocker locker(m_mutex);

	unsigned int space = m_txData.freeSpace();
	if (space < 106U)
		return false;

	unsigned char len = 105U;
	m_txData.addData(&len, 1U);

	m_txData.addData(buffer, 105U);

	m_ptt = true;

	return true;
}

bool CDVRPTRControllerV2::writeData(const unsigned char* data, unsigned int length, bool end)
{
	unsigned char buffer[17U];

	::memset(buffer, 0x00U, 17U);

	buffer[0U] = 'H';
	buffer[1U] = 'E';
	buffer[2U] = 'A';
	buffer[3U] = 'D';
	buffer[4U] = 'Z';

	::memcpy(buffer + 5U, data, DV_FRAME_LENGTH_BYTES);

	if (end) {
		buffer[14U] = 0x55U;
		buffer[15U] = 0x55U;
		buffer[16U] = 0x55U;

		m_ptt = false;
	}

	wxMutexLocker locker(m_mutex);

	unsigned int space = m_txData.freeSpace();
	if (space < 18U)
		return false;

	unsigned char len = 17U;
	m_txData.addData(&len, 1U);

	m_txData.addData(buffer, 17U);

	return true;
}

bool CDVRPTRControllerV2::getPTT() const
{
	return m_ptt;
}

bool CDVRPTRControllerV2::hasSpace()
{
	wxMutexLocker locker(m_mutex);

	return m_txData.freeSpace() > 200U;
}

void CDVRPTRControllerV2::close()
{
	m_stopped = true;

	Wait();
}

bool CDVRPTRControllerV2::getSerial()
{
	unsigned char buffer[105U];

	::memset(buffer, 0x00U, 105U);

	buffer[0U] = 'H';
	buffer[1U] = 'E';
	buffer[2U] = 'A';
	buffer[3U] = 'D';
	buffer[4U] = 'X';
	buffer[5U] = '9';
	buffer[6U] = '0';
	buffer[7U] = '0';
	buffer[8U] = '0';

	// CUtils::dump(wxT("Written"), buffer, 105U);

	int ret = m_serial.write(buffer, 105U);
	if (ret != 105)
		return false;

	unsigned int count = 0U;
	unsigned int length;
	RESP_TYPE_V2 resp;
	do {
		::wxMilliSleep(10UL);

		resp = getResponse(m_buffer, length);

		if (resp != RT2_QUERY) {
			count++;
			if (count >= MAX_RESPONSES) {
				wxLogError(wxT("The DV-RPTR modem is not responding to the query command"));
				return false;
			}
		}
	} while (resp != RT2_QUERY);

	wxLogInfo(wxT("DV-RPTR Modem Hardware serial: 0x%02X%02X%02x%02X"), m_buffer[9U], m_buffer[10U], m_buffer[11U], m_buffer[12U]);

	return true;
}

bool CDVRPTRControllerV2::getSpace()
{
	unsigned char buffer[10U];

	buffer[0U] = 'H';
	buffer[1U] = 'E';
	buffer[2U] = 'A';
	buffer[3U] = 'D';
	buffer[4U] = 'Y';
	buffer[5U] = '9';
	buffer[6U] = '0';
	buffer[7U] = '1';
	buffer[8U] = '1';
	buffer[9U] = 0x00U;

	// CUtils::dump(wxT("Written"), buffer, 10U);

	return m_serial.write(buffer, 10U) == 10;
}

bool CDVRPTRControllerV2::setConfig()
{
	unsigned char buffer[105U];

	::memset(buffer, 0x00U, 105U);

	buffer[0U] = 'H';
	buffer[1U] = 'E';
	buffer[2U] = 'A';
	buffer[3U] = 'D';
	buffer[4U] = 'X';
	buffer[5U] = '9';
	buffer[6U] = '0';
	buffer[7U] = '0';
	buffer[8U] = '1';

	::memset(buffer + 9U, ' ', LONG_CALLSIGN_LENGTH);
	for (unsigned int i = 0U; i < LONG_CALLSIGN_LENGTH && i < m_callsign.Len(); i++)
		buffer[9U + i] = m_callsign.GetChar(i);

	buffer[65U] = m_duplex ? 0x03U : 0x00U;

	buffer[66U] = m_txInvert ? 0x01U : 0x00U;

	buffer[73U] = (m_modLevel * 256U) / 100U;

	buffer[87U] = 0x01U;

	// CUtils::dump(wxT("Written"), buffer, 105U);

	int ret = m_serial.write(buffer, 105U);
	if (ret != 105)
		return false;

	unsigned int count = 0U;
	unsigned int length;
	RESP_TYPE_V2 resp;
	do {

		::wxMilliSleep(10UL);

		resp = getResponse(m_buffer, length);

		if (resp != RT2_CONFIG) {
			count++;
			if (count >= MAX_RESPONSES) {
				wxLogError(wxT("The DV-RPTR modem is not responding to the configure command"));
				return false;
			}
		}
	} while (resp != RT2_CONFIG);

	// CUtils::dump(wxT("Response"), m_buffer, length);

	wxString firmware((char*)(m_buffer + 9U), wxConvLocal);

	wxLogInfo(wxT("DV-RPTR Modem Firmware version: %s"), firmware.c_str());

	return true;
}

RESP_TYPE_V2 CDVRPTRControllerV2::getResponse(unsigned char *buffer, unsigned int& length)
{
	// Get the start of the frame or nothing at all
	int ret = m_serial.read(buffer, 5U);
	if (ret < 0) {
		wxLogError(wxT("Error when reading from the DV-RPTR"));
		return RT2_ERROR;
	}

	if (ret == 0)
		return RT2_TIMEOUT;

	if (::memcmp(buffer + 0U, "HEAD", 4U) != 0) {
		wxLogError(wxT("DV-RPTR frame start is incorrect - 0x%02X 0x%02X 0x%02X 0x%02X"), buffer[0U], buffer[1U], buffer[2U], buffer[3U]);
		return RT2_UNKNOWN;
	}

	if (buffer[4U] == 'X') {
		length = 105U;
	} else if (buffer[4U] == 'Y') {
		length = 10U;
	} else if (buffer[4U] == 'Z') {
		length = 20U;
	} else {
		wxLogError(wxT("DV-RPTR frame type is incorrect - 0x%02X"), buffer[4U]);
		return RT2_UNKNOWN;
	}

	unsigned int offset = 5U;

	while (offset < length) {
		int ret = m_serial.read(buffer + offset, length - offset);
		if (ret < 0) {
			wxLogError(wxT("Error when reading from the DV-RPTR"));
			return RT2_ERROR;
		}

		if (ret > 0)
			offset += ret;

		if (ret == 0)
			Sleep(5UL);
	}

	// CUtils::dump(wxT("Received"), buffer, length);

	if (::memcmp(buffer + 0U, "HEADZ", 5U) == 0) {
		return RT2_DATA_SHORT;
	} else if (::memcmp(buffer + 5U, "0001", 4U) == 0) {
		if (buffer[104U] == 0x01U)
			return RT2_HEADER;
		else
			return RT2_DATA_LONG;
	} else if (::memcmp(buffer + 5U, "9900", 4U) == 0) {
		return RT2_QUERY;
	} else if (::memcmp(buffer + 5U, "9001", 4U) == 0) {
		return RT2_CONFIG;
	} else if (::memcmp(buffer + 5U, "9011", 4U) == 0) {
		return RT2_SPACE;
	} else {
		wxLogError(wxT("DV-RPTR frame type number is incorrect - 0x%02X 0x%02X 0x%02X 0x%02X"), buffer[5U], buffer[6U], buffer[7U], buffer[8U]);
		return RT2_UNKNOWN;
	}
}

wxString CDVRPTRControllerV2::getPath() const
{
	return m_path;
}

bool CDVRPTRControllerV2::findPort()
{
	if (m_path.IsEmpty())
		return false;

#if defined(__WINDOWS__)
#else
	wxDir dir;
	bool ret1 = dir.Open(wxT("/sys/class/tty"));
	if (!ret1) {
		wxLogError(wxT("Cannot open directory /sys/class/tty"));
		return false;
	}

	wxString fileName;
	ret1 = dir.GetFirst(&fileName, wxT("ttyACM*"));
	while (ret1) {
		wxString path;
		path.Printf(wxT("/sys/class/tty/%s"), fileName.c_str());

		char cpath[255U];
		::memset(cpath, 0x00U, 255U);

		for (unsigned int i = 0U; i < path.Len(); i++)
			cpath[i] = path.GetChar(i);

		char symlink[255U];
		int ret2 = ::readlink(cpath, symlink, 255U);
		if (ret2 < 0) {
			::strcat(cpath, "/device");
			ret2 = ::readlink(cpath, symlink, 255U);
			if (ret2 < 0) {
				wxLogError(wxT("Error from readlink()"));
				return false;
			}

			path = wxString(symlink, wxConvLocal, ret2);
		} else {
			// Get all but the last section
			wxString fullPath = wxString(symlink, wxConvLocal, ret2);
			path = fullPath.BeforeLast(wxT('/'));
		}

		if (path.IsSameAs(m_path)) {
			m_port.Printf(wxT("/dev/%s"), fileName.c_str());

			wxLogMessage(wxT("Found modem port of %s based on the path"), m_port.c_str());

			return true;
		}

		ret1 = dir.GetNext(&fileName);
	}
#endif

	return false;
}

bool CDVRPTRControllerV2::findPath()
{
#if defined(__WINDOWS__)
#ifdef notdef
	GUID guids[5U];

	DWORD count;
	BOOL res = ::SetupDiClassGuidsFromName(L"Multifunction", guids, 5U, &count);
	if (!res) {
		wxLogError(wxT("Error from SetupDiClassGuidsFromName: err=%u"), ::GetLastError());
		return false;
	}

	for (DWORD i = 0U; i < count; i++) {
		HDEVINFO devInfo = ::SetupDiGetClassDevs(&guids[i], NULL, NULL, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);
		if (devInfo == INVALID_HANDLE_VALUE) {
			wxLogError(wxT("Error from SetupDiGetClassDevs: err=%u"), ::GetLastError());
			return false;
		}

		SP_DEVICE_INTERFACE_DATA devInfoData;
		devInfoData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

		for (DWORD index = 0U; ::SetupDiEnumDeviceInterfaces(devInfo, NULL, &guids[i], index, &devInfoData); index++) {
			// Find the required length of the device structure
			DWORD length;
			::SetupDiGetDeviceInterfaceDetail(devInfo, &devInfoData, NULL, 0U, &length, NULL);

			PSP_DEVICE_INTERFACE_DETAIL_DATA detailData = PSP_DEVICE_INTERFACE_DETAIL_DATA(::malloc(length));
			detailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

			// Get the detailed data into the newly allocated device structure
			DWORD required;
			res = ::SetupDiGetDeviceInterfaceDetail(devInfo, &devInfoData, detailData, length, &required, NULL);
			if (!res) {
				wxLogError(wxT("Error from SetupDiGetDeviceInterfaceDetail: err=%u"), ::GetLastError());
				::SetupDiDestroyDeviceInfoList(devInfo);
				::free(detailData);
				return false;
			}

			::free(detailData);
		}

		::SetupDiDestroyDeviceInfoList(devInfo);
	}

	return false;
#endif
#else
	wxString path;
	path.Printf(wxT("/sys/class/tty/%s"), m_port.Mid(5U).c_str());	

	char cpath[255U];
	::memset(cpath, 0x00U, 255U);

	for (unsigned int i = 0U; i < path.Len(); i++)
		cpath[i] = path.GetChar(i);

	char symlink[255U];
	int ret = ::readlink(cpath, symlink, 255U);
	if (ret < 0) {
		::strcat(cpath, "/device");
		ret = ::readlink(cpath, symlink, 255U);
		if (ret < 0) {
			wxLogError(wxT("Error from readlink()"));
			return false;
		}

		path = wxString(symlink, wxConvLocal, ret);
	} else {
		wxString fullPath = wxString(symlink, wxConvLocal, ret);
		path = fullPath.BeforeLast(wxT('/'));
	}

	if (m_path.IsEmpty())
		wxLogMessage(wxT("Found modem path of %s"), path.c_str());

	m_path = path;
#endif

	return true;
}

bool CDVRPTRControllerV2::findModem()
{
	m_serial.close();

	// Tell the repeater that the signal has gone away
	if (m_rx) {
		purgeRX();

		m_mutex.Lock();
		unsigned char data[2U];
		data[0U] = DQT_EOT;
		data[1U] = 0U;
		m_rxData.addData(data, 2U);
		m_mutex.Unlock();

		m_rx = false;
	}

	unsigned int count = 0U;

	// Purge the transmit buffer every 500ms to avoid overflow, but only try and reopen the modem every 2s
	while (!m_stopped) {
		purgeTX();

		count++;
		if (count >= 4U) {
			wxLogMessage(wxT("Trying to reopen the modem"));

			bool ret = findPort();
			if (ret) {
				ret = openModem();
				if (ret)
					return true;
			}

			count = 0U;
		}

		Sleep(500UL);
	}

	return false;
}

bool CDVRPTRControllerV2::openModem()
{
	bool ret = m_serial.open();
	if (!ret)
		return false;

	ret = getSerial();
	if (!ret) {
		m_serial.close();
		return false;
	}

	ret = setConfig();
	if (!ret) {
		m_serial.close();
		return false;
	}

	return true;
}


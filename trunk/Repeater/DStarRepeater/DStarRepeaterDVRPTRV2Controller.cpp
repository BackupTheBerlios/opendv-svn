/*
 *   Copyright (C) 2011,2012,2013 by Jonathan Naylor G4KLX
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

#include "DStarRepeaterDVRPTRV2Controller.h"
#include "DStarDefines.h"
#include "Timer.h"

#if defined(__WINDOWS__)
#include <setupapi.h>
#else
#include <wx/dir.h>
#endif

const unsigned int MAX_RESPONSES = 30U;

wxArrayString CDStarRepeaterDVRPTRV2Controller::getDevices()
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

CDStarRepeaterDVRPTRV2Controller::CDStarRepeaterDVRPTRV2Controller(const wxString& port, const wxString& path, bool txInvert, unsigned int modLevel, bool duplex, const wxString& callsign) :
wxThread(wxTHREAD_JOINABLE),
m_connection(CT_USB),
m_usbPort(port),
m_usbPath(path),
m_address(),
m_port(0U),
m_txInvert(txInvert),
m_modLevel(modLevel),
m_duplex(duplex),
m_callsign(callsign),
m_usb(NULL),
m_network(NULL),
m_buffer(NULL),
m_rxData(1000U),
m_txData(1000U),
m_tx(false),
m_rx(false),
m_space(0U),
m_stopped(false),
m_mutex(),
m_readType(DSMTT_NONE),
m_readLength(0U),
m_readBuffer(NULL)
{
	wxASSERT(!port.IsEmpty());

	m_usb = new CSerialDataController(port, SERIAL_115200);

	m_buffer = new unsigned char[BUFFER_LENGTH];
	m_readBuffer = new unsigned char[BUFFER_LENGTH];
}

CDStarRepeaterDVRPTRV2Controller::CDStarRepeaterDVRPTRV2Controller(const wxString& address, unsigned int port, bool txInvert, unsigned int modLevel, bool duplex, const wxString& callsign) :
wxThread(wxTHREAD_JOINABLE),
m_connection(CT_NETWORK),
m_usbPort(),
m_usbPath(),
m_address(address),
m_port(port),
m_txInvert(txInvert),
m_modLevel(modLevel),
m_duplex(duplex),
m_callsign(callsign),
m_usb(NULL),
m_network(NULL),
m_buffer(NULL),
m_rxData(1000U),
m_txData(1000U),
m_tx(false),
m_rx(false),
m_space(0U),
m_stopped(false),
m_mutex(),
m_readType(DSMTT_NONE),
m_readLength(0U),
m_readBuffer(NULL)
{
	wxASSERT(!address.IsEmpty());
	wxASSERT(port > 0U);

	m_network = new CTCPReaderWriter(address, port);

	m_buffer = new unsigned char[BUFFER_LENGTH];
	m_readBuffer = new unsigned char[BUFFER_LENGTH];
}

CDStarRepeaterDVRPTRV2Controller::~CDStarRepeaterDVRPTRV2Controller()
{
	delete m_usb;
	delete m_network;

	delete[] m_buffer;
	delete[] m_readBuffer;
}

bool CDStarRepeaterDVRPTRV2Controller::start()
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

void* CDStarRepeaterDVRPTRV2Controller::Entry()
{
	wxLogMessage(wxT("Starting DV-RPTR2 Modem Controller thread"));

	// Clock every 5ms-ish
	CTimer pollTimer(200U, 0U, 250U);

	pollTimer.start();

	while (!m_stopped) {
		// Poll the modem status every 250ms
		if (pollTimer.hasExpired()) {
			bool ret = readSpace();
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
					wxMutexLocker locker(m_mutex);

					unsigned char data[2U];
					data[0U] = DSMTT_HEADER;
					data[1U] = RADIO_HEADER_LENGTH_BYTES;
					m_rxData.addData(data, 2U);

					m_rxData.addData(m_buffer + 9U, RADIO_HEADER_LENGTH_BYTES - 2U);

					// Dummy checksum
					data[0U] = 0xFFU;
					data[1U] = 0xFFU;
					m_rxData.addData(data, 2U);

					data[0U] = DSMTT_DATA;
					data[1U] = DV_FRAME_LENGTH_BYTES;
					m_rxData.addData(data, 2U);

					m_rxData.addData(m_buffer + 51U, DV_FRAME_LENGTH_BYTES);

					m_rx = true;
				}
				break;

			case RT2_DATA: {
					// CUtils::dump(wxT("RT2_DATA"), m_buffer, length);
					wxMutexLocker locker(m_mutex);

					unsigned char data[2U];
					data[0U] = DSMTT_DATA;
					data[1U] = DV_FRAME_LENGTH_BYTES;
					m_rxData.addData(data, 2U);

					m_rxData.addData(m_buffer + 5U, DV_FRAME_LENGTH_BYTES);

					m_rx = true;

					// End of transmission?
					bool end = (m_buffer[19U] & 0x40U) == 0x40U;
					if (end) {
						data[0U] = DSMTT_EOT;
						data[1U] = 0U;
						m_rxData.addData(data, 2U);

						m_rx = false;
					}
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
			if (m_txData.hasData()) {
				m_mutex.Lock();

				unsigned char len = 0U;
				m_txData.getData(&len, 1U);

				unsigned char data[200U];
				m_txData.getData(data, len);

				m_mutex.Unlock();

				// CUtils::dump(wxT("Write"), data, len);

				bool ret = writeModem(data, len);
				if (!ret) {
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

	closeModem();

	return NULL;
}

DSMT_TYPE CDStarRepeaterDVRPTRV2Controller::read()
{
	m_readLength = 0U;

	if (m_rxData.isEmpty())
		return DSMTT_NONE;

	wxMutexLocker locker(m_mutex);

	unsigned char hdr[2U];
	m_rxData.getData(hdr, 2U);

	m_readType   = DSMT_TYPE(hdr[0U]);
	m_readLength = hdr[1U];
	m_rxData.getData(m_readBuffer, m_readLength);

	return m_readType;
}

CHeaderData* CDStarRepeaterDVRPTRV2Controller::readHeader()
{
	if (m_readType != DSMTT_HEADER)
		return NULL;

	return new CHeaderData(m_readBuffer, RADIO_HEADER_LENGTH_BYTES, false);
}

unsigned int CDStarRepeaterDVRPTRV2Controller::readData(unsigned char* data, unsigned int length)
{
	if (m_readType != DSMTT_DATA)
		return 0U;

	if (length < m_readLength) {
		::memcpy(data, m_readBuffer, length);
		return length;
	} else {
		::memcpy(data, m_readBuffer, m_readLength);
		return m_readLength;
	}
}

bool CDStarRepeaterDVRPTRV2Controller::writeHeader(const CHeaderData& header)
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

	bool ret = m_txData.hasSpace(106U);
	if (!ret)
		return false;

	unsigned char len = 105U;
	m_txData.addData(&len, 1U);

	m_txData.addData(buffer, 105U);

	m_tx = true;

	return true;
}

bool CDStarRepeaterDVRPTRV2Controller::writeData(const unsigned char* data, unsigned int length, bool end)
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

		m_tx = false;
	}

	wxMutexLocker locker(m_mutex);

	bool ret = m_txData.hasSpace(18U);
	if (!ret)
		return false;

	unsigned char len = 17U;
	m_txData.addData(&len, 1U);

	m_txData.addData(buffer, 17U);

	return true;
}

unsigned int CDStarRepeaterDVRPTRV2Controller::getSpace()
{
	return m_space;
}

bool CDStarRepeaterDVRPTRV2Controller::getTX()
{
	return m_tx;
}

void CDStarRepeaterDVRPTRV2Controller::stop()
{
	m_stopped = true;

	Wait();
}

bool CDStarRepeaterDVRPTRV2Controller::readSerial()
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

	bool ret = writeModem(buffer, 105U);
	if (!ret)
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

bool CDStarRepeaterDVRPTRV2Controller::readSpace()
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

	return writeModem(buffer, 10U);
}

bool CDStarRepeaterDVRPTRV2Controller::setConfig()
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

	bool ret = writeModem(buffer, 105U);
	if (!ret)
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

RESP_TYPE_V2 CDStarRepeaterDVRPTRV2Controller::getResponse(unsigned char *buffer, unsigned int& length)
{
	// Get the start of the frame or nothing at all
	int ret = readModem(buffer, 5U);
	if (ret < 0) {
		wxLogError(wxT("Error when reading from the DV-RPTR"));
		return RT2_ERROR;
	}

	if (ret == 0)
		return RT2_TIMEOUT;

	while (ret > 0) {
		if (::memcmp(buffer + 0U, "HEAD", 4U) == 0) {
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
				int ret = readModem(buffer + offset, length - offset);
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
				return RT2_DATA;
			} else if (::memcmp(buffer + 5U, "0001", 4U) == 0) {
				if (buffer[104U] == 0x01U)
					return RT2_HEADER;
			} else if (::memcmp(buffer + 5U, "9900", 4U) == 0) {
				return RT2_QUERY;
			} else if (::memcmp(buffer + 5U, "9001", 4U) == 0) {
				return RT2_CONFIG;
			} else if (::memcmp(buffer + 5U, "9011", 4U) == 0) {
				return RT2_SPACE;
			} else if (::memcmp(buffer + 5U, "9021", 4U) == 0) {
				return RT2_TIMEOUT;
			}

			wxLogError(wxT("DV-RPTR frame type number is incorrect - 0x%02X 0x%02X 0x%02X 0x%02X"), buffer[5U], buffer[6U], buffer[7U], buffer[8U]);

			return RT2_UNKNOWN;
		} else {
			for (unsigned int i = 0U; i < 4U; i++)
				buffer[i] = buffer[i + 1U];

			ret = readModem(buffer + 4U, 1U);
			if (ret < 0) {
				wxLogError(wxT("Error when reading from the DV-RPTR"));
				return RT2_ERROR;
			}

			// CUtils::dump(wxT("Resync"), buffer, 5U);
		}
	}

	return RT2_TIMEOUT;
}

wxString CDStarRepeaterDVRPTRV2Controller::getPath() const
{
	return m_usbPath;
}

bool CDStarRepeaterDVRPTRV2Controller::findPort()
{
	if (m_connection != CT_USB)
		return true;

	if (m_usbPath.IsEmpty())
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

		if (path.IsSameAs(m_usbPath)) {
			m_usbPort.Printf(wxT("/dev/%s"), fileName.c_str());

			wxLogMessage(wxT("Found modem port of %s based on the path"), m_usbPort.c_str());

			return true;
		}

		ret1 = dir.GetNext(&fileName);
	}
#endif

	return false;
}

bool CDStarRepeaterDVRPTRV2Controller::findPath()
{
	if (m_connection != CT_USB)
		return true;

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
	path.Printf(wxT("/sys/class/tty/%s"), m_usbPort.Mid(5U).c_str());	

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

	if (m_usbPath.IsEmpty())
		wxLogMessage(wxT("Found modem path of %s"), path.c_str());

	m_usbPath = path;
#endif

	return true;
}

bool CDStarRepeaterDVRPTRV2Controller::findModem()
{
	closeModem();

	// Tell the repeater that the signal has gone away
	if (m_rx) {
		m_mutex.Lock();
		unsigned char data[2U];
		data[0U] = DSMTT_EOT;
		data[1U] = 0U;
		m_rxData.addData(data, 2U);
		m_mutex.Unlock();

		m_rx = false;
	}

	unsigned int count = 0U;

	// Purge the transmit buffer every 500ms to avoid overflow, but only try and reopen the modem every 2s
	while (!m_stopped) {
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

bool CDStarRepeaterDVRPTRV2Controller::openModem()
{
	bool ret = false;

	switch (m_connection) {
		case CT_USB:
			ret = m_usb->open();
			break;
		case CT_NETWORK:
			ret = m_network->open();
			break;
		default:
			wxLogError(wxT("Invalid connection type: %d"), int(m_connection));
			break;
	}

	if (!ret)
		return false;

	ret = readSerial();
	if (!ret) {
		closeModem();
		return false;
	}

	ret = setConfig();
	if (!ret) {
		closeModem();
		return false;
	}

	return true;
}

int CDStarRepeaterDVRPTRV2Controller::readModem(unsigned char* buffer, unsigned int length)
{
	switch (m_connection) {
		case CT_USB:
			return m_usb->read(buffer, length);
		case CT_NETWORK:
			return m_network->read(buffer, length, 0U);
		default:
			return -1;
	}
}

bool CDStarRepeaterDVRPTRV2Controller::writeModem(const unsigned char* buffer, unsigned int length)
{
	switch (m_connection) {
		case CT_USB:
			return m_usb->write(buffer, length) == int(length);
		case CT_NETWORK:
			return m_network->write(buffer, length);
		default:
			return false;
	}
}

void CDStarRepeaterDVRPTRV2Controller::closeModem()
{
	switch (m_connection) {
		case CT_USB:
			return m_usb->close();
		case CT_NETWORK:
			return m_network->close();
		default:
			return;
	}
}

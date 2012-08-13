/*
 *	Copyright (C) 2009,2010 by Jonathan Naylor, G4KLX
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

#include "URIUSBController.h"

const unsigned int C108_VENDOR_ID     = 0x0D8CU;
const unsigned int C108_PRODUCT_ID    = 0x000CU;
const unsigned int C108AH_PRODUCT_ID  = 0x013CU;
const unsigned int C119_PRODUCT_ID    = 0x0008U;

const char REPORT_ID     = 0x00;

const char HID_OR0       = 0x00;
const char HID_OR1       = 0x00;
const char HID_OR2       = 0x0F;	// Set GPIO1 to GPIO4 to output
const char HID_OR3       = 0x00;
const char HID_OR1_GPIO4 = 0x08;	// GPIO4, Switched
const char HID_OR1_GPIO3 = 0x04;	// GPIO3, PTT
const char HID_OR1_GPIO2 = 0x02;	// GPIO2, Switched
const char HID_OR1_GPIO1 = 0x01;	// GPIO1, Heartbeat
const char HID_IR0_RM    = 0x08;	// Record-Mute, Disable
const char HID_IR0_PM    = 0x04;	// Playback-Mute, Battery
const char HID_IR0_VD    = 0x02;	// Volume-Down, COR_DET
const char HID_IR0_VU    = 0x01;	// Volume-Up, CTCSS_DET

#if defined(__WINDOWS__)

#include <Setupapi.h>
#include "HID.h"

const ULONG USB_BUFSIZE = 5UL;

CURIUSBController::CURIUSBController(unsigned int address) :
m_address(address),
m_state(false),
m_handle(INVALID_HANDLE_VALUE)
{
}

CURIUSBController::~CURIUSBController()
{
}

bool CURIUSBController::open()
{
	wxASSERT(m_handle == INVALID_HANDLE_VALUE);

	GUID guid;
	::HidD_GetHidGuid(&guid);

	HDEVINFO devInfo = ::SetupDiGetClassDevs(&guid, NULL, NULL, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);
	if (devInfo == INVALID_HANDLE_VALUE) {
		wxLogMessage(wxT("Error from SetupDiGetClassDevs: err=%u"), ::GetLastError());
		return false;
	}

	SP_DEVICE_INTERFACE_DATA devInfoData;
	devInfoData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

	unsigned int count = 0U;
	for (DWORD index = 0U; ::SetupDiEnumDeviceInterfaces(devInfo, NULL, &guid, index, &devInfoData); index++) {
		// Find the required length of the device structure
		DWORD length;
		::SetupDiGetDeviceInterfaceDetail(devInfo, &devInfoData, NULL, 0U, &length, NULL);

		PSP_DEVICE_INTERFACE_DETAIL_DATA detailData = PSP_DEVICE_INTERFACE_DETAIL_DATA(::malloc(length));
		detailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

		// Get the detailed data into the newly allocated device structure
		DWORD required;
		BOOL res = ::SetupDiGetDeviceInterfaceDetail(devInfo, &devInfoData, detailData, length, &required, NULL);
		if (!res) {
			wxLogMessage(wxT("Error from SetupDiGetDeviceInterfaceDetail: err=%u"), ::GetLastError());
			::SetupDiDestroyDeviceInfoList(devInfo);
			::free(detailData);
			return false;
		}

		// Get the handle for getting the attributes
		HANDLE handle = ::CreateFile(detailData->DevicePath, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
		if (handle == INVALID_HANDLE_VALUE) {
			wxLogMessage(wxT("Error from CreateFile: err=%u"), ::GetLastError());
			::SetupDiDestroyDeviceInfoList(devInfo);
			::free(detailData);
			return false;
		}

		HIDD_ATTRIBUTES attributes;
		attributes.Size = sizeof(HIDD_ATTRIBUTES);
		res = ::HidD_GetAttributes(handle, &attributes);
		if (!res) {
			wxLogMessage(wxT("Error from HidD_GetAttributes: err=%u"), ::GetLastError());
			::CloseHandle(handle);
			::SetupDiDestroyDeviceInfoList(devInfo);
			::free(detailData);
			return false;
		}

		::CloseHandle(handle);

		// Is this a CM108 or equivalent?
		if (attributes.VendorID  == C108_VENDOR_ID    &&
		   (attributes.ProductID == C108_PRODUCT_ID   ||
			attributes.ProductID == C108AH_PRODUCT_ID ||
			attributes.ProductID == C119_PRODUCT_ID)) {
				count++;

			// Is this the right device?
			if (count == m_address) {
				m_handle = ::CreateFile(detailData->DevicePath, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
				if (m_handle == INVALID_HANDLE_VALUE) {
					wxLogMessage(wxT("Error from CreateFile: err=%u"), ::GetLastError());
					::SetupDiDestroyDeviceInfoList(devInfo);
					::free(detailData);
					return false;
				}

				::SetupDiDestroyDeviceInfoList(devInfo);
				::free(detailData);

				char buffer[USB_BUFSIZE];
				buffer[0] = REPORT_ID;
				buffer[1] = HID_OR0;
				buffer[2] = HID_OR1;
				buffer[3] = HID_OR2;
				buffer[4] = HID_OR3;
				res = ::HidD_SetOutputReport(m_handle, buffer, USB_BUFSIZE);
				if (!res)
					wxLogMessage(wxT("Error from HidD_SetOutputReport: err=%u"), ::GetLastError());

				return true;
			}
		}

		::free(detailData);
	}

	::SetupDiDestroyDeviceInfoList(devInfo);

	return false;
}

bool CURIUSBController::getSquelch()
{
	wxASSERT(m_handle != INVALID_HANDLE_VALUE);

	char buffer[USB_BUFSIZE];
	buffer[0] = REPORT_ID;
	buffer[1] = 0x00;
	buffer[2] = 0x00;
	buffer[3] = 0x00;
	buffer[4] = 0x00;
	BOOL res = ::HidD_GetInputReport(m_handle, buffer, USB_BUFSIZE);
	if (!res) {
		wxLogMessage(wxT("Error from HidD_GetInputReport: err=%u"), ::GetLastError());
		return false;
	}

	return (buffer[1] & HID_IR0_VD) == HID_IR0_VD;
}

void CURIUSBController::setTransmit(bool tx)
{
	wxASSERT(m_handle != INVALID_HANDLE_VALUE);

	if (tx == m_state)
		return;

	char buffer[USB_BUFSIZE];
	buffer[0] = REPORT_ID;
	buffer[1] = HID_OR0;
	buffer[2] = HID_OR1;
	buffer[3] = HID_OR2;
	buffer[4] = HID_OR3;

	if (tx)
		buffer[2] |= HID_OR1_GPIO3;

	BOOL res = ::HidD_SetOutputReport(m_handle, buffer, USB_BUFSIZE);
	if (!res)
		wxLogMessage(wxT("Error from HidD_SetOutputReport: err=%u"), ::GetLastError());

	m_state = tx;
}

void CURIUSBController::close()
{
	wxASSERT(m_handle != INVALID_HANDLE_VALUE);

	char buffer[USB_BUFSIZE];
	buffer[0] = REPORT_ID;
	buffer[1] = HID_OR0;
	buffer[2] = HID_OR1;
	buffer[3] = HID_OR2;
	buffer[4] = HID_OR3;
	::HidD_SetOutputReport(m_handle, buffer, USB_BUFSIZE);

	::CloseHandle(m_handle);
	m_handle = INVALID_HANDLE_VALUE;
}

#else

const unsigned int C108_HID_INTERFACE = 3U;

const unsigned int HID_REPORT_GET = 0x01U;
const unsigned int HID_REPORT_SET = 0x09U;

const unsigned int HID_RT_INPUT   = 0x01U;
const unsigned int HID_RT_OUTPUT  = 0x02U;

const int USB_BUFSIZE = 4;

const int USB_TIMEOUT = 5000;

CURIUSBController::CURIUSBController(unsigned int address) :
m_address(address),
m_state(false),
m_handle(NULL)
{
}

CURIUSBController::~CURIUSBController()
{
}

bool CURIUSBController::open()
{
	wxASSERT(m_handle == NULL);

	::usb_init();
	::usb_find_busses();
	::usb_find_devices();

	unsigned int count = 0U;
	for (struct usb_bus* usb_bus = usb_busses; usb_bus != NULL; usb_bus = usb_bus->next) {
		for (struct usb_device* usb_dev = usb_bus->devices; usb_dev != NULL; usb_dev = usb_dev->next) {
			if (usb_dev->descriptor.idVendor == C108_VENDOR_ID     &&
			   (usb_dev->descriptor.idProduct == C108_PRODUCT_ID   ||
				usb_dev->descriptor.idProduct == C108AH_PRODUCT_ID ||
				usb_dev->descriptor.idProduct == C119_PRODUCT_ID)) { 
				count++;

				// Is this the correct device?
				if (count == m_address) {
					m_handle = ::usb_open(usb_dev);
					if (m_handle == NULL) {
						wxString error(::usb_strerror(), wxConvLocal);
						wxLogMessage(wxT("usb_open failed, err=%s"), error.c_str());
						return false;
					}

					int res = ::usb_claim_interface(m_handle, C108_HID_INTERFACE);
					if (res < 0) {
						res = ::usb_detach_kernel_driver_np(m_handle, C108_HID_INTERFACE);
						if (res < 0) {
							wxString error(::usb_strerror(), wxConvLocal);
							wxLogMessage(wxT("usb_detach_kernel_driver_np failed, res=%d, err=%s"), res, error.c_str());
							return false;
						}

						res = ::usb_claim_interface(m_handle, C108_HID_INTERFACE);
						if (res < 0) {
							wxString error(::usb_strerror(), wxConvLocal);
							wxLogMessage(wxT("usb_claim_interface failed, res=%d, err=%s"), res, error.c_str());
							return false;
						}
					}

					unsigned char buffer[USB_BUFSIZE];
					buffer[0] = HID_OR0;
					buffer[1] = HID_OR1;
					buffer[2] = HID_OR2;
					buffer[3] = HID_OR3;
					res = ::usb_control_msg(m_handle, USB_ENDPOINT_OUT + USB_TYPE_CLASS + USB_RECIP_INTERFACE, HID_REPORT_SET, REPORT_ID + (HID_RT_OUTPUT << 8), C108_HID_INTERFACE, (char*)buffer, USB_BUFSIZE, USB_TIMEOUT);
					if (res != USB_BUFSIZE) {
						wxString error(::usb_strerror(), wxConvLocal);
						wxLogMessage(wxT("Error from usb_control_msg: res=%d, err=%s"), res, error.c_str());
					}

					return true;
				}
			}
		}
	}

	return false;
}

bool CURIUSBController::getSquelch()
{
	wxASSERT(m_handle != NULL);

	unsigned char buffer[USB_BUFSIZE];
	buffer[0] = 0x00;
	buffer[1] = 0x00;
	buffer[2] = 0x00;
	buffer[3] = 0x00;
	int res = ::usb_control_msg(m_handle, USB_ENDPOINT_IN + USB_TYPE_CLASS + USB_RECIP_INTERFACE, HID_REPORT_GET, REPORT_ID + (HID_RT_INPUT << 8), C108_HID_INTERFACE, (char*)buffer, USB_BUFSIZE, USB_TIMEOUT);
	if (res != USB_BUFSIZE) {
		wxString error(::usb_strerror(), wxConvLocal);
		wxLogMessage(wxT("Error from usb_control_msg: res=%d, err=%s"), res, error.c_str());
		return false;
	}

	return (buffer[0] & HID_IR0_VD) == HID_IR0_VD;
}

void CURIUSBController::setTransmit(bool tx)
{
	wxASSERT(m_handle != NULL);

	if (tx == m_state)
		return;

	unsigned char buffer[USB_BUFSIZE];
	buffer[0] = HID_OR0;
	buffer[1] = HID_OR1;
	buffer[2] = HID_OR2;
	buffer[3] = HID_OR3;

	if (tx)
		buffer[1] |= HID_OR1_GPIO3;

	int res = ::usb_control_msg(m_handle, USB_ENDPOINT_OUT + USB_TYPE_CLASS + USB_RECIP_INTERFACE, HID_REPORT_SET, REPORT_ID + (HID_RT_OUTPUT << 8), C108_HID_INTERFACE, (char*)buffer, USB_BUFSIZE, USB_TIMEOUT);
	if (res != USB_BUFSIZE) {
		wxString error(::usb_strerror(), wxConvLocal);
		wxLogMessage(wxT("Error from usb_control_msg: res=%d, err=%s"), res, error.c_str());
	}

	m_state = tx;
}

void CURIUSBController::close()
{
	wxASSERT(m_handle != NULL);

	unsigned char buffer[USB_BUFSIZE];
	buffer[0] = HID_OR0;
	buffer[1] = HID_OR1;
	buffer[2] = HID_OR2;
	buffer[3] = HID_OR3;
	::usb_control_msg(m_handle, USB_ENDPOINT_OUT + USB_TYPE_CLASS + USB_RECIP_INTERFACE, HID_REPORT_SET, REPORT_ID + (HID_RT_OUTPUT << 8), C108_HID_INTERFACE, (char*)buffer, USB_BUFSIZE, USB_TIMEOUT);

	::usb_close(m_handle);
	m_handle = NULL;
}

#endif

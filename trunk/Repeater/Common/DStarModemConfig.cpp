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

#include "DStarModemConfig.h"

const wxString  KEY_MODEM_NAME         = wxT("modemName");
const wxString  KEY_MODEM_TYPE         = wxT("modemType");

const wxString  KEY_DVAP_PORT          = wxT("dvapPort");
const wxString  KEY_DVAP_FREQUENCY     = wxT("dvapFrequency");
const wxString  KEY_DVAP_POWER         = wxT("dvapPower");
const wxString  KEY_DVAP_SQUELCH       = wxT("dvapSquelch");

const wxString  KEY_GMSK_INTERFACE     = wxT("gmskInterfaceType");
const wxString  KEY_GMSK_ADDRESS       = wxT("gmskAddress");

const wxString  KEY_DVRPTR1_PORT       = wxT("dvrptr1Port");
const wxString  KEY_DVRPTR1_RXINVERT   = wxT("dvrptr1RXInvert");
const wxString  KEY_DVRPTR1_TXINVERT   = wxT("dvrptr1TXInvert");
const wxString  KEY_DVRPTR1_CHANNEL    = wxT("dvrptr1Channel");
const wxString  KEY_DVRPTR1_MODLEVEL   = wxT("dvrptr1ModLevel");
const wxString  KEY_DVRPTR1_TXDELAY    = wxT("dvrptr1TXDelay");

const wxString  KEY_DVRPTR2_CONNECTION = wxT("dvrptr2Connection");
const wxString  KEY_DVRPTR2_USBPORT    = wxT("dvrptr2USBPort");
const wxString  KEY_DVRPTR2_ADDRESS    = wxT("dvrptr2Address");
const wxString  KEY_DVRPTR2_PORT       = wxT("dvrptr2Port");
const wxString  KEY_DVRPTR2_TXINVERT   = wxT("dvrptr2TXInvert");
const wxString  KEY_DVRPTR2_MODLEVEL   = wxT("dvrptr2ModLevel");


const wxString        DEFAULT_MODEM_NAME         = wxT("GMSK 1");
const MODEM_TYPE      DEFAULT_MODEM_TYPE         = MT_NONE;

const wxString        DEFAULT_DVAP_PORT          = wxEmptyString;
const unsigned int    DEFAULT_DVAP_FREQUENCY     = 145500000U;
const int             DEFAULT_DVAP_POWER         = 10;
const int             DEFAULT_DVAP_SQUELCH       = -100;

#if defined(WIN32)
const USB_INTERFACE   DEFAULT_GMSK_INTERFACE     = UI_WINUSB;
#else
const USB_INTERFACE   DEFAULT_GMSK_INTERFACE     = UI_LIBUSB;
#endif
const unsigned int    DEFAULT_GMSK_ADDRESS       = 0x0300U;

const wxString        DEFAULT_DVRPTR1_PORT       = wxEmptyString;
const bool            DEFAULT_DVRPTR1_RXINVERT   = false;
const bool            DEFAULT_DVRPTR1_TXINVERT   = false;
const bool            DEFAULT_DVRPTR1_CHANNEL    = false;
const unsigned int    DEFAULT_DVRPTR1_MODLEVEL   = 20U;
const unsigned int    DEFAULT_DVRPTR1_TXDELAY    = 150U;

const CONNECTION_TYPE DEFAULT_DVRPTR2_CONNECTION = CT_USB;
const wxString        DEFAULT_DVRPTR2_USBPORT    = wxEmptyString;
const wxString        DEFAULT_DVRPTR2_ADDRESS    = wxT("127.0.0.1");
const unsigned int    DEFAULT_DVRPTR2_PORT       = 0U;
const bool            DEFAULT_DVRPTR2_TXINVERT   = false;
const unsigned int    DEFAULT_DVRPTR2_MODLEVEL   = 20U;


#if defined(__WINDOWS__)

CDStarModemConfig::CDStarModemConfig(wxConfigBase* config, const wxString& name) :
m_config(config),
m_name(wxT("/")),
m_modemName(name),
m_modemType(DEFAULT_MODEM_TYPE),
m_dvapPort(DEFAULT_DVAP_PORT),
m_dvapFrequency(DEFAULT_DVAP_FREQUENCY),
m_dvapPower(DEFAULT_DVAP_POWER),
m_dvapSquelch(DEFAULT_DVAP_SQUELCH),
m_gmskInterface(DEFAULT_GMSK_INTERFACE),
m_gmskAddress(DEFAULT_GMSK_ADDRESS),
m_dvrptr1Port(DEFAULT_DVRPTR1_PORT),
m_dvrptr1RXInvert(DEFAULT_DVRPTR1_RXINVERT),
m_dvrptr1TXInvert(DEFAULT_DVRPTR1_TXINVERT),
m_dvrptr1Channel(DEFAULT_DVRPTR1_CHANNEL),
m_dvrptr1ModLevel(DEFAULT_DVRPTR1_MODLEVEL),
m_dvrptr1TXDelay(DEFAULT_DVRPTR1_TXDELAY),
m_dvrptr2Connection(DEFAULT_DVRPTR2_CONNECTION),
m_dvrptr2USBPort(DEFAULT_DVRPTR2_USBPORT),
m_dvrptr2Address(DEFAULT_DVRPTR2_ADDRESS),
m_dvrptr2Port(DEFAULT_DVRPTR2_PORT),
m_dvrptr2TXInvert(DEFAULT_DVRPTR2_TXINVERT),
m_dvrptr2ModLevel(DEFAULT_DVRPTR2_MODLEVEL)
{
	wxASSERT(config != NULL);

	m_name = wxT("/") + name + wxT("/");

	long temp;

	m_config->Read(m_name + KEY_MODEM_NAME, &m_modemName, DEFAULT_MODEM_NAME);

	m_config->Read(m_name + KEY_MODEM_TYPE, &temp, long(DEFAULT_MODEM_TYPE));
	m_modemType = MODEM_TYPE(temp);

	m_config->Read(m_name + KEY_DVAP_PORT, &m_dvapPort, DEFAULT_DVAP_PORT);

	m_config->Read(m_name + KEY_DVAP_FREQUENCY, &temp, long(DEFAULT_DVAP_FREQUENCY));
	m_dvapFrequency = (unsigned int)temp;

	m_config->Read(m_name + KEY_DVAP_POWER, &temp, long(DEFAULT_DVAP_POWER));
	m_dvapPower = int(temp);

	m_config->Read(m_name + KEY_DVAP_SQUELCH, &temp, long(DEFAULT_DVAP_SQUELCH));
	m_dvapSquelch = int(temp);

	m_config->Read(m_name + KEY_GMSK_INTERFACE, &temp, long(DEFAULT_GMSK_INTERFACE));
	m_gmskInterface = USB_INTERFACE(temp);

	m_config->Read(m_name + KEY_GMSK_ADDRESS, &temp, long(DEFAULT_GMSK_ADDRESS));
	m_gmskAddress = (unsigned int)temp;

	m_config->Read(m_name + KEY_DVRPTR1_PORT, &m_dvrptr1Port, DEFAULT_DVRPTR1_PORT);

	m_config->Read(m_name + KEY_DVRPTR1_RXINVERT, &m_dvrptr1RXInvert, DEFAULT_DVRPTR1_RXINVERT);

	m_config->Read(m_name + KEY_DVRPTR1_TXINVERT, &m_dvrptr1TXInvert, DEFAULT_DVRPTR1_TXINVERT);

	m_config->Read(m_name + KEY_DVRPTR1_CHANNEL, &m_dvrptr1Channel, DEFAULT_DVRPTR1_CHANNEL);

	m_config->Read(m_name + KEY_DVRPTR1_MODLEVEL, &temp, long(DEFAULT_DVRPTR1_MODLEVEL));
	m_dvrptr1ModLevel = (unsigned int)temp;

	m_config->Read(m_name + KEY_DVRPTR1_TXDELAY, &temp, long(DEFAULT_DVRPTR1_TXDELAY));
	m_dvrptr1TXDelay = (unsigned int)temp;

	m_config->Read(m_name + KEY_DVRPTR2_CONNECTION, &temp, long(DEFAULT_DVRPTR2_CONNECTION));
	m_dvrptr2Connection = CONNECTION_TYPE(temp);

	m_config->Read(m_name + KEY_DVRPTR2_USBPORT, &m_dvrptr2USBPort, DEFAULT_DVRPTR2_USBPORT);

	m_config->Read(m_name + KEY_DVRPTR2_ADDRESS, &m_dvrptr2Address, DEFAULT_DVRPTR2_ADDRESS);

	m_config->Read(m_name + KEY_DVRPTR2_PORT, &temp, long(DEFAULT_DVRPTR2_PORT));
	m_dvrptr2Port = (unsigned int)temp;

	m_config->Read(m_name + KEY_DVRPTR2_TXINVERT, &m_dvrptr2TXInvert, DEFAULT_DVRPTR2_TXINVERT);

	m_config->Read(m_name + KEY_DVRPTR2_MODLEVEL, &temp, long(DEFAULT_DVRPTR2_MODLEVEL));
	m_dvrptr2ModLevel = (unsigned int)temp;
}

CDStarModemConfig::~CDStarModemConfig()
{
	delete m_config;
}

#else

CDStarModemConfig::CDStarModemConfig(const wxString& dir, const wxString& configName, const wxString& name) :
m_fileName(),
m_modemName(name),
m_modemType(DEFAULT_MODEM_TYPE),
m_dvapPort(DEFAULT_DVAP_PORT),
m_dvapFrequency(DEFAULT_DVAP_FREQUENCY),
m_dvapPower(DEFAULT_DVAP_POWER),
m_dvapSquelch(DEFAULT_DVAP_SQUELCH),
m_gmskInterface(DEFAULT_GMSK_INTERFACE),
m_gmskAddress(DEFAULT_GMSK_ADDRESS),
m_dvrptr1Port(DEFAULT_DVRPTR1_PORT),
m_dvrptr1RXInvert(DEFAULT_DVRPTR1_RXINVERT),
m_dvrptr1TXInvert(DEFAULT_DVRPTR1_TXINVERT),
m_dvrptr1Channel(DEFAULT_DVRPTR1_CHANNEL),
m_dvrptr1ModLevel(DEFAULT_DVRPTR1_MODLEVEL),
m_dvrptr1TXDelay(DEFAULT_DVRPTR1_TXDELAY),
m_dvrptr2Connection(DEFAULT_DVRPTR2_CONNECTION),
m_dvrptr2USBPort(DEFAULT_DVRPTR2_USBPORT),
m_dvrptr2Address(DEFAULT_DVRPTR2_ADDRESS),
m_dvrptr2Port(DEFAULT_DVRPTR2_PORT),
m_dvrptr2TXInvert(DEFAULT_DVRPTR2_TXINVERT),
m_dvrptr2ModLevel(DEFAULT_DVRPTR2_MODLEVEL)
{
	wxASSERT(!dir.IsEmpty());

	wxString fileName = configName + wxT("_") + name;
	fileName.Replace(wxT(" "), wxT("_"));

	m_fileName.Assign(dir, fileName);

	wxTextFile file(m_fileName.GetFullPath());

	bool exists = file.Exists();
	if (!exists)
		return;

	bool ret = file.Open();
	if (!ret) {
		wxLogError(wxT("Cannot open the config file - %s"), m_fileName.GetFullPath().c_str());
		return;
	}

	long temp1;
	unsigned long temp2;

	wxString str = file.GetFirstLine();

	while (!file.Eof()) {
		if (str.GetChar(0U) == wxT('#')) {
			str = file.GetNextLine();
			continue;
		}

		int n = str.Find(wxT('='));
		if (n == wxNOT_FOUND) {
			str = file.GetNextLine();
			continue;
		}

		wxString key = str.Left(n);
		wxString val = str.Mid(n + 1U);

		if (key.IsSameAs(KEY_MODEM_NAME)) {
			m_callsign = val;
		} else if (key.IsSameAs(KEY_MODEM_TYPE)) {
			val.ToLong(&temp1);
			m_modemType = DSTAR_MODEM_TYPE(temp1);
		} else if (key.IsSameAs(KEY_DVAP_PORT)) {
			m_dvapPort = val;
		} else if (key.IsSameAs(KEY_DVAP_FREQUENCY)) {
			val.ToULong(&temp2);
			m_dvapFrequency = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_DVAP_POWER)) {
			val.ToLong(&temp1);
			m_dvapPower = int(temp1);
		} else if (key.IsSameAs(KEY_DVAP_SQUELCH)) {
			val.ToLong(&temp1);
			m_dvapSquelch = int(temp1);
		} else if (key.IsSameAs(KEY_GMSK_INTERFACE)) {
			val.ToLong(&temp1);
			m_gmskInterface = USB_INTERFACE(temp1);
		} else if (key.IsSameAs(KEY_GMSK_ADDRESS)) {
			val.ToULong(&temp2);
			m_gmskAddress = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_DVRPTR1_PORT)) {
			m_dvrptr1Port = val;
		} else if (key.IsSameAs(KEY_DVRPTR1_RXINVERT)) {
			val.ToLong(&temp1);
			m_dvrptr1RXInvert = temp1 == 1L;
		} else if (key.IsSameAs(KEY_DVRPTR1_TXINVERT)) {
			val.ToLong(&temp1);
			m_dvrptr1TXInvert = temp1 == 1L;
		} else if (key.IsSameAs(KEY_DVRPTR1_CHANNEL)) {
			val.ToLong(&temp1);
			m_dvrptr1Channel = temp1 == 1L;
		} else if (key.IsSameAs(KEY_DVRPTR1_MODLEVEL)) {
			val.ToULong(&temp2);
			m_dvrptr1ModLevel = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_DVRPTR1_TXDELAY)) {
			val.ToULong(&temp2);
			m_dvrptr1TXDelay = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_DVRPTR2_CONNECTION)) {
			val.ToLong(&temp1);
			m_dvrptr2Connection = CONNECTION_TYPE(temp1);
		} else if (key.IsSameAs(KEY_DVRPTR2_USBPORT)) {
			m_dvrptr2USBPort = val;
		} else if (key.IsSameAs(KEY_DVRPTR2_ADDRESS)) {
			m_dvrptr2Address = val;
		} else if (key.IsSameAs(KEY_DVRPTR2_PORT)) {
			val.ToULong(&temp2);
			m_dvrptr2Port = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_DVRPTR2_TXINVERT)) {
			val.ToLong(&temp1);
			m_dvrptr2TXInvert = temp1 == 1L;
		} else if (key.IsSameAs(KEY_DVRPTR2_MODLEVEL)) {
			val.ToULong(&temp2);
			m_dvrptr2ModLevel = (unsigned int)temp2;
		}

		str = file.GetNextLine();
	}

	file.Close();
}

CDStarModemConfig::~CDStarModemConfig()
{
}

#endif

void CDStarModemConfig::getType(MODEM_TYPE& type) const
{
	type = m_modemType;
}

void CDStarModemConfig::setType(MODEM_TYPE type)
{
	m_modemType = type;
}

void CDStarModemConfig::getDVAP(wxString& port, unsigned int& frequency, int& power, int& squelch) const
{
	port      = m_dvapPort;
	frequency = m_dvapFrequency;
	power     = m_dvapPower;
	squelch   = m_dvapSquelch;
}

void CDStarModemConfig::setDVAP(const wxString& port, unsigned int frequency, int power, int squelch)
{
	m_dvapPort      = port;
	m_dvapFrequency = frequency;
	m_dvapPower     = power;
	m_dvapSquelch   = squelch;
}

void CDStarModemConfig::getGMSK(USB_INTERFACE& type, unsigned int& address) const
{
	type    = m_gmskInterface;
	address = m_gmskAddress;
}

void CDStarModemConfig::setGMSK(USB_INTERFACE type, unsigned int address)
{
	m_gmskInterface = type;
	m_gmskAddress   = address;
}

void CDStarModemConfig::getDVRPTR1(wxString& port, bool& rxInvert, bool& txInvert, bool& channel, unsigned int& modLevel, unsigned int& txDelay) const
{
	port     = m_dvrptr1Port;
	rxInvert = m_dvrptr1RXInvert;
	txInvert = m_dvrptr1TXInvert;
	channel  = m_dvrptr1Channel;
	modLevel = m_dvrptr1ModLevel;
	txDelay  = m_dvrptr1TXDelay;
}

void CDStarModemConfig::setDVRPTR1(const wxString& port, bool rxInvert, bool txInvert, bool channel, unsigned int modLevel, unsigned int txDelay)
{
	m_dvrptr1Port     = port;
	m_dvrptr1RXInvert = rxInvert;
	m_dvrptr1TXInvert = txInvert;
	m_dvrptr1Channel  = channel;
	m_dvrptr1ModLevel = modLevel;
	m_dvrptr1TXDelay  = txDelay;
}

void CDStarModemConfig::getDVRPTR2(CONNECTION_TYPE& connection, wxString& usbPort, wxString& address, unsigned int& port, bool& txInvert, unsigned int& modLevel) const
{
	connection = m_dvrptr2Connection;
	usbPort    = m_dvrptr2USBPort;
	address    = m_dvrptr2Address;
	port       = m_dvrptr2Port;
	txInvert   = m_dvrptr2TXInvert;
	modLevel   = m_dvrptr2ModLevel;
}

void CDStarModemConfig::setDVRPTR2(CONNECTION_TYPE connection, const wxString& usbPort, const wxString& address, unsigned int port, bool txInvert, unsigned int modLevel)
{
	m_dvrptr2Connection = connection;
	m_dvrptr2USBPort    = usbPort;
	m_dvrptr2Address    = address;
	m_dvrptr2Port       = port;
	m_dvrptr2TXInvert   = txInvert;
	m_dvrptr2ModLevel   = modLevel;
}

#if defined(__WINDOWS__)

bool CDStarModemConfig::write()
{
	m_config->Write(m_name + KEY_MODEM_NAME, m_modemName);
	m_config->Write(m_name + KEY_MODEM_TYPE, long(m_modemType));
	m_config->Flush();

	m_config->Write(m_name + KEY_DVAP_PORT, m_dvapPort);
	m_config->Write(m_name + KEY_DVAP_FREQUENCY, long(m_dvapFrequency));
	m_config->Write(m_name + KEY_DVAP_POWER, long(m_dvapPower));
	m_config->Write(m_name + KEY_DVAP_SQUELCH, long(m_dvapSquelch));

	m_config->Write(m_name + KEY_GMSK_INTERFACE, long(m_gmskInterface));
	m_config->Write(m_name + KEY_GMSK_ADDRESS, long(m_gmskAddress));

	m_config->Write(m_name + KEY_DVRPTR1_PORT, m_dvrptr1Port);
	m_config->Write(m_name + KEY_DVRPTR1_RXINVERT, m_dvrptr1RXInvert);
	m_config->Write(m_name + KEY_DVRPTR1_TXINVERT, m_dvrptr1TXInvert);
	m_config->Write(m_name + KEY_DVRPTR1_CHANNEL, m_dvrptr1Channel);
	m_config->Write(m_name + KEY_DVRPTR1_MODLEVEL, long(m_dvrptr1ModLevel));
	m_config->Write(m_name + KEY_DVRPTR1_TXDELAY, long(m_dvrptr1TXDelay));

	m_config->Write(m_name + KEY_DVRPTR2_CONNECTION, long(m_dvrptr2Connection));
	m_config->Write(m_name + KEY_DVRPTR2_USBPORT, m_dvrptr2USBPort);
	m_config->Write(m_name + KEY_DVRPTR2_ADDRESS, m_dvrptr2Address);
	m_config->Write(m_name + KEY_DVRPTR2_PORT, long(m_dvrptr2Port));
	m_config->Write(m_name + KEY_DVRPTR2_TXINVERT, m_dvrptr2TXInvert);
	m_config->Write(m_name + KEY_DVRPTR2_MODLEVEL, long(m_dvrptr2ModLevel));

	return true;
}

#else

bool CDStarModemConfig::write()
{
	wxTextFile file(m_fileName.GetFullPath());

	bool exists = file.Exists();
	if (exists) {
		bool ret = file.Open();
		if (!ret) {
			wxLogError(wxT("Cannot open the config file - %s"), m_fileName.GetFullPath().c_str());
			return false;
		}

		// Remove the existing file entries
		file.Clear();
	} else {
		bool ret = file.Create();
		if (!ret) {
			wxLogError(wxT("Cannot create the config file - %s"), m_fileName.GetFullPath().c_str());
			return false;
		}
	}

	wxString buffer;
	buffer.Printf(wxT("%s=%s"), KEY_MODEM_NAME.c_str(), m_modemName.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_MODEM_TYPE.c_str(), int(m_modemType)); file.AddLine(buffer);

	buffer.Printf(wxT("%s=%s"), KEY_DVAP_PORT.c_str(), m_dvapPort.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_DVAP_FREQUENCY.c_str(), m_dvapFrequency); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_DVAP_POWER.c_str(), m_dvapPower); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_DVAP_SQUELCH.c_str(), m_dvapSquelch); file.AddLine(buffer);

	buffer.Printf(wxT("%s=%d"), KEY_GMSK_INTERFACE.c_str(), int(m_gmskInterface)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_GMSK_ADDRESS.c_str(), m_gmskAddress); file.AddLine(buffer);

	buffer.Printf(wxT("%s=%s"), KEY_DVRPTR1_PORT.c_str(), m_dvrptr1Port.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_DVRPTR1_RXINVERT.c_str(), m_dvrptr1RXInvert ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_DVRPTR1_TXINVERT.c_str(), m_dvrptr1TXInvert ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_DVRPTR1_CHANNEL.c_str(), m_dvrptr1Channel ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_DVRPTR1_MODLEVEL.c_str(), m_dvrptr1ModLevel); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_DVRPTR1_TXDELAY.c_str(), m_dvrptr1TXDelay); file.AddLine(buffer);

	buffer.Printf(wxT("%s=%d"), KEY_DVRPTR2_CONNECTION.c_str(), int(m_dvrptr2Connection)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_DVRPTR2_USBPORT.c_str(), m_dvrptr2USBPort.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_DVRPTR2_ADDRESS.c_str(), m_dvrptr2Address.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_DVRPTR2_PORT.c_str(), m_dvrptr2Port); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_DVRPTR2_TXINVERT.c_str(), m_dvrptr2TXInvert ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_DVRPTR2_MODLEVEL.c_str(), m_dvrptr2ModLevel); file.AddLine(buffer);

	bool ret = file.Write();
	if (!ret) {
		file.Close();
		wxLogError(wxT("Cannot write the config file - %s"), m_fileName.GetFullPath().c_str());
		return false;
	}

	file.Close();

	return true;
}

#endif

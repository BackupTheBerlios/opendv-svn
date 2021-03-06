/*
 *   Copyright (C) 2010,2011,2012 by Jonathan Naylor G4KLX
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

#include "SplitRepeaterConfig.h"

const wxString  KEY_CALLSIGN           = wxT("callsign");
const wxString  KEY_GATEWAY            = wxT("gateway");
const wxString  KEY_MODE               = wxT("mode");
const wxString  KEY_ACK                = wxT("ack");
const wxString  KEY_RESTRICTION        = wxT("restriction");
const wxString  KEY_RPT1_VALIDATION    = wxT("rpt1Validation");
const wxString  KEY_GATEWAY_ADDRESS    = wxT("gatewayAddress");
const wxString  KEY_GATEWAY_PORT       = wxT("gatewayPort");
const wxString  KEY_LOCAL_ADDRESS      = wxT("localAddress");
const wxString  KEY_LOCAL_PORT         = wxT("localPort");
const wxString  KEY_RECEIVER1_ADDRESS  = wxT("receiver1Address");
const wxString  KEY_RECEIVER1_PORT     = wxT("receiver1Port");
const wxString  KEY_RECEIVER2_ADDRESS  = wxT("receiver2Address");
const wxString  KEY_RECEIVER2_PORT     = wxT("receiver2Port");
const wxString  KEY_TRANSMITTER1_ADDRESS = wxT("transmitter1Address");
const wxString  KEY_TRANSMITTER1_PORT    = wxT("transmitter1Port");
const wxString  KEY_TRANSMITTER2_ADDRESS = wxT("transmitter2Address");
const wxString  KEY_TRANSMITTER2_PORT    = wxT("transmitter2Port");
const wxString  KEY_TIMEOUT            = wxT("timeout");
const wxString  KEY_ACK_TIME           = wxT("ackTime");
const wxString  KEY_FRAME_WAIT_TIME    = wxT("frameWaitTime");
const wxString  KEY_BEACON_TIME        = wxT("beaconTime");
const wxString  KEY_BEACON_TEXT        = wxT("beaconText");
const wxString  KEY_BEACON_VOICE       = wxT("beaconVoice");
const wxString  KEY_LANGUAGE           = wxT("language");
const wxString  KEY_CONTROL_ENABLED    = wxT("controlEnabled");
const wxString  KEY_CONTROL_RPT1       = wxT("controlRPT1");
const wxString  KEY_CONTROL_RPT2       = wxT("controlRPT2");
const wxString  KEY_CONTROL_SHUTDOWN   = wxT("controlShutdown");
const wxString  KEY_CONTROL_STARTUP    = wxT("controlStartup");
const wxString  KEY_CONTROL_STATUS1    = wxT("controlStatus1");
const wxString  KEY_CONTROL_STATUS2    = wxT("controlStatus2");
const wxString  KEY_CONTROL_STATUS3    = wxT("controlStatus3");
const wxString  KEY_CONTROL_STATUS4    = wxT("controlStatus4");
const wxString  KEY_CONTROL_STATUS5    = wxT("controlStatus5");
const wxString  KEY_CONTROL_COMMAND1      = wxT("controlCommand1");
const wxString  KEY_CONTROL_COMMAND1_LINE = wxT("controlCommand1Line");
const wxString  KEY_CONTROL_COMMAND2      = wxT("controlCommand2");
const wxString  KEY_CONTROL_COMMAND2_LINE = wxT("controlCommand2Line");
const wxString  KEY_CONTROL_COMMAND3      = wxT("controlCommand3");
const wxString  KEY_CONTROL_COMMAND3_LINE = wxT("controlCommand3Line");
const wxString  KEY_CONTROL_COMMAND4      = wxT("controlCommand4");
const wxString  KEY_CONTROL_COMMAND4_LINE = wxT("controlCommand4Line");
const wxString  KEY_WINDOW_X           = wxT("windowX");
const wxString  KEY_WINDOW_Y           = wxT("windowY");

const wxString        DEFAULT_CALLSIGN           = wxT("GB3IN  C");
const wxString        DEFAULT_GATEWAY            = wxEmptyString;
const DSTAR_MODE      DEFAULT_MODE               = MODE_DUPLEX;
const ACK_TYPE        DEFAULT_ACK                = AT_BER;
const bool            DEFAULT_RESTRICTION        = false;
const bool            DEFAULT_RPT1_VALIDATION    = true;
const wxString        DEFAULT_GATEWAY_ADDRESS    = wxT("127.0.0.1");
const unsigned int    DEFAULT_GATEWAY_PORT       = 20010U;
const wxString        DEFAULT_LOCAL_ADDRESS      = wxEmptyString;
const unsigned int    DEFAULT_LOCAL_PORT         = 20011U;
const wxString        DEFAULT_RECEIVER1_ADDRESS  = wxT("127.0.0.1");
const unsigned int    DEFAULT_RECEIVER1_PORT     = 20012U;
const wxString        DEFAULT_RECEIVER2_ADDRESS  = wxEmptyString;
const unsigned int    DEFAULT_RECEIVER2_PORT     = 0U;
const wxString        DEFAULT_TRANSMITTER1_ADDRESS = wxT("127.0.0.1");
const unsigned int    DEFAULT_TRANSMITTER1_PORT    = 20013U;
const wxString        DEFAULT_TRANSMITTER2_ADDRESS = wxEmptyString;
const unsigned int    DEFAULT_TRANSMITTER2_PORT    = 0U;
const unsigned int    DEFAULT_TIMEOUT            = 180U;
const unsigned int    DEFAULT_ACK_TIME           = 500U;
const unsigned int    DEFAULT_FRAME_WAIT_TIME    = 100U;
const unsigned int    DEFAULT_BEACON_TIME        = 600U;
const wxString        DEFAULT_BEACON_TEXT        = wxT("Split Repeater");
const bool            DEFAULT_BEACON_VOICE       = false;
const TEXT_LANG       DEFAULT_LANGUAGE           = TL_ENGLISH_UK;
const bool            DEFAULT_CONTROL_ENABLED    = false;
const wxString        DEFAULT_CONTROL_RPT1       = wxEmptyString;
const wxString        DEFAULT_CONTROL_RPT2       = wxEmptyString;
const wxString        DEFAULT_CONTROL_SHUTDOWN   = wxEmptyString;
const wxString        DEFAULT_CONTROL_STARTUP    = wxEmptyString;
const wxString        DEFAULT_CONTROL_STATUS1    = wxEmptyString;
const wxString        DEFAULT_CONTROL_STATUS2    = wxEmptyString;
const wxString        DEFAULT_CONTROL_STATUS3    = wxEmptyString;
const wxString        DEFAULT_CONTROL_STATUS4    = wxEmptyString;
const wxString        DEFAULT_CONTROL_STATUS5    = wxEmptyString;
const wxString        DEFAULT_CONTROL_COMMAND1      = wxEmptyString;
const wxString        DEFAULT_CONTROL_COMMAND1_LINE = wxEmptyString;
const wxString        DEFAULT_CONTROL_COMMAND2      = wxEmptyString;
const wxString        DEFAULT_CONTROL_COMMAND2_LINE = wxEmptyString;
const wxString        DEFAULT_CONTROL_COMMAND3      = wxEmptyString;
const wxString        DEFAULT_CONTROL_COMMAND3_LINE = wxEmptyString;
const wxString        DEFAULT_CONTROL_COMMAND4      = wxEmptyString;
const wxString        DEFAULT_CONTROL_COMMAND4_LINE = wxEmptyString;
const int             DEFAULT_WINDOW_X           = -1;
const int             DEFAULT_WINDOW_Y           = -1;


#if defined(__WINDOWS__)

CSplitRepeaterConfig::CSplitRepeaterConfig(wxConfigBase* config, const wxString& name) :
m_config(config),
m_name(wxT("/")),
m_callsign(DEFAULT_CALLSIGN),
m_gateway(DEFAULT_GATEWAY),
m_mode(DEFAULT_MODE),
m_ack(DEFAULT_ACK),
m_restriction(DEFAULT_RESTRICTION),
m_rpt1Validation(DEFAULT_RPT1_VALIDATION),
m_gatewayAddress(DEFAULT_GATEWAY_ADDRESS),
m_gatewayPort(DEFAULT_GATEWAY_PORT),
m_localAddress(DEFAULT_LOCAL_ADDRESS),
m_localPort(DEFAULT_LOCAL_PORT),
m_receiver1Address(DEFAULT_RECEIVER1_ADDRESS),
m_receiver1Port(DEFAULT_RECEIVER1_PORT),
m_receiver2Address(DEFAULT_RECEIVER2_ADDRESS),
m_receiver2Port(DEFAULT_RECEIVER2_PORT),
m_transmitter1Address(DEFAULT_TRANSMITTER1_ADDRESS),
m_transmitter1Port(DEFAULT_TRANSMITTER1_PORT),
m_transmitter2Address(DEFAULT_TRANSMITTER2_ADDRESS),
m_transmitter2Port(DEFAULT_TRANSMITTER2_PORT),
m_timeout(DEFAULT_TIMEOUT),
m_ackTime(DEFAULT_ACK_TIME),
m_frameWaitTime(DEFAULT_FRAME_WAIT_TIME),
m_beaconTime(DEFAULT_BEACON_TIME),
m_beaconText(DEFAULT_BEACON_TEXT),
m_beaconVoice(DEFAULT_BEACON_VOICE),
m_language(DEFAULT_LANGUAGE),
m_controlEnabled(DEFAULT_CONTROL_ENABLED),
m_controlRpt1Callsign(DEFAULT_CONTROL_RPT1),
m_controlRpt2Callsign(DEFAULT_CONTROL_RPT2),
m_controlShutdown(DEFAULT_CONTROL_SHUTDOWN),
m_controlStartup(DEFAULT_CONTROL_STARTUP),
m_controlStatus1(DEFAULT_CONTROL_STATUS1),
m_controlStatus2(DEFAULT_CONTROL_STATUS2),
m_controlStatus3(DEFAULT_CONTROL_STATUS3),
m_controlStatus4(DEFAULT_CONTROL_STATUS4),
m_controlStatus5(DEFAULT_CONTROL_STATUS5),
m_controlCommand1(DEFAULT_CONTROL_COMMAND1),
m_controlCommand1Line(DEFAULT_CONTROL_COMMAND1_LINE),
m_controlCommand2(DEFAULT_CONTROL_COMMAND2),
m_controlCommand2Line(DEFAULT_CONTROL_COMMAND2_LINE),
m_controlCommand3(DEFAULT_CONTROL_COMMAND3),
m_controlCommand3Line(DEFAULT_CONTROL_COMMAND3_LINE),
m_controlCommand4(DEFAULT_CONTROL_COMMAND4),
m_controlCommand4Line(DEFAULT_CONTROL_COMMAND4_LINE),
m_x(DEFAULT_WINDOW_X),
m_y(DEFAULT_WINDOW_Y)
{
	wxASSERT(config != NULL);

	if (!name.IsEmpty())
		m_name = wxT("/") + name + wxT("/");

	long temp;

	m_config->Read(m_name + KEY_CALLSIGN, &m_callsign, DEFAULT_CALLSIGN);

	m_config->Read(m_name + KEY_GATEWAY, &m_gateway, DEFAULT_GATEWAY);

	m_config->Read(m_name + KEY_MODE, &temp, long(DEFAULT_MODE));
	m_mode = DSTAR_MODE(temp);

	m_config->Read(m_name + KEY_ACK, &temp, long(DEFAULT_ACK));
	m_ack = ACK_TYPE(temp);

	m_config->Read(m_name + KEY_RESTRICTION, &m_restriction, DEFAULT_RESTRICTION);

	m_config->Read(m_name + KEY_RPT1_VALIDATION, &m_rpt1Validation, DEFAULT_RPT1_VALIDATION);

	m_config->Read(m_name + KEY_GATEWAY_ADDRESS, &m_gatewayAddress, DEFAULT_GATEWAY_ADDRESS);

	m_config->Read(m_name + KEY_GATEWAY_PORT, &temp, long(DEFAULT_GATEWAY_PORT));
	m_gatewayPort = (unsigned int)temp;

	m_config->Read(m_name + KEY_LOCAL_ADDRESS, &m_localAddress, DEFAULT_LOCAL_ADDRESS);

	m_config->Read(m_name + KEY_LOCAL_PORT, &temp, long(DEFAULT_LOCAL_PORT));
	m_localPort = (unsigned int)temp;

	m_config->Read(m_name + KEY_RECEIVER1_ADDRESS, &m_receiver1Address, DEFAULT_RECEIVER1_ADDRESS);

	m_config->Read(m_name + KEY_RECEIVER1_PORT, &temp, long(DEFAULT_RECEIVER1_PORT));
	m_receiver1Port = (unsigned int)temp;

	m_config->Read(m_name + KEY_RECEIVER2_ADDRESS, &m_receiver2Address, DEFAULT_RECEIVER2_ADDRESS);

	m_config->Read(m_name + KEY_RECEIVER2_PORT, &temp, long(DEFAULT_RECEIVER2_PORT));
	m_receiver2Port = (unsigned int)temp;

	m_config->Read(m_name + KEY_TRANSMITTER1_ADDRESS, &m_transmitter1Address, DEFAULT_TRANSMITTER1_ADDRESS);

	m_config->Read(m_name + KEY_TRANSMITTER1_PORT, &temp, long(DEFAULT_TRANSMITTER1_PORT));
	m_transmitter1Port = (unsigned int)temp;

	m_config->Read(m_name + KEY_TRANSMITTER2_ADDRESS, &m_transmitter2Address, DEFAULT_TRANSMITTER2_ADDRESS);

	m_config->Read(m_name + KEY_TRANSMITTER2_PORT, &temp, long(DEFAULT_TRANSMITTER2_PORT));
	m_transmitter2Port = (unsigned int)temp;

	m_config->Read(m_name + KEY_TIMEOUT, &temp, long(DEFAULT_TIMEOUT));
	m_timeout = (unsigned int)temp;

	m_config->Read(m_name + KEY_ACK_TIME, &temp, long(DEFAULT_ACK_TIME));
	m_ackTime = (unsigned int)temp;

	m_config->Read(m_name + KEY_FRAME_WAIT_TIME, &temp, long(DEFAULT_FRAME_WAIT_TIME));
	m_frameWaitTime = (unsigned int)temp;

	m_config->Read(m_name + KEY_BEACON_TIME, &temp, long(DEFAULT_BEACON_TIME));
	m_beaconTime = (unsigned int)temp;

	m_config->Read(m_name + KEY_BEACON_TEXT, &m_beaconText, DEFAULT_BEACON_TEXT);

	m_config->Read(m_name + KEY_BEACON_VOICE, &m_beaconVoice, DEFAULT_BEACON_VOICE);

	m_config->Read(m_name + KEY_LANGUAGE, &temp, long(DEFAULT_LANGUAGE));
	m_language = TEXT_LANG(temp);

	m_config->Read(m_name + KEY_CONTROL_ENABLED, &m_controlEnabled, DEFAULT_CONTROL_ENABLED);

	m_config->Read(m_name + KEY_CONTROL_RPT1, &m_controlRpt1Callsign, DEFAULT_CONTROL_RPT1);

	m_config->Read(m_name + KEY_CONTROL_RPT2, &m_controlRpt2Callsign, DEFAULT_CONTROL_RPT2);

	m_config->Read(m_name + KEY_CONTROL_SHUTDOWN, &m_controlShutdown, DEFAULT_CONTROL_SHUTDOWN);

	m_config->Read(m_name + KEY_CONTROL_STARTUP, &m_controlStartup, DEFAULT_CONTROL_STARTUP);

	m_config->Read(m_name + KEY_CONTROL_STATUS1, &m_controlStatus1, DEFAULT_CONTROL_STATUS1);

	m_config->Read(m_name + KEY_CONTROL_STATUS2, &m_controlStatus2, DEFAULT_CONTROL_STATUS2);

	m_config->Read(m_name + KEY_CONTROL_STATUS3, &m_controlStatus3, DEFAULT_CONTROL_STATUS3);

	m_config->Read(m_name + KEY_CONTROL_STATUS4, &m_controlStatus4, DEFAULT_CONTROL_STATUS4);

	m_config->Read(m_name + KEY_CONTROL_STATUS5, &m_controlStatus5, DEFAULT_CONTROL_STATUS5);

	m_config->Read(m_name + KEY_CONTROL_COMMAND1, &m_controlCommand1, DEFAULT_CONTROL_COMMAND1);

	m_config->Read(m_name + KEY_CONTROL_COMMAND1_LINE, &m_controlCommand1Line, DEFAULT_CONTROL_COMMAND1_LINE);

	m_config->Read(m_name + KEY_CONTROL_COMMAND2, &m_controlCommand2, DEFAULT_CONTROL_COMMAND2);

	m_config->Read(m_name + KEY_CONTROL_COMMAND2_LINE, &m_controlCommand2Line, DEFAULT_CONTROL_COMMAND2_LINE);

	m_config->Read(m_name + KEY_CONTROL_COMMAND3, &m_controlCommand3, DEFAULT_CONTROL_COMMAND3);

	m_config->Read(m_name + KEY_CONTROL_COMMAND3_LINE, &m_controlCommand3Line, DEFAULT_CONTROL_COMMAND3_LINE);

	m_config->Read(m_name + KEY_CONTROL_COMMAND4, &m_controlCommand4, DEFAULT_CONTROL_COMMAND4);

	m_config->Read(m_name + KEY_CONTROL_COMMAND4_LINE, &m_controlCommand4Line, DEFAULT_CONTROL_COMMAND4_LINE);

	m_config->Read(m_name + KEY_WINDOW_X, &temp, long(DEFAULT_WINDOW_X));
	m_x = int(temp);

	m_config->Read(m_name + KEY_WINDOW_Y, &temp, long(DEFAULT_WINDOW_Y));
	m_y = int(temp);
}

CSplitRepeaterConfig::~CSplitRepeaterConfig()
{
	delete m_config;
}

#else

CSplitRepeaterConfig::CSplitRepeaterConfig(const wxString& dir, const wxString& name) :
m_fileName(),
m_callsign(DEFAULT_CALLSIGN),
m_gateway(DEFAULT_GATEWAY),
m_mode(DEFAULT_MODE),
m_ack(DEFAULT_ACK),
m_restriction(DEFAULT_RESTRICTION),
m_rpt1Validation(DEFAULT_RPT1_VALIDATION),
m_gatewayAddress(DEFAULT_GATEWAY_ADDRESS),
m_gatewayPort(DEFAULT_GATEWAY_PORT),
m_localAddress(DEFAULT_LOCAL_ADDRESS),
m_localPort(DEFAULT_LOCAL_PORT),
m_receiver1Address(DEFAULT_RECEIVER1_ADDRESS),
m_receiver1Port(DEFAULT_RECEIVER1_PORT),
m_receiver2Address(DEFAULT_RECEIVER2_ADDRESS),
m_receiver2Port(DEFAULT_RECEIVER2_PORT),
m_transmitter1Address(DEFAULT_TRANSMITTER1_ADDRESS),
m_transmitter1Port(DEFAULT_TRANSMITTER1_PORT),
m_transmitter2Address(DEFAULT_TRANSMITTER2_ADDRESS),
m_transmitter2Port(DEFAULT_TRANSMITTER2_PORT),
m_timeout(DEFAULT_TIMEOUT),
m_ackTime(DEFAULT_ACK_TIME),
m_frameWaitTime(DEFAULT_FRAME_WAIT_TIME),
m_beaconTime(DEFAULT_BEACON_TIME),
m_beaconText(DEFAULT_BEACON_TEXT),
m_beaconVoice(DEFAULT_BEACON_VOICE),
m_language(DEFAULT_LANGUAGE),
m_controlEnabled(DEFAULT_CONTROL_ENABLED),
m_controlRpt1Callsign(DEFAULT_CONTROL_RPT1),
m_controlRpt2Callsign(DEFAULT_CONTROL_RPT2),
m_controlShutdown(DEFAULT_CONTROL_SHUTDOWN),
m_controlStartup(DEFAULT_CONTROL_STARTUP),
m_controlStatus1(DEFAULT_CONTROL_STATUS1),
m_controlStatus2(DEFAULT_CONTROL_STATUS2),
m_controlStatus3(DEFAULT_CONTROL_STATUS3),
m_controlStatus4(DEFAULT_CONTROL_STATUS4),
m_controlStatus5(DEFAULT_CONTROL_STATUS5),
m_controlCommand1(DEFAULT_CONTROL_COMMAND1),
m_controlCommand1Line(DEFAULT_CONTROL_COMMAND1_LINE),
m_controlCommand2(DEFAULT_CONTROL_COMMAND2),
m_controlCommand2Line(DEFAULT_CONTROL_COMMAND2_LINE),
m_controlCommand3(DEFAULT_CONTROL_COMMAND3),
m_controlCommand3Line(DEFAULT_CONTROL_COMMAND3_LINE),
m_controlCommand4(DEFAULT_CONTROL_COMMAND4),
m_controlCommand4Line(DEFAULT_CONTROL_COMMAND4_LINE),
m_x(DEFAULT_WINDOW_X),
m_y(DEFAULT_WINDOW_Y)
{
	wxASSERT(!dir.IsEmpty());

	wxString fileName = CONFIG_FILE_NAME;
	if (!name.IsEmpty())
		fileName = CONFIG_FILE_NAME + wxT("_") + name;

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

		if (key.IsSameAs(KEY_CALLSIGN)) {
			m_callsign = val;
		} else if (key.IsSameAs(KEY_GATEWAY)) {
			m_gateway = val;
		} else if (key.IsSameAs(KEY_MODE)) {
			val.ToLong(&temp1);
			m_mode = DSTAR_MODE(temp1);
		} else if (key.IsSameAs(KEY_ACK)) {
			val.ToLong(&temp1);
			m_ack = ACK_TYPE(temp1);
		} else if (key.IsSameAs(KEY_RESTRICTION)) {
			val.ToLong(&temp1);
			m_restriction = temp1 == 1L;
		} else if (key.IsSameAs(KEY_RPT1_VALIDATION)) {
			val.ToLong(&temp1);
			m_rpt1Validation = temp1 == 1L;
		} else if (key.IsSameAs(KEY_GATEWAY_ADDRESS)) {
			m_gatewayAddress = val;
		} else if (key.IsSameAs(KEY_GATEWAY_PORT)) {
			val.ToULong(&temp2);
			m_gatewayPort = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_LOCAL_ADDRESS)) {
			m_localAddress = val;
		} else if (key.IsSameAs(KEY_LOCAL_PORT)) {
			val.ToULong(&temp2);
			m_localPort = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_RECEIVER1_ADDRESS)) {
			m_receiver1Address = val;
		} else if (key.IsSameAs(KEY_RECEIVER1_PORT)) {
			val.ToULong(&temp2);
			m_receiver1Port = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_RECEIVER2_ADDRESS)) {
			m_receiver2Address = val;
		} else if (key.IsSameAs(KEY_RECEIVER2_PORT)) {
			val.ToULong(&temp2);
			m_receiver2Port = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_TRANSMITTER1_ADDRESS)) {
			m_transmitter1Address = val;
		} else if (key.IsSameAs(KEY_TRANSMITTER1_PORT)) {
			val.ToULong(&temp2);
			m_transmitter1Port = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_TRANSMITTER2_ADDRESS)) {
			m_transmitter2Address = val;
		} else if (key.IsSameAs(KEY_TRANSMITTER2_PORT)) {
			val.ToULong(&temp2);
			m_transmitter2Port = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_TIMEOUT)) {
			val.ToULong(&temp2);
			m_timeout = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_ACK_TIME)) {
			val.ToULong(&temp2);
			m_ackTime = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_FRAME_WAIT_TIME)) {
			val.ToULong(&temp2);
			m_frameWaitTime = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_BEACON_TIME)) {
			val.ToULong(&temp2);
			m_beaconTime = (unsigned int)temp2;
		} else if (key.IsSameAs(KEY_BEACON_TEXT)) {
			m_beaconText = val;
		} else if (key.IsSameAs(KEY_BEACON_VOICE)) {
			val.ToLong(&temp1);
			m_beaconVoice = temp1 == 1L;
		} else if (key.IsSameAs(KEY_LANGUAGE)) {
			val.ToLong(&temp1);
			m_language = TEXT_LANG(temp1);
		} else if (key.IsSameAs(KEY_CONTROL_ENABLED)) {
			val.ToLong(&temp1);
			m_controlEnabled = temp1 == 1L;
		} else if (key.IsSameAs(KEY_CONTROL_RPT1)) {
			m_controlRpt1Callsign = val;
		} else if (key.IsSameAs(KEY_CONTROL_RPT2)) {
			m_controlRpt2Callsign = val;
		} else if (key.IsSameAs(KEY_CONTROL_SHUTDOWN)) {
			m_controlShutdown = val;
		} else if (key.IsSameAs(KEY_CONTROL_STARTUP)) {
			m_controlStartup = val;
		} else if (key.IsSameAs(KEY_CONTROL_STATUS1)) {
			m_controlStatus1 = val;
		} else if (key.IsSameAs(KEY_CONTROL_STATUS2)) {
			m_controlStatus2 = val;
		} else if (key.IsSameAs(KEY_CONTROL_STATUS3)) {
			m_controlStatus3 = val;
		} else if (key.IsSameAs(KEY_CONTROL_STATUS4)) {
			m_controlStatus4 = val;
		} else if (key.IsSameAs(KEY_CONTROL_STATUS5)) {
			m_controlStatus5 = val;
		} else if (key.IsSameAs(KEY_CONTROL_COMMAND1)) {
			m_controlCommand1 = val;
		} else if (key.IsSameAs(KEY_CONTROL_COMMAND1_LINE)) {
			m_controlCommand1Line = val;
		} else if (key.IsSameAs(KEY_CONTROL_COMMAND2)) {
			m_controlCommand2 = val;
		} else if (key.IsSameAs(KEY_CONTROL_COMMAND2_LINE)) {
			m_controlCommand2Line = val;
		} else if (key.IsSameAs(KEY_CONTROL_COMMAND3)) {
			m_controlCommand3 = val;
		} else if (key.IsSameAs(KEY_CONTROL_COMMAND3_LINE)) {
			m_controlCommand3Line = val;
		} else if (key.IsSameAs(KEY_CONTROL_COMMAND4)) {
			m_controlCommand4 = val;
		} else if (key.IsSameAs(KEY_CONTROL_COMMAND4_LINE)) {
			m_controlCommand4Line = val;
		} else if (key.IsSameAs(KEY_WINDOW_X)) {
			val.ToLong(&temp1);
			m_x = int(temp1);
		} else if (key.IsSameAs(KEY_WINDOW_Y)) {
			val.ToLong(&temp1);
			m_y = int(temp1);
		}

		str = file.GetNextLine();
	}

	file.Close();
}

CSplitRepeaterConfig::~CSplitRepeaterConfig()
{
}

#endif

void CSplitRepeaterConfig::getCallsign(wxString& callsign, wxString& gateway, DSTAR_MODE& mode, ACK_TYPE& ack, bool& restriction, bool& rpt1Validation) const
{
	callsign       = m_callsign;
	gateway        = m_gateway;
	mode           = m_mode;
	ack            = m_ack;
	restriction    = m_restriction;
	rpt1Validation = m_rpt1Validation;
}

void CSplitRepeaterConfig::setCallsign(const wxString& callsign, const wxString& gateway, DSTAR_MODE mode, ACK_TYPE ack, bool restriction, bool rpt1Validation)
{
	m_callsign       = callsign;
	m_gateway        = gateway;
	m_mode           = mode;
	m_ack            = ack;
	m_restriction    = restriction;
	m_rpt1Validation = rpt1Validation;
}

void CSplitRepeaterConfig::getNetwork(wxString& gatewayAddress, unsigned int& gatewayPort, wxString& localAddress, unsigned int& localPort) const
{
	gatewayAddress = m_gatewayAddress;
	gatewayPort    = m_gatewayPort;
	localAddress   = m_localAddress;
	localPort      = m_localPort;
}

void CSplitRepeaterConfig::setNetwork(const wxString& gatewayAddress, unsigned int gatewayPort, const wxString& localAddress, unsigned int localPort)
{
	m_gatewayAddress = gatewayAddress;
	m_gatewayPort    = gatewayPort;
	m_localAddress   = localAddress;
	m_localPort      = localPort;
}

void CSplitRepeaterConfig::getReceiver1(wxString& address, unsigned int& port) const
{
	address = m_receiver1Address;
	port    = m_receiver1Port;
}

void CSplitRepeaterConfig::setReceiver1(const wxString& address, unsigned int port)
{
	m_receiver1Address = address;
	m_receiver1Port    = port;
}

void CSplitRepeaterConfig::getReceiver2(wxString& address, unsigned int& port) const
{
	address = m_receiver2Address;
	port    = m_receiver2Port;
}

void CSplitRepeaterConfig::setReceiver2(const wxString& address, unsigned int port)
{
	m_receiver2Address = address;
	m_receiver2Port    = port;
}

void CSplitRepeaterConfig::getTransmitter1(wxString& address, unsigned int& port) const
{
	address = m_transmitter1Address;
	port    = m_transmitter1Port;
}

void CSplitRepeaterConfig::setTransmitter1(const wxString& address, unsigned int port)
{
	m_transmitter1Address = address;
	m_transmitter1Port    = port;
}

void CSplitRepeaterConfig::getTransmitter2(wxString& address, unsigned int& port) const
{
	address = m_transmitter2Address;
	port    = m_transmitter2Port;
}

void CSplitRepeaterConfig::setTransmitter2(const wxString& address, unsigned int port)
{
	m_transmitter2Address = address;
	m_transmitter2Port    = port;
}

void CSplitRepeaterConfig::getTimes(unsigned int& timeout, unsigned int& ackTime, unsigned int& frameWaitTime) const
{
	timeout       = m_timeout;
	ackTime       = m_ackTime;
	frameWaitTime = m_frameWaitTime;
}

void CSplitRepeaterConfig::setTimes(unsigned int timeout, unsigned int ackTime, unsigned int frameWaitTime)
{
	m_timeout       = timeout;
	m_ackTime       = ackTime;
	m_frameWaitTime = frameWaitTime;
}

void CSplitRepeaterConfig::getBeacon(unsigned int& time, wxString& text, bool& voice, TEXT_LANG& language) const
{
	time     = m_beaconTime;
	text     = m_beaconText;
	voice    = m_beaconVoice;
	language = m_language;
}

void CSplitRepeaterConfig::setBeacon(unsigned int time, const wxString& text, bool voice, TEXT_LANG language)
{
	m_beaconTime  = time;
	m_beaconText  = text;
	m_beaconVoice = voice;
	m_language    = language;
}

void CSplitRepeaterConfig::getControl(bool& enabled, wxString& rpt1Callsign, wxString& rpt2Callsign, wxString& shutdown, wxString& startup, wxString& status1, wxString& status2, wxString& status3, wxString& status4, wxString& status5, wxString& command1, wxString& command1Line, wxString& command2, wxString& command2Line, wxString& command3, wxString& command3Line, wxString& command4, wxString& command4Line) const
{
	enabled      = m_controlEnabled;
	rpt1Callsign = m_controlRpt1Callsign;
	rpt2Callsign = m_controlRpt2Callsign;
	shutdown     = m_controlShutdown;
	startup      = m_controlStartup;
	status1      = m_controlStatus1;
	status2      = m_controlStatus2;
	status3      = m_controlStatus3;
	status4      = m_controlStatus4;
	status5      = m_controlStatus5;
	command1     = m_controlCommand1;
	command1Line = m_controlCommand1Line;
	command2     = m_controlCommand2;
	command2Line = m_controlCommand2Line;
	command3     = m_controlCommand3;
	command3Line = m_controlCommand3Line;
	command4     = m_controlCommand4;
	command4Line = m_controlCommand4Line;
}

void CSplitRepeaterConfig::setControl(bool enabled, const wxString& rpt1Callsign, const wxString& rpt2Callsign, const wxString& shutdown, const wxString& startup, const wxString& status1, const wxString& status2, const wxString& status3, const wxString& status4, const wxString& status5, const wxString& command1, const wxString& command1Line, const wxString& command2, const wxString& command2Line, const wxString& command3, const wxString& command3Line, const wxString& command4, const wxString& command4Line)
{
	m_controlEnabled      = enabled;
	m_controlRpt1Callsign = rpt1Callsign;
	m_controlRpt2Callsign = rpt2Callsign;
	m_controlShutdown     = shutdown;
	m_controlStartup      = startup;
	m_controlStatus1      = status1;
	m_controlStatus2      = status2;
	m_controlStatus3      = status3;
	m_controlStatus4      = status4;
	m_controlStatus5      = status5;
	m_controlCommand1     = command1;
	m_controlCommand1Line = command1Line;
	m_controlCommand2     = command2;
	m_controlCommand2Line = command2Line;
	m_controlCommand3     = command3;
	m_controlCommand3Line = command3Line;
	m_controlCommand4     = command4;
	m_controlCommand4Line = command4Line;
}

void CSplitRepeaterConfig::getPosition(int& x, int& y) const
{
	x = m_x;
	y = m_y;
}

void CSplitRepeaterConfig::setPosition(int x, int y)
{
	m_x = x;
	m_y = y;
}

#if defined(__WINDOWS__)

bool CSplitRepeaterConfig::write()
{
	m_config->Write(m_name + KEY_CALLSIGN, m_callsign);
	m_config->Write(m_name + KEY_GATEWAY, m_gateway);
	m_config->Write(m_name + KEY_MODE, long(m_mode));
	m_config->Write(m_name + KEY_ACK, long(m_ack));
	m_config->Write(m_name + KEY_RESTRICTION, m_restriction);
	m_config->Write(m_name + KEY_RPT1_VALIDATION, m_rpt1Validation);
	m_config->Write(m_name + KEY_GATEWAY_ADDRESS, m_gatewayAddress);
	m_config->Write(m_name + KEY_GATEWAY_PORT, long(m_gatewayPort));
	m_config->Write(m_name + KEY_LOCAL_ADDRESS, m_localAddress);
	m_config->Write(m_name + KEY_LOCAL_PORT, long(m_localPort));
	m_config->Write(m_name + KEY_RECEIVER1_ADDRESS, m_receiver1Address);
	m_config->Write(m_name + KEY_RECEIVER1_PORT, long(m_receiver1Port));
	m_config->Write(m_name + KEY_RECEIVER2_ADDRESS, m_receiver2Address);
	m_config->Write(m_name + KEY_RECEIVER2_PORT, long(m_receiver2Port));
	m_config->Write(m_name + KEY_TRANSMITTER1_ADDRESS, m_transmitter1Address);
	m_config->Write(m_name + KEY_TRANSMITTER1_PORT, long(m_transmitter1Port));
	m_config->Write(m_name + KEY_TRANSMITTER2_ADDRESS, m_transmitter2Address);
	m_config->Write(m_name + KEY_TRANSMITTER2_PORT, long(m_transmitter2Port));
	m_config->Write(m_name + KEY_TIMEOUT, long(m_timeout));
	m_config->Write(m_name + KEY_ACK_TIME, long(m_ackTime));
	m_config->Write(m_name + KEY_FRAME_WAIT_TIME, long(m_frameWaitTime));
	m_config->Write(m_name + KEY_BEACON_TIME, long(m_beaconTime));
	m_config->Write(m_name + KEY_BEACON_TEXT, m_beaconText);
	m_config->Write(m_name + KEY_BEACON_VOICE, m_beaconVoice);
	m_config->Write(m_name + KEY_LANGUAGE, long(m_language));
	m_config->Write(m_name + KEY_CONTROL_ENABLED, m_controlEnabled);
	m_config->Write(m_name + KEY_CONTROL_RPT1, m_controlRpt1Callsign);
	m_config->Write(m_name + KEY_CONTROL_RPT2, m_controlRpt2Callsign);
	m_config->Write(m_name + KEY_CONTROL_SHUTDOWN, m_controlShutdown);
	m_config->Write(m_name + KEY_CONTROL_STARTUP, m_controlStartup);
	m_config->Write(m_name + KEY_CONTROL_STATUS1, m_controlStatus1);
	m_config->Write(m_name + KEY_CONTROL_STATUS2, m_controlStatus2);
	m_config->Write(m_name + KEY_CONTROL_STATUS3, m_controlStatus3);
	m_config->Write(m_name + KEY_CONTROL_STATUS4, m_controlStatus4);
	m_config->Write(m_name + KEY_CONTROL_STATUS5, m_controlStatus5);
	m_config->Write(m_name + KEY_CONTROL_COMMAND1, m_controlCommand1);
	m_config->Write(m_name + KEY_CONTROL_COMMAND1_LINE, m_controlCommand1Line);
	m_config->Write(m_name + KEY_CONTROL_COMMAND2, m_controlCommand2);
	m_config->Write(m_name + KEY_CONTROL_COMMAND2_LINE, m_controlCommand2Line);
	m_config->Write(m_name + KEY_CONTROL_COMMAND3, m_controlCommand3);
	m_config->Write(m_name + KEY_CONTROL_COMMAND3_LINE, m_controlCommand3Line);
	m_config->Write(m_name + KEY_CONTROL_COMMAND4, m_controlCommand4);
	m_config->Write(m_name + KEY_CONTROL_COMMAND4_LINE, m_controlCommand4Line);
	m_config->Write(m_name + KEY_WINDOW_X, long(m_x));
	m_config->Write(m_name + KEY_WINDOW_Y, long(m_y));
	m_config->Flush();

	return true;
}

#else

bool CSplitRepeaterConfig::write()
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
	buffer.Printf(wxT("%s=%s"), KEY_CALLSIGN.c_str(), m_callsign.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_GATEWAY.c_str(), m_gateway.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_MODE.c_str(), int(m_mode)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_ACK.c_str(), int(m_ack)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_RESTRICTION.c_str(), m_restriction ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_RPT1_VALIDATION.c_str(), m_rpt1Validation ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_GATEWAY_ADDRESS.c_str(), m_gatewayAddress.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_GATEWAY_PORT.c_str(), m_gatewayPort); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_LOCAL_ADDRESS.c_str(), m_localAddress.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_LOCAL_PORT.c_str(), m_localPort); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_RECEIVER1_ADDRESS.c_str(), m_receiver1Address.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_RECEIVER1_PORT.c_str(), m_receiver1Port); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_RECEIVER2_ADDRESS.c_str(), m_receiver2Address.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_RECEIVER2_PORT.c_str(), m_receiver2Port); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_TRANSMITTER1_ADDRESS.c_str(), m_transmitter1Address.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_TRANSMITTER1_PORT.c_str(), m_transmitter1Port); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_TRANSMITTER2_ADDRESS.c_str(), m_transmitter2Address.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_TRANSMITTER2_PORT.c_str(), m_transmitter2Port); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_TIMEOUT.c_str(), m_timeout); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_ACK_TIME.c_str(), m_ackTime); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_FRAME_WAIT_TIME.c_str(), m_frameWaitTime); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%u"), KEY_BEACON_TIME.c_str(), m_beaconTime); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_BEACON_TEXT.c_str(), m_beaconText.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_BEACON_VOICE.c_str(), m_beaconVoice ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_LANGUAGE.c_str(), int(m_language)); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_CONTROL_ENABLED.c_str(), m_controlEnabled ? 1 : 0); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_CONTROL_RPT1.c_str(), m_controlRpt1Callsign.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_CONTROL_RPT2.c_str(), m_controlRpt2Callsign.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_CONTROL_SHUTDOWN.c_str(), m_controlShutdown.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_CONTROL_STARTUP.c_str(), m_controlStartup.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_CONTROL_STATUS1.c_str(), m_controlStatus1.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_CONTROL_STATUS2.c_str(), m_controlStatus2.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_CONTROL_STATUS3.c_str(), m_controlStatus3.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_CONTROL_STATUS4.c_str(), m_controlStatus4.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_CONTROL_STATUS5.c_str(), m_controlStatus5.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_CONTROL_COMMAND1.c_str(), m_controlCommand1.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_CONTROL_COMMAND1_LINE.c_str(), m_controlCommand1Line.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_CONTROL_COMMAND2.c_str(), m_controlCommand2.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_CONTROL_COMMAND2_LINE.c_str(), m_controlCommand2Line.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_CONTROL_COMMAND3.c_str(), m_controlCommand3.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_CONTROL_COMMAND3_LINE.c_str(), m_controlCommand3Line.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_CONTROL_COMMAND4.c_str(), m_controlCommand4.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%s"), KEY_CONTROL_COMMAND4_LINE.c_str(), m_controlCommand4Line.c_str()); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_WINDOW_X.c_str(), m_x); file.AddLine(buffer);
	buffer.Printf(wxT("%s=%d"), KEY_WINDOW_Y.c_str(), m_y); file.AddLine(buffer);

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

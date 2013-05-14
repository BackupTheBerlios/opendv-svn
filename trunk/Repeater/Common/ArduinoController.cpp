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

#include "ArduinoController.h"


const char IN_PORT1 = 0x01U;
const char IN_PORT2 = 0x02U;
const char IN_PORT3 = 0x04U;
const char IN_PORT4 = 0x08U;
const char IN_PORT5 = 0x10U;

const char OUT_PORT1 = 0x01U;
const char OUT_PORT2 = 0x02U;
const char OUT_PORT3 = 0x04U;
const char OUT_PORT4 = 0x08U;
const char OUT_PORT5 = 0x10U;
const char OUT_PORT6 = 0x20U;
const char OUT_PORT7 = 0x40U;
const char OUT_PORT8 = 0x80U;


CArduinoController::CArduinoController(const wxString& port) :
m_serial(port, SERIAL_19200),
m_inp1(false),
m_inp2(false),
m_inp3(false),
m_inp4(false),
m_inp5(false)
{
}

CArduinoController::~CArduinoController()
{
}

bool CArduinoController::open()
{
	bool ret = m_serial.open();
	if (!ret)
		return false;

	setDigitalOutputs(false, false, false, false, false, false, false, false);

	return true;
}

void CArduinoController::getDigitalInputs(bool& inp1, bool& inp2, bool& inp3, bool& inp4, bool& inp5)
{
	inp1 = m_inp1;
	inp2 = m_inp2;
	inp3 = m_inp3;
	inp4 = m_inp4;
	inp5 = m_inp5;

	unsigned char buffer;
	int ret = m_serial.read(&buffer, 1U);
	if (ret <= 0)
		return;

	inp1 = m_inp1 = (buffer & IN_PORT1) == IN_PORT1;
	inp2 = m_inp2 = (buffer & IN_PORT2) == IN_PORT2;
	inp3 = m_inp3 = (buffer & IN_PORT3) == IN_PORT3;
	inp4 = m_inp4 = (buffer & IN_PORT4) == IN_PORT4;
	inp5 = m_inp5 = (buffer & IN_PORT5) == IN_PORT5;
}

void CArduinoController::setDigitalOutputs(bool outp1, bool outp2, bool outp3, bool outp4, bool outp5, bool outp6, bool outp7, bool outp8)
{
	unsigned char buffer = 0x00U;
	buffer |= outp1 ? OUT_PORT1 : 0x00U;
	buffer |= outp2 ? OUT_PORT2 : 0x00U;
	buffer |= outp3 ? OUT_PORT3 : 0x00U;
	buffer |= outp4 ? OUT_PORT4 : 0x00U;
	buffer |= outp5 ? OUT_PORT5 : 0x00U;
	buffer |= outp6 ? OUT_PORT6 : 0x00U;
	buffer |= outp7 ? OUT_PORT7 : 0x00U;
	buffer |= outp8 ? OUT_PORT8 : 0x00U;

	m_serial.write(&buffer, 1U);
}

void CArduinoController::close()
{
	setDigitalOutputs(false, false, false, false, false, false, false, false);

	m_serial.close();
}

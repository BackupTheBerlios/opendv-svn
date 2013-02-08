/*
 *   Copyright (C) 2010-2013 by Jonathan Naylor G4KLX
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

#include "RepeaterHandler.h"
#include "DongleHandler.h"
#include "DStarDefines.h"
#include "Utils.h"

unsigned int            CDongleHandler::m_maxReflectors = 0U;
unsigned int            CDongleHandler::m_maxDongles = 0U;
CDongleHandler**        CDongleHandler::m_reflectors = NULL;

wxString                CDongleHandler::m_callsign;
CDongleProtocolHandler* CDongleHandler::m_handler = NULL;

bool                    CDongleHandler::m_stateChange = false;

CHeaderLogger*          CDongleHandler::m_headerLogger = NULL;


CDongleHandler::CDongleHandler(const wxString& reflector, const in_addr& address, unsigned int port) :
m_reflector(reflector),
m_yourAddress(address),
m_yourPort(port),
m_destination(NULL),
m_time(),
m_pollTimer(1000U, 10U),
m_pollInactivityTimer(1000U, 60U),
m_dExtraId(0x00U),
m_dExtraSeq(0x00U),
m_inactivityTimer(1000U, 2U),
m_header(NULL)
{
	wxASSERT(port > 0U);

	m_pollInactivityTimer.start();

	m_time = ::time(NULL);

	m_pollTimer.start();
	m_stateChange = true;
}

CDongleHandler::~CDongleHandler()
{
	delete m_header;
}

void CDongleHandler::initialise(unsigned int maxReflectors)
{
	wxASSERT(maxReflectors > 0U);

	m_maxReflectors = maxReflectors;

	m_reflectors = new CDongleHandler*[m_maxReflectors];
	for (unsigned int i = 0U; i < m_maxReflectors; i++)
		m_reflectors[i] = NULL;
}

void CDongleHandler::setCallsign(const wxString& callsign)
{
	m_callsign = callsign;
	m_callsign.SetChar(LONG_CALLSIGN_LENGTH - 1U, wxT(' '));
}

void CDongleHandler::setDongleProtocolHandler(CDongleProtocolHandler* handler)
{
	wxASSERT(handler != NULL);

	m_handler = handler;
}

void CDongleHandler::setHeaderLogger(CHeaderLogger* logger)
{
	m_headerLogger = logger;
}

void CDongleHandler::setMaxDongles(unsigned int maxDongles)
{
	m_maxDongles = maxDongles;
}

void CDongleHandler::getInfo(IReflectorCallback* handler, CRemoteRepeaterData& data)
{
	wxASSERT(handler != NULL);

	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		CDongleHandler* reflector = m_reflectors[i];
		if (reflector != NULL) {
			if (reflector->m_destination == handler)
				data.addLink(reflector->m_reflector, PROTO_DEXTRA, true, DIR_INCOMING, true);
		}
	}
}

wxString CDongleHandler::getDongles()
{
	wxString dongles;

	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		CDongleHandler* reflector = m_reflectors[i];
		if (reflector != NULL) {
			dongles.Append(wxT("X:"));
			dongles.Append(reflector->m_reflector);
			dongles.Append(wxT("  "));
		}
	}

	return dongles;
}

void CDongleHandler::process(CHeaderData& header)
{
	in_addr   yourAddress = header.getYourAddress();
	unsigned int yourPort = header.getYourPort();

	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		CDongleHandler* reflector = m_reflectors[i];
		if (reflector != NULL) {
			if (reflector->m_yourAddress.s_addr == yourAddress.s_addr && reflector->m_yourPort == yourPort)
				reflector->processInt(header);
		}
	}	
}

void CDongleHandler::process(CAMBEData& data)
{
	in_addr   yourAddress = data.getYourAddress();
	unsigned int yourPort = data.getYourPort();

	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		CDongleHandler* reflector = m_reflectors[i];
		if (reflector != NULL) {
			if (reflector->m_yourAddress.s_addr == yourAddress.s_addr && reflector->m_yourPort == yourPort)
				reflector->processInt(data);
		}
	}	
}

void CDongleHandler::process(const CPollData& poll)
{
	bool found = false;

	wxString    reflector = poll.getData1();
	in_addr   yourAddress = poll.getYourAddress();
	unsigned int yourPort = poll.getYourPort();

	// Check to see if we already have a link
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		if (m_reflectors[i] != NULL) {
			if (m_reflectors[i]->m_reflector.Left(LONG_CALLSIGN_LENGTH - 1U).IsSameAs(reflector.Left(LONG_CALLSIGN_LENGTH - 1U)) &&
				m_reflectors[i]->m_yourAddress.s_addr == yourAddress.s_addr &&
				m_reflectors[i]->m_yourPort           == yourPort) {
				m_reflectors[i]->m_pollInactivityTimer.reset();
				found = true;
			}
		}
	}	

	if (found)
		return;

	// A repeater poll arriving here is an error
	if (!poll.isDongle())
		return;

	// Check to see if we are allowed to accept it
	unsigned int count = 0U;
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		if (m_reflectors[i] != NULL)
			count++;
	}

	if (count >= m_maxDongles)
		return;

	// An unmatched poll indicates the need for a new entry
	wxLogMessage(wxT("New incoming DExtra Dongle from %s"), reflector.c_str());

	CDongleHandler* handler = new CDongleHandler(reflector, yourAddress, yourPort);

	found = false;

	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		if (m_reflectors[i] == NULL) {
			m_reflectors[i] = handler;
			found = true;
			break;
		}
	}

	if (found) {
		// Return the poll
		CPollData poll(m_callsign, yourAddress, yourPort);
		m_handler->writeDExtraPoll(poll);
	} else {
		wxLogError(wxT("No space to add new DExtra Dongle, ignoring"));
		delete handler;
	}
}

void CDongleHandler::writeHeader(IReflectorCallback* handler, CHeaderData& header)
{
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		if (m_reflectors[i] != NULL)
			m_reflectors[i]->writeHeaderInt(handler, header);
	}
}

void CDongleHandler::writeAMBE(IReflectorCallback* handler, CAMBEData& data)
{
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		if (m_reflectors[i] != NULL)
			m_reflectors[i]->writeAMBEInt(handler, data);
	}	
}

void CDongleHandler::clock(unsigned int ms)
{
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		if (m_reflectors[i] != NULL) {
			bool ret = m_reflectors[i]->clockInt(ms);
			if (ret) {
				delete m_reflectors[i];
				m_reflectors[i] = NULL;
			}
		}
	}
}

void CDongleHandler::finalise()
{
	for (unsigned int i = 0U; i < m_maxReflectors; i++)
		delete m_reflectors[i];

	delete[] m_reflectors;
}

void CDongleHandler::processInt(CHeaderData& header)
{
	unsigned int id = header.getId();
	wxString rpt1 = header.getRptCall1();
	wxString rpt2 = header.getRptCall2();

	// A Dongle connection
	// Check the destination callsign
	m_destination = CRepeaterHandler::findDVRepeater(rpt2);
	if (m_destination == NULL) {
		m_destination = CRepeaterHandler::findDVRepeater(rpt1);
		if (m_destination == NULL)
			return;
	}

	// If we're already processing, ignore the new header
	if (m_dExtraId != 0x00U)
		return;

	// Write to Header.log if it's enabled
	if (m_headerLogger != NULL)
		m_headerLogger->write(wxT("DExtra"), header);

	m_dExtraId  = id;
	m_dExtraSeq = 0x00U;
	m_inactivityTimer.start();

	delete m_header;

	m_header = new CHeaderData(header);
	m_header->setCQCQCQ();
	m_header->setFlags(0x00U, 0x00U, 0x00U);

	m_destination->process(*m_header, DIR_INCOMING, AS_DEXTRA);
}

void CDongleHandler::processInt(CAMBEData& data)
{
	if (m_dExtraId != data.getId())
		return;

	m_inactivityTimer.reset();

	m_dExtraSeq = data.getSeq();

	// Send the header every 21 frames
	if (m_dExtraSeq == 0U)
		m_destination->process(*m_header, DIR_INCOMING, AS_DUP);

	// Copy the data to ensure it remains unchanged
	CAMBEData temp(data);

	m_destination->process(temp, DIR_INCOMING, AS_DEXTRA);

	if (temp.isEnd()) {
		delete m_header;
		m_header = NULL;

		m_dExtraId  = 0x00U;
		m_dExtraSeq = 0x00U;

		m_inactivityTimer.stop();
	}
}

bool CDongleHandler::clockInt(unsigned int ms)
{
	m_pollTimer.clock(ms);
	m_inactivityTimer.clock(ms);
	m_pollInactivityTimer.clock(ms);

	if (m_pollInactivityTimer.isRunning() && m_pollInactivityTimer.hasExpired()) {
		m_pollInactivityTimer.reset();

		delete m_header;
		m_header = NULL;

		m_stateChange = true;
		m_dExtraId    = 0x00U;
		m_dExtraSeq   = 0x00U;

		wxLogMessage(wxT("DExtra link to %s has failed (poll inactivity)"), m_reflector.c_str());

		return true;
	}

	if (m_pollTimer.isRunning() && m_pollTimer.hasExpired()) {
		CPollData poll(m_callsign, m_yourAddress, m_yourPort);
		m_handler->writeDExtraPoll(poll);
		m_pollTimer.reset();
	}

	if (m_inactivityTimer.isRunning() && m_inactivityTimer.hasExpired()) {
		delete m_header;
		m_header = NULL;

		m_dExtraId  = 0x00U;
		m_dExtraSeq = 0x00U;

		m_inactivityTimer.stop();
	}

	return false;
}

void CDongleHandler::writeHeaderInt(IReflectorCallback* handler, CHeaderData& header)
{
	// Already in use?
	if (m_dExtraId != 0x00)
		return;

	header.setDestination(m_yourAddress, m_yourPort);
	m_handler->writeDExtraHeader(header);
}

void CDongleHandler::writeAMBEInt(IReflectorCallback* handler, CAMBEData& data)
{
	// Already in use?
	if (m_dExtraId != 0x00)
		return;

	data.setDestination(m_yourAddress, m_yourPort);
	m_handler->writeDExtraAMBE(data);
}

bool CDongleHandler::stateChange()
{
	bool stateChange = m_stateChange;

	m_stateChange = false;

	return stateChange;
}

void CDongleHandler::writeStatus(wxFFile& file)
{
	for (unsigned int i = 0U; i < m_maxReflectors; i++) {
		CDongleHandler* reflector = m_reflectors[i];
		if (reflector != NULL) {
			wxString text;

			struct tm* tm = ::gmtime(&reflector->m_time);

			text.Printf(wxT("%04d-%02d-%02d %02d:%02d:%02d: DExtra link - Type: Dongle User: %s Dir: Incoming\n"),
							tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec, 
							reflector->m_reflector.c_str());
		}
	}
}

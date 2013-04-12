/*
 *   Copyright (C) 2013 by Jonathan Naylor G4KLX
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

#include "DStarModemThread.h"

CDStarModemThread::CDStarModemThread() :
m_server(NULL),
m_modem(NULL),
m_killed(false)
{
}

CDStarModemThread::~CDStarModemThread()
{
}

void CDStarModemThread::setServer(CModemProtocolServer* server)
{
	wxASSERT(server != NULL);

	m_server = server;
}

void CDStarModemThread::setModem(IDStarModem* modem)
{
	wxASSERT(modem != NULL);

	m_modem = modem;
}

void CDStarModemThread::run()
{
	while (!m_killed && (m_server == NULL || m_modem == NULL))
		::wxMilliSleep(500UL);

	if (m_killed)
		return;

	wxLogMessage(wxT("Starting the D-Star Modem thread"));

	unsigned int timeout = 0U;
	bool connected = false;

	while (!m_killed) {
		timeout++;

		MODEM_MSG_TYPE serverType = m_server->read();
		if (serverType == MMT_NONE) {
		} else if (serverType == MMT_BEGIN) {
			m_server->writeBegin();
			connected = true;
			timeout = 0U;
		} else if (serverType == MMT_END) {
			m_server->writeEnd();
			connected = false;
			timeout = 0U;
		} else if (serverType == MMT_HEADER) {
			CHeaderData* header = m_server->readHeader();
			m_modem->writeHeader(*header);
			delete header;
			timeout = 0U;
		} else if (serverType == MMT_DATA) {
			unsigned char data[DV_FRAME_LENGTH_BYTES];
			bool end;
			unsigned int length = m_server->readData(data, DV_FRAME_LENGTH_BYTES, end);
			m_modem->writeData(data, length, end);
			timeout = 0U;
		} else if (serverType == MMT_TX) {
			bool tx = m_server->readTX();
			m_modem->setTX(tx);
			timeout = 0U;
		} else if (serverType == MMT_STATUS) {
			unsigned int space = m_modem->getSpace();
			m_server->writeSpace(space);
			bool tx = m_modem->getTX();
			m_server->writeTX(tx);
			timeout = 0U;
		}

		DSMT_TYPE modemType = m_modem->read();
		if (modemType == DSMTT_HEADER) {
			CHeaderData* header = m_modem->readHeader();

			if (connected)
				m_server->writeHeader(*header);

			delete header;
		} else if (modemType == DSMTT_DATA) {
			unsigned char data[DV_FRAME_LENGTH_BYTES];
			bool end;
			unsigned int length = m_modem->readData(data, DV_FRAME_LENGTH_BYTES, end);

			if (connected)
				m_server->writeData(data, length, end);
		}

		if (timeout >= 20U && connected) {
			wxLogError(wxT("Watchdog has timed out, client has disappeared"));
			connected = false;
		}

		::wxMilliSleep(10UL);
	}

	wxLogMessage(wxT("Stopping the D-Star Modem thread"));

	m_server->close();
	delete m_server;

	m_modem->close();
}

void CDStarModemThread::kill()
{
	m_killed = true;
}

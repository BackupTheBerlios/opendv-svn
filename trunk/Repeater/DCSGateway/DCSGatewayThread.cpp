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

#include "DCSGatewayHeaderData.h"
#include "DCSGatewayAMBEData.h"
#include "DCSGatewayHostFile.h"
#include "DCSGatewayThread.h"
#include "DCSGatewayApp.h"
#include "DStarDefines.h"
#include "Utils.h"

#include <wx/filename.h>

const unsigned int TIME_FRACTION = 2U;

const unsigned int CYCLE_TIME = DSTAR_FRAME_TIME_MS / TIME_FRACTION;

CDCSGatewayThread::CDCSGatewayThread() :
wxThread(wxTHREAD_JOINABLE),
m_killed(false),
m_stopped(true),
m_reflectors(),
m_hostnames(),
m_repeaterHandler(NULL),
m_dcsHandler(),
m_ccsHandler(),
m_audioUnit(NULL),
m_echoUnit(NULL),
m_header(),
m_textCollector(),
m_text(),
m_myCall(),
m_yourCall(),
m_rptCall1(),
m_rptCall2(),
m_repeaterCall(),
m_state(LINK_NONE),
m_reflector(),
m_startupReflector(),
m_atStartup(false),
m_reconnect(RECONNECT_NEVER),
m_language(TL_ENGLISH_UK),
m_reconnectTimer(1000U, 0U),
m_watchdogTimer(1000U, 2U),
m_ccsTimer(1000U, 300U),
m_id(0U),
m_seq(0U),
m_echo(false),
m_info(false)
{
}

CDCSGatewayThread::~CDCSGatewayThread()
{
}

void* CDCSGatewayThread::Entry()
{
	// Wait here until we have the essentials to run
	while (!m_killed && m_repeaterHandler == NULL)
		Sleep(500UL);		// 1/2 sec

	if (m_killed)
		return NULL;

	m_stopped = false;

	wxLogMessage(wxT("Starting the DCS Gateway thread"));

	wxFileName fileName(wxFileName::GetHomeDir(), HOSTS_FILE_NAME);

	CDCSGatewayAMBEData::initialise();
	CDCSGatewayAudioUnit::initialise();
	CDCSGatewayAudioUnit::setLanguage(m_language);

	m_audioUnit = new CDCSGatewayAudioUnit(this, m_repeaterCall);

	m_echoUnit = new CDCSGatewayEchoUnit(this, m_repeaterCall);

	if (!fileName.IsFileReadable()) {
		wxLogMessage(wxT("File %s not readable"), fileName.GetFullPath().c_str());
#if defined(__WINDOWS__)
		fileName.Assign(::wxGetCwd(), HOSTS_FILE_NAME);
#else
		fileName.Assign(wxT(DATA_DIR), HOSTS_FILE_NAME);
#endif
		if (!fileName.IsFileReadable())
			wxLogMessage(wxT("File %s not readable"), fileName.GetFullPath().c_str());
	}

	CDCSGatewayHostFile hostFile(fileName.GetFullPath());
	m_reflectors = hostFile.getNames();
	m_hostnames  = hostFile.getHosts();

	m_reconnectTimer.start();

	if (m_atStartup && !m_startupReflector.IsEmpty()) {
		writeLinkingTo(m_startupReflector);

		wxLogMessage(wxT("Linking at startup to %s"), m_startupReflector.c_str());
		link(m_startupReflector);
	}

	m_ccsHandler.registration(CCS_HOST, CCS_PORT);

	wxStopWatch timer;

	while (!m_killed) {
		timer.Start();

		for (unsigned int i = 0U; i < 10U; i++) {
			CDCSGatewayHeaderData* header;
			CDCSGatewayAMBEData* data;

			bool rptData = true;
			bool dcsData = true;
			bool ccsData = true;

			REPEATER_TYPE rptType = m_repeaterHandler->read();
			switch (rptType) {
				case RT_NONE:
					rptData = false;
					break;
				case RT_HEADER:
					header = m_repeaterHandler->readHeader();
					if (header != NULL) {
						m_seq = 0U;
						wxLogMessage(wxT("Repeater header - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s  Flags: %02X %02X %02X"), header->getMyCall1().c_str(), header->getMyCall2().c_str(), header->getYourCall().c_str(), header->getRptCall1().c_str(), header->getRptCall2().c_str(), header->getFlag1(), header->getFlag2(), header->getFlag3());
						showHeader(*header);
						commandHandler(header->getYourCall());
						m_ccsHandler.writeUserStatus(*header);
						if (m_echo)
							m_echoUnit->writeHeader(*header);
						delete header;

						CDCSGatewayAMBEData::setText(wxEmptyString);
						m_textCollector.reset();
						m_text.Clear();
					}
					break;
				case RT_DATA:
					data = m_repeaterHandler->readData();
					if (data != NULL) {
						// Get the slow data text
						if (m_text.IsEmpty()) {
							m_textCollector.writeData(*data);

							bool hasData = m_textCollector.hasData();
							if (hasData) {
								m_text = m_textCollector.getData();
								CDCSGatewayAMBEData::setText(m_text);
							}
						}

						if (m_echo) {
							m_echoUnit->writeData(*data);
						} else if (m_info) {
							// Do nothing
						} else {
							if (m_state == LINK_LINKED && (m_yourCall.Right(1U).IsSameAs(wxT("L")) || m_yourCall.Left(4U).IsSameAs(wxT("CQCQ")))) {
								data->setRptSeq(m_seq);
								m_seq++;

								m_dcsHandler.writeData(m_header, *data);
							} else if (m_state == LINK_CCS) {
								data->setRptSeq(m_seq);
								m_seq++;

								m_ccsHandler.writeData(m_header, *data);
							}
						}

						bool end = data->isEnd();
						if (end) {
							if (m_info)
								m_audioUnit->sendState(m_state, m_reflector);

							clearHeader();
							m_echo = false;
							m_info = false;
						}
						delete data;
					}
					break;
			}

			DCS_TYPE dcsType = m_dcsHandler.read();
			switch (dcsType) {
				case DT_NONE:
					dcsData = false;
					break;
				case DT_DATA:
					data = m_dcsHandler.readData();
					if (data != NULL) {
						unsigned int id = data->getId();
						if (m_state == LINK_LINKED && m_id == 0x00U) {
							CDCSGatewayHeaderData* header = m_dcsHandler.getHeader();
							if (header != NULL && header->getRptCall2().IsSameAs(m_reflector)) {
								wxLogMessage(wxT("DCS header - My: %s/%s  Your: %s  Rpt1: %s  Rpt2: %s"), header->getMyCall1().c_str(), header->getMyCall2().c_str(), header->getYourCall().c_str(), header->getRptCall1().c_str(), header->getRptCall2().c_str());
								m_id = id;
								showHeader(*header);
								writeHeader(*header);
								m_watchdogTimer.start();
							}
							delete header;
						}

						if (m_state == LINK_LINKED && id == m_id) {
							unsigned int seq = data->getRptSeq();

							// Repeat the header every two seconds
							if ((seq % 100U) == 0U) {
								CDCSGatewayHeaderData* header = m_dcsHandler.getHeader();
								writeHeader(*header);
								delete header;
							}

							writeData(*data);
							m_watchdogTimer.reset();

							bool end = data->isEnd();
							if (end) {
								m_watchdogTimer.stop();
								clearHeader();
								m_id = 0x00U;
							}
						}
						delete data;
					}
					break;
			}

			CCS_TYPE ccsType = m_ccsHandler.read();
			switch (ccsType) {
				case CT_NONE:
					ccsData = false;
					break;
				case CT_DATA:
					data = m_ccsHandler.readData();
					if (data != NULL) {
						unsigned int id = data->getId();
						if (m_id == 0x00U) {
							CDCSGatewayHeaderData* header = m_ccsHandler.getHeader();
							if (header != NULL) {
								wxLogMessage(wxT("CCS header - My: %s/%s  Your: %s"), header->getMyCall1().c_str(), header->getMyCall2().c_str(), header->getYourCall().c_str());
								m_id = id;
								showHeader(*header);
								writeHeader(*header);
								m_watchdogTimer.start();
							}
							delete header;
						}

						if (id == m_id) {
							unsigned int seq = data->getRptSeq();

							// Repeat the header every two seconds
							if ((seq % 100U) == 0U) {
								CDCSGatewayHeaderData* header = m_ccsHandler.getHeader();
								writeHeader(*header);
								delete header;
							}

							writeData(*data);
							m_watchdogTimer.reset();

							bool end = data->isEnd();
							if (end) {
								m_watchdogTimer.stop();
								clearHeader();
								m_id = 0x00U;
							}
						}
						delete data;
					}
					break;
				case CT_TERMINATE:
					if (m_state == LINK_CCS) {
						wxLogMessage(wxT("CCS routing terminated by remote system"));
						m_ccsTimer.stop();
						m_state = LINK_NONE;
					}
					ccsData = false;
					break;
				case CT_QUERY:
					ccsData = false;
					m_ccsHandler.writeQueryReply(m_state == LINK_CCS, m_yourCall, m_ccsTimer.getRemaining());
					break;
				case CT_QUERY_REPLY:
					ccsData = false;
					break;
			}

			// Any activity resets the reconnect timer
			if (rptData || dcsData || ccsData) {
				if (m_state == LINK_CCS)
					m_ccsTimer.start();

				m_reconnectTimer.start();
			}

			// No more work to be done?
			if (!rptData && !dcsData && !ccsData)
				break;
		}

		if (m_watchdogTimer.isRunning() && m_watchdogTimer.hasExpired()) {
			wxLogMessage(wxT("Network watchdog timer has expired"));
			clearHeader();
			m_watchdogTimer.stop();
			m_id = 0x00U;
		}

		if (m_ccsTimer.isRunning() && m_ccsTimer.hasExpired()) {
			wxLogMessage(wxT("CCS timer has expired"));
			m_ccsHandler.writeTerminate();
			m_ccsTimer.stop();
		}

		if (m_reconnectTimer.isRunning() && m_reconnectTimer.hasExpired()) {
			if (!m_startupReflector.IsEmpty() && (m_state == LINK_NONE || !m_reflector.IsSameAs(m_startupReflector))) {
				wxLogMessage(wxT("Reconnecting to %s"), m_startupReflector.c_str());
				unlink();
				link(m_startupReflector);

				writeLinkingTo(m_startupReflector);
			}
		}

		if (m_state == LINK_LINKING || m_state == LINK_LINKED) {
			bool connected = m_dcsHandler.isConnected();

			if (m_state == LINK_LINKING && connected) {
				writeLinkedTo(m_reflector);

				wxLogMessage(wxT("Linked to %s"), m_reflector.c_str());
				m_state = LINK_LINKED;
			} else if (m_state == LINK_LINKED && !connected) {
				writeLinkingTo(m_reflector);

				wxLogMessage(wxT("Link lost with %s, relinking"), m_reflector.c_str());
				m_state = LINK_LINKING;
			}
		}

		unsigned int ms = timer.Time();

		if (ms < CYCLE_TIME) {
			Sleep(CYCLE_TIME - ms);
			ms = timer.Time();
		}

		m_dcsHandler.clock(ms);
		m_reconnectTimer.clock(ms);
		m_watchdogTimer.clock(ms);
		m_ccsTimer.clock(ms);

		m_audioUnit->clock(ms);
		m_echoUnit->clock(ms);
	}

	wxLogMessage(wxT("Stopping the DCS Gateway thread"));

	m_dcsHandler.unlink();
	m_ccsHandler.cancelation();

	m_repeaterHandler->close();
	delete m_repeaterHandler;

	delete m_audioUnit;
	delete m_echoUnit;

	CDCSGatewayAudioUnit::finalise();
	CDCSGatewayAMBEData::finalise();

	return NULL;
}

void CDCSGatewayThread::kill()
{
	m_killed = true;
}

void CDCSGatewayThread::setReflector(const wxString& callsign, const wxString& locator, const wxString& reflector, bool atStartup, RECONNECT reconnect, TEXT_LANG language)
{
	m_dcsHandler.setCallsign(callsign);
	m_ccsHandler.setCallsign(callsign);

	m_ccsHandler.setLocator(locator);

	m_startupReflector = reflector;
	m_atStartup        = atStartup;
	m_reconnect        = reconnect;
	m_language         = language;

	switch (reconnect) {
		case RECONNECT_5MINS:
			m_reconnectTimer.setTimeout(5U * 60U);
			break;
		case RECONNECT_10MINS:
			m_reconnectTimer.setTimeout(10U * 60U);
			break;
		case RECONNECT_15MINS:
			m_reconnectTimer.setTimeout(15U * 60U);
			break;
		case RECONNECT_20MINS:
			m_reconnectTimer.setTimeout(20U * 60U);
			break;
		case RECONNECT_25MINS:
			m_reconnectTimer.setTimeout(25U * 60U);
			break;
		case RECONNECT_30MINS:
			m_reconnectTimer.setTimeout(30U * 60U);
			break;
		default:
			break;
	}
}

void CDCSGatewayThread::setRepeater(const wxString& repeaterCallsign, CDCSGatewayRepeaterProtocolHandler* repeaterHandler)
{
	wxASSERT(!repeaterCallsign.IsEmpty());
	wxASSERT(repeaterHandler != NULL);

	if (!m_stopped)
		return;

	m_repeaterHandler = repeaterHandler;
	m_repeaterCall    = repeaterCallsign;
}

CDCSGatewayStatusData* CDCSGatewayThread::getStatus()
{
	return new CDCSGatewayStatusData(m_myCall, m_yourCall, m_rptCall1, m_rptCall2, m_state,
		m_reflector, m_reconnectTimer.getTimer(), m_reconnectTimer.getTimeout());
}

void CDCSGatewayThread::commandHandler(const wxString& yourCall)
{
	m_echo = false;
	m_info = false;

	if (yourCall.IsSameAs(wxT("       E"))) {
		m_echo = true;
		return;
	}

	if (yourCall.IsSameAs(wxT("       I"))) {
		m_info = true;
		return;
	}

	if (yourCall.IsSameAs(wxT("       X"))) {
		if (m_state == LINK_CCS) {
			wxLogMessage(wxT("CCS routing ended by %s"), m_myCall.c_str());
			m_ccsHandler.writeTerminate();
			m_ccsTimer.stop();
			m_state = LINK_NONE;
		}
		return;
	}

	if (m_reconnect == RECONNECT_FIXED)
		return;

	wxString letter = yourCall.Right(1U);

	if (letter.IsSameAs(wxT("U"))) {
		if (m_state == LINK_LINKING || m_state == LINK_LINKED) {
			writeNotLinked();

			wxLogMessage(wxT("Unlink command issued by %s"), m_myCall.c_str());
			unlink();
		}
	} else if (letter.IsSameAs(wxT("L"))) {
		if (m_state != LINK_CCS) {
			// Extract the callsign "1234567L" -> "123456 7"
			wxString reflector = yourCall.Left(LONG_CALLSIGN_LENGTH - 2U);
			reflector.Append(wxT(" "));
			reflector.Append(yourCall.Mid(LONG_CALLSIGN_LENGTH - 2U, 1));

			// Check for a duplicate link command
			if (m_reflector.IsSameAs(reflector))
				return;

			// Validate the reflector callsign
			wxString baseName = reflector.Left(LONG_CALLSIGN_LENGTH - 1U).Trim();
			int index = m_reflectors.Index(baseName);
			if (index == wxNOT_FOUND) {
				wxLogMessage(wxT("Invalid link command to %s issued by %s"), reflector.c_str(), m_myCall.c_str());
				return;
			}

			wxLogMessage(wxT("Link command to %s issued by %s"), reflector.c_str(), m_myCall.c_str());
			unlink();
			link(reflector);

			writeLinkingTo(reflector);
		}
	} else if (!yourCall.Left(4U).IsSameAs(wxT("CQCQ"))) {
		if (m_state != LINK_CCS) {
			wxLogMessage(wxT("CCS route started by %s -> %s"), m_myCall.c_str(), yourCall.c_str());

			writeNotLinked();
			unlink();

			m_state = LINK_CCS;
		}

		m_ccsTimer.start();
		m_ccsHandler.setRemote(yourCall);
	}
}

void CDCSGatewayThread::unlink()
{
	// Anything to do?
	if (m_state != LINK_LINKING && m_state != LINK_LINKED)
		return;

	m_dcsHandler.unlink();
	m_ccsHandler.setReflector(wxEmptyString);

	m_state = LINK_NONE;
}

bool CDCSGatewayThread::link(const wxString &reflector)
{
	if (m_state == LINK_LINKING || m_state == LINK_LINKED)
		unlink();

	wxString baseName = reflector.Left(LONG_CALLSIGN_LENGTH - 1U).Trim();

	int index = m_reflectors.Index(baseName);
	if (index == wxNOT_FOUND) {
		wxLogError(wxT("Cannot find %s in the reflector list"), baseName.c_str());
		return false;
	}

	wxString hostname = m_hostnames.Item(index);

	bool res = m_dcsHandler.link(reflector, hostname, DCS_PORT);
	if (res) {
		m_reflector = reflector;
		m_state     = LINK_LINKING;
		m_header.setRptCall2(reflector);
		m_header.setRptCall1(m_repeaterCall);
		m_ccsHandler.setReflector(reflector);
	}

	return res;
}

bool CDCSGatewayThread::writeHeader(CDCSGatewayHeaderData &header)
{
	header.setReflector(m_repeaterCall);

	return m_repeaterHandler->writeHeader(header);
}

bool CDCSGatewayThread::writeData(CDCSGatewayAMBEData &data)
{
	return m_repeaterHandler->writeData(data);
}

void CDCSGatewayThread::showHeader(const CDCSGatewayHeaderData &header)
{
	m_myCall   = header.getMyCall1();
	if (!header.getMyCall2().IsSameAs(wxT("    "))) {
		m_myCall.Append(wxT("/"));
		m_myCall.Append(header.getMyCall2());
	}

	m_yourCall = header.getYourCall();
	m_rptCall1 = header.getRptCall1();
	m_rptCall2 = header.getRptCall2();

	m_header.setMyCall1(header.getMyCall1());
	m_header.setMyCall2(header.getMyCall2());
	m_header.setYourCall(wxT("CQCQCQ  "));
}

void CDCSGatewayThread::clearHeader()
{
	m_myCall.Clear();
	m_yourCall.Clear();
	m_rptCall1.Clear();
	m_rptCall2.Clear();
}

void CDCSGatewayThread::writeLinkingTo(const wxString& callsign)
{
	wxString text;

	switch (m_language) {
		case TL_DEUTSCH:
			text.Printf(wxT("Verbinde mit %s"), callsign.c_str());
			break;
		case TL_DANSK:
			text.Printf(wxT("Linker til %s"), callsign.c_str());
			break;
		case TL_FRANCAIS:
			text.Printf(wxT("Connection a %s"), callsign.c_str());
			break;
		case TL_ITALIANO:
			text.Printf(wxT("In conn con %s"), callsign.c_str());
			break;
		case TL_POLSKI:
			text.Printf(wxT("Linkuje do %s"), callsign.c_str());
			break;
		case TL_ESPANOL:
			text.Printf(wxT("Enlazando %s"), callsign.c_str());
			break;
		case TL_SVENSKA:
			text.Printf(wxT("Lankad till %s"), callsign.c_str());
			break;
		case TL_NEDERLANDS:
			text.Printf(wxT("Linken naar %s"), callsign.c_str());
			break;
		case TL_NORSK:
			text.Printf(wxT("Kobler til %s"), callsign.c_str());
			break;
		default:
			text.Printf(wxT("Linking to %s"), callsign.c_str());
			break;
	}

	m_repeaterHandler->writeText(text, LS_LINKING_DCS, callsign);
	m_audioUnit->setText(text);
}

void CDCSGatewayThread::writeLinkedTo(const wxString& callsign)
{
	wxString text;

	switch (m_language) {
		case TL_DEUTSCH:
			text.Printf(wxT("Verlinkt zu %s"), callsign.c_str());
			break;
		case TL_DANSK:
			text.Printf(wxT("Linket til %s"), callsign.c_str());
			break;
		case TL_FRANCAIS:
			text.Printf(wxT("Connecte a %s"), callsign.c_str());
			break;
		case TL_ITALIANO:
			text.Printf(wxT("Connesso a %s"), callsign.c_str());
			break;
		case TL_POLSKI:
			text.Printf(wxT("Polaczony z %s"), callsign.c_str());
			break;
		case TL_ESPANOL:
			text.Printf(wxT("Enlazado %s"), callsign.c_str());
			break;
		case TL_SVENSKA:
			text.Printf(wxT("Lankar till %s"), callsign.c_str());
			break;
		case TL_NEDERLANDS:
			text.Printf(wxT("Gelinkt met %s"), callsign.c_str());
			break;
		case TL_NORSK:
			text.Printf(wxT("Tilkoblet %s"), callsign.c_str());
			break;
		default:
			text.Printf(wxT("Linked to %s"), callsign.c_str());
			break;
	}

	m_repeaterHandler->writeText(text, LS_LINKED_DCS, callsign);
	m_audioUnit->setText(text);
}

void CDCSGatewayThread::writeNotLinked()
{
	wxString text;

	switch (m_language) {
		case TL_DEUTSCH:
			text = wxT("Nicht verbunden");
			break;
		case TL_DANSK:
			text = wxT("Ikke forbundet");
			break;
		case TL_FRANCAIS:
			text = wxT("Non connecte");
			break;
		case TL_ITALIANO:
			text = wxT("Non connesso");
			break;
		case TL_POLSKI:
			text = wxT("Nie polaczony");
			break;
		case TL_ESPANOL:
			text = wxT("No enlazado");
			break;
		case TL_SVENSKA:
			text = wxT("Ej lankad");
			break;
		case TL_NEDERLANDS:
			text = wxT("Niet gelinkt");
			break;
		case TL_NORSK:
			text = wxT("Ikke linket");
			break;
		default:
			text = wxT("Not linked");
			break;
	}

	m_repeaterHandler->writeText(text, LS_NONE, wxEmptyString);
	m_audioUnit->setText(text);
}

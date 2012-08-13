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

#include "DCSGatewayAMBEData.h"

#include "DStarDefines.h"
#include "Utils.h"

const char* HTML = "<table border=\"0\" width=\"95%\"><tr>"
				   "<td width=\"4%\"><img border=\"0\" src=hotspot.jpg></td>"
				   "<td width=\"96%\"><font size=\"2\">"
				   "<b>HOTSPOT</b> DCS Gateway 20120706"
				   "</font></td>"
				   "</tr></table>";

CDCSGatewayAMBEData::CDCSGatewayAMBEData() :
m_outSeq(0U),
m_rptSeq(0U),
m_id(0U),
m_data(NULL)
{
	m_data = new unsigned char[DV_FRAME_LENGTH_BYTES];
}

CDCSGatewayAMBEData::CDCSGatewayAMBEData(const CDCSGatewayAMBEData& data) :
m_outSeq(data.m_outSeq),
m_rptSeq(data.m_rptSeq),
m_id(data.m_id),
m_data(NULL)
{
	m_data = new unsigned char[DV_FRAME_LENGTH_BYTES];

	::memcpy(m_data, data.m_data, DV_FRAME_LENGTH_BYTES);
}

CDCSGatewayAMBEData::~CDCSGatewayAMBEData()
{
	delete[] m_data;
}

bool CDCSGatewayAMBEData::setRepeaterData(const unsigned char *data, unsigned int length)
{
	wxASSERT(data != NULL);

	if (length < 21U) {
		wxLogMessage(wxT("AMBE data from the repeater is too short, %u < 22"), length);
		return false;
	}

	if (data[0] != 'D' || data[1] != 'S' || data[2] != 'R' || data[3] != 'P') {
		CUtils::dump(wxT("Invalid signature from the repeater"), data, length);
		return false;
	}

	m_id     = data[5] * 256U + data[6];
	m_outSeq = data[7];

	if (data[4] != 0x21U) {
		CUtils::dump(wxT("Invalid data type from the repeater"), data, length);
		return false;
	}

	::memcpy(m_data, data + 9U, DV_FRAME_LENGTH_BYTES);

	return true;
}

bool CDCSGatewayAMBEData::setDCSData(const unsigned char *data, unsigned int length)
{
	wxASSERT(data != NULL);

	if (length < 100U) {
		wxLogMessage(wxT("AMBE data from DCS is too short, %u < 100"), length);
		return false;
	}

	if (data[0] != '0' || data[1] != '0' || data[2] != '0' || data[3] != '1') {
		CUtils::dump(wxT("Invalid signature from DCS"), data, length);
		return false;
	}

	m_rptSeq = data[60U] * 65536U + data[59U] * 256U + data[58U];
	m_id     = data[43U] * 256U + data[44U];
	m_outSeq = data[45U];

	::memcpy(m_data, data + 46U, DV_FRAME_LENGTH_BYTES);

	return true;
}

unsigned int CDCSGatewayAMBEData::getRepeaterData(unsigned char *data, unsigned int length) const
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 21U);

	data[0] = 'D';
	data[1] = 'S';
	data[2] = 'R';
	data[3] = 'P';

	data[4] = 0x21U;

	data[5] = m_id / 256U;		// Unique session id
	data[6] = m_id % 256U;

	data[7] = m_outSeq;

	data[8] = 0U;

	::memcpy(data + 9U, m_data, DV_FRAME_LENGTH_BYTES);

	return 21U;
}

unsigned int CDCSGatewayAMBEData::getDCSData(unsigned char* data, unsigned int length) const
{
	wxASSERT(data != NULL);
	wxASSERT(length >= 600U);

	::memset(data, 0x00U, 600U);

	data[0]  = '0';
	data[1]  = '0';
	data[2]  = '0';
	data[3]  = '1';

	data[4]  = 0x00U;
	data[5]  = 0x00U;
	data[6]  = 0x00U;


	data[43U] = m_id % 256U;			// Unique session id
	data[44U] = m_id / 256U;

	data[45U] = m_outSeq;

	::memcpy(data + 46U, m_data, DV_FRAME_LENGTH_BYTES);

	if (isEnd()) {
		data[55U] = 0x55U;
		data[56U] = 0x55U;
		data[57U] = 0x55U;
	}

	data[58U] = (m_rptSeq >> 0)  & 0xFFU;
	data[59U] = (m_rptSeq >> 8)  & 0xFFU;
	data[60U] = (m_rptSeq >> 16) & 0xFFU;

	data[61U] = 0x01U;
	data[62U] = 0x00U;

	// Send the HTML every 2 seconds
	if ((m_rptSeq % 100U) != 0U)
		return  100U;

	::memcpy(data + 100U, HTML, ::strlen(HTML));

	return 600U;
}

void CDCSGatewayAMBEData::setData(const unsigned char *data, unsigned int length)
{
	wxASSERT(data != NULL);
	wxASSERT(length >= DV_FRAME_LENGTH_BYTES);

	::memcpy(m_data, data, DV_FRAME_LENGTH_BYTES);
}

unsigned int CDCSGatewayAMBEData::getId() const
{
	return m_id;
}

void CDCSGatewayAMBEData::setId(unsigned int id)
{
	m_id = id;
}

unsigned int CDCSGatewayAMBEData::getSeq() const
{
	return m_outSeq & 0x0FU;
}

void CDCSGatewayAMBEData::setSeq(unsigned int seqNo)
{
	m_outSeq = seqNo;
}

unsigned int CDCSGatewayAMBEData::getRptSeq() const
{
	return m_rptSeq;
}

void CDCSGatewayAMBEData::setRptSeq(unsigned int seqNo)
{
	m_rptSeq = seqNo;
}

bool CDCSGatewayAMBEData::isEnd() const
{
	return (m_outSeq & 0x40U) == 0x40U;
}

void CDCSGatewayAMBEData::setEnd(bool end)
{
	if (end)
		m_outSeq |= 0x40U;
	else
		m_outSeq &= ~0x40U;
}

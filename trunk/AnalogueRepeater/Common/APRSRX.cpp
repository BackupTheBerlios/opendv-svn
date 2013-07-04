/*
 *   Copyright (C) 1996 by Thomas Sailer HB9JNX
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

#include "AnalogueDefines.h"
#include "APRSRX.h"
#include "Utils.h"

const unsigned int FREQ_MARK  = 1200U;
const unsigned int FREQ_SPACE = 2200U;
const unsigned int BAUD       = 1200U;
const unsigned int SUBSAMP    = 2U;

const unsigned int CORRLEN    = ANALOGUE_RADIO_SAMPLE_RATE / BAUD;
const unsigned int PHASEINC   = 0x10000U * BAUD * SUBSAMP / ANALOGUE_RADIO_SAMPLE_RATE;

const unsigned int MAX_PACKET_LEN = 512U;

inline wxFloat32 mac(const wxFloat32* a, const wxFloat32* b, unsigned int size)
{
	wxFloat32 sum = 0.0F;

	for (unsigned int i = 0U; i < size; i++)
		sum += (*a++) * (*b++);

	return sum;
}

inline wxFloat32 fsqr(wxFloat32 f) 
{
	return f * f;
}

CAPRSRX::CAPRSRX() :
m_dcd(0U),
m_phase(0U),
m_last(0U),
m_subsamp(0U),
m_buf(NULL),
m_ptr(NULL),
m_state(false),
m_bitStream(0x00U),
m_bitBuf(0x00U)
{
	wxFloat32 f;
	unsigned int i;

	m_buf = new unsigned char[MAX_PACKET_LEN];

	m_corrMarkI  = new wxFloat32[CORRLEN];
	m_corrMarkQ  = new wxFloat32[CORRLEN];
	m_corrSpaceI = new wxFloat32[CORRLEN];
	m_corrSpaceQ = new wxFloat32[CORRLEN];

	for (f = 0.0F, i = 0U; i < CORRLEN; i++) {
		m_corrMarkI[i] = ::cos(f);
		m_corrMarkQ[i] = ::sin(f);
		f += wxFloat32(2.0 * M_PI * FREQ_MARK / ANALOGUE_RADIO_SAMPLE_RATE);
	}

	for (f = 0.0F, i = 0U; i < CORRLEN; i++) {
		m_corrSpaceI[i] = ::cos(f);
		m_corrSpaceQ[i] = ::sin(f);
		f += wxFloat32(2.0 * M_PI * FREQ_SPACE / ANALOGUE_RADIO_SAMPLE_RATE);
	}

	return;

	wxString text1 = wxT("wxFloat32 CORR_MARK_I[] = {");
	for (i = 0U; i < CORRLEN; i++) {
		wxString text2;
		text2.Printf(wxT("%.5f, "), m_corrMarkI[i]);
		text1.Append(text2);
	}
	text1.Append(wxT("};"));
	wxLogMessage(text1);

	text1 = wxT("wxFloat32 CORR_MARK_Q[] = {");
	for (i = 0U; i < CORRLEN; i++) {
		wxString text2;
		text2.Printf(wxT("%.5f, "), m_corrMarkQ[i]);
		text1.Append(text2);
	}
	text1.Append(wxT("};"));
	wxLogMessage(text1);

	text1 = wxT("wxFloat32 CORR_SPACE_I[] = {");
	for (i = 0U; i < CORRLEN; i++) {
		wxString text2;
		text2.Printf(wxT("%.5f, "), m_corrSpaceI[i]);
		text1.Append(text2);
	}
	text1.Append(wxT("};"));
	wxLogMessage(text1);

	text1 = wxT("wxFloat32 CORR_SPACE_Q[] = {");
	for (i = 0U; i < CORRLEN; i++) {
		wxString text2;
		text2.Printf(wxT("%.5f, "), m_corrSpaceQ[i]);
		text1.Append(text2);
	}
	text1.Append(wxT("};"));
	wxLogMessage(text1);
}

CAPRSRX::~CAPRSRX()
{
	delete[] m_buf;
	delete[] m_corrMarkI;
	delete[] m_corrMarkQ;
	delete[] m_corrSpaceI;
	delete[] m_corrSpaceQ;
}

void CAPRSRX::process(const wxFloat32* audio, unsigned int length)
{
	if (m_subsamp > 0U) {
		unsigned int numfill = SUBSAMP - m_subsamp;
		if (length < numfill) {
			m_subsamp += length;
			return;
		}

		audio  += numfill;
		length -= numfill;

		m_subsamp = 0U;
	}

	for (; length >= SUBSAMP; length -= SUBSAMP, audio += SUBSAMP) {
		wxFloat32 f = ::fsqr(::mac(audio, m_corrMarkI, CORRLEN)) + ::fsqr(::mac(audio, m_corrMarkQ, CORRLEN)) - ::fsqr(::mac(audio, m_corrSpaceI, CORRLEN)) - ::fsqr(::mac(audio, m_corrSpaceQ, CORRLEN));

		m_dcd <<= 1;
		m_dcd |= (f > 0.0F);

		/*
		 * Check if transition
		 */
		if ((m_dcd ^ (m_dcd >> 1)) & 1) {
			if (m_phase < (0x8000U - (PHASEINC / 2U)))
				m_phase += PHASEINC / 8U;
			else
				m_phase -= PHASEINC / 8U;
		}

		m_phase += PHASEINC;

		if (m_phase >= 0x10000U) {
			m_phase &= 0xFFFFU;

			m_last <<= 1;
			m_last |= m_dcd & 1;

			bool curbit = (m_last ^ (m_last >> 1) ^ 1) & 1;
			rxbit(curbit);
		}
	}

	m_subsamp = length;
}

void CAPRSRX::rxbit(bool bit)
{
	m_bitStream <<= 1;
	m_bitStream |= bit ? 0x01U : 0x00U;

	if ((m_bitStream & 0xFFU) == 0x7EU) {
		if (m_state && (m_ptr - m_buf) > 9U)
			CUtils::dump(wxT("AX.25 Packet"), m_buf, m_ptr - m_buf);

		m_state  = true;
		m_ptr    = m_buf;
		m_bitBuf = 0x80U;
		return;
	}

	if ((m_bitStream & 0x7FU) == 0x7FU) {
		m_state = false;
		return;
	}

	if (!m_state)
		return;

	if ((m_bitStream & 0x3FU) == 0x3EU)		/* Stuffed bit */
		return;

	if ((m_bitStream & 0x01U) == 0x01U)
		m_bitBuf |= 0x100U;

	if ((m_bitBuf & 0x01U) == 0x01U) {
		if (m_ptr >= (m_buf + MAX_PACKET_LEN)) {
			m_state = false;
			return;
		}

		*m_ptr++ = m_bitBuf >> 1;
		m_bitBuf = 0x80U;
		return;
	}

	m_bitBuf >>= 1;
}

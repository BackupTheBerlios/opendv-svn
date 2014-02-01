/*
 *	Copyright (C) 2009 by Jonathan Naylor, G4KLX
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

#include "DStarGMSKModulator.h"

#include "DStarDefines.h"

// Generated by
// gaussfir(0.5, 2, 10)
const wxFloat32 COEFFS_TABLE[] = {
	6.455906007234699e-014F, 1.037067381285011e-012F, 1.444835156335346e-011F,
	1.745786683011439e-010F, 1.829471305298363e-009F, 1.662729407135958e-008F,
	1.310626978701910e-007F, 8.959797186410516e-007F, 5.312253663302771e-006F,
	2.731624380156465e-005F, 1.218217140199093e-004F, 4.711833994209542e-004F,
	1.580581180127418e-003F, 4.598383433830095e-003F, 1.160259430889949e-002F,
	2.539022692626253e-002F, 4.818807833062393e-002F, 7.931844341164322e-002F,
	1.132322945270602e-001F, 1.401935338024111e-001F, 1.505383695578516e-001F,
	1.401935338024111e-001F, 1.132322945270601e-001F, 7.931844341164328e-002F,
	4.818807833062393e-002F, 2.539022692626253e-002F, 1.160259430889949e-002F,
	4.598383433830090e-003F, 1.580581180127420e-003F, 4.711833994209542e-004F,
	1.218217140199093e-004F, 2.731624380156465e-005F, 5.312253663302753e-006F,
	8.959797186410563e-007F, 1.310626978701910e-007F, 1.662729407135958e-008F,
	1.829471305298363e-009F, 1.745786683011426e-010F, 1.444835156335356e-011F,
	1.037067381285011e-012F, 6.455906007234699e-014F};

const unsigned int COEFFS_LENGTH = 41U;


CDStarGMSKModulator::CDStarGMSKModulator() :
m_filter(COEFFS_TABLE, COEFFS_LENGTH),
m_invert(false)
{
}

CDStarGMSKModulator::~CDStarGMSKModulator()
{
}

unsigned int CDStarGMSKModulator::code(bool bit, wxFloat32* buffer, unsigned int length)
{
	wxASSERT(buffer != NULL);
	wxASSERT(length == DSTAR_RADIO_BIT_LENGTH);

	if (m_invert)
		bit = !bit;

	for (unsigned int i = 0U; i < DSTAR_RADIO_BIT_LENGTH; i++) {
		if (bit)
			buffer[i] = m_filter.process(-0.5F);
		else
			buffer[i] = m_filter.process(0.5F);
	}

	return DSTAR_RADIO_BIT_LENGTH;
}

void CDStarGMSKModulator::setInvert(bool set)
{
	m_invert = set;
}
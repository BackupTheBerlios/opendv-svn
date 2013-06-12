/*
 *   Copyright (C) 2009-2013 by Jonathan Naylor G4KLX
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

#include "DStarRepeaterSoundCardController.h"
#include "CCITTChecksumReverse.h"
#include "DStarDefines.h"

const unsigned int BUFFER_LENGTH = 200U;

const unsigned char TAG_HEADER   = 0x00U;
const unsigned char TAG_DATA     = 0x01U;
const unsigned char TAG_DATA_END = 0x02U;

const unsigned char BIT_SYNC    = 0xAAU;

const unsigned char FRAME_SYNC0 = 0xEAU;
const unsigned char FRAME_SYNC1 = 0xA6U;
const unsigned char FRAME_SYNC2 = 0x00U;

const unsigned char BIT_MASK_TABLE0[] = {0x7FU, 0xBFU, 0xDFU, 0xEFU, 0xF7U, 0xFBU, 0xFDU, 0xFEU};
const unsigned char BIT_MASK_TABLE1[] = {0x80U, 0x40U, 0x20U, 0x10U, 0x08U, 0x04U, 0x02U, 0x01U};

const unsigned char INTERLEAVE_TABLE_TX[] = {
  0x00U, 0x04U, 0x04U, 0x00U, 0x07U, 0x04U, 0x0BU, 0x00U, 0x0EU, 0x04U,
  0x12U, 0x00U, 0x15U, 0x04U, 0x19U, 0x00U, 0x1CU, 0x04U, 0x20U, 0x00U,
  0x23U, 0x04U, 0x27U, 0x00U, 0x2AU, 0x04U, 0x2DU, 0x07U, 0x31U, 0x02U,
  0x34U, 0x05U, 0x38U, 0x00U, 0x3BU, 0x03U, 0x3EU, 0x06U, 0x42U, 0x01U,
  0x45U, 0x04U, 0x48U, 0x07U, 0x4CU, 0x02U, 0x4FU, 0x05U, 0x00U, 0x05U,
  0x04U, 0x01U, 0x07U, 0x05U, 0x0BU, 0x01U, 0x0EU, 0x05U, 0x12U, 0x01U,
  0x15U, 0x05U, 0x19U, 0x01U, 0x1CU, 0x05U, 0x20U, 0x01U, 0x23U, 0x05U,
  0x27U, 0x01U, 0x2AU, 0x05U, 0x2EU, 0x00U, 0x31U, 0x03U, 0x34U, 0x06U,
  0x38U, 0x01U, 0x3BU, 0x04U, 0x3EU, 0x07U, 0x42U, 0x02U, 0x45U, 0x05U,
  0x49U, 0x00U, 0x4CU, 0x03U, 0x4FU, 0x06U, 0x00U, 0x06U, 0x04U, 0x02U,
  0x07U, 0x06U, 0x0BU, 0x02U, 0x0EU, 0x06U, 0x12U, 0x02U, 0x15U, 0x06U,
  0x19U, 0x02U, 0x1CU, 0x06U, 0x20U, 0x02U, 0x23U, 0x06U, 0x27U, 0x02U,
  0x2AU, 0x06U, 0x2EU, 0x01U, 0x31U, 0x04U, 0x34U, 0x07U, 0x38U, 0x02U,
  0x3BU, 0x05U, 0x3FU, 0x00U, 0x42U, 0x03U, 0x45U, 0x06U, 0x49U, 0x01U,
  0x4CU, 0x04U, 0x4FU, 0x07U, 0x00U, 0x07U, 0x04U, 0x03U, 0x07U, 0x07U,
  0x0BU, 0x03U, 0x0EU, 0x07U, 0x12U, 0x03U, 0x15U, 0x07U, 0x19U, 0x03U,
  0x1CU, 0x07U, 0x20U, 0x03U, 0x23U, 0x07U, 0x27U, 0x03U, 0x2AU, 0x07U,
  0x2EU, 0x02U, 0x31U, 0x05U, 0x35U, 0x00U, 0x38U, 0x03U, 0x3BU, 0x06U,
  0x3FU, 0x01U, 0x42U, 0x04U, 0x45U, 0x07U, 0x49U, 0x02U, 0x4CU, 0x05U,
  0x50U, 0x00U, 0x01U, 0x00U, 0x04U, 0x04U, 0x08U, 0x00U, 0x0BU, 0x04U,
  0x0FU, 0x00U, 0x12U, 0x04U, 0x16U, 0x00U, 0x19U, 0x04U, 0x1DU, 0x00U,
  0x20U, 0x04U, 0x24U, 0x00U, 0x27U, 0x04U, 0x2BU, 0x00U, 0x2EU, 0x03U,
  0x31U, 0x06U, 0x35U, 0x01U, 0x38U, 0x04U, 0x3BU, 0x07U, 0x3FU, 0x02U,
  0x42U, 0x05U, 0x46U, 0x00U, 0x49U, 0x03U, 0x4CU, 0x06U, 0x50U, 0x01U,
  0x01U, 0x01U, 0x04U, 0x05U, 0x08U, 0x01U, 0x0BU, 0x05U, 0x0FU, 0x01U,
  0x12U, 0x05U, 0x16U, 0x01U, 0x19U, 0x05U, 0x1DU, 0x01U, 0x20U, 0x05U,
  0x24U, 0x01U, 0x27U, 0x05U, 0x2BU, 0x01U, 0x2EU, 0x04U, 0x31U, 0x07U,
  0x35U, 0x02U, 0x38U, 0x05U, 0x3CU, 0x00U, 0x3FU, 0x03U, 0x42U, 0x06U,
  0x46U, 0x01U, 0x49U, 0x04U, 0x4CU, 0x07U, 0x50U, 0x02U, 0x01U, 0x02U,
  0x04U, 0x06U, 0x08U, 0x02U, 0x0BU, 0x06U, 0x0FU, 0x02U, 0x12U, 0x06U,
  0x16U, 0x02U, 0x19U, 0x06U, 0x1DU, 0x02U, 0x20U, 0x06U, 0x24U, 0x02U,
  0x27U, 0x06U, 0x2BU, 0x02U, 0x2EU, 0x05U, 0x32U, 0x00U, 0x35U, 0x03U,
  0x38U, 0x06U, 0x3CU, 0x01U, 0x3FU, 0x04U, 0x42U, 0x07U, 0x46U, 0x02U,
  0x49U, 0x05U, 0x4DU, 0x00U, 0x50U, 0x03U, 0x01U, 0x03U, 0x04U, 0x07U,
  0x08U, 0x03U, 0x0BU, 0x07U, 0x0FU, 0x03U, 0x12U, 0x07U, 0x16U, 0x03U,
  0x19U, 0x07U, 0x1DU, 0x03U, 0x20U, 0x07U, 0x24U, 0x03U, 0x27U, 0x07U,
  0x2BU, 0x03U, 0x2EU, 0x06U, 0x32U, 0x01U, 0x35U, 0x04U, 0x38U, 0x07U,
  0x3CU, 0x02U, 0x3FU, 0x05U, 0x43U, 0x00U, 0x46U, 0x03U, 0x49U, 0x06U,
  0x4DU, 0x01U, 0x50U, 0x04U, 0x01U, 0x04U, 0x05U, 0x00U, 0x08U, 0x04U,
  0x0CU, 0x00U, 0x0FU, 0x04U, 0x13U, 0x00U, 0x16U, 0x04U, 0x1AU, 0x00U,
  0x1DU, 0x04U, 0x21U, 0x00U, 0x24U, 0x04U, 0x28U, 0x00U, 0x2BU, 0x04U,
  0x2EU, 0x07U, 0x32U, 0x02U, 0x35U, 0x05U, 0x39U, 0x00U, 0x3CU, 0x03U,
  0x3FU, 0x06U, 0x43U, 0x01U, 0x46U, 0x04U, 0x49U, 0x07U, 0x4DU, 0x02U,
  0x50U, 0x05U, 0x01U, 0x05U, 0x05U, 0x01U, 0x08U, 0x05U, 0x0CU, 0x01U,
  0x0FU, 0x05U, 0x13U, 0x01U, 0x16U, 0x05U, 0x1AU, 0x01U, 0x1DU, 0x05U,
  0x21U, 0x01U, 0x24U, 0x05U, 0x28U, 0x01U, 0x2BU, 0x05U, 0x2FU, 0x00U,
  0x32U, 0x03U, 0x35U, 0x06U, 0x39U, 0x01U, 0x3CU, 0x04U, 0x3FU, 0x07U,
  0x43U, 0x02U, 0x46U, 0x05U, 0x4AU, 0x00U, 0x4DU, 0x03U, 0x50U, 0x06U,
  0x01U, 0x06U, 0x05U, 0x02U, 0x08U, 0x06U, 0x0CU, 0x02U, 0x0FU, 0x06U,
  0x13U, 0x02U, 0x16U, 0x06U, 0x1AU, 0x02U, 0x1DU, 0x06U, 0x21U, 0x02U,
  0x24U, 0x06U, 0x28U, 0x02U, 0x2BU, 0x06U, 0x2FU, 0x01U, 0x32U, 0x04U,
  0x35U, 0x07U, 0x39U, 0x02U, 0x3CU, 0x05U, 0x40U, 0x00U, 0x43U, 0x03U,
  0x46U, 0x06U, 0x4AU, 0x01U, 0x4DU, 0x04U, 0x50U, 0x07U, 0x01U, 0x07U,
  0x05U, 0x03U, 0x08U, 0x07U, 0x0CU, 0x03U, 0x0FU, 0x07U, 0x13U, 0x03U,
  0x16U, 0x07U, 0x1AU, 0x03U, 0x1DU, 0x07U, 0x21U, 0x03U, 0x24U, 0x07U,
  0x28U, 0x03U, 0x2BU, 0x07U, 0x2FU, 0x02U, 0x32U, 0x05U, 0x36U, 0x00U,
  0x39U, 0x03U, 0x3CU, 0x06U, 0x40U, 0x01U, 0x43U, 0x04U, 0x46U, 0x07U,
  0x4AU, 0x02U, 0x4DU, 0x05U, 0x51U, 0x00U, 0x02U, 0x00U, 0x05U, 0x04U,
  0x09U, 0x00U, 0x0CU, 0x04U, 0x10U, 0x00U, 0x13U, 0x04U, 0x17U, 0x00U,
  0x1AU, 0x04U, 0x1EU, 0x00U, 0x21U, 0x04U, 0x25U, 0x00U, 0x28U, 0x04U,
  0x2CU, 0x00U, 0x2FU, 0x03U, 0x32U, 0x06U, 0x36U, 0x01U, 0x39U, 0x04U,
  0x3CU, 0x07U, 0x40U, 0x02U, 0x43U, 0x05U, 0x47U, 0x00U, 0x4AU, 0x03U,
  0x4DU, 0x06U, 0x51U, 0x01U, 0x02U, 0x01U, 0x05U, 0x05U, 0x09U, 0x01U,
  0x0CU, 0x05U, 0x10U, 0x01U, 0x13U, 0x05U, 0x17U, 0x01U, 0x1AU, 0x05U,
  0x1EU, 0x01U, 0x21U, 0x05U, 0x25U, 0x01U, 0x28U, 0x05U, 0x2CU, 0x01U,
  0x2FU, 0x04U, 0x32U, 0x07U, 0x36U, 0x02U, 0x39U, 0x05U, 0x3DU, 0x00U,
  0x40U, 0x03U, 0x43U, 0x06U, 0x47U, 0x01U, 0x4AU, 0x04U, 0x4DU, 0x07U,
  0x51U, 0x02U, 0x02U, 0x02U, 0x05U, 0x06U, 0x09U, 0x02U, 0x0CU, 0x06U,
  0x10U, 0x02U, 0x13U, 0x06U, 0x17U, 0x02U, 0x1AU, 0x06U, 0x1EU, 0x02U,
  0x21U, 0x06U, 0x25U, 0x02U, 0x28U, 0x06U, 0x2CU, 0x02U, 0x2FU, 0x05U,
  0x33U, 0x00U, 0x36U, 0x03U, 0x39U, 0x06U, 0x3DU, 0x01U, 0x40U, 0x04U,
  0x43U, 0x07U, 0x47U, 0x02U, 0x4AU, 0x05U, 0x4EU, 0x00U, 0x51U, 0x03U,
  0x02U, 0x03U, 0x05U, 0x07U, 0x09U, 0x03U, 0x0CU, 0x07U, 0x10U, 0x03U,
  0x13U, 0x07U, 0x17U, 0x03U, 0x1AU, 0x07U, 0x1EU, 0x03U, 0x21U, 0x07U,
  0x25U, 0x03U, 0x28U, 0x07U, 0x2CU, 0x03U, 0x2FU, 0x06U, 0x33U, 0x01U,
  0x36U, 0x04U, 0x39U, 0x07U, 0x3DU, 0x02U, 0x40U, 0x05U, 0x44U, 0x00U,
  0x47U, 0x03U, 0x4AU, 0x06U, 0x4EU, 0x01U, 0x51U, 0x04U, 0x02U, 0x04U,
  0x06U, 0x00U, 0x09U, 0x04U, 0x0DU, 0x00U, 0x10U, 0x04U, 0x14U, 0x00U,
  0x17U, 0x04U, 0x1BU, 0x00U, 0x1EU, 0x04U, 0x22U, 0x00U, 0x25U, 0x04U,
  0x29U, 0x00U, 0x2CU, 0x04U, 0x2FU, 0x07U, 0x33U, 0x02U, 0x36U, 0x05U,
  0x3AU, 0x00U, 0x3DU, 0x03U, 0x40U, 0x06U, 0x44U, 0x01U, 0x47U, 0x04U,
  0x4AU, 0x07U, 0x4EU, 0x02U, 0x51U, 0x05U, 0x02U, 0x05U, 0x06U, 0x01U,
  0x09U, 0x05U, 0x0DU, 0x01U, 0x10U, 0x05U, 0x14U, 0x01U, 0x17U, 0x05U,
  0x1BU, 0x01U, 0x1EU, 0x05U, 0x22U, 0x01U, 0x25U, 0x05U, 0x29U, 0x01U,
  0x2CU, 0x05U, 0x30U, 0x00U, 0x33U, 0x03U, 0x36U, 0x06U, 0x3AU, 0x01U, 
  0x3DU, 0x04U, 0x40U, 0x07U, 0x44U, 0x02U, 0x47U, 0x05U, 0x4BU, 0x00U,
  0x4EU, 0x03U, 0x51U, 0x06U, 0x02U, 0x06U, 0x06U, 0x02U, 0x09U, 0x06U,
  0x0DU, 0x02U, 0x10U, 0x06U, 0x14U, 0x02U, 0x17U, 0x06U, 0x1BU, 0x02U,
  0x1EU, 0x06U, 0x22U, 0x02U, 0x25U, 0x06U, 0x29U, 0x02U, 0x2CU, 0x06U,
  0x30U, 0x01U, 0x33U, 0x04U, 0x36U, 0x07U, 0x3AU, 0x02U, 0x3DU, 0x05U,
  0x41U, 0x00U, 0x44U, 0x03U, 0x47U, 0x06U, 0x4BU, 0x01U, 0x4EU, 0x04U,
  0x51U, 0x07U, 0x02U, 0x07U, 0x06U, 0x03U, 0x09U, 0x07U, 0x0DU, 0x03U,
  0x10U, 0x07U, 0x14U, 0x03U, 0x17U, 0x07U, 0x1BU, 0x03U, 0x1EU, 0x07U,
  0x22U, 0x03U, 0x25U, 0x07U, 0x29U, 0x03U, 0x2CU, 0x07U, 0x30U, 0x02U,
  0x33U, 0x05U, 0x37U, 0x00U, 0x3AU, 0x03U, 0x3DU, 0x06U, 0x41U, 0x01U,
  0x44U, 0x04U, 0x47U, 0x07U, 0x4BU, 0x02U, 0x4EU, 0x05U, 0x52U, 0x00U,
  0x03U, 0x00U, 0x06U, 0x04U, 0x0AU, 0x00U, 0x0DU, 0x04U, 0x11U, 0x00U,
  0x14U, 0x04U, 0x18U, 0x00U, 0x1BU, 0x04U, 0x1FU, 0x00U, 0x22U, 0x04U,
  0x26U, 0x00U, 0x29U, 0x04U, 0x2DU, 0x00U, 0x30U, 0x03U, 0x33U, 0x06U,
  0x37U, 0x01U, 0x3AU, 0x04U, 0x3DU, 0x07U, 0x41U, 0x02U, 0x44U, 0x05U,
  0x48U, 0x00U, 0x4BU, 0x03U, 0x4EU, 0x06U, 0x52U, 0x01U, 0x03U, 0x01U,
  0x06U, 0x05U, 0x0AU, 0x01U, 0x0DU, 0x05U, 0x11U, 0x01U, 0x14U, 0x05U,
  0x18U, 0x01U, 0x1BU, 0x05U, 0x1FU, 0x01U, 0x22U, 0x05U, 0x26U, 0x01U,
  0x29U, 0x05U, 0x2DU, 0x01U, 0x30U, 0x04U, 0x33U, 0x07U, 0x37U, 0x02U,
  0x3AU, 0x05U, 0x3EU, 0x00U, 0x41U, 0x03U, 0x44U, 0x06U, 0x48U, 0x01U,
  0x4BU, 0x04U, 0x4EU, 0x07U, 0x52U, 0x02U, 0x03U, 0x02U, 0x06U, 0x06U,
  0x0AU, 0x02U, 0x0DU, 0x06U, 0x11U, 0x02U, 0x14U, 0x06U, 0x18U, 0x02U,
  0x1BU, 0x06U, 0x1FU, 0x02U, 0x22U, 0x06U, 0x26U, 0x02U, 0x29U, 0x06U,
  0x2DU, 0x02U, 0x30U, 0x05U, 0x34U, 0x00U, 0x37U, 0x03U, 0x3AU, 0x06U,
  0x3EU, 0x01U, 0x41U, 0x04U, 0x44U, 0x07U, 0x48U, 0x02U, 0x4BU, 0x05U,
  0x4FU, 0x00U, 0x52U, 0x03U, 0x03U, 0x03U, 0x06U, 0x07U, 0x0AU, 0x03U,
  0x0DU, 0x07U, 0x11U, 0x03U, 0x14U, 0x07U, 0x18U, 0x03U, 0x1BU, 0x07U,
  0x1FU, 0x03U, 0x22U, 0x07U, 0x26U, 0x03U, 0x29U, 0x07U, 0x2DU, 0x03U,
  0x30U, 0x06U, 0x34U, 0x01U, 0x37U, 0x04U, 0x3AU, 0x07U, 0x3EU, 0x02U,
  0x41U, 0x05U, 0x45U, 0x00U, 0x48U, 0x03U, 0x4BU, 0x06U, 0x4FU, 0x01U,
  0x52U, 0x04U, 0x03U, 0x04U, 0x07U, 0x00U, 0x0AU, 0x04U, 0x0EU, 0x00U,
  0x11U, 0x04U, 0x15U, 0x00U, 0x18U, 0x04U, 0x1CU, 0x00U, 0x1FU, 0x04U,
  0x23U, 0x00U, 0x26U, 0x04U, 0x2AU, 0x00U, 0x2DU, 0x04U, 0x30U, 0x07U,
  0x34U, 0x02U, 0x37U, 0x05U, 0x3BU, 0x00U, 0x3EU, 0x03U, 0x41U, 0x06U,
  0x45U, 0x01U, 0x48U, 0x04U, 0x4BU, 0x07U, 0x4FU, 0x02U, 0x52U, 0x05U,
  0x03U, 0x05U, 0x07U, 0x01U, 0x0AU, 0x05U, 0x0EU, 0x01U, 0x11U, 0x05U,
  0x15U, 0x01U, 0x18U, 0x05U, 0x1CU, 0x01U, 0x1FU, 0x05U, 0x23U, 0x01U,
  0x26U, 0x05U, 0x2AU, 0x01U, 0x2DU, 0x05U, 0x31U, 0x00U, 0x34U, 0x03U,
  0x37U, 0x06U, 0x3BU, 0x01U, 0x3EU, 0x04U, 0x41U, 0x07U, 0x45U, 0x02U,
  0x48U, 0x05U, 0x4CU, 0x00U, 0x4FU, 0x03U, 0x52U, 0x06U, 0x03U, 0x06U,
  0x07U, 0x02U, 0x0AU, 0x06U, 0x0EU, 0x02U, 0x11U, 0x06U, 0x15U, 0x02U,
  0x18U, 0x06U, 0x1CU, 0x02U, 0x1FU, 0x06U, 0x23U, 0x02U, 0x26U, 0x06U,
  0x2AU, 0x02U, 0x2DU, 0x06U, 0x31U, 0x01U, 0x34U, 0x04U, 0x37U, 0x07U,
  0x3BU, 0x02U, 0x3EU, 0x05U, 0x42U, 0x00U, 0x45U, 0x03U, 0x48U, 0x06U,
  0x4CU, 0x01U, 0x4FU, 0x04U, 0x52U, 0x07U, 0x03U, 0x07U, 0x07U, 0x03U,
  0x0AU, 0x07U, 0x0EU, 0x03U, 0x11U, 0x07U, 0x15U, 0x03U, 0x18U, 0x07U,
  0x1CU, 0x03U, 0x1FU, 0x07U, 0x23U, 0x03U, 0x26U, 0x07U, 0x2AU, 0x03U
};

const unsigned char INTERLEAVE_TABLE_RX[] = {
  0x00U, 0x00U, 0x03U, 0x00U, 0x06U, 0x00U, 0x09U, 0x00U, 0x0CU, 0x00U,
  0x0FU, 0x00U, 0x12U, 0x00U, 0x15U, 0x00U, 0x18U, 0x00U, 0x1BU, 0x00U,
  0x1EU, 0x00U, 0x21U, 0x00U, 0x24U, 0x00U, 0x27U, 0x00U, 0x2AU, 0x00U,
  0x2DU, 0x00U, 0x30U, 0x00U, 0x33U, 0x00U, 0x36U, 0x00U, 0x39U, 0x00U,
  0x3CU, 0x00U, 0x3FU, 0x00U, 0x42U, 0x00U, 0x45U, 0x00U, 0x48U, 0x00U,
  0x4BU, 0x00U, 0x4EU, 0x00U, 0x51U, 0x00U, 0x00U, 0x01U, 0x03U, 0x01U,
  0x06U, 0x01U, 0x09U, 0x01U, 0x0CU, 0x01U, 0x0FU, 0x01U, 0x12U, 0x01U,
  0x15U, 0x01U, 0x18U, 0x01U, 0x1BU, 0x01U, 0x1EU, 0x01U, 0x21U, 0x01U,
  0x24U, 0x01U, 0x27U, 0x01U, 0x2AU, 0x01U, 0x2DU, 0x01U, 0x30U, 0x01U,
  0x33U, 0x01U, 0x36U, 0x01U, 0x39U, 0x01U, 0x3CU, 0x01U, 0x3FU, 0x01U,
  0x42U, 0x01U, 0x45U, 0x01U, 0x48U, 0x01U, 0x4BU, 0x01U, 0x4EU, 0x01U,
  0x51U, 0x01U, 0x00U, 0x02U, 0x03U, 0x02U, 0x06U, 0x02U, 0x09U, 0x02U,
  0x0CU, 0x02U, 0x0FU, 0x02U, 0x12U, 0x02U, 0x15U, 0x02U, 0x18U, 0x02U,
  0x1BU, 0x02U, 0x1EU, 0x02U, 0x21U, 0x02U, 0x24U, 0x02U, 0x27U, 0x02U,
  0x2AU, 0x02U, 0x2DU, 0x02U, 0x30U, 0x02U, 0x33U, 0x02U, 0x36U, 0x02U,
  0x39U, 0x02U, 0x3CU, 0x02U, 0x3FU, 0x02U, 0x42U, 0x02U, 0x45U, 0x02U,
  0x48U, 0x02U, 0x4BU, 0x02U, 0x4EU, 0x02U, 0x51U, 0x02U, 0x00U, 0x03U,
  0x03U, 0x03U, 0x06U, 0x03U, 0x09U, 0x03U, 0x0CU, 0x03U, 0x0FU, 0x03U,
  0x12U, 0x03U, 0x15U, 0x03U, 0x18U, 0x03U, 0x1BU, 0x03U, 0x1EU, 0x03U,
  0x21U, 0x03U, 0x24U, 0x03U, 0x27U, 0x03U, 0x2AU, 0x03U, 0x2DU, 0x03U,
  0x30U, 0x03U, 0x33U, 0x03U, 0x36U, 0x03U, 0x39U, 0x03U, 0x3CU, 0x03U,
  0x3FU, 0x03U, 0x42U, 0x03U, 0x45U, 0x03U, 0x48U, 0x03U, 0x4BU, 0x03U,
  0x4EU, 0x03U, 0x51U, 0x03U, 0x00U, 0x04U, 0x03U, 0x04U, 0x06U, 0x04U,
  0x09U, 0x04U, 0x0CU, 0x04U, 0x0FU, 0x04U, 0x12U, 0x04U, 0x15U, 0x04U,
  0x18U, 0x04U, 0x1BU, 0x04U, 0x1EU, 0x04U, 0x21U, 0x04U, 0x24U, 0x04U,
  0x27U, 0x04U, 0x2AU, 0x04U, 0x2DU, 0x04U, 0x30U, 0x04U, 0x33U, 0x04U,
  0x36U, 0x04U, 0x39U, 0x04U, 0x3CU, 0x04U, 0x3FU, 0x04U, 0x42U, 0x04U,
  0x45U, 0x04U, 0x48U, 0x04U, 0x4BU, 0x04U, 0x4EU, 0x04U, 0x51U, 0x04U,
  0x00U, 0x05U, 0x03U, 0x05U, 0x06U, 0x05U, 0x09U, 0x05U, 0x0CU, 0x05U,
  0x0FU, 0x05U, 0x12U, 0x05U, 0x15U, 0x05U, 0x18U, 0x05U, 0x1BU, 0x05U,
  0x1EU, 0x05U, 0x21U, 0x05U, 0x24U, 0x05U, 0x27U, 0x05U, 0x2AU, 0x05U,
  0x2DU, 0x05U, 0x30U, 0x05U, 0x33U, 0x05U, 0x36U, 0x05U, 0x39U, 0x05U,
  0x3CU, 0x05U, 0x3FU, 0x05U, 0x42U, 0x05U, 0x45U, 0x05U, 0x48U, 0x05U,
  0x4BU, 0x05U, 0x4EU, 0x05U, 0x51U, 0x05U, 0x00U, 0x06U, 0x03U, 0x06U,
  0x06U, 0x06U, 0x09U, 0x06U, 0x0CU, 0x06U, 0x0FU, 0x06U, 0x12U, 0x06U,
  0x15U, 0x06U, 0x18U, 0x06U, 0x1BU, 0x06U, 0x1EU, 0x06U, 0x21U, 0x06U,
  0x24U, 0x06U, 0x27U, 0x06U, 0x2AU, 0x06U, 0x2DU, 0x06U, 0x30U, 0x06U,
  0x33U, 0x06U, 0x36U, 0x06U, 0x39U, 0x06U, 0x3CU, 0x06U, 0x3FU, 0x06U,
  0x42U, 0x06U, 0x45U, 0x06U, 0x48U, 0x06U, 0x4BU, 0x06U, 0x4EU, 0x06U,
  0x51U, 0x06U, 0x00U, 0x07U, 0x03U, 0x07U, 0x06U, 0x07U, 0x09U, 0x07U,
  0x0CU, 0x07U, 0x0FU, 0x07U, 0x12U, 0x07U, 0x15U, 0x07U, 0x18U, 0x07U,
  0x1BU, 0x07U, 0x1EU, 0x07U, 0x21U, 0x07U, 0x24U, 0x07U, 0x27U, 0x07U,
  0x2AU, 0x07U, 0x2DU, 0x07U, 0x30U, 0x07U, 0x33U, 0x07U, 0x36U, 0x07U,
  0x39U, 0x07U, 0x3CU, 0x07U, 0x3FU, 0x07U, 0x42U, 0x07U, 0x45U, 0x07U,
  0x48U, 0x07U, 0x4BU, 0x07U, 0x4EU, 0x07U, 0x51U, 0x07U, 0x01U, 0x00U,
  0x04U, 0x00U, 0x07U, 0x00U, 0x0AU, 0x00U, 0x0DU, 0x00U, 0x10U, 0x00U,
  0x13U, 0x00U, 0x16U, 0x00U, 0x19U, 0x00U, 0x1CU, 0x00U, 0x1FU, 0x00U,
  0x22U, 0x00U, 0x25U, 0x00U, 0x28U, 0x00U, 0x2BU, 0x00U, 0x2EU, 0x00U,
  0x31U, 0x00U, 0x34U, 0x00U, 0x37U, 0x00U, 0x3AU, 0x00U, 0x3DU, 0x00U,
  0x40U, 0x00U, 0x43U, 0x00U, 0x46U, 0x00U, 0x49U, 0x00U, 0x4CU, 0x00U,
  0x4FU, 0x00U, 0x52U, 0x00U, 0x01U, 0x01U, 0x04U, 0x01U, 0x07U, 0x01U,
  0x0AU, 0x01U, 0x0DU, 0x01U, 0x10U, 0x01U, 0x13U, 0x01U, 0x16U, 0x01U,
  0x19U, 0x01U, 0x1CU, 0x01U, 0x1FU, 0x01U, 0x22U, 0x01U, 0x25U, 0x01U,
  0x28U, 0x01U, 0x2BU, 0x01U, 0x2EU, 0x01U, 0x31U, 0x01U, 0x34U, 0x01U,
  0x37U, 0x01U, 0x3AU, 0x01U, 0x3DU, 0x01U, 0x40U, 0x01U, 0x43U, 0x01U,
  0x46U, 0x01U, 0x49U, 0x01U, 0x4CU, 0x01U, 0x4FU, 0x01U, 0x52U, 0x01U,
  0x01U, 0x02U, 0x04U, 0x02U, 0x07U, 0x02U, 0x0AU, 0x02U, 0x0DU, 0x02U,
  0x10U, 0x02U, 0x13U, 0x02U, 0x16U, 0x02U, 0x19U, 0x02U, 0x1CU, 0x02U,
  0x1FU, 0x02U, 0x22U, 0x02U, 0x25U, 0x02U, 0x28U, 0x02U, 0x2BU, 0x02U,
  0x2EU, 0x02U, 0x31U, 0x02U, 0x34U, 0x02U, 0x37U, 0x02U, 0x3AU, 0x02U,
  0x3DU, 0x02U, 0x40U, 0x02U, 0x43U, 0x02U, 0x46U, 0x02U, 0x49U, 0x02U,
  0x4CU, 0x02U, 0x4FU, 0x02U, 0x52U, 0x02U, 0x01U, 0x03U, 0x04U, 0x03U,
  0x07U, 0x03U, 0x0AU, 0x03U, 0x0DU, 0x03U, 0x10U, 0x03U, 0x13U, 0x03U,
  0x16U, 0x03U, 0x19U, 0x03U, 0x1CU, 0x03U, 0x1FU, 0x03U, 0x22U, 0x03U,
  0x25U, 0x03U, 0x28U, 0x03U, 0x2BU, 0x03U, 0x2EU, 0x03U, 0x31U, 0x03U,
  0x34U, 0x03U, 0x37U, 0x03U, 0x3AU, 0x03U, 0x3DU, 0x03U, 0x40U, 0x03U,
  0x43U, 0x03U, 0x46U, 0x03U, 0x49U, 0x03U, 0x4CU, 0x03U, 0x4FU, 0x03U,
  0x52U, 0x03U, 0x01U, 0x04U, 0x04U, 0x04U, 0x07U, 0x04U, 0x0AU, 0x04U,
  0x0DU, 0x04U, 0x10U, 0x04U, 0x13U, 0x04U, 0x16U, 0x04U, 0x19U, 0x04U,
  0x1CU, 0x04U, 0x1FU, 0x04U, 0x22U, 0x04U, 0x25U, 0x04U, 0x28U, 0x04U,
  0x2BU, 0x04U, 0x2EU, 0x04U, 0x31U, 0x04U, 0x34U, 0x04U, 0x37U, 0x04U,
  0x3AU, 0x04U, 0x3DU, 0x04U, 0x40U, 0x04U, 0x43U, 0x04U, 0x46U, 0x04U,
  0x49U, 0x04U, 0x4CU, 0x04U, 0x4FU, 0x04U, 0x01U, 0x05U, 0x04U, 0x05U,
  0x07U, 0x05U, 0x0AU, 0x05U, 0x0DU, 0x05U, 0x10U, 0x05U, 0x13U, 0x05U,
  0x16U, 0x05U, 0x19U, 0x05U, 0x1CU, 0x05U, 0x1FU, 0x05U, 0x22U, 0x05U,
  0x25U, 0x05U, 0x28U, 0x05U, 0x2BU, 0x05U, 0x2EU, 0x05U, 0x31U, 0x05U,
  0x34U, 0x05U, 0x37U, 0x05U, 0x3AU, 0x05U, 0x3DU, 0x05U, 0x40U, 0x05U,
  0x43U, 0x05U, 0x46U, 0x05U, 0x49U, 0x05U, 0x4CU, 0x05U, 0x4FU, 0x05U,
  0x01U, 0x06U, 0x04U, 0x06U, 0x07U, 0x06U, 0x0AU, 0x06U, 0x0DU, 0x06U,
  0x10U, 0x06U, 0x13U, 0x06U, 0x16U, 0x06U, 0x19U, 0x06U, 0x1CU, 0x06U,
  0x1FU, 0x06U, 0x22U, 0x06U, 0x25U, 0x06U, 0x28U, 0x06U, 0x2BU, 0x06U,
  0x2EU, 0x06U, 0x31U, 0x06U, 0x34U, 0x06U, 0x37U, 0x06U, 0x3AU, 0x06U,
  0x3DU, 0x06U, 0x40U, 0x06U, 0x43U, 0x06U, 0x46U, 0x06U, 0x49U, 0x06U,
  0x4CU, 0x06U, 0x4FU, 0x06U, 0x01U, 0x07U, 0x04U, 0x07U, 0x07U, 0x07U,
  0x0AU, 0x07U, 0x0DU, 0x07U, 0x10U, 0x07U, 0x13U, 0x07U, 0x16U, 0x07U,
  0x19U, 0x07U, 0x1CU, 0x07U, 0x1FU, 0x07U, 0x22U, 0x07U, 0x25U, 0x07U,
  0x28U, 0x07U, 0x2BU, 0x07U, 0x2EU, 0x07U, 0x31U, 0x07U, 0x34U, 0x07U,
  0x37U, 0x07U, 0x3AU, 0x07U, 0x3DU, 0x07U, 0x40U, 0x07U, 0x43U, 0x07U,
  0x46U, 0x07U, 0x49U, 0x07U, 0x4CU, 0x07U, 0x4FU, 0x07U, 0x02U, 0x00U,
  0x05U, 0x00U, 0x08U, 0x00U, 0x0BU, 0x00U, 0x0EU, 0x00U, 0x11U, 0x00U,
  0x14U, 0x00U, 0x17U, 0x00U, 0x1AU, 0x00U, 0x1DU, 0x00U, 0x20U, 0x00U,
  0x23U, 0x00U, 0x26U, 0x00U, 0x29U, 0x00U, 0x2CU, 0x00U, 0x2FU, 0x00U,
  0x32U, 0x00U, 0x35U, 0x00U, 0x38U, 0x00U, 0x3BU, 0x00U, 0x3EU, 0x00U,
  0x41U, 0x00U, 0x44U, 0x00U, 0x47U, 0x00U, 0x4AU, 0x00U, 0x4DU, 0x00U,
  0x50U, 0x00U, 0x02U, 0x01U, 0x05U, 0x01U, 0x08U, 0x01U, 0x0BU, 0x01U,
  0x0EU, 0x01U, 0x11U, 0x01U, 0x14U, 0x01U, 0x17U, 0x01U, 0x1AU, 0x01U,
  0x1DU, 0x01U, 0x20U, 0x01U, 0x23U, 0x01U, 0x26U, 0x01U, 0x29U, 0x01U,
  0x2CU, 0x01U, 0x2FU, 0x01U, 0x32U, 0x01U, 0x35U, 0x01U, 0x38U, 0x01U,
  0x3BU, 0x01U, 0x3EU, 0x01U, 0x41U, 0x01U, 0x44U, 0x01U, 0x47U, 0x01U,
  0x4AU, 0x01U, 0x4DU, 0x01U, 0x50U, 0x01U, 0x02U, 0x02U, 0x05U, 0x02U,
  0x08U, 0x02U, 0x0BU, 0x02U, 0x0EU, 0x02U, 0x11U, 0x02U, 0x14U, 0x02U,
  0x17U, 0x02U, 0x1AU, 0x02U, 0x1DU, 0x02U, 0x20U, 0x02U, 0x23U, 0x02U,
  0x26U, 0x02U, 0x29U, 0x02U, 0x2CU, 0x02U, 0x2FU, 0x02U, 0x32U, 0x02U,
  0x35U, 0x02U, 0x38U, 0x02U, 0x3BU, 0x02U, 0x3EU, 0x02U, 0x41U, 0x02U,
  0x44U, 0x02U, 0x47U, 0x02U, 0x4AU, 0x02U, 0x4DU, 0x02U, 0x50U, 0x02U,
  0x02U, 0x03U, 0x05U, 0x03U, 0x08U, 0x03U, 0x0BU, 0x03U, 0x0EU, 0x03U,
  0x11U, 0x03U, 0x14U, 0x03U, 0x17U, 0x03U, 0x1AU, 0x03U, 0x1DU, 0x03U,
  0x20U, 0x03U, 0x23U, 0x03U, 0x26U, 0x03U, 0x29U, 0x03U, 0x2CU, 0x03U,
  0x2FU, 0x03U, 0x32U, 0x03U, 0x35U, 0x03U, 0x38U, 0x03U, 0x3BU, 0x03U,
  0x3EU, 0x03U, 0x41U, 0x03U, 0x44U, 0x03U, 0x47U, 0x03U, 0x4AU, 0x03U,
  0x4DU, 0x03U, 0x50U, 0x03U, 0x02U, 0x04U, 0x05U, 0x04U, 0x08U, 0x04U,
  0x0BU, 0x04U, 0x0EU, 0x04U, 0x11U, 0x04U, 0x14U, 0x04U, 0x17U, 0x04U,
  0x1AU, 0x04U, 0x1DU, 0x04U, 0x20U, 0x04U, 0x23U, 0x04U, 0x26U, 0x04U,
  0x29U, 0x04U, 0x2CU, 0x04U, 0x2FU, 0x04U, 0x32U, 0x04U, 0x35U, 0x04U,
  0x38U, 0x04U, 0x3BU, 0x04U, 0x3EU, 0x04U, 0x41U, 0x04U, 0x44U, 0x04U,
  0x47U, 0x04U, 0x4AU, 0x04U, 0x4DU, 0x04U, 0x50U, 0x04U, 0x02U, 0x05U,
  0x05U, 0x05U, 0x08U, 0x05U, 0x0BU, 0x05U, 0x0EU, 0x05U, 0x11U, 0x05U,
  0x14U, 0x05U, 0x17U, 0x05U, 0x1AU, 0x05U, 0x1DU, 0x05U, 0x20U, 0x05U,
  0x23U, 0x05U, 0x26U, 0x05U, 0x29U, 0x05U, 0x2CU, 0x05U, 0x2FU, 0x05U,
  0x32U, 0x05U, 0x35U, 0x05U, 0x38U, 0x05U, 0x3BU, 0x05U, 0x3EU, 0x05U,
  0x41U, 0x05U, 0x44U, 0x05U, 0x47U, 0x05U, 0x4AU, 0x05U, 0x4DU, 0x05U,
  0x50U, 0x05U, 0x02U, 0x06U, 0x05U, 0x06U, 0x08U, 0x06U, 0x0BU, 0x06U,
  0x0EU, 0x06U, 0x11U, 0x06U, 0x14U, 0x06U, 0x17U, 0x06U, 0x1AU, 0x06U,
  0x1DU, 0x06U, 0x20U, 0x06U, 0x23U, 0x06U, 0x26U, 0x06U, 0x29U, 0x06U,
  0x2CU, 0x06U, 0x2FU, 0x06U, 0x32U, 0x06U, 0x35U, 0x06U, 0x38U, 0x06U,
  0x3BU, 0x06U, 0x3EU, 0x06U, 0x41U, 0x06U, 0x44U, 0x06U, 0x47U, 0x06U,
  0x4AU, 0x06U, 0x4DU, 0x06U, 0x50U, 0x06U, 0x02U, 0x07U, 0x05U, 0x07U,
  0x08U, 0x07U, 0x0BU, 0x07U, 0x0EU, 0x07U, 0x11U, 0x07U, 0x14U, 0x07U,
  0x17U, 0x07U, 0x1AU, 0x07U, 0x1DU, 0x07U, 0x20U, 0x07U, 0x23U, 0x07U,
  0x26U, 0x07U, 0x29U, 0x07U, 0x2CU, 0x07U, 0x2FU, 0x07U, 0x32U, 0x07U,
  0x35U, 0x07U, 0x38U, 0x07U, 0x3BU, 0x07U, 0x3EU, 0x07U, 0x41U, 0x07U,
  0x44U, 0x07U, 0x47U, 0x07U, 0x4AU, 0x07U, 0x4DU, 0x07U, 0x50U, 0x07U,
};

const unsigned char SCRAMBLE_TABLE_TX[] = {
  0x00U, 0xF7U, 0x34U, 0x09U, 0x44U, 0x46U, 0xD7U, 0x06U, 0xB3U, 0x72U,
  0xDEU, 0x42U, 0xF5U, 0xA5U, 0xD8U, 0xF1U, 0x87U, 0x7BU, 0x9AU, 0x04U,
  0x22U, 0xA3U, 0x6BU, 0x83U, 0x59U, 0x39U, 0x6FU, 0xA1U, 0xFAU, 0x52U,
  0xECU, 0xF8U, 0xC3U, 0x3DU, 0x4DU, 0x02U, 0x91U, 0xD1U, 0xB5U, 0xC1U,
  0xACU, 0x9CU, 0xB7U, 0x50U, 0x7DU, 0x29U, 0x76U, 0xFCU, 0xE1U, 0x9EU,
  0x26U, 0x81U, 0xC8U, 0xE8U, 0xDAU, 0x60U, 0x56U, 0xCEU, 0x5BU, 0xA8U,
  0xBEU, 0x14U, 0x3BU, 0xFEU, 0x70U, 0x4FU, 0x93U, 0x40U, 0x64U, 0x74U,
  0x6DU, 0x30U, 0x2BU, 0xE7U, 0x2DU, 0x54U, 0x5FU, 0x8AU, 0x1DU, 0x7FU,
  0xB8U, 0xA7U, 0x49U, 0x20U, 0x32U, 0xBAU, 0x36U, 0x98U, 0x95U, 0xF3U,
  0x06U};

const unsigned char SCRAMBLE_TABLE_RX[] = {
  0x0EU, 0xF2U, 0xC9U, 0x02U, 0x26U, 0x2EU, 0xB6U, 0x0CU, 0xD4U, 0xE7U,
  0xB4U, 0x2AU, 0xFAU, 0x51U, 0xB8U, 0xFEU, 0x1DU, 0xE5U, 0x92U, 0x04U,
  0x4CU, 0x5DU, 0x6CU, 0x19U, 0xA9U, 0xCFU, 0x68U, 0x55U, 0xF4U, 0xA3U,
  0x71U, 0xFCU, 0x3BU, 0xCBU, 0x24U, 0x08U, 0x98U, 0xBAU, 0xD8U, 0x33U,
  0x53U, 0x9EU, 0xD0U, 0xABU, 0xE9U, 0x46U, 0xE3U, 0xF8U, 0x77U, 0x96U,
  0x48U, 0x11U, 0x31U, 0x75U, 0xB0U, 0x66U, 0xA7U, 0x3DU, 0xA1U, 0x57U,
  0xD2U, 0x8DU, 0xC7U, 0xF0U, 0xEFU, 0x2CU, 0x90U, 0x22U, 0x62U, 0xEBU,
  0x60U, 0xCDU, 0x4EU, 0x7BU, 0x42U, 0xAFU, 0xA5U, 0x1BU, 0x8FU, 0xE1U,
  0xDEU, 0x59U, 0x20U, 0x44U, 0xC5U, 0xD6U, 0xC1U, 0x9AU, 0x9CU, 0xF6U,
  0x00U};

CDStarRepeaterSoundCardController::CDStarRepeaterSoundCardController(const wxString& rxDevice, const wxString& txDevice, bool rxInvert, bool txInvert, wxFloat32 modLevel, unsigned int txDelay) :
wxThread(wxTHREAD_JOINABLE),
m_sound(rxDevice, txDevice, DSTAR_RADIO_SAMPLE_RATE, DSTAR_RADIO_BLOCK_SIZE),
m_modLevel(modLevel),
m_txDelay(txDelay),
m_rxData(1000U),
m_rxAudio(4800U),
m_txAudio(48000U),
m_stopped(false),
m_mutex(),
m_readType(),
m_readLength(0U),
m_readBuffer(NULL),
m_demodulator(),
m_modulator()
{
	wxASSERT(!rxDevice.IsEmpty());
	wxASSERT(!txDevice.IsEmpty());

	m_modulator.setInvert(txInvert);
	m_demodulator.setInvert(rxInvert);

	m_sound.setCallback(this, 0U);

	m_readBuffer = new unsigned char[BUFFER_LENGTH];
}

CDStarRepeaterSoundCardController::~CDStarRepeaterSoundCardController()
{
	delete[] m_readBuffer;
}

bool CDStarRepeaterSoundCardController::start()
{
	bool ret = m_sound.open();
	if (!ret)
		return false;

	Create();
	SetPriority(100U);
	Run();

	return true;
}

void* CDStarRepeaterSoundCardController::Entry()
{
	wxLogMessage(wxT("Starting Sound Card Controller thread"));

	while (!m_stopped) {
		Sleep(10UL);
	}

	wxLogMessage(wxT("Stopping Sound Card Controller thread"));

	m_sound.close();

	return NULL;
}

DSMT_TYPE CDStarRepeaterSoundCardController::read()
{
	m_readLength = 0U;

	wxMutexLocker locker(m_mutex);

	if (m_rxData.isEmpty())
		return DSMTT_NONE;

	unsigned char hdr[2U];
	m_rxData.getData(hdr, 2U);

	m_readType   = hdr[0U];
	m_readLength = hdr[1U];
	m_rxData.getData(m_readBuffer, m_readLength);

	switch (m_readType) {
		case TAG_HEADER:
			return DSMTT_HEADER;

		case TAG_DATA:
		case TAG_DATA_END:
			return DSMTT_DATA;

		default:
			return DSMTT_NONE;
	}
}

CHeaderData* CDStarRepeaterSoundCardController::readHeader()
{
	if (m_readType != TAG_HEADER || m_readLength == 0U)
		return NULL;

	return new CHeaderData(m_readBuffer, RADIO_HEADER_LENGTH_BYTES, false);
}

unsigned int CDStarRepeaterSoundCardController::readData(unsigned char* data, unsigned int length, bool& end)
{
	end = false;

	if ((m_readType != TAG_DATA && m_readType != TAG_DATA_END) || m_readLength == 0U)
		return 0U;

	end = m_readType == TAG_DATA_END;

	if (length < m_readLength) {
		::memcpy(data, m_readBuffer, length);
		return length;
	} else {
		::memcpy(data, m_readBuffer, m_readLength);
		return m_readLength;
	}
}

bool CDStarRepeaterSoundCardController::writeHeader(const CHeaderData& header)
{
	unsigned char buffer1[RADIO_HEADER_LENGTH_BYTES];

	::memset(buffer1, ' ', RADIO_HEADER_LENGTH_BYTES);

	buffer1[0U] = header.getFlag1();
	buffer1[1U] = header.getFlag2();
	buffer1[2U] = header.getFlag3();

	wxString rpt2 = header.getRptCall2();
	for (unsigned int i = 0U; i < rpt2.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer1[i + 3U]  = rpt2.GetChar(i);

	wxString rpt1 = header.getRptCall1();
	for (unsigned int i = 0U; i < rpt1.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer1[i + 11U] = rpt1.GetChar(i);

	wxString your = header.getYourCall();
	for (unsigned int i = 0U; i < your.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer1[i + 19U] = your.GetChar(i);

	wxString my1 = header.getMyCall1();
	for (unsigned int i = 0U; i < my1.Len() && i < LONG_CALLSIGN_LENGTH; i++)
		buffer1[i + 27U] = my1.GetChar(i);

	wxString my2 = header.getMyCall2();
	for (unsigned int i = 0U; i < my2.Len() && i < SHORT_CALLSIGN_LENGTH; i++)
		buffer1[i + 35U] = my2.GetChar(i);

	CCCITTChecksumReverse cksum1;
	cksum1.update(buffer1 + 0U, RADIO_HEADER_LENGTH_BYTES - 2U);
	cksum1.result(buffer1 + 39U);

	unsigned int bytes = m_txDelay + 60U;
	for (unsigned int i = 0U; i < bytes; i++)
		writeBits(BIT_SYNC);

	unsigned char buffer2[86U];
	txHeader(buffer1, buffer2 + 2U);

	buffer2[0U]  = FRAME_SYNC0;
	buffer2[1U]  = FRAME_SYNC1;
	buffer2[2U] |= FRAME_SYNC2;

	for (unsigned int i = 0U; i < 85U; i++)
		writeBits(buffer2[i]);

	return true;
}

bool CDStarRepeaterSoundCardController::writeData(const unsigned char* data, unsigned int length, bool end)
{
	if (end) {
		for (unsigned int i = 0U; i < END_PATTERN_LENGTH_BYTES; i++)
			writeBits(END_PATTERN_BYTES[i]);
	} else {
		for (unsigned int i = 0U; i < length; i++)
			writeBits(data[i]);
	}

	return true;
}

unsigned int CDStarRepeaterSoundCardController::getSpace()
{
	unsigned int space = m_txAudio.freeSpace() / DV_FRAME_LENGTH_BYTES;
	if (space > 10U)
		space = 10U;

	return space;
}

bool CDStarRepeaterSoundCardController::getTX()
{
	return m_txAudio.dataSpace() > 0U;
}

void CDStarRepeaterSoundCardController::stop()
{
	m_stopped = true;

	Wait();
}

void CDStarRepeaterSoundCardController::callback(const wxFloat32* input, wxFloat32* output, unsigned int n, int id)
{
	::memset(output, 0x00, n * sizeof(wxFloat32));

	if (!m_stopped) {
		m_rxAudio.addData(input, n);
		m_txAudio.getData(output, n);
	}
}

void CDStarRepeaterSoundCardController::txHeader(const unsigned char* in, unsigned char* out)
{
	unsigned char intermediate[84U];
	unsigned int i;

	for (i = 0U; i < 83U; i++) {
		intermediate[i] = 0x00U;
		out[i] = 0x00U;
	}

	// Convolve the header
	unsigned char d, d1 = 0U, d2 = 0U, g0, g1;
	unsigned int k = 0U;
	for (i = 0U; i < 42U; i++) {
		for (unsigned int j = 0U; j < 8U; j++) {
			unsigned char mask = (0x01U << j);
			d = 0U;

			if (in[i] & mask)
				d = 1U;

			g0 = (d + d2) % 2U;
			g1 = (d + d1 + d2) % 2U;
			d2 = d1;
			d1 = d;

			if (g1)
				intermediate[k / 8U] |= BIT_MASK_TABLE1[k % 8U];

			k++;

			if (g0)
				intermediate[k / 8U] |= BIT_MASK_TABLE1[k % 8U];

			k++;
		}
	}

	// Interleave the header
	i = 0U;
	while (i < 660U) {
		unsigned char d = intermediate[i / 8U];

		if (d & 0x80U)
			out[INTERLEAVE_TABLE_TX[i * 2U]] |= (0x01U << INTERLEAVE_TABLE_TX[i * 2U + 1U]);
		i++;

		if (d & 0x40U)
			out[INTERLEAVE_TABLE_TX[i * 2U]] |= (0x01U << INTERLEAVE_TABLE_TX[i * 2U + 1U]);
		i++;

		if (d & 0x20U)
			out[INTERLEAVE_TABLE_TX[i * 2U]] |= (0x01U << INTERLEAVE_TABLE_TX[i * 2U + 1U]);
		i++;

		if (d & 0x10U)
			out[INTERLEAVE_TABLE_TX[i * 2U]] |= (0x01U << INTERLEAVE_TABLE_TX[i * 2U + 1U]);
		i++;

		if (i < 660U) {
			if (d & 0x08U)
				out[INTERLEAVE_TABLE_TX[i * 2U]] |= (0x01U << INTERLEAVE_TABLE_TX[i * 2U + 1U]);
			i++;

			if (d & 0x04U)
				out[INTERLEAVE_TABLE_TX[i * 2U]] |= (0x01U << INTERLEAVE_TABLE_TX[i * 2U + 1U]);
			i++;

			if (d & 0x02U)
				out[INTERLEAVE_TABLE_TX[i * 2U]] |= (0x01U << INTERLEAVE_TABLE_TX[i * 2U + 1U]);
			i++;

			if (d & 0x01U)
				out[INTERLEAVE_TABLE_TX[i * 2U]] |= (0x01U << INTERLEAVE_TABLE_TX[i * 2U + 1U]);
			i++;
		}
	}

	// Scramble the header
	for (i = 0U; i < 83U; i++)
		out[i] ^= SCRAMBLE_TABLE_TX[i];
}

void CDStarRepeaterSoundCardController::writeBits(unsigned char c)
{
	wxFloat32 buffer[DSTAR_RADIO_BIT_LENGTH];

	unsigned char mask = 0x01U;
	for (unsigned int i = 0U; i < 8U; i++) {
		bool bit = (c & mask) == mask;

		m_modulator.code(bit, buffer, DSTAR_RADIO_BIT_LENGTH);

		for (unsigned int j = 0U; j < DSTAR_RADIO_BIT_LENGTH; j++)
			buffer[j] *= m_modLevel;

		m_txAudio.addData(buffer, DSTAR_RADIO_BIT_LENGTH);

		mask <<= 1;
	}
}

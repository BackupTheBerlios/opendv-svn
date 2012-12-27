/*
 *	Copyright (C) 2012 by Jonathan Naylor, G4KLX
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

#include "RaspberryController.h"

CRaspberryController::CRaspberryController() :
m_outp1(false),
m_outp2(false),
m_outp3(false),
m_outp4(false),
m_outp5(false),
m_outp6(false),
m_outp7(false),
m_outp8(false)
{
}

CRaspberryController::~CRaspberryController()
{
}

// XXX ????
#if !defined(RASPBERRYPI)

bool CRaspberryController::open()
{
	return false;
}

void CRaspberryController::getDigitalInputs(bool& inp1, bool& inp2, bool& inp3, bool& inp4, bool& inp5)
{
}

void CRaspberryController::setDigitalOutputs(bool outp1, bool outp2, bool outp3, bool outp4, bool outp5, bool outp6, bool outp7, bool outp8)
{
}

void CRaspberryController::close()
{
}

#else

#include <bcm2835.h>

bool CRaspberryController::open()
{
	bool ret = ::bcm2835_init();
	if (!ret) {
		wxLogError(wxT("Unable to initialise the BCM2835"));
		return false;
	}

	// Set GPIOs 7,8,10,11,12 to be inputs
	::bcm2835_gpio_fsel(RPI_V2_GPIO_P1_07, BCM2835_GPIO_FSEL_INPT);
	::bcm2835_gpio_fsel(RPI_V2_GPIO_P1_08, BCM2835_GPIO_FSEL_INPT);
	::bcm2835_gpio_fsel(RPI_V2_GPIO_P1_10, BCM2835_GPIO_FSEL_INPT);
	::bcm2835_gpio_fsel(RPI_V2_GPIO_P1_11, BCM2835_GPIO_FSEL_INPT);
	::bcm2835_gpio_fsel(RPI_V2_GPIO_P1_12, BCM2835_GPIO_FSEL_INPT);

	// Set pull downs on the input pins
	::bcm2835_gpio_set_pud(RPI_V2_GPIO_P1_07, BCM2835_GPIO_PUD_DOWN);
	::bcm2835_gpio_set_pud(RPI_V2_GPIO_P1_08, BCM2835_GPIO_PUD_DOWN);
	::bcm2835_gpio_set_pud(RPI_V2_GPIO_P1_10, BCM2835_GPIO_PUD_DOWN);
	::bcm2835_gpio_set_pud(RPI_V2_GPIO_P1_11, BCM2835_GPIO_PUD_DOWN);
	::bcm2835_gpio_set_pud(RPI_V2_GPIO_P1_12, BCM2835_GPIO_PUD_DOWN);

	// Set GPIOs 13,15,16,18,19,21,22,23 to be outputs
	::bcm2835_gpio_fsel(RPI_V2_GPIO_P1_13, BCM2835_GPIO_FSEL_OUTP);
	::bcm2835_gpio_fsel(RPI_V2_GPIO_P1_15, BCM2835_GPIO_FSEL_OUTP);
	::bcm2835_gpio_fsel(RPI_V2_GPIO_P1_16, BCM2835_GPIO_FSEL_OUTP);
	::bcm2835_gpio_fsel(RPI_V2_GPIO_P1_18, BCM2835_GPIO_FSEL_OUTP);
	::bcm2835_gpio_fsel(RPI_V2_GPIO_P1_19, BCM2835_GPIO_FSEL_OUTP);
	::bcm2835_gpio_fsel(RPI_V2_GPIO_P1_21, BCM2835_GPIO_FSEL_OUTP);
	::bcm2835_gpio_fsel(RPI_V2_GPIO_P1_22, BCM2835_GPIO_FSEL_OUTP);
	::bcm2835_gpio_fsel(RPI_V2_GPIO_P1_23, BCM2835_GPIO_FSEL_OUTP);

	setDigitalOutputs(false, false, false, false, false, false, false, false);

	return true;
}

void CRaspberryController::getDigitalInputs(bool& inp1, bool& inp2, bool& inp3, bool& inp4, bool& inp5)
{
	uint8_t level = ::bcm2835_gpio_lev(RPI_V2_GPIO_P1_07);
	inp1 = level > 0U;

	level = ::bcm2835_gpio_lev(RPI_V2_GPIO_P1_08);
	inp2 = level > 0U;

	level = ::bcm2835_gpio_lev(RPI_V2_GPIO_P1_10);
	inp3 = level > 0U;

	level = ::bcm2835_gpio_lev(RPI_V2_GPIO_P1_11);
	inp4 = level > 0U;

	level = ::bcm2835_gpio_lev(RPI_V2_GPIO_P1_12);
	inp5 = level > 0U;
}

void CRaspberryController::setDigitalOutputs(bool outp1, bool outp2, bool outp3, bool outp4, bool outp5, bool outp6, bool outp7, bool outp8)
{
	if (outp1 != m_outp1) {
		::bcm2835_gpio_write(RPI_V2_GPIO_P1_13, outp1 ? HIGH : LOW);
		m_outp1 = outp1;
	}

	if (outp2 != m_outp2) {
		::bcm2835_gpio_write(RPI_V2_GPIO_P1_15, outp2 ? HIGH : LOW);
		m_outp2 = outp2;
	}

	if (outp3 != m_outp3) {
		::bcm2835_gpio_write(RPI_V2_GPIO_P1_16, outp3 ? HIGH : LOW);
		m_outp3 = outp3;
	}

	if (outp4 != m_outp4) {
		::bcm2835_gpio_write(RPI_V2_GPIO_P1_18, outp4 ? HIGH : LOW);
		m_outp4 = outp4;
	}

	if (outp5 != m_outp5) {
		::bcm2835_gpio_write(RPI_V2_GPIO_P1_19, outp5 ? HIGH : LOW);
		m_outp5 = outp5;
	}

	if (outp6 != m_outp6) {
		::bcm2835_gpio_write(RPI_V2_GPIO_P1_21, outp6 ? HIGH : LOW);
		m_outp6 = outp6;
	}

	if (outp7 != m_outp7) {
		::bcm2835_gpio_write(RPI_V2_GPIO_P1_22, outp7 ? HIGH : LOW);
		m_outp7 = outp7;
	}

	if (outp8 != m_outp8) {
		::bcm2835_gpio_write(RPI_V2_GPIO_P1_23, outp8 ? HIGH : LOW);
		m_outp8 = outp8;
	}
}

void CRaspberryController::close()
{
	::bcm2835_close();
}

#endif

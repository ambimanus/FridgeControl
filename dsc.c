/*
 * dsc.c
 *
 *  Created on: 09.03.2009
 *      Author: chh
 */

#include "dsc.h"

uint8_t flag_unload = 0;
int32_t delay = 0UL;

void dsc_load(uint16_t spread) {
	if (basecontroller_get_phase() == PHASE_WARMING) {
		flag_unload = 0;

	}
}

void dsc_unload(uint16_t spread) {
	if (basecontroller_get_phase() == PHASE_COOLING) {
		flag_unload = 1;

	}
}

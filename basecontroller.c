/*
 * basecontroller.c
 *
 *  Created on: 06.03.2009
 *      Author: chh
 */

#include "basecontroller.h"

float tau_cooling;
float tau_warming;
float tau_switch;
float T_from;
uint32_t starttime = 0L;
uint16_t counter;
uint8_t phase;

float inline basecontroller_ac(void) {
	return (T_MIN - T_MAX) / tau_cooling;
}

float inline basecontroller_aw(void) {
	return (T_MAX - T_MIN) / tau_warming;
}

float inline basecontroller_tau_reqc(float T_from, float T_dest) {
	return (T_dest - T_from) / basecontroller_ac();
}

float inline basecontroller_tau_reqw(float T_from, float T_dest) {
	return (T_dest - T_from) / basecontroller_aw();
}

void basecontroller_begin_warming(float T_current, float T_dest) {
	// Update tau_cooling
	tau_cooling = (T_MAX - T_MIN) * counter / (T_from - T_current);
	// Switch phase
	relais_set(0);
	T_from = T_current;
	counter = 0;
	phase = PHASE_WARMING;
	// Predict next phase switch
	tau_switch = basecontroller_tau_reqw(T_from, T_dest);
}

void basecontroller_begin_cooling(float T_current, float T_dest) {
	// Update tau_warming
	tau_warming = (T_MAX - T_MIN) * counter / (T_current - T_from);
	// Switch phase
	relais_set(1);
	T_from = T_current;
	counter = 0;
	phase = PHASE_COOLING;
	// Predict next phase switch
	tau_switch = basecontroller_tau_reqc(T_from, T_dest);
}

void basecontroller_init(float T_current, uint32_t time) {
	T_from = T_current;
	tau_cooling = TAU_COOLING_INIT;
	tau_warming = TAU_WARMING_INIT;
	starttime = time;
	counter = 0;
	if (T_current >= T_MAX) {
		basecontroller_begin_cooling(T_current, T_MIN);
	} else {
		basecontroller_begin_warming(T_current, T_MAX);
	}
}

void basecontroller_poll(float T_current, uint32_t time) {
	counter = time - starttime;
	if (phase == PHASE_COOLING) {
		if (counter >= tau_switch || T_current >= T_MAX) {
			basecontroller_begin_cooling(T_current, T_MIN);
		}
	} else {
		if (counter >= tau_switch || T_current <= T_MIN) {
			basecontroller_begin_warming(T_current, T_MAX);
		}
	}
}

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
basecontroller_states basecontroller_state = BASE_WARMING;

float inline basecontroller_get_tau_cooling() {
	return tau_cooling;
}

float inline basecontroller_get_tau_warming() {
	return tau_warming;
}

float inline basecontroller_ac(void) {
	return (T_MIN- T_MAX) / tau_cooling;
}

float inline basecontroller_aw(void) {
	return (T_MAX- T_MIN) / tau_warming;
}

float inline basecontroller_tau_reqc(float T_from, float T_dest) {
	return (T_dest - T_from) / basecontroller_ac();
}

float inline basecontroller_tau_reqw(float T_from, float T_dest) {
	return (T_dest - T_from) / basecontroller_aw();
}

void basecontroller_begin_warming(float T_current, float T_dest) {
	// Update tau_cooling
	if (T_from != T_current) {
		tau_cooling = (T_MAX- T_MIN) * counter / (T_from - T_current);
	}
	// Switch phase
	relais_set(0);
	T_from = T_current;
	counter = 0;
	basecontroller_state = BASE_WARMING;
	// Predict next phase switch
	tau_switch = basecontroller_tau_reqw(T_from, T_dest);
#ifdef INFO
	char buf_s[32];
	uart_puts_P(PSTR(CR));
	uart_puts_P(PSTR("base:\t> switched to warming."));
	uart_puts_P(PSTR(CR));
	uart_puts_P(PSTR("\t> T_dest is "));
	dtostrf(T_dest, -1, 3, &buf_s[0]);
	uart_puts(buf_s);
	uart_puts_P(PSTR(CR));
	uart_puts_P(PSTR("\t> tau_cooling is now "));
	dtostrf(tau_cooling, -1, 3, &buf_s[0]);
	uart_puts(buf_s);
	uart_puts_P(PSTR(CR));
	uart_puts_P(PSTR("\t> predicted tau_switch is "));
	dtostrf(tau_switch, -1, 3, &buf_s[0]);
	uart_puts(buf_s);
	uart_puts_P(PSTR(CR));
#endif
}

void basecontroller_begin_cooling(float T_current, float T_dest) {
	// Update tau_warming
	if (T_from != T_current) {
		tau_warming = (T_MAX- T_MIN) * counter / (T_current - T_from);
	}
	// Switch phase
	relais_set(1);
	T_from = T_current;
	counter = 0;
	basecontroller_state = BASE_COOLING;
	// Predict next phase switch
	tau_switch = basecontroller_tau_reqc(T_from, T_dest);
#ifdef INFO
	char buf_s[32];
	uart_puts_P(PSTR(CR));
	uart_puts_P(PSTR("base:\t> switched to cooling."));
	uart_puts_P(PSTR(CR));
	uart_puts_P(PSTR("\t> T_dest is "));
	dtostrf(T_dest, -1, 3, &buf_s[0]);
	uart_puts(buf_s);
	uart_puts_P(PSTR(CR));
	uart_puts_P(PSTR("\t> tau_warming is now "));
	dtostrf(tau_warming, -1, 3, &buf_s[0]);
	uart_puts(buf_s);
	uart_puts_P(PSTR(CR));
	uart_puts_P(PSTR("\t> predicted tau_switch is "));
	dtostrf(tau_switch, -1, 3, &buf_s[0]);
	uart_puts(buf_s);
	uart_puts_P(PSTR(CR));
#endif
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
	switch (basecontroller_state) {
	case BASE_COOLING: {
		if (counter >= tau_switch || T_current <= T_MIN) {
			basecontroller_begin_warming(T_current, T_MAX);
		}
		break;
	}
	case BASE_WARMING: {
		if (counter >= tau_switch || T_current >= T_MAX) {
			basecontroller_begin_cooling(T_current, T_MIN);
		}
		break;
	}
	}
}

basecontroller_states basecontroller_get_state() {
	return basecontroller_state;
}

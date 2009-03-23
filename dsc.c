/*
 * dsc.c
 *
 *  Created on: 09.03.2009
 *      Author: chh
 */

#include "dsc.h"

dsc_states dsc_state = DSC_IDLE;
uint8_t flag_unload = 0;
uint32_t delay = 0UL;

void dsc_start(uint8_t doUnload, uint16_t spread) {
	flag_unload = doUnload;
	if ((flag_unload == 0 && basecontroller_get_state() == BASE_WARMING)
			|| (flag_unload != 0 && basecontroller_get_state() == BASE_COOLING)) {
		dsc_state = DSC_WAIT_RANDOM;
		delay = random_get_random(0UL, spread);
#ifdef INFO
		uart_puts_P(PSTR(CR));
		uart_puts_P(PSTR("dsc:\t> flag_unload is "));
		if (flag_unload == 0) {
			uart_puts_P(PSTR("false."));
		} else {
			uart_puts_P(PSTR("true."));
		}
		uart_puts_P(PSTR(CR));
		uart_puts_P(PSTR("\t> state is now 'wait_random'."));
		uart_puts_P(PSTR(CR));
		uart_puts_P(PSTR("\t> delay is "));
		char buf_s[32];
		itoa(delay, &buf_s[0], 10);
		uart_puts(buf_s);
		uart_puts_P(PSTR(CR));
#endif
	} else {
#ifdef INFO
		uart_puts_P(PSTR(CR));
		uart_puts_P(PSTR("dsc:\t> no action required, staying in state 'idle'"));
		uart_puts_P(PSTR(CR));
#endif
	}
}

void dsc_poll(float T_current, uint32_t time) {
	switch (dsc_state) {
	case DSC_IDLE : break;
	case DSC_WAIT_RANDOM: {
		if (delay > 0) {
			delay--;
		} else {
			if (flag_unload) {
				basecontroller_begin_warming(T_current,
						basecontroller_get_t_max(), time);
			} else {
				basecontroller_begin_cooling(T_current,
						basecontroller_get_t_min(), time);
			}
			dsc_state = DSC_WAIT_RESTORE;
			delay = basecontroller_get_tau_cooling()
					+ basecontroller_get_tau_warming();
#ifdef INFO
			uart_puts_P(PSTR(CR));
			uart_puts_P(PSTR("dsc:\t> state is now 'wait_restore'."));
			uart_puts_P(PSTR(CR));
			uart_puts_P(PSTR("\t> delay is "));
			char buf_s[32];
			itoa(delay, &buf_s[0], 10);
			uart_puts(buf_s);
			uart_puts_P(PSTR(CR));
#endif
		}
		break;
	}
	case DSC_WAIT_RESTORE: {
		if (delay > 0) {
			delay--;
		} else {
			// a) Randomization
//			float T_dest = random_get_randomf(basecontroller_get_t_min(),
//					basecontroller_get_t_max());
//			if (T_dest <= T_current) {
//				basecontroller_begin_warming(T_current, T_dest, time);
//			} else {
//				basecontroller_begin_cooling(T_current, T_dest, time);
//			}
			// or b) State switching
			if (basecontroller_get_state() == BASE_COOLING) {
				basecontroller_begin_warming(T_current,
						basecontroller_get_t_max(), time);
			} else {
				basecontroller_begin_cooling(T_current,
						basecontroller_get_t_min(), time);
			}
			dsc_state = DSC_IDLE;
			delay = 0;
#ifdef INFO
			uart_puts_P(PSTR(CR));
			uart_puts_P(PSTR("dsc:\t> state is now 'idle'."));
			uart_puts_P(PSTR(CR));
#endif
		}
		break;
	}
	}
}

dsc_states dsc_get_state() {
	return dsc_state;
}

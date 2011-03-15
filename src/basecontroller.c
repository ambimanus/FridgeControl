/*
 * basecontroller.c
 *
 *  Created on: 06.03.2009
 *      Author: chh
 */

#include "basecontroller.h"

float T_MIN;
float T_MAX;
float T_from;
float T_dest;
uint16_t tau_cooling;
uint16_t tau_warming;
uint16_t tau_switch;
uint32_t starttime = 0L;
uint16_t counter;
basecontroller_states basecontroller_state = BASE_WARMING;

float inline basecontroller_get_t_min() {
    return T_MIN;
}

float inline basecontroller_get_t_max() {
    return T_MAX;
}

void basecontroller_set_t_min(float t_min) {
    T_MIN = t_min;
}

void basecontroller_set_t_max(float t_max) {
    T_MAX = t_max;
}

uint16_t inline basecontroller_get_tau_cooling() {
    return tau_cooling;
}

uint16_t inline basecontroller_get_tau_warming() {
    return tau_warming;
}

float inline basecontroller_ac(void) {
    return (T_MIN - T_MAX) / ((float) tau_cooling);
}

float inline basecontroller_aw(void) {
    return (T_MAX - T_MIN) / ((float) tau_warming);
}

uint16_t inline basecontroller_tau_reqc(float T_from, float T_dest) {
    return ROUND_UINT_16((T_dest - T_from) / basecontroller_ac());
}

uint16_t inline basecontroller_tau_reqw(float T_from, float T_dest) {
    return ROUND_UINT_16((T_dest - T_from) / basecontroller_aw());
}

void basecontroller_begin_warming() {
    float T_current = ds18s20_get_temperature();
    // Check reasonability of desired switch
    if ((T_current < T_MAX) && (T_MAX - T_current < 0.25)) {
        // We would switch again shortly after this switch, so ignore the command!
#ifdef INFO
        uart_puts_P(PSTR(CR));
        uart_puts_P(PSTR("base:\t> switch to 'warming' denied!"));
        uart_puts_P(PSTR(" (preventing oscillations around the temperature border.)"));
        uart_puts_P(PSTR(CR));
#endif
        return;
    }
    // Update tau_cooling
    if (T_from != T_current) {
        tau_cooling = ROUND_UINT_16((T_MAX - T_MIN) * (float) counter / (T_from - T_current));
    }
    // Switch phase
    relais_set(0);
    T_from = T_current;
    T_dest = T_MAX;
    counter = 0;
    starttime = rtc_getTime();
    basecontroller_state = BASE_WARMING;
    // Predict next phase switch
    tau_switch = basecontroller_tau_reqw(T_from, T_dest);
#ifdef INFO
    char buf_s[32];
    uart_puts_P(PSTR(CR));
    uart_puts_P(PSTR("base:\t> switched to warming."));
    uart_puts_P(PSTR(CR));
    uart_puts_P(PSTR("\t> T_current is "));
    dtostrf(T_current, -1, 3, &buf_s[0]);
    uart_puts(buf_s);
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

void basecontroller_begin_cooling() {
    float T_current = ds18s20_get_temperature();
    // Check reasonability of desired switch
    if ((T_current > T_MIN) && (T_current - T_MIN < 0.25)) {
        // We would switch again shortly after this switch, so ignore the command!
#ifdef INFO
        uart_puts_P(PSTR(CR));
        uart_puts_P(PSTR("base:\t> switch to 'cooling' denied!"));
        uart_puts_P(PSTR(" (preventing oscillations around the temperature border.)"));
        uart_puts_P(PSTR(CR));
#endif
        return;
    }
    // Update tau_warming
    if (T_from != T_current) {
        tau_warming = ROUND_UINT_16((T_MAX - T_MIN) * (float) counter / (T_current - T_from));
    }
    // Switch phase
    relais_set(1);
    T_from = T_current;
    T_dest = T_MIN;
    counter = 0;
    starttime = rtc_getTime();
    basecontroller_state = BASE_COOLING;
    // Predict next phase switch
    tau_switch = basecontroller_tau_reqc(T_from, T_dest);
#ifdef INFO
    char buf_s[32];
    uart_puts_P(PSTR(CR));
    uart_puts_P(PSTR("base:\t> switched to cooling."));
    uart_puts_P(PSTR(CR));
    uart_puts_P(PSTR("\t> T_current is "));
    dtostrf(T_current, -1, 3, &buf_s[0]);
    uart_puts(buf_s);
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

void basecontroller_init() {
    T_MIN = T_MIN_INIT;
    T_MAX = T_MAX_INIT;
    T_from = ds18s20_get_temperature();
    tau_cooling = TAU_COOLING_INIT;
    tau_warming = TAU_WARMING_INIT;
    starttime = rtc_getTime();
    counter = 0;
    if (ds18s20_get_temperature() >= T_MAX) {
        basecontroller_begin_cooling();
    } else {
        basecontroller_begin_warming();
    }
}

void basecontroller_poll() {
    float T_current = ds18s20_get_temperature();
    counter = rtc_getTime() - starttime;
    switch (basecontroller_state) {
    case BASE_COOLING: {
        // Take tau_switch only into account, if we're targeting an intermediate temperature
        if ((T_dest != T_MIN && counter >= tau_switch) || T_current <= T_MIN) {
            basecontroller_begin_warming();
        }
        break;
    }
    case BASE_WARMING: {
        // Take tau_switch only into account, if we're targeting an intermediate temperature
        if ((T_dest != T_MAX && counter >= tau_switch) || T_current >= T_MAX) {
            basecontroller_begin_cooling();
        }
        break;
    }
    }
}

basecontroller_states basecontroller_get_state() {
    return basecontroller_state;
}

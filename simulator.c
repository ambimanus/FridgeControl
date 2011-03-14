/*
 * simulator.c
 *
 *  Created on: 23.03.2009
 *      Author: chh
 */

#include "simulator.h"

uint32_t lasttime;
float lasttemp;

void simulator_init() {
    lasttime = rtc_getTime();
    lasttemp = T_MIN_INIT;
}

float simulator_get_temperature() {
    uint32_t time = rtc_getTime();
    float a;
    if (basecontroller_get_state() == BASE_COOLING) {
        a = (basecontroller_get_t_min() - basecontroller_get_t_max())
                / TAU_COOLING_INIT;

    } else {
        a = (basecontroller_get_t_max() - basecontroller_get_t_min())
                / TAU_WARMING_INIT;
    }
    float temp = lasttemp + (a * (time - lasttime));
    lasttime = time;
    lasttemp = temp;
    return temp;
}

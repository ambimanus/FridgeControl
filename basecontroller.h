/*
 * basecontroller.h
 *
 *  Created on: 06.03.2009
 *      Author: chh
 */

#ifndef BASECONTROLLER_H_
#define BASECONTROLLER_H_

#include "main.h"

#define T_MIN				6.5f	// degrees celsius
#define T_MAX				8.0f	// degrees celsius
//#define T_MIN				16.5f	// degrees celsius
//#define T_MAX				28.0f	// degrees celsius
#define TAU_COOLING_INIT	5400.0f	// seconds
#define TAU_WARMING_INIT	9000.0f	// seconds

typedef enum {
	BASE_COOLING,
	BASE_WARMING
} basecontroller_states;

float basecontroller_get_tau_cooling(void);
float basecontroller_get_tau_warming(void);
//float basecontroller_ac(void);
//float basecontroller_aw(void);
//float basecontroller_tau_reqc(float T_from, float T_dest);
//float basecontroller_tau_reqw(float T_from, float T_dest);
void basecontroller_begin_warming(float T_current, float T_dest);
void basecontroller_begin_cooling(float T_current, float T_dest);
void basecontroller_init(float T_current, uint32_t time);
void basecontroller_poll(float T_current, uint32_t time);
basecontroller_states basecontroller_get_state(void);

#endif /* BASECONTROLLER_H_ */

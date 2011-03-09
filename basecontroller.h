/*
 * basecontroller.h
 *
 *  Created on: 06.03.2009
 *      Author: chh
 */

#ifndef BASECONTROLLER_H_
#define BASECONTROLLER_H_

#include "main.h"

#define T_MIN_INIT			7.2f	// degrees celsius
#define T_MAX_INIT			8.9f	// degrees celsius
#if SIMULATE
#define TAU_COOLING_INIT	10.0f	// seconds
#define TAU_WARMING_INIT	15.0f	// seconds
#else
#define TAU_COOLING_INIT	5400.0f	// seconds
#define TAU_WARMING_INIT	9000.0f	// seconds
#endif

typedef enum {
	BASE_COOLING,
	BASE_WARMING
} basecontroller_states;

float basecontroller_get_t_min(void);
float basecontroller_get_t_max(void);
void basecontroller_set_t_min(float t_min);
void basecontroller_set_t_max(float t_max);
uint16_t basecontroller_get_tau_cooling(void);
uint16_t basecontroller_get_tau_warming(void);
//float basecontroller_ac(void);
//float basecontroller_aw(void);
//uint16_t basecontroller_tau_reqc(float T_from, float T_dest);
//uint16_t basecontroller_tau_reqw(float T_from, float T_dest);
void basecontroller_begin_warming(float T_current, float T_dest, uint32_t time);
void basecontroller_begin_cooling(float T_current, float T_dest, uint32_t time);
void basecontroller_init(float T_current, uint32_t time);
void basecontroller_poll(float T_current, uint32_t time);
basecontroller_states basecontroller_get_state(void);

#endif /* BASECONTROLLER_H_ */

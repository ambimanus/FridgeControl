/*
 * basecontroller.h
 *
 *  Created on: 06.03.2009
 *      Author: chh
 */

#ifndef BASECONTROLLER_H_
#define BASECONTROLLER_H_

#include "main.h"

#define T_MIN_INIT          7.2f    // degrees celsius
#define T_MAX_INIT          8.9f    // degrees celsius
#define TAU_COOLING_INIT    5400.0f // seconds
#define TAU_WARMING_INIT    9000.0f // seconds
#define ROUND_INT_8(d)      ( (int8_t) ((d) + ((d) > 0 ? 0.5 : -0.5)) )
#define ROUND_INT_16(d)     ( (int16_t) ((d) + ((d) > 0 ? 0.5 : -0.5)) )
#define ROUND_INT_32(d)     ( (int32_t) ((d) + ((d) > 0 ? 0.5 : -0.5)) )
#define ROUND_UINT_8(d)     ( (uint8_t) ((d) + ((d) > 0 ? 0.5 : -0.5)) )
#define ROUND_UINT_16(d)    ( (uint16_t) ((d) + ((d) > 0 ? 0.5 : -0.5)) )
#define ROUND_UINT_32(d)    ( (uint32_t) ((d) + ((d) > 0 ? 0.5 : -0.5)) )

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
void basecontroller_begin_warming(void);
void basecontroller_begin_cooling(void);
void basecontroller_init(void);
void basecontroller_poll(void);
basecontroller_states basecontroller_get_state(void);

#endif /* BASECONTROLLER_H_ */

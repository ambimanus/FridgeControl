/*
 * events.h
 *
 *  Created on: 13.03.2009
 *      Author: chh
 *
 *  Adapted from:
 *  http://www.sics.se/~adam/contiki/docs/a02334.html
 */

#ifndef EVENTS_H_
#define EVENTS_H_

#include "main.h"

#define MAX_EVENTS		10

typedef enum {
	EV_DSC_LOAD,
	EV_DSC_UNLOAD,
	EV_TLR
} event_type;

typedef struct {
	// id
	uint32_t id;
	// creation timestamp
	uint32_t creation;
	// activation timestamp
	uint32_t activation;
	// event type
	event_type type;
	// parameters
	float *parameters[];
} event;

uint8_t events_is_event_pending(void);
void events_push_event(event *ev);
event* events_pop_event(uint32_t timestamp);

#endif /* EVENTS_H_ */

/*
 * dsc.h
 *
 *  Created on: 09.03.2009
 *      Author: chh
 */

#ifndef DSC_H_
#define DSC_H_

#include "main.h"

typedef enum {
    DSC_IDLE,
    DSC_WAIT_RANDOM,
    DSC_WAIT_RESTORE
} dsc_states;

void dsc_start(uint8_t doUnload, uint16_t spread);
void dsc_poll(float T_current, uint32_t time);
dsc_states dsc_get_state(void);

#endif /* DSC_H_ */

/*
 * random.c
 *
 *  Created on: 09.03.2009
 *      Author: chh
 */

#include "random.h"

uint16_t random_get_seed() {
	uint16_t seed = 0;
	uint16_t *p = (uint16_t*) (RAMEND+1);
	extern uint16_t __heap_start;

	while (p >= &__heap_start + 1)
		seed ^= *(--p);

	return seed;
}

uint32_t random_get_random(uint32_t low, uint32_t high) {
	uint32_t rnd = rand();
	uint32_t delay = ((int32_t) ((uint16_t) rnd) * (int32_t) high)
			/ (uint16_t) RANDOM_MAX;
	char buf_s[32];
	sprintf(buf_s, "%lu\t=>\t%lu\n\r", rnd, delay);
	uart_puts(buf_s);

	return delay;
}

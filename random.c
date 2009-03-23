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

uint32_t random_get_random(int32_t low, int32_t high) {
	if (low >= high) {
		return 0UL;
	}
	uint32_t rnd = random() / (RANDOM_MAX / (high - low) + 1);
	rnd += low;
	return rnd;
}

float random_get_randomf(float low, float high) {
	if (low >= high) {
		return 0.0;
	}
	float rnd = ((float) random() / ((float) RANDOM_MAX + 1) * (high - low));
	rnd += low;
	return rnd;
}

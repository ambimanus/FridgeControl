/*
 * random.h
 *
 *  Created on: 09.03.2009
 *      Author: chh
 */

#ifndef RANDOM_H_
#define RANDOM_H_

#include "main.h"

uint16_t random_get_seed(void);
uint32_t random_get_random(int32_t low, int32_t high);
float random_get_randomf(float low, float high);

#endif /* RANDOM_H_ */

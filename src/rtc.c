/*
 * rtc.c
 *
 *  Created on: 05.03.2009
 *      Author: chh
 */

#include "rtc.h"

uint16_t prescaler;
uint32_t volatile time;
uint8_t volatile flag;

ISR (TIMER1_COMPA_vect) {
    #if F_CPU % DEBOUNCE
        OCR1A = F_CPU / DEBOUNCE - 1; // compare DEBOUNCE - 1 times
    #endif
        if (--prescaler == 0) {
            prescaler = (uint16_t) DEBOUNCE;
            time++; // exact one second over
            flag = 1; // enable flag
//          if (*second == 60)
//              *second = 0;
//          // display second (binary)
//          PORTC = ~(*second);
    #if F_CPU % DEBOUNCE            // handle remainder
            OCR1A = F_CPU / DEBOUNCE + F_CPU % DEBOUNCE - 1; // compare once per second
    #endif
        }
}

void rtc_init() {
    // divide by 1
    TCCR1B = 1 << WGM12 ^ 1 << CS10;
    // clear on compare
    OCR1A = F_CPU / DEBOUNCE - 1;
    TCNT1 = 0;
    time = 0;
    flag = 0;
    prescaler = (uint16_t) DEBOUNCE;

    TIMSK = 1 << OCIE1A;
    sei();
}

uint8_t inline rtc_hasChanged() {
    return flag;
}

void inline rtc_clearFlag() {
    flag = 0;
}

uint32_t inline rtc_getTime() {
    return time;
}

#include "myavr.h"

void port_a_setpin(const uint8_t pin, const uint8_t high) {
    if (high)
        PORTA |= (1 << pin); //set PORTA.pin high
    else
        PORTA &= ~(1 << pin); //set PORTA.pin low
}

void port_b_setpin(const uint8_t pin, const uint8_t high) {
    if (high)
        PORTA |= (1 << pin); //set PORTA.pin high
    else
        PORTA &= ~(1 << pin); //set PORTA.pin low
}

void port_c_setpin(const uint8_t pin, const uint8_t high) {
    if (high)
        PORTA |= (1 << pin); //set PORTA.pin high
    else
        PORTA &= ~(1 << pin); //set PORTA.pin low
}

void port_d_setpin(const uint8_t pin, const uint8_t high) {
    if (high)
        PORTA |= (1 << pin); //set PORTA.pin high
    else
        PORTA &= ~(1 << pin); //set PORTA.pin low
}

void relais_init(void) {
    // Set relais pin as output
    RELAIS_DDR |= (1 << RELAIS_PIN);
}

void relais_test(void) {
    relais_set(1);
    _delay_ms(1000);
    relais_set(0);
}

void relais_set(uint8_t enable) {
    if (enable) {
        // Enable led 2
        led_set(2, 1);
        // Enable relais
        RELAIS_PORT |= (1 << RELAIS_PIN);
    } else {
        // Disable led 2
        led_set(2, 0);
        // Disable relais
        RELAIS_PORT &= ~(1 << RELAIS_PIN);
    }
}

uint8_t inline relais_get_state(void) {
    return ((RELAIS_PORT >> RELAIS_PIN) & 1);
}

void led_init(void) {
    // Set Port C as output for LEDs
    LED_DDR |= 0xFF;    //11111111
    // Turn LEDs off
    LED_PORT |= 0xFF;   //11111111
}

void led_set(uint8_t led, uint8_t enable) {
    if (enable)
        LED_PORT &= ~(1 << led);
    else
        LED_PORT |= (1 << led);
}

uint16_t adc_read(uint8_t channel) {
    // setup
    uint16_t ret = 0;
    ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
    ADMUX = channel;
    // dummy-readout
    ADCSRA |= (1<<ADSC);
    while(ADCSRA & (1<<ADSC)) {}
    ret = ADCW;
    // actual readout: mean of 4 measurements
    ret = 0;
    for(uint8_t i = 0; i < 4; i++) {
        ADCSRA |= (1<<ADSC);
        while(ADCSRA & (1<<ADSC)) {}
        ret = ret + ADCW;
    }
    // setdown
    ADCSRA &= ~(1<<ADEN);
    return ret / 4;
}

uint8_t button_read(void) {
    // read button port (port 7)
    uint16_t val = adc_read(7);
    // debounce
    port_a_setpin(7, 1);
    _delay_ms(1);
    port_a_setpin(7, 1);
    // calculate button
    if((val >= 384) && (val <= 424)) {return 1;}
    else if((val >= 316) && (val <= 356)) {return 2;}
    else if((val >= 244) && (val <= 284)) {return 3;}
    else if((val >= 167) && (val <= 207)) {return 4;}
    else if((val >= 84) && (val <= 124)) {return 5;}
    else {return 0;}
}

void speaker_init() {
    // Set speaker pin as output
    SPEAKER_DDR |= (1 << SPEAKER_PIN);
    // Set pin low
    SPEAKER_PORT &= ~(1 << SPEAKER_PIN);
}

void speaker_sound(uint16_t f, uint16_t dur) {
    uint32_t cycles;
    // holdtime in us
    uint32_t holdtime;
    cycles = (uint32_t) f * (uint32_t) dur / 1000UL;
    holdtime = 500000UL / (uint32_t) f;

    for (uint32_t i = 0; i < cycles; i++) {
        SPEAKER_PORT |= (1 << SPEAKER_PIN);
        _delay_us(holdtime);
        SPEAKER_PORT &= ~(1 << SPEAKER_PIN);
        _delay_us(holdtime);
    }
}

void speaker_tune(uint16_t bpm, uint16_t duration, char note, uint16_t octave) {
    // Dauer:
    uint32_t dur = 240000UL / ((uint32_t) bpm * (uint32_t) duration);

    // Frequenz:
    uint16_t f = 440;
    switch (note) {
        case 'c': f = 261 * octave; break;
        case 'd': f = 293 * octave; break;
        case 'e': f = 329 * octave; break;
        case 'f': f = 349 * octave; break;
        case 'g': f = 392 * octave; break;
        case 'a': f = 440 * octave; break;
        case 'h': f = 493 * octave; break;
        default: break;
    }

    speaker_sound(f, dur);
}

void speaker_tune_indy(void) {

    uint16_t bpm = 150;

    speaker_tune(bpm, 4, 'e', 1);
    speaker_tune(bpm, 16, 'f', 1);
    speaker_tune(bpm, 8, 'g', 1);
    speaker_tune(bpm, 2, 'c', 2);
    speaker_tune(bpm, 4, 'd', 1);
    speaker_tune(bpm, 16, 'e', 1);
    speaker_tune(bpm, 1, 'f', 1);

    speaker_tune(bpm, 4, 'g', 1);
    speaker_tune(bpm, 16, 'a', 1);
    speaker_tune(bpm, 8, 'b', 1);
    speaker_tune(bpm, 2, 'f', 2);
    speaker_tune(bpm, 4, 'a', 1);
    speaker_tune(bpm, 16, 'b', 1);
    speaker_tune(bpm, 4, 'c', 2);
    speaker_tune(bpm, 4, 'd', 2);
    speaker_tune(bpm, 4, 'e', 2);

//  speaker_tune(bpm, 4, 'e', 1);
//  speaker_tune(bpm, 16, 'f', 1);
//  speaker_tune(bpm, 8, 'g', 1);
//  speaker_tune(bpm, 1, 'c', 2);
//  speaker_tune(bpm, 4, 'd', 2);
//  speaker_tune(bpm, 16, 'e', 2);
//  speaker_tune(bpm, 2, 'f', 2);
//  speaker_tune(bpm, 4, 'g', 1);
//  speaker_tune(bpm, 16, 'g', 1);
//  speaker_tune(bpm, 4, 'e', 2);
//  speaker_tune(bpm, 4, 'd', 2);
//  speaker_tune(bpm, 16, 'g', 1);
//  speaker_tune(bpm, 4, 'e', 2);
//  speaker_tune(bpm, 4, 'd', 2);
//  speaker_tune(bpm, 16, 'g', 1);
//  speaker_tune(bpm, 4, 'f', 2);
//  speaker_tune(bpm, 4, 'e', 2);
//  speaker_tune(bpm, 16, 'd', 2);
//  speaker_tune(bpm, 2, 'c', 2);
}

void speaker_tune_imperial(void) {

    uint16_t bpm = 112;

    speaker_tune(bpm, 4, 'a', 1);
    speaker_tune(bpm, 4, 'a', 1);
    speaker_tune(bpm, 4, 'a', 1);
    speaker_tune(bpm, 4, 'f', 1);
    speaker_tune(bpm, 16, 'c', 2);
    speaker_tune(bpm, 4, 'a', 1);
    speaker_tune(bpm, 4, 'f', 1);
    speaker_tune(bpm, 16, 'c', 2);
    speaker_tune(bpm, 2, 'a', 1);

//  speaker_tune(bpm, 4, 'e', 2);
//  speaker_tune(bpm, 4, 'e', 2);
//  speaker_tune(bpm, 4, 'e', 2);
//  speaker_tune(bpm, 4, 'f', 2);
//  speaker_tune(bpm, 16, 'c', 2);
//  speaker_tune(bpm, 4, 'g', 1);
//  speaker_tune(bpm, 4, 'f', 1);
//  speaker_tune(bpm, 16, 'c', 2);
//  speaker_tune(bpm, 2, 'a', 1);
//  speaker_tune(bpm, 4, 'a', 2);
//  speaker_tune(bpm, 4, 'a', 1);
//  speaker_tune(bpm, 16, 'a', 1);
//  speaker_tune(bpm, 4, 'a', 2);
//  speaker_tune(bpm, 4, 'g', 2);
//  speaker_tune(bpm, 16, 'g', 2);
//  speaker_tune(bpm, 16, 'f', 2);
//  speaker_tune(bpm, 16, 'f', 2);
//  speaker_tune(bpm, 4, 'f', 2);
//  speaker_tune(bpm, 8, 'a', 1);
//  speaker_tune(bpm, 4, 'd', 2);
//  speaker_tune(bpm, 16, 'c', 2);
//  speaker_tune(bpm, 16, 'c', 2);
//  speaker_tune(bpm, 16, 'b', 1);
//  speaker_tune(bpm, 4, 'c', 2);
//  speaker_tune(bpm, 8, 'f', 1);
//  speaker_tune(bpm, 4, 'g', 2);
//  speaker_tune(bpm, 4, 'f', 1);
//  speaker_tune(bpm, 16, 'g', 1);
//  speaker_tune(bpm, 4, 'c', 2);
//  speaker_tune(bpm, 4, 'a', 1);
//  speaker_tune(bpm, 16, 'c', 2);
//  speaker_tune(bpm, 2, 'e', 2);
//  speaker_tune(bpm, 4, 'a', 2);
//  speaker_tune(bpm, 4, 'a', 1);
//  speaker_tune(bpm, 16, 'a', 1);
//  speaker_tune(bpm, 4, 'a', 2);
//  speaker_tune(bpm, 4, 'g', 2);
//  speaker_tune(bpm, 16, 'g', 2);
//  speaker_tune(bpm, 16, 'f', 2);
//  speaker_tune(bpm, 16, 'f', 2);
}

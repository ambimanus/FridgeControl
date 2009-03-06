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
	if (enable)
		RELAIS_PORT |= (1 << RELAIS_PIN);
	else
		RELAIS_PORT &= ~(1 << RELAIS_PIN);
}

void led_init(void) {
	// Set Port C as output for LEDs
	LED_DDR |= 0xFF;	//11111111
	// Turn LEDs off
	LED_PORT |= 0xFF;	//11111111
}

void led_set(uint8_t led, uint8_t enable) {
	if (enable)
		LED_PORT &= ~(1 << led);
	else
		LED_PORT |= (1 << led);
}

void speaker_init() {
	// Set speaker pin as output
	SPEAKER_DDR |= SPEAKER_PIN;
	// Set pin low
	SPEAKER_PORT &= ~(1 << SPEAKER_PIN);
}

void speaker_sound(uint16_t f, uint16_t dur) {
	uint32_t cycles;
	// holdtime in us
	uint32_t holdtime;
	cycles = ((uint32_t) f * (uint32_t) dur) / 1000UL;
	holdtime = 1000000UL / (2UL * (uint32_t) f);

	for (uint32_t i = 0; i < cycles; i++) {
		SPEAKER_PORT |= (1 << SPEAKER_PIN);
		_delay_us(holdtime);
		SPEAKER_PORT &= ~(1 << SPEAKER_PIN);
		_delay_us(holdtime);
	}
}

void speaker_tune(uint16_t bpm, uint16_t duration, char note, uint16_t octave) {
	// Dauer:
	uint32_t tmp_d = bpm;
	tmp_d = tmp_d * duration;
	uint32_t dur = 240000/tmp_d;

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

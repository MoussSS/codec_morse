#include "delay.h"

#ifndef UTEST
static uint32_t duration_ticks;
const uint32_t system_frequency_hz = 2000000u;
#else
uint32_t duration_ticks;
uint32_t system_frequency_hz;
#endif

void delay_ms(uint32_t duration) {
    duration_ticks = system_frequency_hz * duration / 1000u;
    for(uint32_t delay=0; delay < duration_ticks; delay++) {
	    asm volatile ("NOP");
	}
}

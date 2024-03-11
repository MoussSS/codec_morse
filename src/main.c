#include <stdint.h>
#include "main.h"

#ifndef UTEST
#include "user_led.h"
#include "delay.h"
#else
#include "mock_user_led.h"
#include "mock_delay.h"
#endif

#define STK_CTRL (*(volatile uint32_t *)0xE000E010)
#define STK_RELOAD (*(volatile uint32_t *)0xE000E014)
#define STK_VAL (*(volatile uint32_t *)0xE000E018)
#define STK_CALIB (*(volatile uint32_t *)0xE000E01C)

extern volatile uint32_t systick_counter;

/* Blink User Led on Nucleo-f446re board
 * User Led = GPIOA5
 */
int MAIN(void) {

	/* After reset, the CPU clock frequency is 16MHz */

    STK_CTRL |= 0x00000007;
    STK_RELOAD = 16000u; // each 1 ms

	userLed_setup();

	LOOP {
		userLed_set(USER_LED_STATE_OFF);
		delay_ms(500u);
        userLed_set(USER_LED_STATE_ON);
		delay_ms(500u);
	}

	return 0;
}

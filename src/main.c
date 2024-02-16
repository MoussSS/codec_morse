#include <stdint.h>
#include "main.h"

#ifndef UTEST
#include "user_led.h"
#include "delay.h"
#else
#include "mock_user_led.h"
#include "mock_delay.h"
#endif

/* Blink User Led on Nucleo-f446re board
 * User Led = GPIOA5
 */
int MAIN(void) {

	/* After reset, the CPU clock frequency is 16MHz */

	userLed_setup();

	LOOP {
		userLed_set(USER_LED_STATE_OFF);
		delay_ms(500u);
        userLed_set(USER_LED_STATE_ON);
		delay_ms(500u);
	}

	return 0;
}

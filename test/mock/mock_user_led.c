#include "mock_user_led.h"

#include <setjmp.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <cmocka.h>

/******************************************************************************/

void mock_assert_call_userLed_setup(void) {
    expect_function_call(userLed_setup);
}

void userLed_setup(void) {
    function_called();
}

/******************************************************************************/

void mock_assert_call_userLed_set(userLed_state_e state) {
    expect_function_call(userLed_set);
    expect_value(userLed_set, state, state);
}

void userLed_set(userLed_state_e state) {
    function_called();
    check_expected(state);
}

/******************************************************************************/

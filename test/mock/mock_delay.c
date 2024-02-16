#include "mock_delay.h"

#include <setjmp.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <cmocka.h>

/******************************************************************************/

void mock_assert_call_delay_ms(uint32_t duration) {
    expect_function_call(delay_ms);
    expect_value(delay_ms, duration, duration);
}

void delay_ms(uint32_t duration) {
    function_called();
    check_expected(duration);
}

/******************************************************************************/

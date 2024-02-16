#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "../../src/main.h"
#include "../mock/mock_user_led.h"
#include "../mock/mock_delay.h"

static void test_main(void **state) {
    (void) state;

    int result;
    mock_assert_call_userLed_setup();
    mock_assert_call_userLed_set(USER_LED_STATE_OFF);
    mock_assert_call_delay_ms(500u);
    mock_assert_call_userLed_set(USER_LED_STATE_ON);
    mock_assert_call_delay_ms(500u);
    result = MAIN();
    assert_int_equal(result, 0);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_main),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
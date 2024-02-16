#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "../../src/delay.h"

extern uint32_t duration_ticks;
extern uint32_t system_frequency_hz;

static void test_delay_ms(void **state) {
    (void) state;
    system_frequency_hz = 2000000u;
    delay_ms(1u);
    assert_int_equal(duration_ticks, 2000u);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_delay_ms),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}

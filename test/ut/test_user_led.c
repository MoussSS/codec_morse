#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "../../src/user_led.h"

extern uint32_t RCC_AHB1ENR;
extern uint32_t GPIOA_MODER;
extern uint32_t GPIOA_OTYPER;
extern uint32_t GPIOA_OSPEEDR;
extern uint32_t GPIOA_PUPDR;
extern uint32_t GPIOA_ODR;

static void test_userLed_setup(void **state) {
    (void) state;
    userLed_setup();
    assert_int_equal(RCC_AHB1ENR, 0x00000001);
    assert_int_equal(GPIOA_MODER, 0x00000400);
    assert_int_equal(GPIOA_OTYPER, 0x00000000);
    assert_int_equal(GPIOA_OSPEEDR, 0x00000000);
    assert_int_equal(GPIOA_PUPDR, 0x00000000);
    assert_int_equal(GPIOA_ODR, 0x00000000);
}

static void test_userLed_set_off(void **state) {
    (void) state;
    userLed_set(USER_LED_STATE_OFF);
    assert_int_equal(GPIOA_ODR, 0x00000000);
}

static void test_userLed_set_on(void **state) {
    (void) state;
    userLed_set(USER_LED_STATE_ON);
    assert_int_equal(GPIOA_ODR, 0x00000020);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_userLed_setup),
        cmocka_unit_test(test_userLed_set_off),
        cmocka_unit_test(test_userLed_set_on),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "../../src/user_led.h"

typedef struct { 
  volatile uint32_t MODER;
  volatile uint32_t OTYPER;
  volatile uint32_t OSPEEDR;
  volatile uint32_t PUPDR;
  volatile uint32_t IDR;
  volatile uint32_t ODR; 
} gpio_s;

volatile uint32_t RCC_AHB1ENR = 0;
gpio_s GPIOA = {0};

static void test_userLed_setup(void **state) {
    (void) state;
    userLed_setup();
    assert_int_equal(RCC_AHB1ENR, 0x00000001);
    assert_int_equal(GPIOA.MODER, 0x00000400);
    assert_int_equal(GPIOA.OTYPER, 0x00000000);
    assert_int_equal(GPIOA.OSPEEDR, 0x00000000);
    assert_int_equal(GPIOA.PUPDR, 0x00000000);
    assert_int_equal(GPIOA.ODR, 0x00000000);
}

static void test_userLed_set_off(void **state) {
    (void) state;
    userLed_set(USER_LED_STATE_OFF);
    assert_int_equal(GPIOA.ODR, 0x00000000);
}

static void test_userLed_set_on(void **state) {
    (void) state;
    userLed_set(USER_LED_STATE_ON);
    assert_int_equal(GPIOA.ODR, 0x00000020);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_userLed_setup),
        cmocka_unit_test(test_userLed_set_off),
        cmocka_unit_test(test_userLed_set_on),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}

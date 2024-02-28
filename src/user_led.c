#include "user_led.h"
#include <stdint.h>

typedef struct { 
  volatile uint32_t MODER;
  volatile uint32_t OTYPER;
  volatile uint32_t OSPEEDR;
  volatile uint32_t PUPDR;
  volatile uint32_t IDR;
  volatile uint32_t ODR; 
} gpio_s;

#ifndef UTEST
#define RCC_AHB1ENR (*(volatile uint32_t *)0x40023830)
#define GPIOA (*(gpio_s*)(0x40020000))
#else
extern volatile uint32_t RCC_AHB1ENR;
extern gpio_s GPIOA;
#endif

void userLed_setup(void) {
    RCC_AHB1ENR |= (0b1 << 0); // Enable periph clock for GPIOA port

	/* GPIOA5 as Output */
	GPIOA.MODER &= ~(0b11 << 10); // Reset configuration mode for pin 5
	GPIOA.MODER |= (0b01 << 10); // Set Output mode for pin 5

	/* GPIOA5 on Push-Pull */
	GPIOA.OTYPER &= ~(0b1 << 5);

	/* GPIOA5 on Low speed */
	GPIOA.OSPEEDR &= ~(0b11 << 10);

	/* GPIOA5 on No pull-up/pull-down */
	GPIOA.PUPDR &= ~(0b11 << 10);
}

void userLed_set(userLed_state_e state) {
	switch (state)
	{
	case USER_LED_STATE_OFF:
		GPIOA.ODR &= ~(0b1 << 5);
		break;
	case USER_LED_STATE_ON:
		GPIOA.ODR |= (0b1 << 5);
		break;
	default:
		break;
	}
}

#include "user_led.h"
#include <stdint.h>

#ifndef UTEST
#define RCC_AHB1ENR (*(volatile uint32_t *)0x40023830)
#define GPIOA_MODER (*(volatile uint32_t *)0x40020000)
#define GPIOA_OTYPER (*(volatile uint32_t *)0x40020004)
#define GPIOA_OSPEEDR (*(volatile uint32_t *)0x40020008)
#define GPIOA_PUPDR (*(volatile uint32_t *)0x4002000C)
#define GPIOA_ODR (*(volatile uint32_t *)0x40020014)
#else
uint32_t RCC_AHB1ENR;
uint32_t GPIOA_MODER;
uint32_t GPIOA_OTYPER;
uint32_t GPIOA_OSPEEDR;
uint32_t GPIOA_PUPDR;
uint32_t GPIOA_ODR;
#endif

void userLed_setup(void) {
    RCC_AHB1ENR |= (0b1 << 0); // Enable periph clock for GPIOA port

	/* GPIOA5 as Output */
	GPIOA_MODER &= ~(0b11 << 10); // Reset configuration mode for pin 5
	GPIOA_MODER |= (0b01 << 10); // Set Output mode for pin 5

	/* GPIOA5 on Push-Pull */
	GPIOA_OTYPER &= ~(0b1 << 5);

	/* GPIOA5 on Low speed */
	GPIOA_OSPEEDR &= ~(0b11 << 10);

	/* GPIOA5 on No pull-up/pull-down */
	GPIOA_PUPDR &= ~(0b11 << 10);
}

void userLed_set(userLed_state_e state) {
	switch (state)
	{
	case USER_LED_STATE_OFF:
		GPIOA_ODR &= ~(0b1 << 5);
		break;
	case USER_LED_STATE_ON:
		GPIOA_ODR |= (0b1 << 5);
		break;
	default:
		break;
	}
}

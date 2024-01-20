#include <stdint.h>

/* Blink User Led on Nucleo-f446re board
 * User Led = GPIOA5
 */

#define RCC_AHB1ENR (*(volatile uint32_t *)0x40023830)

#define GPIOA_MODER (*(volatile uint32_t *)0x40020000)
#define GPIOA_OTYPER (*(volatile uint32_t *)0x40020004)
#define GPIOA_OSPEEDR (*(volatile uint32_t *)0x40020008)
#define GPIOA_PUPDR (*(volatile uint32_t *)0x4002000C)
#define GPIOA_ODR (*(volatile uint32_t *)0x40020014)

int main(void) {
	RCC_AHB1ENR |= (1 << 0); // Enable periph clock for GPIOA port

	/* GPIOA5 as Output */
	GPIOA_MODER &= ~(0b11 << 10); // Reset configuration mode for pin 5
	GPIOA_MODER |= (0b01 << 10); // Set Output mode for pin 5

	/* GPIOA5 on Push-Pull */
	GPIOA_OTYPER &= ~(0b1 << 5);

	/* GPIOA5 on Low speed */
	GPIOA_OSPEEDR &= ~(0b11 << 10);

	/* GPIOA5 on No pull-up/pull-down */
	GPIOA_PUPDR &= ~(0b11 << 10);

	while (1) {
		/* GPIOA5 set at 0 */
        GPIOA_ODR &= ~(0b1 << 5);
		/* GPIOA5 set at 1 */
        GPIOA_ODR |= (0b1 << 5);
	}

	return 0;
}
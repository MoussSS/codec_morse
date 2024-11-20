#include <stdint.h>
#include <stdbool.h>
#include "morse.h"

#define RAM_START 0x20000000u
#define RAM_SIZE (128u * 1024u) // 128K
#define RAM_END (RAM_START + RAM_SIZE)
#define STACK_START RAM_END

extern uint32_t _estack;
extern uint32_t _etext;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _la_data;
extern uint32_t _sbss;
extern uint32_t _ebss;

static volatile uint32_t systick_counter = 0;
static volatile bool main_loop_sync_received = false;

void Reset_Handler(void);
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void TIM2_Handler(void);

int main(void);
void main_loop(void);

void set_morse_signal(bool is_on);

#define RCC_AHB1ENR (*(volatile uint32_t *)0x40023830)
#define RCC_APB1ENR (*(volatile uint32_t *)0x40023840)
#define GPIOA (*(gpio_s*)(0x40020000))

#define NVIC_ISER (*(volatile uint32_t *)0xE000E100)


#define TIM2_BASE (0x40000000)
#define TIM2_CR1 (*(volatile uint32_t*)(TIM2_BASE + 0x00))
#define TIM2_SMCR (*(volatile uint32_t*)(TIM2_BASE + 0x08))
#define TIM2_DIER (*(volatile uint32_t*)(TIM2_BASE + 0x0C))
#define TIM2_SR (*(volatile uint32_t*)(TIM2_BASE + 0x10))
#define TIM2_CNT (*(volatile uint32_t*)(TIM2_BASE + 0x24))
#define TIM2_PSC (*(volatile uint32_t*)(TIM2_BASE + 0x28))
#define TIM2_ARR (*(volatile uint32_t*)(TIM2_BASE + 0x2C))

#define USART2_BASE (0x40004400)
#define USART2_SR (*(volatile uint32_t*)(USART2_BASE + 0x00))
#define USART2_DR (*(volatile uint32_t*)(USART2_BASE + 0x04))
#define USART2_BRR (*(volatile uint32_t*)(USART2_BASE + 0x08))
#define USART2_CR1 (*(volatile uint32_t*)(USART2_BASE + 0x0C))

uint32_t vectors[] __attribute__((section(".isr_vector"))) = { // See reference manual §10 - p239
    (uint32_t) STACK_START,          // 0x0000 0000
    (uint32_t) &Reset_Handler,
    (uint32_t) &NMI_Handler,
    (uint32_t) &HardFault_Handler,
    (uint32_t) &MemManage_Handler,   // 0x0000 0010
    (uint32_t) &BusFault_Handler,
    (uint32_t) &UsageFault_Handler,
    (uint32_t) 0, // reserved
    (uint32_t) 0, // reserved        // 0x0000 0020
    (uint32_t) 0, // reserved
    (uint32_t) 0, // reserved
    (uint32_t) &SVC_Handler,
    (uint32_t) &DebugMon_Handler,    // 0x0000 0030
    (uint32_t) 0, // reserved
    (uint32_t) &PendSV_Handler,
    (uint32_t) &SysTick_Handler,
    (uint32_t) 0, // reserved        // 0x0000 0040
    (uint32_t) 0, // reserved
    (uint32_t) 0, // reserved
    (uint32_t) 0, // reserved
    (uint32_t) 0, // reserved        // 0x0000 0050
    (uint32_t) 0, // reserved
    (uint32_t) 0, // reserved
    (uint32_t) 0, // reserved
    (uint32_t) 0, // reserved        // 0x0000 0060
    (uint32_t) 0, // reserved
    (uint32_t) 0, // reserved
    (uint32_t) 0, // reserved
    (uint32_t) 0, // reserved        // 0x0000 0070
    (uint32_t) 0, // reserved
    (uint32_t) 0, // reserved
    (uint32_t) 0, // reserved
    (uint32_t) 0, // reserved        // 0x0000 0080
    (uint32_t) 0, // reserved
    (uint32_t) 0, // reserved
    (uint32_t) 0, // reserved
    (uint32_t) 0, // reserved        // 0x0000 0090
    (uint32_t) 0, // reserved
    (uint32_t) 0, // reserved
    (uint32_t) 0, // reserved
    (uint32_t) 0, // reserved        // 0x0000 00A0
    (uint32_t) 0, // reserved
    (uint32_t) 0, // reserved
    (uint32_t) 0, // reserved
    (uint32_t) &TIM2_Handler,        // 0x0000 00B0
    (uint32_t) 0, // reserved
    (uint32_t) 0, // reserved
    (uint32_t) 0, // reserved
};

void Reset_Handler(void) {
	// Copy .data section to RAM
	uint8_t *ramData = (uint8_t *)&_sdata;
	uint8_t *romData = (uint8_t *)&_la_data;
	uint32_t dataSize = (uint32_t)&_edata - (uint32_t)&_sdata;
	for (uint32_t index = 0; index < dataSize; index++) {
		*ramData++ = *romData++;
	}

	// Init the .bss section to 0
	uint32_t bssSize = (uint32_t)&_ebss - (uint32_t)&_sbss;
	uint8_t *bssData = (uint8_t *)&_sbss;
	for (uint32_t index = 0; index < bssSize; index++) {
		*bssData++ = 0;
	}

	// Now invoke main
	(void)main();
}

void NMI_Handler(void) {
	while(1);
}

void HardFault_Handler(void) {
	while(1);
}

void MemManage_Handler(void) {
	while(1);
}

void BusFault_Handler(void) {
	while(1);
}

void UsageFault_Handler(void) {
	while(1);
}

void SVC_Handler(void) {
	while(1);
}

void DebugMon_Handler(void) {
	while(1);
}

void PendSV_Handler(void) {
	while(1);
}

void SysTick_Handler(void) {
	systick_counter++;
}

void TIM2_Handler(void) {
	TIM2_SR &= ~(0b1); /* Clear UIF flag */
	main_loop_sync_received = true;
}

typedef struct { 
  volatile uint32_t MODER;
  volatile uint32_t OTYPER;
  volatile uint32_t OSPEEDR;
  volatile uint32_t PUPDR;
  volatile uint32_t IDR;
  volatile uint32_t ODR; 
  volatile uint32_t unused1; 
  volatile uint32_t unused2; 
  volatile uint32_t AFRL; 
} gpio_s;

int main(void) {

	/* After reset, the CPU clock frequency is 16MHz */

	RCC_AHB1ENR |= (0b1 << 0); // Enable periph clock for GPIOA port
	RCC_APB1ENR |= (0b1 << 0); // Enable periph clock for TIM2 port

	/* GPIO A5 & A6 as Output */
	GPIOA.MODER &= ~(0b1111 << 10); // Reset configuration mode for pin 5
	GPIOA.MODER |= (0b0101 << 10); // Set Output mode for pin 5

	/* GPIO A5 & A6 on Push-Pull */
	GPIOA.OTYPER &= ~(0b11 << 5);

	/* GPIO A5 & A6 on Low speed */
	GPIOA.OSPEEDR &= ~(0b1111 << 10);

	/* GPIO A5 & A6 on No pull-up/pull-down */
	GPIOA.PUPDR &= ~(0b1111 << 10);

	/* GPIO A5 & A6 set to 0 */
	GPIOA.ODR &= ~(0b11 << 5);

	/* GPIOA5 set to 1 */
	//GPIOA.ODR |= (0b1 << 5);

	/* Set prescaler on 16 bits */
	TIM2_PSC = 80 - 1; /* Ticks timer at 5 µs */

	/* Set auto-reload */
	TIM2_ARR = 2000 - 1; // 2000 * 5 µs => 10 ms

	//TIM2_SMCR = 0x0; Default value so not needed

    //Enable timer interrupt
    TIM2_DIER = 0x01;
    NVIC_ISER = 0x10000000; // Unmask IRQ 28 (TIM2)

	/* Set bit 2 (URS) and 0 (EN) at 1
	*  Todo MOUSS: Add register description
	*/
	TIM2_CR1 = 0x5;

    /* Init RX USART2
     * RX => PA3
     * TX => PA2
     */
    RCC_APB1ENR |= (0b1 << 17); // Enable periph clock for USART2 port

	/* GPIO A2 & A3 as alternate function */
	GPIOA.MODER &= ~(0b1111 << 4); // Reset configuration mode for pin PA2 & PA3
	GPIOA.MODER |= (0b1010 << 4);  // Set alternate function for PA2 and PA3

    /* Map alternate function on USART2 */
    GPIOA.AFRL &= ~(0xFF << 8);
    GPIOA.AFRL |= (0x77 << 8);

    USART2_CR1 = 0x200C;
    USART2_BRR = 0x8B; // 115 107.9

	while(1) {
		while (!main_loop_sync_received);
		main_loop_sync_received = false;
        GPIOA.ODR ^= (0b1 << 6);  // Toggle main loop heart beat
		main_loop();
	}

	return 0;
}

static uint16_t toggle_cycle_counter = 0;
#define TOGGLE_NB_OF_CYCLE 25
static char uart_char = 'a';

/* Main loop (100Hz) */
void main_loop(void) {
	if (toggle_cycle_counter < (TOGGLE_NB_OF_CYCLE - 1)) {
        toggle_cycle_counter++;
	} else {
        //GPIOA.ODR ^= (0b1 << 5);
        USART2_DR = uart_char;
        toggle_cycle_counter = 0;
	}
    if ((USART2_SR & 0x20) != 0x0) {
        uart_char = USART2_DR;
        push_character(uart_char);
    }
    encode_morse_message();
}

void set_morse_signal(bool is_on) {
    uint32_t data = GPIOA.ODR & ~(0b1 << 5);
    if (is_on)
        data |= (0b1 << 5);
    GPIOA.ODR = data;
}

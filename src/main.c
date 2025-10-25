#include <stdio.h>
#include "diag/Trace.h"
#include "cmsis/cmsis_device.h"
#include "myEXTI.h"
#include "tim2.h"
#include "globals.h"
#include "misc.h"
#include "adc.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

void myGPIOB_Init(void);
void myGPIOA_Init(void);

volatile int timerTriggered = 0;
volatile int frequency = 0;
volatile unsigned int resistance = 0;

int main(int argc, char *argv[])
{

	SystemClock48MHz();

	trace_printf("This is Part 2 of Introductory Lab...\n");
	trace_printf("System clock: %u Hz\n", SystemCoreClock);

	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN; /* Enable SYSCFG clock */

	myGPIOB_Init(); /* Initialize I/O port PB */
	myGPIOA_Init(); /* Initialize I/O port PA */
	// myTIM2_Init();	/* Initialize timer TIM2 */
	// myEXTI_Init();	/* Initialize EXTI */
	myADC_Init(); /* Initialize ADC */

	while (1)
	{
		myADC_StartConversion();
		trace_printf("Resistance ADC Value: %u\n", resistance);
	}

	return 0;
}

void myGPIOB_Init()
{
	/* Enable clock for GPIOB peripheral */
	// Relevant register: RCC->AHBENR
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	/* Configure PB2 as input */
	// Relevant register: GPIOB->MODER
	GPIOB->MODER &= ~(GPIO_MODER_MODER2);
	/* Ensure no pull-up/pull-down for PB2 */
	// Relevant register: GPIOB->PUPDR
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR0);
}

void myGPIOA_Init()
{
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;	  /* Enable clock for GPIOA peripheral */
	GPIOA->MODER |= 0b1100;				  /* Configure PA1 as analog input */
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR1); /* Ensure no pull-up/pull-down for PA1 */
}

#pragma GCC diagnostic pop
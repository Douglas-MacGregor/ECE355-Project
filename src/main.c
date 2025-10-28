#include <stdio.h>
#include "diag/Trace.h"
#include "cmsis/cmsis_device.h"
#include "myEXTI.h"
#include "myTIM2.h"
#include "globals.h"
#include "misc.h"
#include "myADC.h"
#include "myDAC.h"
#include "mySPI.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

void myGPIO_Init(void);

volatile int timerTriggered = 0;
volatile int frequency = 0;
volatile unsigned int resistance = 0;
volatile int fivefivefivefive = 0;

int main(int argc, char *argv[])
{

	SystemClock48MHz();
	trace_printf("This is Part 2 of Introductory Lab...\n");
	trace_printf("System clock: %u Hz\n", SystemCoreClock);
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;
	myGPIO_Init();
	mySPI_Init();
	myTIM3_Init();
	oled_Config(); // sends init commands to OLED
	trace_printf("OLED initialized.\n");
	myTIM2_Init(); /* Initialize timer TIM2 */
	myEXTI_Init(); /* Initialize EXTI */
	myADC_Init();  /* Initialize ADC */
	myDAC_Init();  /* Initialize DAC */

	while (1)
	{
		myADC_StartConversion();
		trace_printf("Resistance ADC Value: %u\n", resistance);
		refresh_OLED();
		myDAC_SetValue(resistance);
	}
	return 0;
}

void myGPIO_Init(void)
{
	// clock bit enable
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOCEN;
	// PA modes
	GPIOA->MODER &= ~(GPIO_MODER_MODER0); // PA0 user push button (input)
	GPIOA->MODER |= (GPIO_MODER_MODER1);  // PA1 ADC input (analog)
	GPIOA->MODER |= (GPIO_MODER_MODER4);  // PA4 DAC output (analog)
	// PB modes
	GPIOB->MODER &= ~(GPIO_MODER_MODER2); // PB2 function generator (input)
	GPIOB->MODER &= ~(GPIO_MODER_MODER3); // PB3 555 timer (input)
	GPIOB->MODER &= ~(3UL << (8 * 2));	  // PB8 CS#("Chip Select") (output)
	GPIOB->MODER |= (1UL << (8 * 2));
	GPIOB->MODER &= ~(3UL << (9 * 2)); // PB9 D/C#("Data/Command") (output)
	GPIOB->MODER |= (1UL << (9 * 2));
	GPIOB->MODER &= ~(3UL << (11 * 2)); // PB11 RES#("Reset") (output)
	GPIOB->MODER |= (1UL << (11 * 2));
	GPIOB->MODER &= ~(3UL << (13 * 2)); // PB13 SCLK("Serial Clock") output (AF)
	GPIOB->MODER |= (2UL << (13 * 2));
	GPIOB->MODER &= ~(3UL << (15 * 2)); // PB15 SDIN("Serial Data") output (AF)
	GPIOB->MODER |= (2UL << (15 * 2));
	// PC modes
	GPIOC->MODER &= ~(3UL << (8 * 2)); // PC8 blue LED (output)
	GPIOC->MODER |= (1UL << (8 * 2));
	GPIOC->MODER &= ~(3UL << (9 * 2)); // PC9 green LED (output)
	GPIOC->MODER |= (1UL << (9 * 2));
	// alternate function setup
	GPIOB->AFR[1] &= ~(0xF << (4 * (13 - 8))); // PB13 SPI_SCK
	GPIOB->AFR[1] |= (0x0 << (4 * (13 - 8)));
	GPIOB->AFR[1] &= ~(0xF << (4 * (15 - 8))); // SPI_MOSI
	GPIOB->AFR[1] |= (0x0 << (4 * (15 - 8)));
}

#pragma GCC diagnostic pop
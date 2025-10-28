
#include "myDAC.h"
#include "globals.h"
#include <stdio.h>
#include "diag/Trace.h"
#include "cmsis/cmsis_device.h"

void myDAC_Init(void)
{
    /* Enable clock for DAC peripheral */
    // Relevant register: RCC->APB1ENR
    RCC->APB1ENR |= RCC_APB1ENR_DACEN;
    /* Configure DAC: output buffer enabled by default */
    // Relevant register: DAC->CR (BOFF1 = 0 means buffer enabled)
    DAC->CR &= ~(0b111);
    DAC->CR |= 0b001;
}

void myDAC_SetValue(unsigned int value)
{
    /* Set the 12-bit right-aligned data for DAC channel 1 */
    // Relevant register: DAC->DHR12R1
    DAC->DHR12R1 = value & 0x0FFF; // Ensure only lower 12 bits are used
}
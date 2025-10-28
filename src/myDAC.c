
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
    /* Configure DAC: enable output buffer */
    // Relevant register: DAC->CR
    DAC->CR |= DAC_CR_BOFF1;
    /* Enable DAC channel 1 */
    // Relevant register: DAC->CR
    DAC->CR |= DAC_CR_EN1;
}

void myDAC_SetValue(uint16_t value)
{
    /* Set the 12-bit right-aligned data for DAC channel 1 */
    // Relevant register: DAC->DHR12R1
    DAC->DHR12R1 = value & 0x0FFF; // Ensure only lower 12 bits are used
}
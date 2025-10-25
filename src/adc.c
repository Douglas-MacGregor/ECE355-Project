#include "adc.h"
#include "globals.h"
#include <stdio.h>
#include "diag/Trace.h"
#include "cmsis/cmsis_device.h"

void myADC_Init(void)
{
    /* Enable clock for ADC1 peripheral */
    // Relevant register: RCC->APB2ENR
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    /* Configure ADC1: 12-bit resolution, right data alignment, single conversion mode */
    ADC1->CFGR1 |= (0b11 << 12);
    ADC1->CFGR1 &= ~(0b111000);
    ADC1->CHSELR |= 0b1;
    ADC1->SMPR |= 0b111;
    ADC1->CR |= 0b1;
    while ((ADC1->ISR & 0b1) == 0)
        ;
}

void myADC_StartConversion(void)
{
    ADC1->CR |= ADC_CR_ADSTART;
    while ((ADC1->ISR & ADC_ISR_EOC) == 0)
        ;
    resistance = ADC1->DR;
}

#include <stdio.h>
#include "diag/Trace.h"
#include "cmsis/cmsis_device.h"
#include "globals.h"

void SystemClock48MHz(void)
{
    //
    // Disable the PLL
    //
    RCC->CR &= ~(RCC_CR_PLLON);
    //
    // Wait for the PLL to unlock
    //
    while ((RCC->CR & RCC_CR_PLLRDY) != 0)
        ;
    //
    // Configure the PLL for 48-MHz system clock
    //
    RCC->CFGR = 0x00280000;
    //
    // Enable the PLL
    //
    RCC->CR |= RCC_CR_PLLON;
    //
    // Wait for the PLL to lock
    //
    while ((RCC->CR & RCC_CR_PLLRDY) != RCC_CR_PLLRDY)
        ;
    //
    // Switch the processor to the PLL clock source
    //
    RCC->CFGR = (RCC->CFGR & (~RCC_CFGR_SW_Msk)) | RCC_CFGR_SW_PLL;
    //
    // Update the system with the new clock frequency
    //
    SystemCoreClockUpdate();
}
#include <stdio.h>
#include "diag/Trace.h"
#include "cmsis/cmsis_device.h"

void myEXTI_Init()
{
    /* Map EXTI2 line to PB2 */
    // Relevant register: SYSCFG->EXTICR[0]
    SYSCFG->EXTICR[0] &= SYSCFG_EXTICR1_EXTI2;
    SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI2_PB;
    /* EXTI2 line interrupts: set rising-edge trigger */
    // Relevant register: EXTI->RTSR
    EXTI->RTSR |= EXTI_RTSR_TR2;
    /* Unmask interrupts from EXTI2 line */
    // Relevant register: EXTI->IMR
    EXTI->IMR |= EXTI_IMR_MR2;
    /* Assign EXTI2 interrupt priority = 0 in NVIC */
    // Relevant register: NVIC->IP[2], or use NVIC_SetPriority
    NVIC_SetPriority(EXTI2_3_IRQn, 0);
    /* Enable EXTI2 interrupts in NVIC */
    // Relevant register: NVIC->ISER[0], or use NVIC_EnableIRQ
    NVIC_EnableIRQ(EXTI2_3_IRQn);
}

void EXTI2_3_IRQHandler()
{
    if ((EXTI->PR & EXTI_PR_PR2) != 0) // rising edge on PB2
    {
        if (timerTriggered == 0) // first edge detected
        {
            TIM2->CNT = 0;            // reset counter
            overFlow = 0;             // reset overflow count
            TIM2->CR1 |= TIM_CR1_CEN; // start timer
            timerTriggered = 1;       // mark timer running
        }
        else // second edge detected
        {
            TIM2->CR1 &= ~(TIM_CR1_CEN); // stop timer
            // Calculate and print period and frequency in seconds, microseconds, and hertz
            uint64_t cycles = TIM2->CNT + ((uint64_t)overFlow * (uint64_t)(myTIM2_PERIOD));
            // Convert to microseconds
            uint32_t period_us = (uint32_t)((cycles * 1000000ULL) / SystemCoreClock); // µs
            uint32_t freq_hz = (cycles ? (uint32_t)((SystemCoreClock + (cycles / 2)) / cycles) : 0);
            // period into whole seconds
            uint32_t period_s_whole = period_us / 1000000U; // integer seconds
            uint32_t period_s_frac = period_us % 1000000U;  // leftover microseconds
            trace_printf("--------------------------------------------------\n");
            trace_printf("Total timer cycles: %u\n", (unsigned int)cycles);
            // Show both seconds and microseconds
            trace_printf("Period: %u.%06u s  (%u µs total)\n",
                         period_s_whole, period_s_frac, period_us);
            // Show frequency in Hertz
            trace_printf("Frequency: %u Hz\n", freq_hz);
            trace_printf("--------------------------------------------------\n");
            timerTriggered = 0; // ready for next
        }
        // 1. If this is the first edge:
        //	- Clear count register (TIM2->CNT).
        //	- Start timer (TIM2->CR1).
        //    Else (this is the second edge):
        //	- Stop timer (TIM2->CR1).
        //	- Read out count register (TIM2->CNT).
        //	- Calculate signal period and frequency.
        //	- Print calculated values to the console.
        //	  NOTE: Function trace_printf does not work
        //	  with floating-point numbers: you must use
        //	  "unsigned int" type to print your signal
        //	  period and frequency.
        //
        // 2. Clear EXTI2 interrupt pending flag (EXTI->PR).
        // NOTE: A pending register (PR) bit is cleared
        // by writing 1 to it.
        EXTI->PR |= EXTI_PR_PR2; // clear EXTI2 pending flag
    }
}

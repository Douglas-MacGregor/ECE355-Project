#include <stdint.h>

#define myTIM2_PRESCALER ((uint16_t)0x0000)
/* Maximum possible setting for overflow */
#define myTIM2_PERIOD ((uint32_t)0xFFFFFFFF)

extern volatile int timerTriggered;
extern volatile int frequency;
extern volatile unsigned int resistance;
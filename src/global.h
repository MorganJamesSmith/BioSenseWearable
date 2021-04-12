#ifndef global_h
#define global_h

#include <stdint.h>

//#include "nrfx_rtc.h"

//extern const nrfx_rtc_t rtc;

/**
 *  Number of milliseconds elapsed since system was reset
 */
extern volatile uint32_t millis;
#define MS_TO_MILLIS(x) (x)

//#define millis (nrfx_rtc_counter_get(&rtc))
//#define MS_TO_MILLIS(x) (((x * 1024) + 500) / 1000)

#endif /* global_h */

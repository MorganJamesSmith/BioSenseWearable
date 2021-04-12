#include "profiling.h"

#include "config.h"

static NRF_TIMER_Type *profile_timer;

#ifdef ENABLE_PROFILING
uint16_t profiler_count = 0;
uint32_t profiler_samples[PROFILER_NUM_SAMPLES];

static uint8_t ignore_first;
#endif


void init_profiling(NRF_TIMER_Type *timer, int ignore_first_stop)
{
    profile_timer = timer;
#ifdef ENABLE_PROFILING
    ignore_first = !!ignore_first_stop;
#endif

    // Shutdown timer in case it is running
    profile_timer->TASKS_SHUTDOWN = 1;
    // Set timer to timer mode
    profile_timer->MODE = 0;
    // Configure timer to be 32 bits wide
    profile_timer->BITMODE = 3;
    // Configure prescaller to run timer at 8 MHz
    profile_timer->PRESCALER = 1;
    // Disable all interupts
    profile_timer->INTENCLR = 0xFFFFFFFF;
    // Enable shortcuts so that when compare 0 is trigged the timer will be
    // stopped and cleared
    //profile_timer->SHORTS = (1 << 8) | (1 << 0);
    // NOTE: SHORTS doesn't seem to work, it is not well documented so maybe I
    //       am using it wrong.
    // Start the timer, then stop it and clear it to zero the timer out
    profile_timer->TASKS_START = 1;
    profile_timer->TASKS_STOP = 1;
    profile_timer->TASKS_CLEAR = 1;
}

void profiling_start_measurement(void)
{
    // Start timer
    profile_timer->TASKS_START = 1;
}

void profiling_stop_measurement(void)
{
    // Store timer value, clear timer and stop timer
    profile_timer->TASKS_CAPTURE[0] = 1;
    profile_timer->TASKS_STOP = 1;
    profile_timer->TASKS_CLEAR = 1;
#ifdef ENABLE_PROFILING
    if (ignore_first) {
        ignore_first = 0;
        return;
    }
    // Store timer value in array if we have space left
    if (profiler_count < PROFILER_NUM_SAMPLES) {
        profiler_samples[profiler_count] = profile_timer->CC[0];
        profiler_count++;
    }
#endif
}

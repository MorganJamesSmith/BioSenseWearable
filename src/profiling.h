#ifndef profiling_h
#define profiling_h

#include <nrfx.h>

#ifdef ENABLE_PROFILING
extern uint16_t profiler_count;
extern uint32_t profiler_samples[PROFILER_NUM_SAMPLES];
#endif

extern void init_profiling(NRF_TIMER_Type *timer, int ignore_first_stop);

extern void profiling_start_measurement(void);

extern void profiling_stop_measurement(void);

#endif /* profiling_h */

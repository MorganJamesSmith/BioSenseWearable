#ifndef icm_20948_states_h
#define icm_20948_states_h

#include "icm_20948.h"

typedef int (*icm_20948_state_handler_t)(struct icm_20948_desc *inst);

/**
 *  Array of functions for handling FSM states.
 *
 *  Each state handler returns 0 if the service function should return or 1 if
 *  the service function should call the handler for the next state immediately.
 */
extern const icm_20948_state_handler_t icm_20948_state_handlers[];

#endif /* icm_20948_states_h */

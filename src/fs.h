#ifndef fs_h
#define fs_h

#include <stdint.h>
#include <ff.h>

#include "data_logger.h"

extern FATFS fs;

extern int filesystem_init_status;

extern uint32_t sd_card_capacity;

extern struct data_logger_descriptor data_logger;

extern int log_init_status;

#endif /* fs_h */

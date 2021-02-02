#ifndef cli_commands_h
#define cli_commands_h

#include "cli.h"

extern const struct cli_func_desc_t debug_commands_funcs[];

// MARK: Debug CLI helper functions

extern void debug_print_fixed_point (const struct cli_io_funcs_t *console,
                                     int32_t value, uint8_t decimal_places);

extern void debug_print_byte_with_pad (const struct cli_io_funcs_t *console,
                                       const char *line_start, uint8_t byte,
                                       const char *line_end);

#endif /* cli_commands_h */

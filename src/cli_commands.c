#include "cli_commands.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

// MARK: Helpers
void debug_print_fixed_point (const struct cli_io_funcs_t *console,
                              int32_t value, uint8_t decimal_places)
{
    int32_t scale = pow(10, decimal_places);

    char str[10];
    int32_t whole = value / scale;

    if ((whole == 0) && (value < 0)) {
        console->write_string("-0.");
    } else {
        itoa(whole, str, 10);
        console->write_string(str);
        console->write_string(".");
    }

    int32_t frac = abs(value - (whole * scale));
    itoa(frac, str, 10);
    for (int i = strlen(str); i < decimal_places; i++) {
        console->write_string("0");
    }
    console->write_string(str);
}

void debug_print_byte_with_pad (const struct cli_io_funcs_t *console,
                                const char *line_start, uint8_t byte,
                                const char *line_end)
{
    char str[9];

    console->write_string(line_start);

    utoa(byte, str, 2);
    for (uint8_t i = strlen(str); i < 8; i++) {
        console->write_string("0");
    }
    console->write_string(str);

    console->write_string(line_end);
}

// MARK: Test commands

#define DEBUG_ECHO_NAME "echo"
#define DEBUG_ECHO_HELP "Writes arguments back to serial interface."

static void debug_echo(uint8_t argc, char **argv,
                       const struct cli_io_funcs_t *console)
{
    for (uint8_t i = 1; i < argc; i++) {
        console->write_string(argv[i]);
        console->write_string(" ");
    }
    console->write_string("\n");
}

// MARK: Commands table

const struct cli_func_desc_t debug_commands_funcs[] = {
    // General
    {.func = debug_echo, .name = DEBUG_ECHO_NAME,
        .help_string = DEBUG_ECHO_HELP},
    {.func = NULL, .name = NULL, .help_string = NULL}
};

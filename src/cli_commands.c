#include "cli_commands.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "global.h"
#include "adc.h"

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

#define DEBUG_ANALOG_NAME   "analog"
#define DEBUG_ANALOG_HELP   "Print values of analog inputs."

static void debug_analog_print_channel (const struct cli_io_funcs_t *console,
                                        int channel, int32_t voltage,
                                        int16_t parsed_value, uint8_t decimals,
                                        const char *name, const char *unit)
{
    char str[10];

    if (*name == '\0') {
        itoa(channel, str, 10);
        for (uint8_t i = strlen(str); i < 16; i++) {
            console->write_string(" ");
        }
        console->write_string(str);
    } else {
        for (uint8_t i = strlen(name); i < 16; i++) {
            console->write_string(" ");
        }
        console->write_string(name);
    }
    console->write_string(": ");

    debug_print_fixed_point(console, voltage, 3);
    console->write_string(" V");

    if (*unit != '\0') {
        console->write_string(" (");

        debug_print_fixed_point(console, parsed_value, decimals);

        console->write_string(" ");
        console->write_string(unit);
        console->write_string(")\n");
    } else {
        console->write_string("\n");
    }
}

static void debug_analog (uint8_t argc, char **argv,
                          const struct cli_io_funcs_t *console)
{
    char str[16];

    console->write_string("Last sweep was at ");
    utoa(adc_get_last_measurment_time(), str, 10);
    console->write_string(str);
    console->write_string(" (");
    utoa(millis - adc_get_last_measurment_time(), str, 10);
    console->write_string(str);
    console->write_string(" milliseconds ago)\n");

    // Channel 0: AIN 0 - IR Temp
    debug_analog_print_channel(console, 0, adc_get_channel_0_volts(),
                               adc_most_recent_ir_temp(), 2, "IR Temp", "°C");

    // Channel 1: AIN 1 - Ambient Temp
    debug_analog_print_channel(console, 1, adc_get_channel_1_volts(),
                               adc_most_recent_ambient_temp(), 2,
                               "Ambient Temp", "°C");

    // Channel 2: AIN 2 - Battery Voltage
    debug_analog_print_channel(console, 2, adc_get_channel_2_volts(),
                               adc_most_recent_battery_voltage(), 3,
                               "Battery Voltage", "V");

    // Channel 3: AIN 3 - Supply Voltage
    debug_analog_print_channel(console, 3, adc_get_channel_3_volts(),
                               adc_most_recent_supply_voltage(), 3,
                               "Supply Voltage", "V");
}

// MARK: Commands table

const struct cli_func_desc_t debug_commands_funcs[] = {
    // General
    {.func = debug_echo, .name = DEBUG_ECHO_NAME,
        .help_string = DEBUG_ECHO_HELP},
    {.func = debug_analog, .name = DEBUG_ANALOG_NAME,
        .help_string = DEBUG_ANALOG_HELP},
    {.func = NULL, .name = NULL, .help_string = NULL}
};

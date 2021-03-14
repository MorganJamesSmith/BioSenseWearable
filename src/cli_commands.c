#include "cli_commands.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <ff.h>

#include "global.h"
#include "fs.h"
#include "data_logger.h"
#include "sensors.h"
#include "adc.h"
#include "icm_20948.h"

// MARK: Helpers
void debug_print_fixed_point (const struct cli_io_funcs_t *console,
                              int32_t value, uint8_t decimal_places)
{
    int32_t scale = pow(10, decimal_places);

    char str[10];
    int32_t whole = value / scale;

    if ((whole == 0) && (value < 0)) {
        console->write_string_blocking("-0.");
    } else {
        itoa(whole, str, 10);
        console->write_string_blocking(str);
        console->write_string_blocking(".");
    }

    int32_t frac = abs(value - (whole * scale));
    itoa(frac, str, 10);
    for (int i = strlen(str); i < decimal_places; i++) {
        console->write_string_blocking("0");
    }
    console->write_string_blocking(str);
}

void debug_print_byte_with_pad (const struct cli_io_funcs_t *console,
                                const char *line_start, uint8_t byte,
                                const char *line_end)
{
    char str[9];

    console->write_string_blocking(line_start);

    utoa(byte, str, 2);
    for (uint8_t i = strlen(str); i < 8; i++) {
        console->write_string_blocking("0");
    }
    console->write_string_blocking(str);

    console->write_string_blocking(line_end);
}

// MARK: Test commands

#define DEBUG_ECHO_NAME "echo"
#define DEBUG_ECHO_HELP "Writes arguments back to serial interface."

static void debug_echo(uint8_t argc, char **argv,
                       const struct cli_io_funcs_t *console)
{
    for (uint8_t i = 1; i < argc; i++) {
        console->write_string_blocking(argv[i]);
        console->write_string_blocking(" ");
    }
    console->write_string_blocking("\n");
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
            console->write_string_blocking(" ");
        }
        console->write_string_blocking(str);
    } else {
        for (uint8_t i = strlen(name); i < 16; i++) {
            console->write_string_blocking(" ");
        }
        console->write_string_blocking(name);
    }
    console->write_string_blocking(": ");

    debug_print_fixed_point(console, voltage, 3);
    console->write_string_blocking(" V");

    if (*unit != '\0') {
        console->write_string_blocking(" (");

        debug_print_fixed_point(console, parsed_value, decimals);

        console->write_string_blocking(" ");
        console->write_string_blocking(unit);
        console->write_string_blocking(")\n");
    } else {
        console->write_string_blocking("\n");
    }
}

static void debug_analog (uint8_t argc, char **argv,
                          const struct cli_io_funcs_t *console)
{
    char str[16];

    console->write_string_blocking("Last sweep was at ");
    utoa(adc_get_last_measurment_time(), str, 10);
    console->write_string_blocking(str);
    console->write_string_blocking(" (");
    utoa(millis - adc_get_last_measurment_time(), str, 10);
    console->write_string_blocking(str);
    console->write_string_blocking(" milliseconds ago)\n");

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

#define DEBUG_IMU_NAME   "imu"
#define DEBUG_IMU_HELP   "Print most recent measurments from IMU."

static void debug_imu (uint8_t argc, char **argv,
                       const struct cli_io_funcs_t *console)
{
    char str[16];

    // Print Who Am I value
    console->write_string_blocking("WAI: 0x");
    utoa(imu.wai, str, 16);
    if (imu.wai < 0x1) {
        console->write_string_blocking("0");
    }
    console->write_string_blocking(str);

    // Print when we last got data from the sensor
    console->write_string_blocking("\nLast sweep was at ");
    utoa(icm_20948_get_last_measurment_time(&imu), str, 10);
    console->write_string_blocking(str);
    console->write_string_blocking(" (");
    utoa(millis - icm_20948_get_last_measurment_time(&imu), str, 10);
    console->write_string_blocking(str);
    console->write_string_blocking(" milliseconds ago)\n");

    // Print accelerometer data
    console->write_string_blocking("Accerometer:\n\tX: ");
    debug_print_fixed_point(console, icm_20948_get_last_accel_x(&imu), 4);
    console->write_string_blocking(" g\n\tY: ");
    debug_print_fixed_point(console, icm_20948_get_last_accel_y(&imu), 4);
    console->write_string_blocking(" g\n\tZ: ");
    debug_print_fixed_point(console, icm_20948_get_last_accel_z(&imu), 4);

    // Print gyroscope data
    console->write_string_blocking(" g\nGyroscope:\n\tX: ");
    debug_print_fixed_point(console, icm_20948_get_last_gyro_x(&imu), 3);
    console->write_string_blocking(" °/s\n\tY: ");
    debug_print_fixed_point(console, icm_20948_get_last_gyro_y(&imu), 3);
    console->write_string_blocking(" °/s\n\tZ: ");
    debug_print_fixed_point(console, icm_20948_get_last_gyro_z(&imu), 3);

    // Print temperature
    console->write_string_blocking(" °/s\nTemperature: ");
    debug_print_fixed_point(console, icm_20948_get_last_temp(&imu), 3);
    console->write_string_blocking(" °C\n");
}

#define DEBUG_LS_NAME "ls"
#define DEBUG_LS_HELP "Lists the files on the SD card.\nUsage: ls <dir>\n"

static void debug_ls(uint8_t argc, char **argv,
                     const struct cli_io_funcs_t *console)
{
    char str[16];

    if (filesystem_init_status != 0) {
        console->write_string_blocking("SD card is not initialized.\n");
        return;
    }

    FRESULT ff_result;

    DIR dir;
    FILINFO fno;

    const char *dir_name = "/";
    if (argc > 2) {
        console->write_string_blocking("Too many arguments.\n");
    } else if (argc == 2) {
        dir_name = argv[1];
    }

    // Open root dir
    ff_result = f_opendir(&dir, dir_name);

    if (ff_result != FR_OK) {
        console->write_string_blocking("Failed to open directory.\n");
        return;
    }

    // Iterate over files in directory
    for (;;) {
        ff_result = f_readdir(&dir, &fno);
        if (ff_result != FR_OK) {
            console->write_string_blocking("Failed to read file info.\n");
            return;
        }

        if (!fno.fname[0]) {
            break;
        }

        console->write_string_blocking(fno.fname);
        if (fno.fattrib & AM_DIR) {
            console->write_string_blocking(" (directory)\n");
        } else {
            console->write_string_blocking(" (");
            utoa(fno.fsize, str, 10);
            console->write_string_blocking(str);
            console->write_string_blocking(")\n");
        }
    }

    // Close directory
    ff_result = f_closedir(&dir);

    if (ff_result != FR_OK) {
        console->write_string_blocking("Failed to close directory\n");
        return;
    }
}

#define DEBUG_CAT_NAME "cat"
#define DEBUG_CAT_HELP "Print the contents of a file on the SD card.\nUsage: " \
                       "cat <file>"

static void debug_cat(uint8_t argc, char **argv,
                      const struct cli_io_funcs_t *console)
{
    if (filesystem_init_status != 0) {
        console->write_string_blocking("SD card is not initialized.\n");
        return;
    }

    FRESULT ff_result;

    FIL file;

    if (argc > 2) {
        console->write_string_blocking("Too many arguments.\n");
    } else if (argc < 2) {
        console->write_string_blocking("No file specified.\n");
    }

    // Open file
    ff_result = f_open(&file, argv[1], FA_READ);

    if (ff_result != FR_OK) {
        console->write_string_blocking("Failed to open file.\n");
        return;
    }

    char buffer;
    unsigned bytes_read;

    for(;;) {
        ff_result = f_read(&file, &buffer, 1, &bytes_read);

        if (ff_result != FR_OK) {
            console->write_string_blocking("Failed to read from file.\n");
            return;
        }

        if (bytes_read != 0) {
            console->write_bytes_blocking((uint8_t*)&buffer, 1);
        } else {
            break;
        }
    }


    // Close file
    ff_result = f_close(&file);

    if (ff_result != FR_OK) {
        console->write_string_blocking("Failed to close file\n");
        return;
    }
}

#define DEBUG_HCAT_NAME "hcat"
#define DEBUG_HCAT_HELP "Print the contents of a file on the SD card in hex. " \
                        "\nUsage: hcat [file] <offset> <num_bytes>"

static void debug_hcat(uint8_t argc, char **argv,
                       const struct cli_io_funcs_t *console)
{
    char str[16];

    unsigned offset = 0;
    unsigned num_bytes = 0;

    // Parse arguments
    if (argc > 4) {
        // We have to many arguments
        console->write_string_blocking("Too many arguments.\n");
        return;
    } else if (argc < 2) {
        console->write_string_blocking("No file specified.\n");
    }

    if (argc > 2) {
        // We have an offset to parse
        char *end;
        offset = (unsigned)strtoul(argv[2], &end, 0);
        if (*end != '\0') {
            console->write_string_blocking("Invalid offset.\n");
            return;
        }
    }

    if (argc > 3) {
        // We have a number of bytes to parse
        char *end;
        num_bytes = (unsigned)strtoul(argv[3], &end, 0);
        if (*end != '\0') {
            console->write_string_blocking("Invalid number of bytes.\n");
            return;
        }
    }


    if (filesystem_init_status != 0) {
        console->write_string_blocking("SD card is not initialized.\n");
        return;
    }

    FRESULT ff_result;

    FIL file;

    // Open file
    ff_result = f_open(&file, argv[1], FA_READ);

    if (ff_result != FR_OK) {
        console->write_string_blocking("Failed to open file.\n");
        return;
    }

    // Seek file
    ff_result = f_lseek(&file, offset);

    if (ff_result != FR_OK) {
        console->write_string_blocking("Failed to seek file.\n");
        return;
    }

    // Read file

    char buffer[32];
    unsigned total_bytes_read = 0;
    unsigned bytes_read;

    for (;;) {
        unsigned bytes_to_read = (num_bytes == 0) ? 32 :
                                 ((num_bytes - total_bytes_read >= 32)) ? 32 :
                                 (num_bytes - total_bytes_read);

        if (bytes_to_read == 0) {
            break;
        }

        ff_result = f_read(&file, buffer, bytes_to_read, &bytes_read);
        total_bytes_read += bytes_read;

        if (ff_result != FR_OK) {
            console->write_string_blocking("Failed to read from file.\n");
            return;
        }

        for (unsigned i = 0; i < bytes_read; i++) {
            if (buffer[i] < 16) {
                console->write_string_blocking("0");
            }

            utoa(buffer[i], str, 16);
            console->write_string_blocking(str);
        }

        if (bytes_read != 0) {
            console->write_string_blocking("\n");
        } else {
            break;
        }
    }


    // Close file
    ff_result = f_close(&file);

    if (ff_result != FR_OK) {
        console->write_string_blocking("Failed to close file\n");
        return;
    }
}

#define DEBUG_LOG_NAME  "log"
#define DEBUG_LOG_HELP  "Logs a message to the log file.\nUsage: log <message>"

static void debug_log (uint8_t argc, char **argv,
                       const struct cli_io_funcs_t *console)
{
    if (log_init_status != 0) {
        console->write_string_blocking("Data logger not initialized (");
        char str[8];
        itoa(log_init_status, str, 10);
        console->write_string_blocking(str);
        console->write_string_blocking(")\n");
    }

    if (argc != 2) {
        console->write_string_blocking("Must provide exactly one argument.\n");
        return;
    }

    int ret = data_logger_log(&data_logger, millis, DATA_ENTRY_LOG_MSG,
                              (uint8_t*)argv[1], strlen(argv[1]));

    if (ret != 0) {
        console->write_string_blocking("Failed to log message (");
        char str[8];
        utoa(ret, str, 10);
        console->write_string_blocking(str);
        console->write_string_blocking(")\n");
    }
}

#define DEBUG_TIME_NAME  "time"
#define DEBUG_TIME_HELP  "Record current time.\nUsage: time <unix time>"

static void debug_time (uint8_t argc, char **argv,
                        const struct cli_io_funcs_t *console)
{
    if (log_init_status != 0) {
        console->write_string_blocking("Data logger not initialized (");
        char str[8];
        itoa(log_init_status, str, 10);
        console->write_string_blocking(str);
        console->write_string_blocking(")\n");
    }

    if (argc != 2) {
        console->write_string_blocking("Must provide exactly one argument.\n");
        return;
    }

    char *end;
    struct time_data_entry t;
    t.unix_time = (uint64_t)strtoull(argv[1], &end, 0);
    if (*end != '\0') {
        console->write_string_blocking("Invalid time.\n");
        return;
    }

    int ret = data_logger_log(&data_logger, millis, DATA_ENTRY_TIME,
                        (uint8_t*)&t, sizeof(t));

    if (ret != 0) {
        console->write_string_blocking("Failed to log time (");
        char str[8];
        utoa(ret, str, 10);
        console->write_string_blocking(str);
        console->write_string_blocking(")\n");
    }
}

// MARK: Commands table

const struct cli_func_desc_t debug_commands_funcs[] = {
    // General
    {.func = debug_echo, .name = DEBUG_ECHO_NAME,
        .help_string = DEBUG_ECHO_HELP},
    {.func = debug_analog, .name = DEBUG_ANALOG_NAME,
        .help_string = DEBUG_ANALOG_HELP},
    {.func = debug_imu, .name = DEBUG_IMU_NAME, .help_string = DEBUG_IMU_HELP},
    {.func = debug_ls, .name = DEBUG_LS_NAME, .help_string = DEBUG_LS_HELP},
    {.func = debug_cat, .name = DEBUG_CAT_NAME, .help_string = DEBUG_CAT_HELP},
    {.func = debug_hcat, .name = DEBUG_HCAT_NAME,
        .help_string = DEBUG_HCAT_HELP},
    {.func = debug_log, .name = DEBUG_LOG_NAME, .help_string = DEBUG_LOG_HELP},
    {.func = debug_time, .name = DEBUG_TIME_NAME,
        .help_string = DEBUG_TIME_HELP},
    {.func = NULL, .name = NULL, .help_string = NULL}
};

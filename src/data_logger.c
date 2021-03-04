#include "data_logger.h"

#include <string.h>
#include <stdlib.h>

#include "global.h"


#define LOG_FILE_NAME   "p21"
#define RESET_OFFSETS_FILE_NAME "p21_off"


struct data_entry {
    uint32_t timestamp;
    uint16_t type;
    uint16_t length;
    uint8_t payload[];
};

static int write_reset_offset(void)
{
    FRESULT ff_result;

    // Get size of data file
    FILINFO info;

    uint32_t data_file_size = 0;
    ff_result = f_stat(LOG_FILE_NAME, &info);

    if (ff_result == FR_OK) {
        data_file_size = info.fsize;
    }

    // Open reset offsets file
    FIL offsets_file;
    ff_result = f_open(&offsets_file, RESET_OFFSETS_FILE_NAME,
                       (FA_WRITE | FA_OPEN_APPEND));

    if (ff_result != FR_OK) {
        return ff_result;
    }

    // Write data file size to reset offsets file
    unsigned bytes_written = 0;
    while (bytes_written < 4) {
        unsigned bw;
        ff_result = f_write(&offsets_file,
                            ((const uint8_t*)&data_file_size) + bytes_written,
                            4 - bytes_written, &bw);

        if (ff_result != FR_OK) {
            return -1;
        }

        bytes_written += bw;
    }

    // Close file
    ff_result = f_close(&offsets_file);

    if (ff_result != FR_OK) {
        return -1;
    }

    return 0;
}

int init_data_logger(struct data_logger_descriptor *inst)
{
    FRESULT ff_result;

    // Write the current offset in the data file to the reset offsets file
    int ret = write_reset_offset();
    if (ret != 0) {
        return ret;
    }

    // Open data file
    ff_result = f_open(&inst->file, LOG_FILE_NAME, (FA_WRITE | FA_OPEN_APPEND));

    if (ff_result != FR_OK) {
        return -1;
    }

    // Write a entry to the data file
    data_logger_log(inst, millis, DATA_ENTRY_RESET, NULL, 0);

    return 0;
}

int data_logger_log(struct data_logger_descriptor *inst, uint32_t timestamp,
                    enum data_entry_type type, const uint8_t *data,
                    uint32_t length)
{
    const uint32_t entry_length = sizeof(struct data_entry) + length;
    // This would be much cleaner with alloca, but SES's libc doesn't seem to have it
    uint8_t p[entry_length];
    struct data_entry *packet = (struct data_entry *)(&p);
    packet->timestamp = timestamp;
    packet->type = type;
    packet->length = length;
    if (length != 0) {
        memcpy(packet->payload, data, length);
    }

    FRESULT ff_result;
    unsigned bytes_written = 0;
    while (bytes_written < entry_length) {
        unsigned bw = 0;
        ff_result = f_write(&inst->file, ((void*)packet) + bytes_written,
                            entry_length - bytes_written, &bw);

        if (ff_result != FR_OK) {
            return ff_result;
        }

        bytes_written += bw;
    }

    // Need to sync the file to get bytes to actually be writen to card.
    // Ideally we wouldn't actually sync after every single log entry, but
    // we never know when we might reset so it's hard to know when we should
    // sync.
    ff_result = f_sync(&inst->file);
    if (ff_result != FR_OK) {
        return ff_result;
    }

    return 0;
}

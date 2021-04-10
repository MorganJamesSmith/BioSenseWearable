#ifndef data_logger_h
#define data_logger_h

#include <stdint.h>

#include <ff.h>

#define DL_BUFFER_LENGTH 512
#define DL_BUFFER_WATERLINE 500

struct data_logger_descriptor {
    FIL file;
    uint8_t buffer[2][DL_BUFFER_LENGTH];
    uint16_t buffer_used[2];
    uint16_t current_buffer;
    uint8_t buffer_full[2];
};

enum data_entry_type {
    DATA_ENTRY_RESET = 0x00000000,
    DATA_ENTRY_TIME = 0x00000001,
    DATA_ENTRY_VOLTAGES = 0x00000002,
    DATA_ENTRY_TEMPERATURES = 0x00000003,
    DATA_ENTRY_IMU = 0x00000004,
    DATA_ENTRY_LOG_MSG = 0x00000005,
    DATA_ENTRY_PPG = 0x0006,
    DATA_ENTRY_AIR_QUALITY = 0x0007,
    DATA_ENTRY_UV_LIGHT = 0x0008
};


extern int init_data_logger(struct data_logger_descriptor *inst);

extern void data_logger_service(struct data_logger_descriptor *inst);

extern int data_logger_log(struct data_logger_descriptor *inst,
                           uint32_t timestamp, enum data_entry_type type,
                           const uint8_t *data, uint32_t length);




struct imu_data_entry {
    int32_t accel_x; // 100 µg per LSB (0.0001g/LSB)
    int32_t accel_y;
    int32_t accel_z;
    int32_t gyro_x;  // 1 milladps per LSB (0.001 dps/LSB)
    int32_t gyro_y;
    int32_t gyro_z;
    int32_t temperature; // 1 milladegree Celsius per LSB (0.001 °C/LSB)
};

struct voltage_data_entry {
    int16_t supply_voltage;     // 1 mV per LSB
    int16_t battery_voltage;    // 1 mV per LSB
};

struct temperature_data_entry {
    int16_t ir_temp;        // 10 milladegrees Celsius per LSB (0.01 °C/LSB)
    int16_t ambient_temp;   // 10 milladegrees Celsius per LSB (0.01 °C/LSB)
};

struct time_data_entry {
    uint64_t unix_time;
};

#endif /* data_logger_h */

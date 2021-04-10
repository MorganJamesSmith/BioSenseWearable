#ifndef icm_20948_h
#define icm_20948_h

#include <stdint.h>

#include <nrfx_spim.h>
#include <nrf_gpio.h>

#include "data_logger.h"

#define ICM_20948_SPI_MISO_PIN      28
#define ICM_20948_SPI_MOSI_PIN      29
#define ICM_20948_SPI_SCK_PIN       30
#define ICM_20948_SPI_SS_PIN        31

#define ICM_20948_INT_PORT          NRF_P0
#define ICM_20948_INT_PIN           27



enum icm_20948_state {
    // Bank selection
    ICM_20948_SELECT_BANK,
    // Sensor Configuration
    ICM_20948_CONFIG_GYRO,
    ICM_20948_CONFIG_ACCEL,
    ICM_20948_CONFIG_TEMP,
    // Initialization Sequence
    ICM_20948_RESET,                // Reset the part
    ICM_20948_READ_WAI,             // Read Who Am I register (for debugging)
    ICM_20948_DISABLE_I2C,          // Disable I2C slave interface
    ICM_20948_WAKE,                 // Wake from sleep mode and configure osc
    ICM_20948_FIFO_RESET_SET,       // Assert the FIFO reset bits
    ICM_20948_FIFO_RESET_CLEAR,     // Deassert the FIFI reset bits
    ICM_20948_CONFIGURE_FIFO,
    ICM_20948_WRITE_FIFO_CFG,
    // We switch to bank 2 at this point in in initalization
    ICM_20948_ENABLE_ODR_ALIGN,
    ICM_20948_CONFIG_GYRO_NORM_OP,
    // We jump back to ICM_20948_CONFIG_GYRO here
    ICM_20948_CONFIG_ACCEL_NORM_OP,
    // We jump back to ICM_20948_CONFIG_ACCEL here
    ICM_20948_CONFIG_TEMP_NORM_OP,
    // We jump back to ICM_20948_CONFIG_TEMP here
    ICM_20948_INIT_SELECT_BANK_0,   // Dummy state to jump back to bank select
    // We switch to bank 0 at this point in in initalization
    ICM_20948_CONFIG_INTERUPTS,     // Write to interupt configuration registers
    ICM_20948_ENABLE_FIFO,
    ICM_20948_READ_FIFO_CONF,
    ICM_20948_IDLE,
    ICM_20948_READ_FIFO_COUNT,      // Find out how much data is in FIFO
    ICM_20948_READ_FIFO,            // Read data from FIFO
    ICM_20948_READ_DATA             // Read data from SRAM
};



struct icm_20948_desc {
    const nrfx_spim_t *spi;
    NRF_GPIO_Type *int_port;

    struct data_logger_descriptor *logger;

    uint32_t last_sample_time;

    // Field used for passing data between ajacent states
    union {
        uint32_t time;
        struct {
            uint8_t bank;
            enum icm_20948_state next_state:8;
        } bank_sel_info;
        struct {
            uint8_t sample_rate_div;
            uint16_t lpf_config:3;
            uint16_t full_scale_range:2;
            uint16_t lpf_enable:1;
            uint16_t x_self_test:1;
            uint16_t y_self_test:1;
            uint16_t z_self_test:1;
            uint16_t oversampling:3;
            enum icm_20948_state next_state:8;
        } gyro_config_info;
        struct {
            uint32_t sample_rate_div:12;
            uint32_t lpf_config:3;
            uint32_t full_scale_range:2;
            uint32_t lpf_enable:1;
            uint32_t x_self_test:1;
            uint32_t y_self_test:1;
            uint32_t z_self_test:1;
            uint32_t oversampling:2;
            enum icm_20948_state next_state:8;
        } accel_config_info;
        struct {
            uint32_t lpf_config:3;
            enum icm_20948_state next_state:8;
        } temp_config_info;
    };

    uint16_t fifo_count;
    uint16_t last_accel_x;
    uint16_t last_accel_y;
    uint16_t last_accel_z;
    uint16_t last_gyro_x;
    uint16_t last_gyro_y;
    uint16_t last_gyro_z;
    uint16_t last_temp;

    uint8_t spi_buffer[513];

    uint8_t wai;

    uint8_t int_pin:5;
    uint8_t spi_output_buf_ready:1;
    uint8_t spi_in_progress:1;
    uint8_t spi_done:1;

    enum icm_20948_state state:5;
};


extern void init_icm_20948(struct icm_20948_desc *inst, const nrfx_spim_t *spi,
                           NRF_GPIO_Type *int_port, uint8_t int_pin,
                           struct data_logger_descriptor *logger);

extern void icm_20948_service(struct icm_20948_desc *inst);

extern void icm_20948_spim_event_handler(const nrfx_spim_evt_t *event,
                                         void *context);


extern uint32_t icm_20948_get_last_measurment_time(struct icm_20948_desc *inst);

/**
 *  Convert a raw sensor acceleration value into useful units
 *
 *  @param value The raw sensor value to be converted
 *
 *  @param return The acceleration in units of 100 µg per LSB (0.0001g/LSB)
 */
extern int32_t icm_20948_parse_accel_value(int16_t value);

/**
 *  Convert a raw sensor roational velocity value into useful units
 *
 *  @param value The raw sensor value to be converted
 *
 *  @param return The roational velocity in units of 1 milladps per LSB
 *                (0.001 dps/LSB)
 */
extern int32_t icm_20948_parse_gyro_value(int16_t value);

/**
 *  Convert a raw sensor temperature value into useful units
 *
 *  @param value The raw sensor value to be converted
 *
 *  @param return The temperature in units of 1 milladegree Celsius per LSB
 *                (0.001 °C/LSB)
 */
extern int32_t icm_20948_parse_temp_value(int16_t value);

/**
 *  Get the most recent x axis acceleration value that the sensor read.
 *
 *  @param inst The driver instance from which the data should be read
 *
 *  @param return The acceleration value in units of 100 µg per LSB
 */
extern int32_t icm_20948_get_last_accel_x(struct icm_20948_desc *inst);

/**
 *  Get the most recent y axis acceleration value that the sensor read.
 *
 *  @param inst The driver instance from which the data should be read
 *
 *  @param return The acceleration value in units of 100 µg per LSB
 */
extern int32_t icm_20948_get_last_accel_y(struct icm_20948_desc *inst);

/**
 *  Get the most recent z axis acceleration value that the sensor read.
 *
 *  @param inst The driver instance from which the data should be read
 *
 *  @param return The acceleration value in units of 100 µg per LSB
 */
extern int32_t icm_20948_get_last_accel_z(struct icm_20948_desc *inst);

/**
 *  Get the most recent x axis rotational velocity value that the sensor read.
 *
 *  @param inst The driver instance from which the data should be read
 *
 *  @param return The rotational velocity value in units of 1 milladps per LSB
 */
extern int32_t icm_20948_get_last_gyro_x(struct icm_20948_desc *inst);

/**
 *  Get the most recent y axis rotational velocity value that the sensor read.
 *
 *  @param inst The driver instance from which the data should be read
 *
 *  @param return The rotational velocity value in units of 1 milladps per LSB
 */
extern int32_t icm_20948_get_last_gyro_y(struct icm_20948_desc *inst);

/**
 *  Get the most recent z axis rotational velocity value that the sensor read.
 *
 *  @param inst The driver instance from which the data should be read
 *
 *  @param return The rotational velocity value in units of 1 milladps per LSB
 */
extern int32_t icm_20948_get_last_gyro_z(struct icm_20948_desc *inst);

/**
 *  Get the most recent temperature value that the sensor read.
 *
 *  @param inst The driver instance from which the data should be read
 *
 *  @param return The temperature value in units of 1 milladegree Celsius per
 *                LSB
 */
extern int32_t icm_20948_get_last_temp(struct icm_20948_desc *inst);

#endif /* icm_20948_h */

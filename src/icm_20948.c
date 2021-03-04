#include "icm_20948.h"
#include "icm_20948_registers.h"
#include "icm_20948_states.h"

#include <string.h>

void init_icm_20948(struct icm_20948_desc *inst, const nrfx_spim_t *spi,
                    NRF_GPIO_Type *int_port, uint8_t int_pin,
                    struct data_logger_descriptor *logger)
{
    inst->spi = spi;
    inst->int_port = int_port;
    inst->int_pin = int_pin;

    inst->logger = logger;

    inst->spi_output_buf_ready = 0;
    inst->spi_in_progress = 0;
    inst->spi_done = 0;

    inst->last_sample_time = 0;

    // Configure interupt pin as input
    inst->int_port->DIRCLR = (1 << inst->int_pin);

    // Start by selecting bank 0, then reset
    inst->state = ICM_20948_SELECT_BANK;
    inst->bank_sel_info.bank = 0;
    inst->bank_sel_info.next_state = ICM_20948_RESET;
    icm_20948_service(inst);
}


void icm_20948_service(struct icm_20948_desc *inst)
{
    int ret;

    do {
        if (inst->spi_in_progress && !inst->spi_done) {
            // We are waiting for a transaction on the SPI bus to finish
            return;
        }

        inst->spi_done = 0;

        if (icm_20948_state_handlers[inst->state] == NULL) {
            // This is a dead end state
            return;
        }

        ret = icm_20948_state_handlers[inst->state](inst);
    } while (ret);
}

void icm_20948_spim_event_handler(nrfx_spim_evt_t const *event,
                                  void *context)
{
    struct icm_20948_desc *inst = (struct icm_20948_desc *)context;

    inst->spi_done = 1;
}


uint32_t icm_20948_get_last_measurment_time(struct icm_20948_desc *inst)
{
    return inst->last_sample_time;
}


int32_t icm_20948_parse_accel_value(int16_t value)
{
    // At +/-8 g full scale range we have 4096 LSB/g
    return ((int32_t)value * 10000)/4096;
}

int32_t icm_20948_parse_gyro_value(int16_t value)
{
    // At +/-2000 dps full scale range we have 16.4 LSB/dps
    return ((int32_t)value * 10000)/164;
}

int32_t icm_20948_parse_temp_value(int16_t value)
{
    // Value from sensor is in units of 333.87 LSB/°C with a room temeperature
    // offset of 0 LSB at 21 °C.
    return (int32_t)((((int64_t)value - 0) * 100000) / 33387) + 21000;
}


int32_t icm_20948_get_last_accel_x(struct icm_20948_desc *inst)
{
    return icm_20948_parse_accel_value((int16_t)inst->last_accel_x);
}

int32_t icm_20948_get_last_accel_y(struct icm_20948_desc *inst)
{
    return icm_20948_parse_accel_value((int16_t)inst->last_accel_y);
}

int32_t icm_20948_get_last_accel_z(struct icm_20948_desc *inst)
{
    return icm_20948_parse_accel_value((int16_t)inst->last_accel_z);
}

int32_t icm_20948_get_last_gyro_x(struct icm_20948_desc *inst)
{
    return icm_20948_parse_gyro_value((int16_t)inst->last_gyro_x);
}

int32_t icm_20948_get_last_gyro_y(struct icm_20948_desc *inst)
{
    return icm_20948_parse_gyro_value((int16_t)inst->last_gyro_y);
}

int32_t icm_20948_get_last_gyro_z(struct icm_20948_desc *inst)
{
    return icm_20948_parse_gyro_value((int16_t)inst->last_gyro_z);
}

int32_t icm_20948_get_last_temp(struct icm_20948_desc *inst)
{
    return icm_20948_parse_temp_value((int16_t)inst->last_temp);
}

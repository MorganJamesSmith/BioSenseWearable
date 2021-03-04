#include "icm_20948_states.h"
#include "icm_20948_registers.h"

#include "global.h"

// MARK: Helpers

/**
 * Handle a state in which a value should be read from the sensor and stored in
 * the isntance's SPI buffer.
 *
 * @param inst The ICM-20948 driver instance
 * @param addr The address of the register from which data should be read
 * @param width The width of the data to be read in bytes
 *
 * @return 1 if the FSM should procceed to the next state, 0 otherwise
 */
static uint8_t handle_read_state (struct icm_20948_desc *inst, uint8_t addr,
                                  uint8_t width)
{
    if (inst->spi_in_progress) {
        // Just finished read transaction
        inst->spi_in_progress = 0;
        inst->spi_output_buf_ready = 0;
        // Go to next state
        return 1;
    }
    // Need to start read transaction
    inst->spi_buffer[0] = addr | (1 << 7);
    const nrfx_spim_xfer_desc_t xfer = NRFX_SPIM_XFER_TRX(inst->spi_buffer, 1,
                                                          inst->spi_buffer,
                                                          width + 1);
    nrfx_err_t ret = nrfx_spim_xfer(inst->spi, &xfer, 0);
    inst->spi_in_progress = ret == NRFX_SUCCESS;

    // Check if transaction is complete on next call of service
    return 0;
}

/**
 * Handle a state in which a value should be written to the sensor from the
 * instance's SPI buffer. The data that the state needs to write should start at
 * inst->spi_buffer + 1, this function will place the regiser address in the
 * first byte of the SPI buffer.
 *
 * @param inst The ICM-20948 driver instance
 * @param addr The address of the register to which data should be written
 * @param length The length of the data to be sent
 *
 * @return 1 if the FSM should procceed to the next state, 0 otherwise
 */
static uint8_t handle_write_state (struct icm_20948_desc *inst, uint8_t addr,
                                   uint8_t length)
{
    if (inst->spi_in_progress) {
        // Just finished write transaction
        inst->spi_in_progress = 0;
        inst->spi_output_buf_ready = 0;
        // Go to next state
        return 1;
    }
    // Need to send data
    inst->spi_buffer[0] = addr & ~(1 << 7);
    const nrfx_spim_xfer_desc_t xfer = NRFX_SPIM_XFER_TX(inst->spi_buffer,
                                                         length + 1);
    nrfx_err_t ret = nrfx_spim_xfer(inst->spi, &xfer, 0);
    inst->spi_in_progress = ret == NRFX_SUCCESS;

    // Stay in same state
    return 0;
}


// MARK: State Handlers

static int icm_20948_handler_select_bank (struct icm_20948_desc *inst)
{
    if (!inst->spi_output_buf_ready) {
        inst->spi_buffer[1] = ((inst->bank_sel_info.bank <<
                                ICM_20948_REG_BANK_SEL_POS_USER_BANK) &
                               ICM_20948_REG_BANK_SEL_MSK_USER_BANK);
        inst->spi_output_buf_ready = 1;
    }

    if (handle_write_state(inst, ICM_20948_REG_ADDR_REG_BANK_SEL, 1)) {
        // Go to the next state right away
        inst->state = inst->bank_sel_info.next_state;
        return 1;
    }

    return 0;
}

static int icm_20948_handler_config_gyro (struct icm_20948_desc *inst)
{
    if (!inst->spi_output_buf_ready) {
        // GYRO_SMPLRT_DIV
        inst->spi_buffer[1] = inst->gyro_config_info.sample_rate_div;
        // GYRO_CONFIG_1
        inst->spi_buffer[2] = (inst->gyro_config_info.lpf_config <<
                               ICM_20948_GYRO_CONFIG_1_POS_GYRO_DLPFCFG);
        inst->spi_buffer[2] |= (inst->gyro_config_info.full_scale_range <<
                                ICM_20948_GYRO_CONFIG_1_POS_FS_SEL);
        if (inst->gyro_config_info.lpf_enable) {
            inst->spi_buffer[2] |= ICM_20948_GYRO_CONFIG_1_MSK_GYRO_FCHOICE;
        }
        // GYRO_CONFIG_2
        inst->spi_buffer[3] = (inst->gyro_config_info.oversampling <<
                               ICM_20948_GYRO_CONFIG_2_POS_GYRO_AVGCFG);
        if (inst->gyro_config_info.x_self_test) {
            inst->spi_buffer[3] |= ICM_20948_GYRO_CONFIG_2_MSK_XGYRO_CTEN;
        }
        if (inst->gyro_config_info.y_self_test) {
            inst->spi_buffer[3] |= ICM_20948_GYRO_CONFIG_2_MSK_YGYRO_CTEN;
        }
        if (inst->gyro_config_info.z_self_test) {
            inst->spi_buffer[3] |= ICM_20948_GYRO_CONFIG_2_MSK_ZGYRO_CTEN;
        }

        inst->spi_output_buf_ready = 1;
    }

    if (handle_write_state(inst, ICM_20948_REG_ADDR_GYRO_SMPLRT_DIV, 3)) {
        // Go to the next state right away
        inst->state = inst->gyro_config_info.next_state;
        return 1;
    }

    return 0;
}

static int icm_20948_handler_config_accel (struct icm_20948_desc *inst)
{
    if (!inst->spi_output_buf_ready) {
        // ACCEL_SMPLRT_DIV_1
        inst->spi_buffer[1] = inst->accel_config_info.sample_rate_div >> 8;
        // ACCEL_SMPLRT_DIV_2
        inst->spi_buffer[2] = inst->accel_config_info.sample_rate_div & 0xff;
        // ACCEL_INTEL_CTRL
        inst->spi_buffer[3] = 0;
        // ACCEL_WOM_THR
        inst->spi_buffer[4] = 0;
        // ACCEL_CONFIG
        inst->spi_buffer[5] = (inst->accel_config_info.lpf_config <<
                               ICM_20948_ACCEL_CONFIG_POS_ACCEL_DLPFCFG);
        inst->spi_buffer[5] |= (inst->accel_config_info.full_scale_range <<
                                ICM_20948_ACCEL_CONFIG_POS_ACCEL_FS_SEL);
        if (inst->accel_config_info.lpf_enable) {
            inst->spi_buffer[5] |= ICM_20948_ACCEL_CONFIG_MSK_ACCEL_FCHOICE;
        }
        // ACCEL_CONFIG_2
        inst->spi_buffer[6] = (inst->accel_config_info.oversampling <<
                               ICM_20948_ACCEL_CONFIG_2_POS_DEC3_CFG);
        if (inst->accel_config_info.x_self_test) {
            inst->spi_buffer[6] |= ICM_20948_ACCEL_CONFIG_2_MSK_AX_ST_EN_REG;
        }
        if (inst->accel_config_info.y_self_test) {
            inst->spi_buffer[6] |= ICM_20948_ACCEL_CONFIG_2_MSK_AY_ST_EN_REG;
        }
        if (inst->accel_config_info.z_self_test) {
            inst->spi_buffer[6] |= ICM_20948_ACCEL_CONFIG_2_MSK_AZ_ST_EN_REG;
        }

        inst->spi_output_buf_ready = 1;
    }

    if (handle_write_state(inst, ICM_20948_REG_ADDR_ACCEL_SMPLRT_DIV_1, 6)) {
        // Go to the next state right away
        inst->state = inst->accel_config_info.next_state;
        return 1;
    }
    
    return 0;
}

static int icm_20948_handler_config_temp (struct icm_20948_desc *inst)
{
    if (!inst->spi_output_buf_ready) {
        // TEMP_CONFIG
        inst->spi_buffer[1] = (inst->temp_config_info.lpf_config <<
                               ICM_20948_TEMP_CONFIG_POS_TEMP_DLPFCFG);

        inst->spi_output_buf_ready = 1;
    }

    if (handle_write_state(inst, ICM_20948_REG_ADDR_TEMP_CONFIG, 1)) {
        // Go to the next state right away
        inst->state = inst->temp_config_info.next_state;
        return 1;
    }

    return 0;
}

static int icm_20948_handler_reset (struct icm_20948_desc *inst)
{
    if (!inst->spi_output_buf_ready) {
        inst->spi_buffer[1] = ICM_20948_PWR_MGMT_1_MSK_DEVICE_RESET;
        inst->spi_output_buf_ready = 1;
    }

    if (handle_write_state(inst, ICM_20948_REG_ADDR_PWR_MGMT_1, 1)) {
        // Go to the next state
        inst->state = ICM_20948_READ_WAI;
        // Record time
        inst->time = millis;
        return 1;
    }

    return 0;
}

static int icm_20948_handler_read_wai (struct icm_20948_desc *inst)
{
    // Make sure that some time has ellapsed since we sent the reset
    // command
    if ((millis - inst->time) < 2) {
        return 0;
    }
    // Read the WAI register
    if (handle_read_state(inst, ICM_20948_REG_ADDR_WHO_AM_I, 1)) {
        // Get WAI value from buffer
        inst->wai = inst->spi_buffer[1];
        // Go to the next state
        inst->state = ICM_20948_DISABLE_I2C;
        return 1;
    }

    return 0;
}

static int icm_20948_handler_disable_i2c (struct icm_20948_desc *inst)
{
    if (!inst->spi_output_buf_ready) {
        inst->spi_buffer[1] = (ICM_20948_USER_CTRL_MSK_I2C_IF_DIS |
                               ICM_20948_USER_CTRL_MSK_DMP_RST);
        inst->spi_output_buf_ready = 1;
    }

    if (handle_write_state(inst, ICM_20948_REG_ADDR_USER_CTRL, 1)) {
        // Go to the next state
        inst->state = ICM_20948_WAKE;
        // Record time
        inst->time = millis;
        return 1;
    }

    return 0;
}

static int icm_20948_handler_wake (struct icm_20948_desc *inst)
{
    if (!inst->spi_output_buf_ready) {
        inst->spi_buffer[1] = (1 << ICM_20948_PWR_MGMT_1_POS_CLKSEL);
        inst->spi_output_buf_ready = 1;
    }

    if (handle_write_state(inst, ICM_20948_REG_ADDR_PWR_MGMT_1, 1)) {
        // Go to next state
        inst->state = ICM_20948_FIFO_RESET_SET;
        return 1;
    }

    return 0;
}

static int icm_20948_handler_fifo_reset_set (struct icm_20948_desc *inst)
{
    if (!inst->spi_output_buf_ready) {
        inst->spi_buffer[1] = (ICM_20948_FIFO_RST_MSK_FIFO_RESET_4 |
                               ICM_20948_FIFO_RST_MSK_FIFO_RESET_3 |
                               ICM_20948_FIFO_RST_MSK_FIFO_RESET_2 |
                               ICM_20948_FIFO_RST_MSK_FIFO_RESET_1 |
                               ICM_20948_FIFO_RST_MSK_FIFO_RESET_0);
        inst->spi_output_buf_ready = 1;
    }

    if (handle_write_state(inst, ICM_20948_REG_ADDR_FIFO_RST, 1)) {
        // Go to next state
        inst->state = ICM_20948_FIFO_RESET_CLEAR;
        // Record time
        inst->time = millis;
        return 1;
    }

    return 0;
}

static int icm_20948_handler_fifo_reset_clr (struct icm_20948_desc *inst)
{
    // Make sure that some time has ellapsed since we set the reset bits
    if ((millis - inst->time) < 2) {
        return 0;
    }

    if (!inst->spi_output_buf_ready) {
        inst->spi_buffer[1] = 0;
        inst->spi_output_buf_ready = 1;
    }

    if (handle_write_state(inst, ICM_20948_REG_ADDR_FIFO_RST, 1)) {
        // Go to next state
        inst->state = ICM_20948_CONFIGURE_FIFO;
        return 1;
    }

    return 0;
}

static int icm_20948_handler_configure_fifo (struct icm_20948_desc *inst)
{
    if (!inst->spi_output_buf_ready) {
        inst->spi_buffer[1] = ICM_20948_FIFO_EN_2_MSK_ACCEL_FIFO_EN;
        inst->spi_buffer[1] |= ICM_20948_FIFO_EN_2_MSK_GYRO_Z_FIFO_EN;
        inst->spi_buffer[1] |= ICM_20948_FIFO_EN_2_MSK_GYRO_X_FIFO_EN;
        inst->spi_buffer[1] |= ICM_20948_FIFO_EN_2_MSK_GYRO_Y_FIFO_EN;
        //inst->spi_buffer[1] |= ICM_20948_FIFO_EN_2_MSK_TEMP_FIFO_EN;
        inst->spi_output_buf_ready = 1;
    }

    if (handle_write_state(inst, ICM_20948_REG_ADDR_FIFO_EN_2, 1)) {
        // Go to next state
        inst->state = ICM_20948_WRITE_FIFO_CFG;
        return 1;
    }

    return 0;
}

static int icm_20948_handler_write_fifo_cfg (struct icm_20948_desc *inst)
{
    if (!inst->spi_output_buf_ready) {
        inst->spi_buffer[1] = ICM_20948_FIFO_CFG_MSK_FIFO_CFG;
        inst->spi_output_buf_ready = 1;
    }

    if (handle_write_state(inst, ICM_20948_REG_ADDR_FIFO_CFG, 1)) {
        // Switch to bank 2 then go to next state
        inst->bank_sel_info.bank = 2;

        inst->state = ICM_20948_SELECT_BANK;
        inst->bank_sel_info.next_state = ICM_20948_ENABLE_ODR_ALIGN;
        return 1;
    }

    return 0;
}

static int icm_20948_handler_enable_odr_align (struct icm_20948_desc *inst)
{
    if (!inst->spi_output_buf_ready) {
        inst->spi_buffer[1] = ICM_20948_ODR_ALIGN_EN_MSK_ODR_ALIGN_EN;
        inst->spi_output_buf_ready = 1;
    }

    if (handle_write_state(inst, ICM_20948_REG_ADDR_ODR_ALIGN_EN, 1)) {
        // Go to the next state
        inst->state = ICM_20948_CONFIG_GYRO_NORM_OP;
        return 1;
    }

    return 0;
}

static int icm_20948_handler_config_gyro_norm_op (struct icm_20948_desc *inst)
{
    // Configure gyroscope for an output data rate of 48.9 Hz with 32x
    // oversampling and a LPF bandswidth of 17.8 Hz.
    inst->gyro_config_info.sample_rate_div = 22;
    inst->gyro_config_info.lpf_config = 5;
    inst->gyro_config_info.full_scale_range = 3; // +/- 2000 dps
    inst->gyro_config_info.lpf_enable = 1;
    inst->gyro_config_info.x_self_test = 0;
    inst->gyro_config_info.y_self_test = 0;
    inst->gyro_config_info.z_self_test = 0;
    inst->gyro_config_info.oversampling =
                                    ICM_20948_GYRO_CONFIG_2_GYRO_AVGCFG_32X;

    inst->state = ICM_20948_CONFIG_GYRO;
    inst->gyro_config_info.next_state = ICM_20948_CONFIG_ACCEL_NORM_OP;

    return 1;
}

static int icm_20948_handler_config_accel_norm_op (struct icm_20948_desc *inst)
{
    // Configure accelerometer for an output data rate of 48.9 Hz with 32x
    // oversampling.
    inst->accel_config_info.sample_rate_div = 22;
    inst->accel_config_info.lpf_config = 7;
    inst->accel_config_info.full_scale_range = 2; // +/- 8g
    inst->accel_config_info.lpf_enable = 1;
    inst->accel_config_info.x_self_test = 0;
    inst->accel_config_info.y_self_test = 0;
    inst->accel_config_info.z_self_test = 0;
    inst->accel_config_info.oversampling =
                                    ICM_20948_ACCEL_CONFIG_2_DEC3_CFG_32X;

    inst->state = ICM_20948_CONFIG_ACCEL;
    inst->accel_config_info.next_state = ICM_20948_CONFIG_TEMP_NORM_OP;

    return 1;
}

static int icm_20948_handler_config_temp_norm_op (struct icm_20948_desc *inst)
{
    inst->temp_config_info.lpf_config = 4;

    inst->state = ICM_20948_CONFIG_TEMP;
    inst->temp_config_info.next_state = ICM_20948_INIT_SELECT_BANK_0;

    return 1;
}

static int icm_20948_handler_init_select_bank_0 (struct icm_20948_desc *inst)
{
    inst->bank_sel_info.bank = 0;

    inst->state = ICM_20948_SELECT_BANK;
    inst->bank_sel_info.next_state = ICM_20948_CONFIG_INTERUPTS;

    return 1;
}

static int icm_20948_handler_config_interupts (struct icm_20948_desc *inst)
{
    if (!inst->spi_output_buf_ready) {
        // INT_PIN_CFG
        inst->spi_buffer[1] = (ICM_20948_INT_PIN_CFG_MSK_INT1_LATCH_EN |
                               ICM_20948_INT_PIN_CFG_MSK_INT1_ANYRD_2CLEAR);
        // INT_ENABLE
        inst->spi_buffer[2] = 0x9f;
        // INT_ENABLE_1
        inst->spi_buffer[3] = 1;
        // INT_ENABLE_2
        inst->spi_buffer[4] = (ICM_20948_INT_ENABLE_2_MSK_FIFO_OVERFLOW_EN_4 |
                               ICM_20948_INT_ENABLE_2_MSK_FIFO_OVERFLOW_EN_3 |
                               ICM_20948_INT_ENABLE_2_MSK_FIFO_OVERFLOW_EN_2 |
                               ICM_20948_INT_ENABLE_2_MSK_FIFO_OVERFLOW_EN_1 |
                               ICM_20948_INT_ENABLE_2_MSK_FIFO_OVERFLOW_EN_0);
        // INT_ENABLE_3
        inst->spi_buffer[5] = (ICM_20948_INT_ENABLE_3_MSK_FIFO_WM_EN_4 |
                               ICM_20948_INT_ENABLE_3_MSK_FIFO_WM_EN_3 |
                               ICM_20948_INT_ENABLE_3_MSK_FIFO_WM_EN_2 |
                               ICM_20948_INT_ENABLE_3_MSK_FIFO_WM_EN_1 |
                               ICM_20948_INT_ENABLE_3_MSK_FIFO_WM_EN_0);

        inst->spi_output_buf_ready = 1;
    }

    if (handle_write_state(inst, ICM_20948_REG_ADDR_INT_PIN_CFG, 5)) {
        // Go to next state
        inst->state = ICM_20948_READ_FIFO_CONF;
        return 1;
    }

    return 0;
}

static int icm_20948_handler_enable_fifo (struct icm_20948_desc *inst)
{
    if (!inst->spi_output_buf_ready) {
        inst->spi_buffer[1] = (ICM_20948_USER_CTRL_MSK_FIFO_EN |
                               ICM_20948_USER_CTRL_MSK_I2C_IF_DIS);
        inst->spi_output_buf_ready = 1;
    }

    if (handle_write_state(inst, ICM_20948_REG_ADDR_USER_CTRL, 1)) {
        inst->state = ICM_20948_READ_FIFO_CONF;
//        inst->bank_sel_info.bank = 2;
//        inst->state = ICM_20948_SELECT_BANK;
//        inst->bank_sel_info.next_state = ICM_20948_READ_FIFO_CONF;
        return 1;
    }

    return 0;
}

static int icm_20948_handler_read_fifo_conf (struct icm_20948_desc *inst)
{
    // Read the WAI register
    if (handle_read_state(inst, ICM_20948_REG_ADDR_FIFO_COUNTH, 2)) {
        // Get WAI value from buffer
        inst->wai = inst->spi_buffer[1];
        // Go to the next state
        inst->state = ICM_20948_IDLE;
//        inst->bank_sel_info.bank = 0;
//        inst->state = ICM_20948_SELECT_BANK;
//        inst->bank_sel_info.next_state = ICM_20948_IDLE;
        return 1;
    }

    return 0;
}

static int icm_20948_handler_idle (struct icm_20948_desc *inst)
{
//    if (inst->int_port->IN & (1 << inst->int_pin)) {
//        // Interupt is asserted
//        inst->state = ICM_20948_READ_FIFO_COUNT;
//        inst->time = millis;
//        return 1;
//    }

    static uint32_t last_time = 0;
    if ((millis - last_time) > 100) {
        inst->state = ICM_20948_READ_DATA;
        inst->time = millis;
        last_time = millis;
        return 1;
    }

    return 0;
}

static int icm_20948_handler_read_fifo_count (struct icm_20948_desc *inst)
{
    // Read the FIFO_COUNTH and FIFO_COUNTL registers
    if (handle_read_state(inst, ICM_20948_REG_ADDR_FIFO_COUNTH, 2)) {
        // Get count from spi buffer
        inst->fifo_count = inst->spi_buffer[1] << 8;
        inst->fifo_count |= inst->spi_buffer[2];
        // Go to the next state
        inst->state = ICM_20948_READ_FIFO;
        return 1;
    }

    return 0;
}

static int icm_20948_handler_read_fifo (struct icm_20948_desc *inst)
{
    // Read data from FIFO
    const int available = (inst->fifo_count <= 512) ? inst->fifo_count : 512;
    const int read_length = available - (available % 12);

    if (read_length == 0) {
        // Go back to idle
        inst->state = ICM_20948_IDLE;
        return 1;
    }

    if (handle_read_state(inst, ICM_20948_REG_ADDR_FIFO_R_W, read_length)) {
        // Store components of the most recent sample
        const int last_sample_offset = ((read_length / 12) - 1) * 12;
        uint8_t *const sample_buf = inst->spi_buffer + 1 + last_sample_offset;

        inst->last_accel_x = (sample_buf[0] << 8) | sample_buf[1];
        inst->last_accel_y = (sample_buf[2] << 8) | sample_buf[3];
        inst->last_accel_z = (sample_buf[4] << 8) | sample_buf[5];
        inst->last_gyro_x = (sample_buf[6] << 8) | sample_buf[7];
        inst->last_gyro_y = (sample_buf[8] << 8) | sample_buf[9];
        inst->last_gyro_z = (sample_buf[10] << 8) | sample_buf[11];
        //inst->last_temp = (sample_buf[12] << 8) | sample_buf[13];
        inst->last_sample_time = inst->time;

        // Pass the rest of the data to the data logging service

        // Go back to idle
        inst->state = ICM_20948_IDLE;
        return 1;
    }

    return 0;
}

static int icm_20948_handler_read_data (struct icm_20948_desc *inst)
{
    // Read data from SRAM
    if (handle_read_state(inst, ICM_20948_REG_ADDR_ACCEL_XOUT_H, 14)) {
        // Record data as most recent sample
        inst->last_accel_x = (inst->spi_buffer[1] << 8) | inst->spi_buffer[2];
        inst->last_accel_y = (inst->spi_buffer[3] << 8) | inst->spi_buffer[4];
        inst->last_accel_z = (inst->spi_buffer[5] << 8) | inst->spi_buffer[6];
        inst->last_gyro_x = (inst->spi_buffer[7] << 8) | inst->spi_buffer[8];
        inst->last_gyro_y = (inst->spi_buffer[9] << 8) | inst->spi_buffer[10];
        inst->last_gyro_z = (inst->spi_buffer[11] << 8) | inst->spi_buffer[12];
        inst->last_temp = (inst->spi_buffer[13] << 8) | inst->spi_buffer[14];
        inst->last_sample_time = inst->time;
        // Pass the data to the data logging service
        if (inst->logger != NULL) {
            struct imu_data_entry data = {
                .accel_x = icm_20948_parse_accel_value(inst->last_accel_x),
                .accel_y = icm_20948_parse_accel_value(inst->last_accel_y),
                .accel_z = icm_20948_parse_accel_value(inst->last_accel_z),
                .gyro_x = icm_20948_parse_gyro_value(inst->last_gyro_x),
                .gyro_y = icm_20948_parse_gyro_value(inst->last_gyro_y),
                .gyro_z = icm_20948_parse_gyro_value(inst->last_gyro_z),
                .temperature = icm_20948_parse_temp_value(inst->last_temp)
            };
            data_logger_log(inst->logger, inst->last_sample_time,
                            DATA_ENTRY_IMU, (const uint8_t*)&data,
                            sizeof(data));
        }
        // Go back to idle
        inst->state = ICM_20948_IDLE;
        return 1;
    }

    return 0;
}


// MARK: State Handlers Table

const icm_20948_state_handler_t icm_20948_state_handlers[] = {
    /* General purpose states that we jump back to */
    icm_20948_handler_select_bank,          // ICM_20948_SELECT_BANK
    icm_20948_handler_config_gyro,          // ICM_20948_CONFIG_GYRO
    icm_20948_handler_config_accel,         // ICM_20948_CONFIG_ACCEL
    icm_20948_handler_config_temp,          // ICM_20948_CONFIG_TEMP
    /* Initialization sequence */
    icm_20948_handler_reset,                // ICM_20948_RESET
    icm_20948_handler_read_wai,             // ICM_20948_READ_WAI
    icm_20948_handler_disable_i2c,          // ICM_20948_DISABLE_I2C
    icm_20948_handler_wake,                 // ICM_20948_WAKE
    icm_20948_handler_fifo_reset_set,       // ICM_20948_FIFO_RESET_SET
    icm_20948_handler_fifo_reset_clr,       // ICM_20948_FIFO_RESET_CLEAR
    icm_20948_handler_configure_fifo,       // ICM_20948_CONFIGURE_FIFO
    icm_20948_handler_write_fifo_cfg,       // ICM_20948_WRITE_FIFO_CFG
    // Switch to bank 2 (jump to ICM_20948_SELECT_BANK)
    icm_20948_handler_enable_odr_align,     // ICM_20948_ENABLE_ODR_ALIGN
    icm_20948_handler_config_gyro_norm_op,  // ICM_20948_CONFIG_GYRO_NORM_OP
    // Configure gyro (jump to ICM_20948_CONFIG_GYRO)
    icm_20948_handler_config_accel_norm_op, // ICM_20948_CONFIG_ACCEL_NORM_OP
    // Configure accel (jump to ICM_20948_CONFIG_ACCEL)
    icm_20948_handler_config_temp_norm_op,  // ICM_20948_CONFIG_TEMP_NORM_OP
    // Configure temp (jump to ICM_20948_CONFIG_TEMP)
    icm_20948_handler_init_select_bank_0,   // ICM_20948_INIT_SELECT_BANK_0
    // SWitch to bank 0 (jump to ICM_20948_SELECT_BANK)
    icm_20948_handler_config_interupts,     // ICM_20948_CONFIG_INTERUPTS
    icm_20948_handler_enable_fifo,          // ICM_20948_ENABLE_FIFO
    icm_20948_handler_read_fifo_conf,       // ICM_20948_READ_FIFO_CONF
    /* Normal Operation */
    icm_20948_handler_idle,                 // ICM_20948_IDLE
    icm_20948_handler_read_fifo_count,      // ICM_20948_READ_FIFO_COUNT
    icm_20948_handler_read_fifo,            // ICM_20948_READ_FIFO
    icm_20948_handler_read_data             // ICM_20948_READ_DATA
};



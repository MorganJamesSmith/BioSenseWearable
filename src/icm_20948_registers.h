#ifndef icm_20948_registers_h
#define icm_20948_registers_h

// User Bank 0 Register Addresses
#define ICM_20948_REG_ADDR_WHO_AM_I                 0x00
#define ICM_20948_REG_ADDR_USER_CTRL                0x03
#define ICM_20948_REG_ADDR_LP_CONFIG                0x05
#define ICM_20948_REG_ADDR_PWR_MGMT_1               0x06
#define ICM_20948_REG_ADDR_PWR_MGMT_2               0x07
#define ICM_20948_REG_ADDR_INT_PIN_CFG              0x0f
#define ICM_20948_REG_ADDR_INT_ENABLE               0x10
#define ICM_20948_REG_ADDR_INT_ENABLE_1             0x11
#define ICM_20948_REG_ADDR_INT_ENABLE_2             0x12
#define ICM_20948_REG_ADDR_INT_ENABLE_3             0x13
#define ICM_20948_REG_ADDR_I2C_MST_STATUS           0x17
#define ICM_20948_REG_ADDR_INT_STATUS               0x19
#define ICM_20948_REG_ADDR_INT_STATUS_1             0x1a
#define ICM_20948_REG_ADDR_INT_STATUS_2             0x1b
#define ICM_20948_REG_ADDR_INT_STATUS_3             0x1c
#define ICM_20948_REG_ADDR_DELAY_TIMEH              0x28
#define ICM_20948_REG_ADDR_DELAY_TIMEL              0x29
#define ICM_20948_REG_ADDR_ACCEL_XOUT_H             0x2D
#define ICM_20948_REG_ADDR_ACCEL_XOUT_L             0x2E
#define ICM_20948_REG_ADDR_ACCEL_YOUT_H             0x2F
#define ICM_20948_REG_ADDR_ACCEL_YOUT_L             0x30
#define ICM_20948_REG_ADDR_ACCEL_ZOUT_H             0x31
#define ICM_20948_REG_ADDR_ACCEL_ZOUT_L             0x32
#define ICM_20948_REG_ADDR_GYRO_XOUT_H              0x33
#define ICM_20948_REG_ADDR_GYRO_XOUT_L              0x34
#define ICM_20948_REG_ADDR_GYRO_YOUT_H              0x35
#define ICM_20948_REG_ADDR_GYRO_YOUT_L              0x36
#define ICM_20948_REG_ADDR_GYRO_ZOUT_H              0x37
#define ICM_20948_REG_ADDR_GYRO_ZOUT_L              0x38
#define ICM_20948_REG_ADDR_TEMP_OUT_H               0x39
#define ICM_20948_REG_ADDR_TEMP_OUT_L               0x3a
#define ICM_20948_REG_ADDR_EXT_SLV_SENS_DATA_00     0x3b
#define ICM_20948_REG_ADDR_EXT_SLV_SENS_DATA_01     0x3c
#define ICM_20948_REG_ADDR_EXT_SLV_SENS_DATA_02     0x3d
#define ICM_20948_REG_ADDR_EXT_SLV_SENS_DATA_03     0x3e
#define ICM_20948_REG_ADDR_EXT_SLV_SENS_DATA_04     0x3f
#define ICM_20948_REG_ADDR_EXT_SLV_SENS_DATA_05     0x40
#define ICM_20948_REG_ADDR_EXT_SLV_SENS_DATA_06     0x41
#define ICM_20948_REG_ADDR_EXT_SLV_SENS_DATA_07     0x42
#define ICM_20948_REG_ADDR_EXT_SLV_SENS_DATA_08     0x43
#define ICM_20948_REG_ADDR_EXT_SLV_SENS_DATA_09     0x44
#define ICM_20948_REG_ADDR_EXT_SLV_SENS_DATA_10     0x45
#define ICM_20948_REG_ADDR_EXT_SLV_SENS_DATA_11     0x46
#define ICM_20948_REG_ADDR_EXT_SLV_SENS_DATA_12     0x47
#define ICM_20948_REG_ADDR_EXT_SLV_SENS_DATA_13     0x48
#define ICM_20948_REG_ADDR_EXT_SLV_SENS_DATA_14     0x49
#define ICM_20948_REG_ADDR_EXT_SLV_SENS_DATA_15     0x4a
#define ICM_20948_REG_ADDR_EXT_SLV_SENS_DATA_16     0x4b
#define ICM_20948_REG_ADDR_EXT_SLV_SENS_DATA_17     0x4c
#define ICM_20948_REG_ADDR_EXT_SLV_SENS_DATA_18     0x4d
#define ICM_20948_REG_ADDR_EXT_SLV_SENS_DATA_19     0x4e
#define ICM_20948_REG_ADDR_EXT_SLV_SENS_DATA_20     0x4f
#define ICM_20948_REG_ADDR_EXT_SLV_SENS_DATA_21     0x50
#define ICM_20948_REG_ADDR_EXT_SLV_SENS_DATA_22     0x51
#define ICM_20948_REG_ADDR_EXT_SLV_SENS_DATA_23     0x52
#define ICM_20948_REG_ADDR_FIFO_EN_1                0x66
#define ICM_20948_REG_ADDR_FIFO_EN_2                0x67
#define ICM_20948_REG_ADDR_FIFO_RST                 0x68
#define ICM_20948_REG_ADDR_FIFO_MODE                0x69
#define ICM_20948_REG_ADDR_FIFO_COUNTH              0x70
#define ICM_20948_REG_ADDR_FIFO_COUNTL              0x71
#define ICM_20948_REG_ADDR_FIFO_R_W                 0x72
#define ICM_20948_REG_ADDR_DATA_RDY_STATUS          0x74
#define ICM_20948_REG_ADDR_FIFO_CFG                 0x76
#define ICM_20948_REG_ADDR_REG_BANK_SEL             0x7F

// User Bank 1 Register Addresses
#define ICM_20948_REG_ADDR_SELF_TEST_X_GYRO         0x02
#define ICM_20948_REG_ADDR_SELF_TEST_Y_GYRO         0x03
#define ICM_20948_REG_ADDR_SELF_TEST_Z_GYRO         0x04
#define ICM_20948_REG_ADDR_SELF_TEST_X_ACCEL        0x0E
#define ICM_20948_REG_ADDR_SELF_TEST_Y_ACCEL        0x0F
#define ICM_20948_REG_ADDR_SELF_TEST_Z_ACCEL        0x10
#define ICM_20948_REG_ADDR_XA_OFFS_H                0x14
#define ICM_20948_REG_ADDR_XA_OFFS_L                0x15
#define ICM_20948_REG_ADDR_YA_OFFS_H                0x17
#define ICM_20948_REG_ADDR_YA_OFFS_L                0x18
#define ICM_20948_REG_ADDR_ZA_OFFS_H                0x1A
#define ICM_20948_REG_ADDR_ZA_OFFS_L                0x1B
#define ICM_20948_REG_ADDR_TIMEBASE_CORRECTION_PLL  0x28

// User Bank 2 Register Addresses
#define ICM_20948_REG_ADDR_GYRO_SMPLRT_DIV          0x00
#define ICM_20948_REG_ADDR_GYRO_CONFIG_1            0x01
#define ICM_20948_REG_ADDR_GYRO_CONFIG_2            0x02
#define ICM_20948_REG_ADDR_XG_OFFS_H                0x03
#define ICM_20948_REG_ADDR_XG_OFFS_L                0x04
#define ICM_20948_REG_ADDR_YG_OFFS_H                0x05
#define ICM_20948_REG_ADDR_YG_OFFS_L                0x06
#define ICM_20948_REG_ADDR_ZG_OFFS_H                0x07
#define ICM_20948_REG_ADDR_ZG_OFFS_L                0x08
#define ICM_20948_REG_ADDR_ODR_ALIGN_EN             0x09
#define ICM_20948_REG_ADDR_ACCEL_SMPLRT_DIV_1       0x10
#define ICM_20948_REG_ADDR_ACCEL_SMPLRT_DIV_2       0x11
#define ICM_20948_REG_ADDR_ACCEL_INTEL_CTRL         0x12
#define ICM_20948_REG_ADDR_ACCEL_WOM_THR            0x13
#define ICM_20948_REG_ADDR_ACCEL_CONFIG             0x14
#define ICM_20948_REG_ADDR_ACCEL_CONFIG_2           0x15
#define ICM_20948_REG_ADDR_FSYNC_CONFIG             0x52
#define ICM_20948_REG_ADDR_TEMP_CONFIG              0x53
#define ICM_20948_REG_ADDR_MOD_CTRL_USR             0x54

// User Bank 3 Register Addresses
#define ICM_20948_REG_ADDR_I2C_MST_ODR_CONFIG       0x00
#define ICM_20948_REG_ADDR_I2C_MST_CTRL             0x01
#define ICM_20948_REG_ADDR_I2C_MST_DELAY_CTRL       0x02
#define ICM_20948_REG_ADDR_I2C_MST_SLV0_ADDR        0x03
#define ICM_20948_REG_ADDR_I2C_MST_SLV0_REG         0x04
#define ICM_20948_REG_ADDR_I2C_MST_SLV0_CTRL        0x05
#define ICM_20948_REG_ADDR_I2C_MST_SLV0_DO          0x06
#define ICM_20948_REG_ADDR_I2C_MST_SLV1_ADDR        0x07
#define ICM_20948_REG_ADDR_I2C_MST_SLV1_REG         0x08
#define ICM_20948_REG_ADDR_I2C_MST_SLV1_CTRL        0x09
#define ICM_20948_REG_ADDR_I2C_MST_SLV1_DO          0x0a
#define ICM_20948_REG_ADDR_I2C_MST_SLV2_ADDR        0x0b
#define ICM_20948_REG_ADDR_I2C_MST_SLV2_REG         0x0c
#define ICM_20948_REG_ADDR_I2C_MST_SLV2_CTRL        0x0d
#define ICM_20948_REG_ADDR_I2C_MST_SLV2_DO          0x0e
#define ICM_20948_REG_ADDR_I2C_MST_SLV3_ADDR        0x0f
#define ICM_20948_REG_ADDR_I2C_MST_SLV3_REG         0x10
#define ICM_20948_REG_ADDR_I2C_MST_SLV3_CTRL        0x11
#define ICM_20948_REG_ADDR_I2C_MST_SLV3_DO          0x12
#define ICM_20948_REG_ADDR_I2C_MST_SLV4_ADDR        0x13
#define ICM_20948_REG_ADDR_I2C_MST_SLV4_REG         0x14
#define ICM_20948_REG_ADDR_I2C_MST_SLV4_CTRL        0x15
#define ICM_20948_REG_ADDR_I2C_MST_SLV4_DO          0x16
#define ICM_20948_REG_ADDR_I2C_MST_SLV4_DI          0x17

// Who Am I Value
#define ICM_20948_WHO_AM_I  0xea

// User Control Register Bit Masks
#define ICM_20948_USER_CTRL_MSK_DMP_EN              (1 << 7)
#define ICM_20948_USER_CTRL_MSK_FIFO_EN             (1 << 6)
#define ICM_20948_USER_CTRL_MSK_I2C_MST_EN          (1 << 5)
#define ICM_20948_USER_CTRL_MSK_I2C_IF_DIS          (1 << 4)
#define ICM_20948_USER_CTRL_MSK_DMP_RST             (1 << 3)
#define ICM_20948_USER_CTRL_MSK_SRAM_RST            (1 << 2)
#define ICM_20948_USER_CTRL_MSK_I2C_MST_RST         (1 << 1)

// LP Configuration Register Bit Masks
#define ICM_20948_LP_CONFIG_MSK_I2C_MST_CYCLE       (1 << 6)
#define ICM_20948_LP_CONFIG_MSK_ACCEL_CYCLE         (1 << 5)
#define ICM_20948_LP_CONFIG_MSK_GYRO_CYCLE          (1 << 4)

// Power Management 1 Register Bit Masks
#define ICM_20948_PWR_MGMT_1_MSK_DEVICE_RESET       (1 << 7)
#define ICM_20948_PWR_MGMT_1_MSK_SLEEP              (1 << 6)
#define ICM_20948_PWR_MGMT_1_MSK_LP_EN              (1 << 5)
#define ICM_20948_PWR_MGMT_1_MSK_TEMP_DIS           (1 << 3)
#define ICM_20948_PWR_MGMT_1_MSK_CLKSEL             ((1 << 2)|(1 << 1)|(1 << 0))
#define ICM_20948_PWR_MGMT_1_POS_CLKSEL             0

// Power Management 2 Register Bit Masks
#define ICM_20948_PWR_MGMT_2_MSK_DISABLE_ACCEL      ((1 << 5)|(1 << 4)|(1 << 3))
#define ICM_20948_PWR_MGMT_2_POS_DISABLE_ACCEL      3
#define ICM_20948_PWR_MGMT_2_MSK_DISABLE_GYRO       ((1 << 2)|(1 < <1)|(1 << 0))
#define ICM_20948_PWR_MGMT_2_POS_DISABLE_GYRO       0

// Interupt Pin Configuration Register Bit Masks
#define ICM_20948_INT_PIN_CFG_MSK_INT1_ACTL         (1 << 7)
#define ICM_20948_INT_PIN_CFG_MSK_INT1_OPEN         (1 << 6)
#define ICM_20948_INT_PIN_CFG_MSK_INT1_LATCH_EN     (1 << 5)
#define ICM_20948_INT_PIN_CFG_MSK_INT1_ANYRD_2CLEAR (1 << 4)
#define ICM_20948_INT_PIN_CFG_MSK_ACTL_FSYNC        (1 << 3)
#define ICM_20948_INT_PIN_CFG_MSK_FSYNC_INT_MODE_EN (1 << 2)
#define ICM_20948_INT_PIN_CFG_MSK_BYPASS_EN         (1 << 1)

// Interupt Enable Register Bit Masks
#define ICM_20948_INT_ENABLE_MSK_REG_WOF_EN         (1 << 7)
#define ICM_20948_INT_ENABLE_MSK_WOM_INT_EN         (1 << 3)
#define ICM_20948_INT_ENABLE_MSK_PLL_RDY_EN         (1 << 2)
#define ICM_20948_INT_ENABLE_MSK_DMP_INT1_EN        (1 << 1)
#define ICM_20948_INT_ENABLE_MSK_I2C_MST_INT_EN     (1 << 0)

// Interupt Enable 1 Register Bit Masks
#define ICM_20948_INT_ENABLE_1_MSK_RAW_DATA_0_RDY_EN    (1 << 0)

// Interupt Enable 2 Register Bit Masks
#define ICM_20948_INT_ENABLE_2_MSK_FIFO_OVERFLOW_EN_4   (1 << 4)
#define ICM_20948_INT_ENABLE_2_MSK_FIFO_OVERFLOW_EN_3   (1 << 3)
#define ICM_20948_INT_ENABLE_2_MSK_FIFO_OVERFLOW_EN_2   (1 << 2)
#define ICM_20948_INT_ENABLE_2_MSK_FIFO_OVERFLOW_EN_1   (1 << 1)
#define ICM_20948_INT_ENABLE_2_MSK_FIFO_OVERFLOW_EN_0   (1 << 0)

// Interupt Enable 3 Register Bit Masks
#define ICM_20948_INT_ENABLE_3_MSK_FIFO_WM_EN_4         (1 << 4)
#define ICM_20948_INT_ENABLE_3_MSK_FIFO_WM_EN_3         (1 << 3)
#define ICM_20948_INT_ENABLE_3_MSK_FIFO_WM_EN_2         (1 << 2)
#define ICM_20948_INT_ENABLE_3_MSK_FIFO_WM_EN_1         (1 << 1)
#define ICM_20948_INT_ENABLE_3_MSK_FIFO_WM_EN_0         (1 << 0)

// I2C Master Status Register Bit Masks
#define ICM_20948_I2C_MST_STATUS_MSK_PASS_THROUGH   (1 << 7)
#define ICM_20948_I2C_MST_STATUS_MSK_I2C_SLV4_DONE  (1 << 6)
#define ICM_20948_I2C_MST_STATUS_MSK_I2C_LOST_ARB   (1 << 5)
#define ICM_20948_I2C_MST_STATUS_MSK_I2C_SLV4_NACK  (1 << 4)
#define ICM_20948_I2C_MST_STATUS_MSK_I2C_SLV3_NACK  (1 << 3)
#define ICM_20948_I2C_MST_STATUS_MSK_I2C_SLV2_NACK  (1 << 2)
#define ICM_20948_I2C_MST_STATUS_MSK_I2C_SLV1_NACK  (1 << 1)
#define ICM_20948_I2C_MST_STATUS_MSK_I2C_SLV0_NACK  (1 << 0)

// Interupt Status Register Bit Masks
#define ICM_20948_INT_STATUS_MSK_WOM_INT            (1 << 3)
#define ICM_20948_INT_STATUS_MSK_PLL_RDY            (1 << 2)
#define ICM_20948_INT_STATUS_MSK_DMP_INT1           (1 << 1)
#define ICM_20948_INT_STATUS_MSK_I2C_MST_INT        (1 << 0)

// Interupt Status Register 1 Bit Masks
#define ICM_20948_INT_STATUS_1_RAW_DATA_0_RDY_INT       (1 << 0)

// Interupt Status Register 2 Bit Masks
#define ICM_20948_INT_STATUS_2_MSK_FIFO_OVERFLOW_INT_4  (1 << 4)
#define ICM_20948_INT_STATUS_2_MSK_FIFO_OVERFLOW_INT_3  (1 << 3)
#define ICM_20948_INT_STATUS_2_MSK_FIFO_OVERFLOW_INT_2  (1 << 2)
#define ICM_20948_INT_STATUS_2_MSK_FIFO_OVERFLOW_INT_1  (1 << 1)
#define ICM_20948_INT_STATUS_2_MSK_FIFO_OVERFLOW_INT_0  (1 << 0)

// Interupt Status Register 3 Bit Masks
#define ICM_20948_INT_STATUS_3_MSK_FIFO_WM_INT_4        (1 << 4)
#define ICM_20948_INT_STATUS_3_MSK_FIFO_WM_INT_3        (1 << 3)
#define ICM_20948_INT_STATUS_3_MSK_FIFO_WM_INT_2        (1 << 2)
#define ICM_20948_INT_STATUS_3_MSK_FIFO_WM_INT_1        (1 << 1)
#define ICM_20948_INT_STATUS_3_MSK_FIFO_WM_INT_0        (1 << 0)

// Interupt Status Register 3 Bit Masks
#define ICM_20948_INT_STATUS_3_MSK_RAW_DATA_0_RDY       (1 << 0)

// FIFO Enable 1 Register Bit Masks
#define ICM_20948_FIFO_EN_1_MSK_SLV_3_FIFO_EN       (1 << 3)
#define ICM_20948_FIFO_EN_1_MSK_SLV_2_FIFO_EN       (1 << 2)
#define ICM_20948_FIFO_EN_1_MSK_SLV_1_FIFO_EN       (1 << 1)
#define ICM_20948_FIFO_EN_1_MSK_SLV_0_FIFO_EN       (1 << 0)

// FIFO Enable 2 Register Bit Masks
#define ICM_20948_FIFO_EN_2_MSK_ACCEL_FIFO_EN       (1 << 4)
#define ICM_20948_FIFO_EN_2_MSK_GYRO_Z_FIFO_EN      (1 << 3)
#define ICM_20948_FIFO_EN_2_MSK_GYRO_Y_FIFO_EN      (1 << 2)
#define ICM_20948_FIFO_EN_2_MSK_GYRO_X_FIFO_EN      (1 << 1)
#define ICM_20948_FIFO_EN_2_MSK_TEMP_FIFO_EN        (1 << 0)

// FIFO Reset Register Bit Masks
#define ICM_20948_FIFO_RST_MSK_FIFO_RESET_4         (1 << 4)
#define ICM_20948_FIFO_RST_MSK_FIFO_RESET_3         (1 << 3)
#define ICM_20948_FIFO_RST_MSK_FIFO_RESET_2         (1 << 2)
#define ICM_20948_FIFO_RST_MSK_FIFO_RESET_1         (1 << 1)
#define ICM_20948_FIFO_RST_MSK_FIFO_RESET_0         (1 << 0)

// FIFO Mode Register Bit Masks
#define ICM_20948_FIFO_MODE_MSK_FIFO_MODE_4         (1 << 4)
#define ICM_20948_FIFO_MODE_MSK_FIFO_MODE_3         (1 << 3)
#define ICM_20948_FIFO_MODE_MSK_FIFO_MODE_2         (1 << 2)
#define ICM_20948_FIFO_MODE_MSK_FIFO_MODE_1         (1 << 1)
#define ICM_20948_FIFO_MODE_MSK_FIFO_MODE_0         (1 << 0)

// Data Ready Status Register Bit Masks
#define ICM_20948_DATA_RDY_STATUS_MSK_WOF_STATUS        (1 << 4)
#define ICM_20948_DATA_RDY_STATUS_MSK_RAW_DATA_RDY_3    (1 << 3)
#define ICM_20948_DATA_RDY_STATUS_MSK_RAW_DATA_RDY_2    (1 << 2)
#define ICM_20948_DATA_RDY_STATUS_MSK_RAW_DATA_RDY_1    (1 << 1)
#define ICM_20948_DATA_RDY_STATUS_MSK_RAW_DATA_RDY_0    (1 << 0)

// FIFO Configuration Register Bit Masks
#define ICM_20948_FIFO_CFG_MSK_FIFO_CFG             (1 << 0)

// Register Bank Selection Register Bit Masks
#define ICM_20948_REG_BANK_SEL_MSK_USER_BANK        ((1 << 5)|(1 << 4))
#define ICM_20948_REG_BANK_SEL_POS_USER_BANK        4



// Gyroscope Configuration Register 1 Bit Masks
#define ICM_20948_GYRO_CONFIG_1_MSK_GYRO_DLPFCFG    ((1 << 5)|(1 << 4)|(1 << 3))
#define ICM_20948_GYRO_CONFIG_1_POS_GYRO_DLPFCFG    3
#define ICM_20948_GYRO_CONFIG_1_MSK_FS_SEL          ((1 << 2)|(1 << 1))
#define ICM_20948_GYRO_CONFIG_1_POS_FS_SEL          1
#define ICM_20948_GYRO_CONFIG_1_MSK_GYRO_FCHOICE    (1 << 0)

// Gyroscope Configuration Register 2 Bit Masks
#define ICM_20948_GYRO_CONFIG_2_MSK_XGYRO_CTEN      (1 << 5)
#define ICM_20948_GYRO_CONFIG_2_MSK_YGYRO_CTEN      (1 << 4)
#define ICM_20948_GYRO_CONFIG_2_MSK_ZGYRO_CTEN      (1 << 3)
#define ICM_20948_GYRO_CONFIG_2_MSK_GYRO_AVGCFG     ((1 << 2)|(1 << 1)|(1 << 0))
#define ICM_20948_GYRO_CONFIG_2_POS_GYRO_AVGCFG     0

#define ICM_20948_GYRO_CONFIG_2_GYRO_AVGCFG_1X      0
#define ICM_20948_GYRO_CONFIG_2_GYRO_AVGCFG_2X      1
#define ICM_20948_GYRO_CONFIG_2_GYRO_AVGCFG_4X      2
#define ICM_20948_GYRO_CONFIG_2_GYRO_AVGCFG_8X      3
#define ICM_20948_GYRO_CONFIG_2_GYRO_AVGCFG_16X     4
#define ICM_20948_GYRO_CONFIG_2_GYRO_AVGCFG_32X     5
#define ICM_20948_GYRO_CONFIG_2_GYRO_AVGCFG_64X     6
#define ICM_20948_GYRO_CONFIG_2_GYRO_AVGCFG_128X    7

// Output Data Rate Alignment Enable Register Bit Masks
#define ICM_20948_ODR_ALIGN_EN_MSK_ODR_ALIGN_EN     (1 << 0)

// Accelerometer INTEL Control Register Bit Masks
#define ICM_20948_ACCEL_INTEL_CTRL_MSK_ACCEL_INTEL_EN       (1 << 1)
#define ICM_20948_ACCEL_INTEL_CTRL_MSK_ACCEL_INTEL_MODE_INT (1 << 0)

// Accelerometer Configuration Register Bit Masks
#define ICM_20948_ACCEL_CONFIG_MSK_ACCEL_DLPFCFG    ((1 << 5)|(1 << 4)|(1 << 3))
#define ICM_20948_ACCEL_CONFIG_POS_ACCEL_DLPFCFG    3
#define ICM_20948_ACCEL_CONFIG_MSK_ACCEL_FS_SEL     ((1 << 2)|(1 << 1))
#define ICM_20948_ACCEL_CONFIG_POS_ACCEL_FS_SEL     1
#define ICM_20948_ACCEL_CONFIG_MSK_ACCEL_FCHOICE    (1 << 0)

// Accelerometer Configuration Register 2 Bit Masks
#define ICM_20948_ACCEL_CONFIG_2_MSK_AX_ST_EN_REG   (1 << 4)
#define ICM_20948_ACCEL_CONFIG_2_MSK_AY_ST_EN_REG   (1 << 3)
#define ICM_20948_ACCEL_CONFIG_2_MSK_AZ_ST_EN_REG   (1 << 2)
#define ICM_20948_ACCEL_CONFIG_2_MSK_DEC3_CFG       ((1 << 1)|(1 << 0))
#define ICM_20948_ACCEL_CONFIG_2_POS_DEC3_CFG       0

#define ICM_20948_ACCEL_CONFIG_2_DEC3_CFG_1X        0
#define ICM_20948_ACCEL_CONFIG_2_DEC3_CFG_8X        1
#define ICM_20948_ACCEL_CONFIG_2_DEC3_CFG_16X       2
#define ICM_20948_ACCEL_CONFIG_2_DEC3_CFG_32X       3

// FSYNC Configuration Register Bit Masks
#define ICM_20948_FSYNC_CONFIG_MSK_DELAY_TIME_EN    (1 << 7)
#define ICM_20948_FSYNC_CONFIG_MSK_WOF_DEGLITCH_EN  (1 << 5)
#define ICM_20948_FSYNC_CONFIG_MSK_WOF_EDGE_INT     (1 << 4)
#define ICM_20948_FSYNC_CONFIG_MSK_EXT_SYNC_SET     ((1 << 3)|(1 << 2)|\
                                                     (1 << 1)|(1 << 0))
#define ICM_20948_FSYNC_CONFIG_POS_EXT_SYNC_SET     0

#define ICM_20948_FSYNC_CONFIG_EXT_SYNC_SET_DISABLED    0
#define ICM_20948_FSYNC_CONFIG_EXT_SYNC_SET_TEMP_OUTL   1
#define ICM_20948_FSYNC_CONFIG_EXT_SYNC_SET_GYRO_XOUTL  2
#define ICM_20948_FSYNC_CONFIG_EXT_SYNC_SET_GYRO_YOUTL  3
#define ICM_20948_FSYNC_CONFIG_EXT_SYNC_SET_GYRO_ZOUTL  4
#define ICM_20948_FSYNC_CONFIG_EXT_SYNC_SET_ACCEL_XOUTL 5
#define ICM_20948_FSYNC_CONFIG_EXT_SYNC_SET_ACCEL_YOUTL 6
#define ICM_20948_FSYNC_CONFIG_EXT_SYNC_SET_ACCEL_ZOUTL 7

// Temperature Sensor Configuration Register Bit Masks
#define ICM_20948_TEMP_CONFIG_MSK_TEMP_DLPFCFG      ((1 << 2)|(1 << 1)|(1 << 0))
#define ICM_20948_TEMP_CONFIG_POS_TEMP_DLPFCFG      0

// MOD_CTRL_USR Register Bit Masks
#define ICM_20948_MOD_CTRL_USR_MSK_REG_LP_DMP_EN    (1 << 0)



// I2C Master Output Data Rate Configuration Register Bit Masks
#define ICM_20948_I2C_MST_ODR_CONFIG_MSK_I2C_MST_ODR_CONFIG ((1 << 3)|(1 << 2)|\
                                                             (1 << 1)|(1 << 0))
#define ICM_20948_I2C_MST_ODR_CONFIG_POS_I2C_MST_ODR_CONFIG 0

// I2C Master Control Register Bit Masks
#define ICM_20948_I2C_MST_CTRL_MSK_MULTI_MST_EN     (1 << 7)
#define ICM_20948_I2C_MST_CTRL_MSK_I2C_MST_P_NSR    (1 << 4)
#define ICM_20948_I2C_MST_CTRL_MSK_I2C_MST_CLK      ((1 << 3)|(1 << 2)|\
                                                     (1 << 1)|(1 << 0))
#define ICM_20948_I2C_MST_CTRL_POS_I2C_MST_CLK      0

// I2C Master Delay Control Register Bit Masks
#define ICM_20948_I2C_MST_DELAY_CTRL_MSK_DELAY_ES_SHADOW    (1 << 7)
#define ICM_20948_I2C_MST_DELAY_CTRL_MSK_I2C_SLV4_DELAY_EN  (1 << 4)
#define ICM_20948_I2C_MST_DELAY_CTRL_MSK_I2C_SLV3_DELAY_EN  (1 << 3)
#define ICM_20948_I2C_MST_DELAY_CTRL_MSK_I2C_SLV2_DELAY_EN  (1 << 2)
#define ICM_20948_I2C_MST_DELAY_CTRL_MSK_I2C_SLV1_DELAY_EN  (1 << 1)
#define ICM_20948_I2C_MST_DELAY_CTRL_MSK_I2C_SLV0_DELAY_EN  (1 << 0)

// I2C Slave Control 0-3 Register Bit Masks
#define ICM_20948_I2C_SLV_CTRL_MSK_I2C_SLV_EN       (1 << 7)
#define ICM_20948_I2C_SLV_CTRL_MSK_I2C_SLV_BYTE_SW  (1 << 6)
#define ICM_20948_I2C_SLV_CTRL_MSK_I2C_SLV_REG_DIS  (1 << 5)
#define ICM_20948_I2C_SLV_CTRL_MSK_I2C_SLV_GRP      (1 << 4)
#define ICM_20948_I2C_SLV_CTRL_MSK_I2C_SLV_LENG     ((1 << 3)|(1 << 2)|\
                                                     (1 << 1)|(1 << 0))
#define ICM_20948_I2C_SLV_CTRL_POS_I2C_SLV_LENG     0

// I2C Slave Control 4 Register Bit Masks
#define ICM_20948_I2C_SLV4_CTRL_MSK_I2C_SLV4_EN         (1 << 7)
#define ICM_20948_I2C_SLV4_CTRL_MSK_I2C_SLV4_INT_EN     (1 << 6)
#define ICM_20948_I2C_SLV4_CTRL_MSK_I2C_SLV4_REG_DIS    (1 << 5)
#define ICM_20948_I2C_SLV4_CTRL_MSK_I2C_SLV4_DLY        ((1 << 4)|(1 << 3)|\
                                                         (1 << 3)|(1 << 1)|\
                                                         (1 << 0))
#define ICM_20948_I2C_SLV4_CTRL_POS_I2C_SLV4_DLY        0







// Magnetometer Register Addresses
#define AK09916_REG_WIA2        0x01    // Device ID
#define AK09916_REG_ST1         0x10    // Data Status 1
#define AK09916_REG_HXL         0x11    // X-axis Data - low byte
#define AK09916_REG_HXH         0x12    // X-axis Data - high byte
#define AK09916_REG_HYL         0x13    // Y-axis Data - low byte
#define AK09916_REG_HYH         0x14    // Y-axis Data - high byte
#define AK09916_REG_HZL         0x15    // Z-axis Data - low byte
#define AK09916_REG_HZH         0x16    // Z-axis Data - high byte
#define AK09916_REG_ST2         0x18    // Data Status 2
#define AK09916_REG_CNTL2       0x31    // Control 2
#define AK09916_REG_CNTL3       0x32    // Control 3

// Device ID
#define AK09916_DEVICE_ID       0x09

// Data Status Register 1 Bit Masks
#define AK09916_ST1_MSK_DOR     (1 << 1)
#define AK09916_ST1_MSK_DRDY    (1 << 0)

// Data Status Register 2 Bit Masks
#define AK09916_ST2_MSK_MODE    ((1 << 4)|(1 << 3)|(1 << 2)|(1 << 1)|(1 << 0))
#define AK09916_ST2_POS_MODE    0

#define AK09916_ST2_MODE_POWER_DOWN             0
#define AK09916_ST2_MODE_SINGLE_MEASURMENT      1
#define AK09916_ST2_MODE_CONT_MEASURMENT_1      2
#define AK09916_ST2_MODE_CONT_MEASURMENT2       4
#define AK09916_ST2_MODE_CONT_MEASURMENT3       6
#define AK09916_ST2_MODE_CONT_MEASURMENT4       8
#define AK09916_ST2_MODE_SELF_TEST              16

// Control Register 1 Bit Masks
#define AK09916_CNTL1_MSK_MODE  ((1 << 4)|(1 << 3)|(1 << 2)|(1 << 1)|(1 << 0))
#define AK09916_CNTL1_POS_MODE  0

#define AK09916_CNTL1_MODE_POWER_DOWN           0
#define AK09916_CNTL1_MODE_SINGLE_MEASURMENT    1
#define AK09916_CNTL1_MODE_CONT_MEASURMENT_1    2
#define AK09916_CNTL1_MODE_CONT_MEASURMENT2     4
#define AK09916_CNTL1_MODE_CONT_MEASURMENT3     6
#define AK09916_CNTL1_MODE_CONT_MEASURMENT4     8
#define AK09916_CNTL1_MODE_SELF_TEST            16

// Control Register 2 Bit Masks
#define AK09916_CNTL2_MSK_SRST  (1 << 0)


#endif /* icm_20948_registers_h */

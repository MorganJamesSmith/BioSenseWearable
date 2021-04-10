#include "nordic_common.h"
#include "nrf.h"
#include "boards.h"
#include "app_timer.h"
#include "nrfx_spim.h"
//#include "nrfx_rtc.h"
#include "ff.h"
#include "diskio_blkdev.h"
#include "nrf_block_dev_sdc.h"

#include "global.h"
#include "sensors.h"
#include "usb_serial.h"
#include "bluetooth.h"
#include "cli.h"
#include "cli_commands.h"
#include "data_logger.h"
#include "adc.h"
#include "icm_20948.h"

// MARK: Variable Definitions
//const nrfx_rtc_t rtc = NRFX_RTC_INSTANCE(2);

volatile uint32_t millis;

// MARK: ISR Prototypes
void SysTick_Handler(void);

// MARK: Module Descriptors
static const struct cli_io_funcs_t usb_io_funcs = {
    .set_ready_callback = usb_cdc_set_ready_callback,
    .write_string = usb_cdc_put_string,
    .write_string_blocking = usb_cdc_put_string_blocking,
    .write_bytes = usb_cdc_put_bytes,
    .write_bytes_blocking = usb_cdc_put_bytes_blocking,
    .read_string = usb_cdc_get_string,
    .has_delim = usb_cdc_has_delim,
    .read_line_delim = usb_cdc_get_line_delim,
    .has_line = usb_cdc_has_line,
    .read_line = usb_cdc_get_line
};

static struct cli_desc_t usb_cli;

static const struct cli_io_funcs_t bluetooth_io_funcs = {
    .set_ready_callback = bluetooth_set_ready_callback,
    .write_string = bluetooth_put_string,
    .write_string_blocking = bluetooth_put_string_blocking,
    .write_bytes = bluetooth_put_bytes,
    .write_bytes_blocking = bluetooth_put_bytes_blocking,
    .read_string = bluetooth_get_string,
    .has_delim = bluetooth_has_delim,
    .read_line_delim = bluetooth_get_line_delim,
    .has_line = bluetooth_has_line,
    .read_line = bluetooth_get_line
};

static struct cli_desc_t ble_cli;

static const nrfx_spim_t imu_spi = NRFX_SPIM_INSTANCE(0);

struct icm_20948_desc imu;

#define SDC_SCK_PIN     19
#define SDC_MOSI_PIN    21
#define SDC_MISO_PIN    20
#define SDC_CS_PIN      17

NRF_BLOCK_DEV_SDC_DEFINE(block_dev_sdc,
                         NRF_BLOCK_DEV_SDC_CONFIG(SDC_SECTOR_SIZE,
                         APP_SDCARD_CONFIG(SDC_MOSI_PIN, SDC_MISO_PIN,
                                           SDC_SCK_PIN, SDC_CS_PIN)),
                         NFR_BLOCK_DEV_INFO_CONFIG("Nordic", "SDC", "1.00"));

FATFS fs;
int filesystem_init_status = 1;
uint32_t sd_card_capacity;

struct data_logger_descriptor data_logger;
int log_init_status = -2;

int initialize_sd_card_fs(void)
{
    FRESULT ff_result;
    DSTATUS disk_state = STA_NOINIT;

    // Initialize FATFS disk I/O
    static diskio_blkdev_t drives[] = {
        DISKIO_BLOCKDEV_CONFIG(NRF_BLOCKDEV_BASE_ADDR(block_dev_sdc, block_dev),
                               NULL)
    };

    app_sdc_info_get();

    diskio_blockdev_register(drives, ARRAY_SIZE(drives));

    for (uint32_t retries = 3; retries && disk_state; --retries) {
        disk_state = disk_initialize(0);
    }

    if (disk_state) {
        return -1;
    }

    const uint32_t blocks_per_mb = ((1024uL * 1024uL) /
                                    block_dev_sdc.block_dev.p_ops->geometry(
                                        &block_dev_sdc.block_dev)->blk_size);
    sd_card_capacity = (block_dev_sdc.block_dev.p_ops->geometry(
                                    &block_dev_sdc.block_dev)->blk_count /
                                        blocks_per_mb);

    // Mount the filesystem
    ff_result = f_mount(&fs, "", 1);
    if (ff_result) {
        return -1;
    }

    return 0;
}

int main(void)
{
    ret_code_t ret;

    /* Enable SysTick for an interupt every millisecond */
    SysTick_Config(64000);
    NVIC_SetPriority(SysTick_IRQn, 2);

    /* Enable RTC */
//    nrfx_rtc_config_t config = NRFX_RTC_DEFAULT_CONFIG;
//    config.prescaler = 31;
//    ret = nrfx_rtc_init(&rtc, &config, NULL);
//    APP_ERROR_CHECK(ret);

    //Power on RTC instance
//    nrfx_rtc_enable(&rtc);

    /* Enable timer module */
    ret = app_timer_init();
    APP_ERROR_CHECK(ret);

    /* Configue IO */
    // RGB LED
    NRF_P0->DIRSET = (1 << 22);
    NRF_P0->DIRSET = (1 << 23);
    NRF_P0->DIRSET = (1 << 24);

//    NRF_P0->OUTSET = (1 << 22);
//    NRF_P0->OUTSET = (1 << 23);
//    NRF_P0->OUTSET = (1 << 24);

    // Turn on power
    NRF_P1->DIRSET = (1 << 12);
    NRF_P1->OUT = (1 << 12);

    init_usb_cdc();
    init_cli(&usb_cli, &usb_io_funcs, "> ", debug_commands_funcs, '\r');

    /* Use this to turn LED on when bluetooth is connected 
    BSP_LED_0_PORT->DIRSET = BSP_LED_0_MASK;
    BSP_LED_0_PORT->OUT |= BSP_LED_0_MASK;
    void ble_connected(){ BSP_LED_0_PORT->OUT &= ~BSP_LED_0_MASK; }
    void ble_disconnected(){ BSP_LED_0_PORT->OUT |= BSP_LED_0_MASK; }
    bluetooth_set_connected_callback(ble_connected);
    bluetooth_set_disconnected_callback(ble_disconnected);*/
    bluetooth_init(); //UNCOMMENT THIS TO TURN ON BLUETOOTH
    init_cli(&ble_cli, &bluetooth_io_funcs, "> ", debug_commands_funcs, '\n');

    // Filesystem
    filesystem_init_status = initialize_sd_card_fs();

    // Data logging service
    log_init_status = init_data_logger(&data_logger);

    struct data_logger_descriptor *logger = NULL;
    if ((filesystem_init_status == 0) && (log_init_status == 0)) {
        logger = &data_logger;
    }

    // ADC service
    init_adc(logger);

    // IMU SPI Interface
    nrfx_spim_config_t imu_spi_config = NRFX_SPIM_DEFAULT_CONFIG;
    imu_spi_config.ss_pin   = ICM_20948_SPI_SS_PIN;
    imu_spi_config.miso_pin = ICM_20948_SPI_MISO_PIN;
    imu_spi_config.mosi_pin = ICM_20948_SPI_MOSI_PIN;
    imu_spi_config.sck_pin  = ICM_20948_SPI_SCK_PIN;

    nrfx_spim_init(&imu_spi, &imu_spi_config, &icm_20948_spim_event_handler,
                   (void*)&imu);

    // IMU
    init_icm_20948(&imu, &imu_spi, ICM_20948_INT_PORT, ICM_20948_INT_PIN,
                   logger);

    uint32_t last_blink;

    /* Main Loop */
    for (;;) {
        if ((millis - last_blink) > 1000) {
            last_blink = millis;

//            NRF_P0->OUT ^= (1 << 22);
//            NRF_P0->OUT ^= (1 << 23);
//            NRF_P0->OUT ^= (1 << 24);
        }

        adc_service();

        icm_20948_service(&imu);

        usb_cdc_service();
        cli_service(&usb_cli);

        bluetooth_service();
        cli_service(&ble_cli);

        data_logger_service(&data_logger);

        __WFI();
    }

	return 0;
}

/* Interupt Service Routines */
void SysTick_Handler(void)
{
    millis++;
}

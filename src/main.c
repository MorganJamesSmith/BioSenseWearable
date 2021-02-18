#include "nordic_common.h"
#include "nrf.h"
#include "boards.h"
#include "app_timer.h"
#include "nrfx_spim.h"

#include "global.h"
#include "sensors.h"
#include "usb_serial.h"
#include "bluetooth.h"
#include "cli.h"
#include "cli_commands.h"
#include "adc.h"
#include "icm_20948.h"

// MARK: Variable Definitions
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




int main(void)
{
    ret_code_t ret;

    /* Enable SysTick for an interupt every millisecond */
    SysTick_Config(64000);

    /* Enable timer module */
    ret = app_timer_init();
    APP_ERROR_CHECK(ret);

    /* Configue IO */
    // RGB LED
//    NRF_P0->DIRSET = (1 << 22);
//    NRF_P0->DIRSET = (1 << 23);
//    NRF_P0->DIRSET = (1 << 24);
//
//    NRF_P0->OUTSET = (1 << 22);
//    NRF_P0->OUTSET = (1 << 23);
//    NRF_P0->OUTSET = (1 << 24);

    // Turn on power
    NRF_P1->DIRSET = (1 << 12);
    NRF_P1->OUT = (1 << 12);

    init_usb_cdc();
    init_cli(&usb_cli, &usb_io_funcs, "> ", debug_commands_funcs, '\r');

    //A call to "nrf_sdh_enable_request()" in bluetooth_init() kills USB
    //Use comments to turn bluetooth on and off for now until this is fixed
    //bluetooth_init();
    init_cli(&ble_cli, &bluetooth_io_funcs, "> ", debug_commands_funcs, '\n');

    init_adc();

    // IMU SPI Interface
    nrfx_spim_config_t imu_spi_config = NRFX_SPIM_DEFAULT_CONFIG;
    imu_spi_config.ss_pin   = ICM_20948_I2C_SS_PIN;
    imu_spi_config.miso_pin = ICM_20948_I2C_MISO_PIN;
    imu_spi_config.mosi_pin = ICM_20948_I2C_MOSI_PIN;
    imu_spi_config.sck_pin  = ICM_20948_I2C_SCK_PIN;

    nrfx_spim_init(&imu_spi, &imu_spi_config, &icm_20948_spim_event_handler,
                   (void*)&imu);

    // IMU
    init_icm_20948(&imu, &imu_spi, ICM_20948_INT_PORT, ICM_20948_INT_PIN);


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

        __WFI();
    }

	return 0;
}

/* Interupt Service Routines */
void SysTick_Handler(void)
{
    millis++;
}

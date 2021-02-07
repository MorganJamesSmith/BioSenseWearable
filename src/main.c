#include "nordic_common.h"
#include "nrf.h"
#include "boards.h"
#include "app_timer.h"

#include "usb_serial.h"
#include "bluetooth.h"
#include "cli.h"
#include "cli_commands.h"

// MARK: Variable Definitions
volatile uint32_t millis;

// MARK: ISR Prototypes
void SysTick_Handler(void);

const struct cli_io_funcs_t usb_io_funcs = {
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

struct cli_desc_t usb_cli;

const struct cli_io_funcs_t bluetooth_io_funcs = {
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

struct cli_desc_t ble_cli;

int main(void)
{
    ret_code_t ret;

    /* Enable SysTick for an interupt every millisecond */
    SysTick_Config(64000);

    /* Enable timer module */
    ret = app_timer_init();
    APP_ERROR_CHECK(ret);

    /* Configue IO */
    BSP_LED_0_PORT->DIRSET = BSP_LED_0_MASK;

    uint32_t last_blink = 0;

    init_usb_cdc();
    init_cli(&usb_cli, &usb_io_funcs, "> ", debug_commands_funcs, '\r');

    //A call to "nrf_sdh_enable_request()" in bluetooth_init() kills USB
    //Use comments to turn bluetooth on and off for now until this is fixed
    //bluetooth_init();
    init_cli(&ble_cli, &bluetooth_io_funcs, "> ", debug_commands_funcs, '\n');

    /* Main Loop */
    for (;;) {
        if ((millis - last_blink) > 1000) {
            last_blink = millis;
            BSP_LED_0_PORT->OUT ^= BSP_LED_0_MASK;
        }

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

#include "nordic_common.h"
#include "nrf.h"
#include "boards.h"
#include "app_timer.h"

#include "usb_serial.h"
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

struct cli_desc_t cli;

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
    init_cli(&cli, &usb_io_funcs, "> ", debug_commands_funcs, '\r');


    /* Main Loop */
    for (;;) {
        if ((millis - last_blink) > 1000) {
            last_blink = millis;
            BSP_LED_0_PORT->OUT ^= BSP_LED_0_MASK;
        }

        usb_cdc_service();
        cli_service(&cli);

        __WFI();
    }

	return 0;
}

/* Interupt Service Routines */
void SysTick_Handler(void)
{
    millis++;
}

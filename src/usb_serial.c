#include "usb_serial.h"

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <ctype.h>

#include "nrf.h"
#include "nrf_drv_usbd.h"
#include "nrf_drv_clock.h"
#include "nrf_delay.h"
#include "nrf_drv_power.h"

#include "app_error.h"
#include "app_util.h"
#include "app_usbd_core.h"
#include "app_usbd.h"
#include "app_usbd_string_desc.h"
#include "app_usbd_cdc_acm.h"
#include "app_usbd_serial_num.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "circular_buffer.h"

#define BTN_CDC_DATA_SEND       0
#define BTN_CDC_NOTIFY_SEND     1

#define BTN_CDC_DATA_KEY_RELEASE        (bsp_event_t)(BSP_EVENT_KEY_LAST + 1)

// Enable power USB detection
#ifndef USBD_POWER_DETECTION
#define USBD_POWER_DETECTION true
#endif


static void cdc_acm_user_ev_handler(app_usbd_class_inst_t const * p_inst,
                                    app_usbd_cdc_acm_user_event_t event);

#define CDC_ACM_COMM_INTERFACE  0
#define CDC_ACM_COMM_EPIN       NRF_DRV_USBD_EPIN2

#define CDC_ACM_DATA_INTERFACE  1
#define CDC_ACM_DATA_EPIN       NRF_DRV_USBD_EPIN1
#define CDC_ACM_DATA_EPOUT      NRF_DRV_USBD_EPOUT1


static uint8_t in_callback;
static uint16_t tx_length;

static void (*ready_callback)(void*);
static void *ready_callback_context;

/* Circular Buffers */
#define USB_CDC_BUFF_SIZE  128
static struct circular_buffer_t rx_circ_buff;
static struct circular_buffer_t tx_circ_buff;

static uint8_t rx_buff[USB_CDC_BUFF_SIZE];
static uint8_t tx_buff[USB_CDC_BUFF_SIZE];


/**
 * @brief CDC_ACM class instance
 * */
APP_USBD_CDC_ACM_GLOBAL_DEF(m_app_cdc_acm,
                            cdc_acm_user_ev_handler,
                            CDC_ACM_COMM_INTERFACE,
                            CDC_ACM_DATA_INTERFACE,
                            CDC_ACM_COMM_EPIN,
                            CDC_ACM_DATA_EPIN,
                            CDC_ACM_DATA_EPOUT,
                            APP_USBD_CDC_COMM_PROTOCOL_AT_V250
);

/**
 * @brief User event handler @ref app_usbd_cdc_acm_user_ev_handler_t (headphones)
 * */
static void cdc_acm_user_ev_handler(app_usbd_class_inst_t const * p_inst,
                                    app_usbd_cdc_acm_user_event_t event)
{
    in_callback = 1;

    static uint8_t read_buffer[NRF_DRV_USBD_EPSIZE];

    app_usbd_cdc_acm_t const * p_cdc_acm = app_usbd_cdc_acm_class_get(p_inst);

    switch (event)
    {
        case APP_USBD_CDC_ACM_USER_EVT_PORT_OPEN:
        {
            /*Setup first transfer*/
            ret_code_t ret = app_usbd_cdc_acm_read_any(p_cdc_acm,
                                                       read_buffer,
                                                       NRF_DRV_USBD_EPSIZE);
            UNUSED_VARIABLE(ret);
            if (ready_callback != NULL) {
                ready_callback(ready_callback_context);
            }
            break;
        }
        case APP_USBD_CDC_ACM_USER_EVT_PORT_CLOSE:
            break;
        case APP_USBD_CDC_ACM_USER_EVT_TX_DONE:
            // Advance circular buffer head to account for data that was just
            // written from it
            circular_buffer_move_head(&tx_circ_buff, tx_length);
            tx_length = 0;
            // If there is more data is the buffer to be send start sending it
            usb_cdc_service();
            break;
        case APP_USBD_CDC_ACM_USER_EVT_RX_DONE:
        {
            // We got some bytes! Copy them into our RX circular buffer and
            // echo any that need to be echoed.

            ret_code_t ret;
            do {
                size_t length = app_usbd_cdc_acm_rx_size(p_cdc_acm);
                for (size_t i = 0; i < length; i++) {
                    uint8_t data = read_buffer[i];
                    uint8_t full = 0;

                    if (!iscntrl(data) || (data == '\r')) {
                        // Should add byte to input buffer
                        full = circular_buffer_try_push(&rx_circ_buff, data);

                        if (!full && isprint(data)) {
                            // Echo
                            usb_cdc_put_char((char)data);
                        } else if (!full && (data == '\r')) {
                            // Echo newline
                            usb_cdc_put_char('\n');
                        }
                    } else if (data == 127) {
                        // Backspace
                        uint8_t empty = circular_buffer_unpush(&rx_circ_buff);

                        if (!empty) {
                            usb_cdc_put_string("\x1B[1D\x1B[K");
                        }
                    }
                }

                // Read more data
                ret = app_usbd_cdc_acm_read_any(p_cdc_acm, read_buffer,
                                            NRF_DRV_USBD_EPSIZE);
            } while (ret == NRF_SUCCESS);
            break;
        }
        default:
            break;
    }

    in_callback = 0;
}

static void usbd_user_ev_handler(app_usbd_event_type_t event)
{
    switch (event)
    {
        case APP_USBD_EVT_DRV_SUSPEND:
            //bsp_board_led_off(LED_USB_RESUME);
            break;
        case APP_USBD_EVT_DRV_RESUME:
            //bsp_board_led_on(LED_USB_RESUME);
            break;
        case APP_USBD_EVT_STARTED:
            break;
        case APP_USBD_EVT_STOPPED:
            app_usbd_disable();
            //bsp_board_leds_off();
            break;
        case APP_USBD_EVT_POWER_DETECTED:
            //NRF_LOG_INFO("USB power detected");

            if (!nrf_drv_usbd_is_enabled())
            {
                app_usbd_enable();
            }
            break;
        case APP_USBD_EVT_POWER_REMOVED:
            //NRF_LOG_INFO("USB power removed");
            app_usbd_stop();
            break;
        case APP_USBD_EVT_POWER_READY:
            //NRF_LOG_INFO("USB ready");
            app_usbd_start();
            break;
        default:
            break;
    }
}

void init_usb_cdc(void)
{
    ret_code_t ret;
    static const app_usbd_config_t usbd_config = {
        .ev_state_proc = usbd_user_ev_handler
    };

    /* Start the low frequency clock */
    ret = nrf_drv_clock_init();
    APP_ERROR_CHECK(ret);

    nrf_drv_clock_lfclk_request(NULL);

    while(!nrf_drv_clock_lfclk_is_running())
    {
        // Wait for low frequency clock to be running
    }

    /* Generate a serial number */
    app_usbd_serial_num_generate();

    /* Initialize USB */
    ret = app_usbd_init(&usbd_config);
    APP_ERROR_CHECK(ret);

    app_usbd_class_inst_t const * class_cdc_acm =
                            app_usbd_cdc_acm_class_inst_get(&m_app_cdc_acm);
    ret = app_usbd_class_append(class_cdc_acm);
    APP_ERROR_CHECK(ret);

    if (USBD_POWER_DETECTION) {
        ret = app_usbd_power_events_enable();
        APP_ERROR_CHECK(ret);
    } else {
        app_usbd_enable();
        app_usbd_start();
    }

    /* Initialize circular buffers */
    init_circular_buffer(&rx_circ_buff, rx_buff, USB_CDC_BUFF_SIZE);
    init_circular_buffer(&tx_circ_buff, tx_buff, USB_CDC_BUFF_SIZE);

    in_callback = 0;
    tx_length = 0;
}

void usb_cdc_service(void)
{
    /* Let the USB stack do its thing */
    if (!in_callback) {
        while (app_usbd_event_queue_process());
    }

    /* If there is data to be sent, start sending it */
    if (tx_length) {
        return;
    }

    // Find the head of the tx buffer and the number of contiguous bytes after
    // the head that we can send.
    uint8_t *head;
    uint16_t len = circular_buffer_get_head(&tx_circ_buff, &head);

    if (!len) {
        // No data to be sent
        return;
    }

    // Start sending data
    tx_length = len;
    app_usbd_cdc_acm_write(&m_app_cdc_acm, head, len);
}

void usb_cdc_set_ready_callback (void (*callback)(void*), void *context)
{
    ready_callback = callback;
    ready_callback_context = context;
}

uint16_t usb_cdc_put_string(const char *str)
{
    uint16_t i = 0;
    for (; str[i] != '\0'; i++) {
        // Make sure that we have enough space for the next character, or two
        // characters if the next character is a newline since we need to insert
        // a carriage return as well.
        uint16_t unused = circular_buffer_unused(&tx_circ_buff);
        if ((unused < 1) || ((str[i] == '\n') && (unused < 2))) {
            break;
        }

        circular_buffer_push(&tx_circ_buff, (uint8_t)str[i]);

        if (str[i] == '\n') {
            // Add carriage return as some terminal emulators seem to think that
            // they are typewriters.
            circular_buffer_push(&tx_circ_buff, (uint8_t)'\r');
        }
    }

    // Make sure that we start transmission right away if there is no
    // transmission already in progress.
    usb_cdc_service();

    return i;
}

void usb_cdc_put_string_blocking(const char *str)
{
    uint8_t carriage_return = 0;

    for (const char *i = str; *i != '\0';) {
        // Wait for a character worth of space to become avaliable in the buffer
        while (circular_buffer_is_full(&tx_circ_buff)) {
            // Make sure that we aren't waiting for a transaction which is not
            // in progress.
            usb_cdc_service();
        }

        if (carriage_return) {
            // Push a carriage return
            circular_buffer_push(&tx_circ_buff, (uint8_t)'\r');
        } else {
            // Push the next character
            circular_buffer_push(&tx_circ_buff, (uint8_t)*i);
        }

        if (*i == '\n' && !carriage_return) {
            // Add carriage return after newlines
            carriage_return = 1;
        } else {
            i++;
            carriage_return = 0;
        }
    }

    // Make sure that we start transmission right away if there is no
    // transmission already in progress.
    usb_cdc_service();
}

uint16_t usb_cdc_put_bytes(const uint8_t *bytes, uint16_t length)
{
    uint16_t i = 0;
    for (; i < length; i++) {
        if (circular_buffer_is_full(&tx_circ_buff)) {
            break;
        }

        circular_buffer_push(&tx_circ_buff, (uint8_t)bytes[i]);
    }

    // Make sure that we start transmission right away if there is no
    // transmission already in progress.
    usb_cdc_service();

    return i;
}

void usb_cdc_put_bytes_blocking(const uint8_t *bytes, uint16_t length)
{
    for (uint16_t i = 0; i < length; i++) {
        // Wait for a character worth of space to become avaliable in the buffer
        while (circular_buffer_is_full(&tx_circ_buff)) {
            // Make sure that we aren't waiting for a transaction which is not
            // in progress.
            usb_cdc_service();
        }

        circular_buffer_push(&tx_circ_buff, bytes[i]);
    }

    // Make sure that we start transmission right away if there is no
    // transmission already in progress.
    usb_cdc_service();
}

void usb_cdc_put_char (const char c)
{
    circular_buffer_push(&tx_circ_buff, (uint8_t)c);

    if (c == '\n') {
        // Add carriage return as some terminal emulators seem to think that
        // they are typewriters.
        circular_buffer_push(&tx_circ_buff, (uint8_t)'\r');
    }

    // Make sure that we start transmition right away if there is no transmition
    // already in progress.
    usb_cdc_service();
}

void usb_cdc_get_string (char *str, uint16_t len)
{
    for (uint16_t i = 0; i < (len - 1); i++) {
        uint8_t pop_failed = circular_buffer_pop(&rx_circ_buff,
                                                 (uint8_t*)(str + i));

        if (pop_failed) {
            str[i] = '\0';
            return;
        }
    }
    // Make sure that string is terminated.
    str[len - 1] = '\0';
}

uint8_t usb_cdc_has_delim (char delim)
{
    return circular_buffer_has_char(&rx_circ_buff, delim);
}

void usb_cdc_get_line_delim (char delim, char *str, uint16_t len)
{
    for (uint16_t i = 0; i < (len - 1); i++) {
        uint8_t pop_failed = circular_buffer_pop(&rx_circ_buff,
                                                 (uint8_t*)(str + i));

        if (pop_failed || str[i] == delim) {
            str[i] = '\0';
            return;
        }
    }
    // Make sure that string is terminated.
    str[len - 1] = '\0';
}

uint8_t usb_cdc_has_line (void)
{
    return circular_buffer_has_line(&rx_circ_buff);
}

void usb_cdc_get_line (char *str, uint16_t len)
{
    uint8_t last_char_cr = 0;
    for (uint16_t i = 0; i < (len - 1); i++) {
        uint8_t pop_failed = circular_buffer_pop(&rx_circ_buff,
                                                 (uint8_t*)(str + i));

        if (pop_failed) {
            str[i] = '\0';
            return;
        } else if (last_char_cr && str[i] == '\n') {
            str[i - 1] = '\0';
            return;
        }

        last_char_cr = str[i] == '\r';
    }

    // We ran out of space in the buffer to pop the next character, we might
    // have just popped a carriage return, and the next character might be a
    // newline, in which case we can pop the newline even though the buffer is
    // full since we don't need to put it in our buffer
    uint8_t c;
    if (last_char_cr && !circular_buffer_peak(&rx_circ_buff, &c)) {
        if (c == '\n') {
            circular_buffer_pop(&rx_circ_buff, &c);
        }
    }

    // Make sure that string is terminated.
    str[len - 1] = '\0';
}

char usb_cdc_get_char (void)
{
    char c = '\0';
    circular_buffer_pop(&rx_circ_buff, (uint8_t*)&c);
    return c;
}

uint8_t usb_cdc_out_buffer_empty (void)
{
    return circular_buffer_is_empty(&rx_circ_buff);
}

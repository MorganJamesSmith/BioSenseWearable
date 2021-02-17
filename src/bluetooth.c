#include "bluetooth.h"

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <ctype.h>

#include "ble_hci.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "ble_conn_params.h"
#include "nrf_sdh.h"
#include "nrf_sdh_soc.h"
#include "nrf_sdh_ble.h"
#include "nrf_ble_gatt.h"
#include "nrf_ble_qwr.h"
#include "ble_nus.h"
#include "app_util_platform.h"
#include "app_timer.h"
#include "circular_buffer.h"

#define APP_BLE_CONN_CFG_TAG            1                                           /**< A tag identifying the SoftDevice BLE configuration. */

#define DEVICE_NAME                     "Project21"                                 /**< Name of device. Will be included in the advertising data. */
#define NUS_SERVICE_UUID_TYPE           BLE_UUID_TYPE_VENDOR_BEGIN                  /**< UUID type for the Nordic UART Service (vendor specific). */

#define APP_BLE_OBSERVER_PRIO           3                                           /**< Application's BLE observer priority. You shouldn't need to modify this value. */

#define APP_ADV_INTERVAL                64                                          /**< The advertising interval (in units of 0.625 ms. This value corresponds to 40 ms). */

#define APP_ADV_DURATION                18000                                       /**< The advertising duration (180 seconds) in units of 10 milliseconds. */

#define MIN_CONN_INTERVAL               MSEC_TO_UNITS(20, UNIT_1_25_MS)             /**< Minimum acceptable connection interval (20 ms), Connection interval uses 1.25 ms units. */
#define MAX_CONN_INTERVAL               MSEC_TO_UNITS(75, UNIT_1_25_MS)             /**< Maximum acceptable connection interval (75 ms), Connection interval uses 1.25 ms units. */
#define SLAVE_LATENCY                   0                                           /**< Slave latency. */
#define CONN_SUP_TIMEOUT                MSEC_TO_UNITS(4000, UNIT_10_MS)             /**< Connection supervisory timeout (4 seconds), Supervision Timeout uses 10 ms units. */
#define FIRST_CONN_PARAMS_UPDATE_DELAY  APP_TIMER_TICKS(5000)                       /**< Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (5 seconds). */
#define NEXT_CONN_PARAMS_UPDATE_DELAY   APP_TIMER_TICKS(30000)                      /**< Time between each call to sd_ble_gap_conn_param_update after the first call (30 seconds). */
#define MAX_CONN_PARAMS_UPDATE_COUNT    3

BLE_NUS_DEF(m_nus, NRF_SDH_BLE_TOTAL_LINK_COUNT);                                   /**< BLE NUS service instance. */
NRF_BLE_GATT_DEF(m_gatt);                                                           /**< GATT module instance. */
NRF_BLE_QWR_DEF(m_qwr);                                                             /**< Context for the Queued Write module.*/
BLE_ADVERTISING_DEF(m_advertising);

static uint16_t   m_conn_handle          = BLE_CONN_HANDLE_INVALID;                 /**< Handle of the current connection. */
static uint16_t   m_ble_nus_max_data_len = BLE_GATT_ATT_MTU_DEFAULT - 3;            /**< Maximum length of data (in bytes) that can be transmitted to the peer by the Nordic UART service module. */
static ble_uuid_t m_adv_uuids[]          =                                          /**< Universally unique service identifier. */
{
    {BLE_UUID_NUS_SERVICE, NUS_SERVICE_UUID_TYPE}
};

static void (*connected_callback)();
static void (*disconnected_callback)();
static void (*ready_callback)(void*);
static void *ready_callback_context;

static bool ble_uart_busy;

/* Circular Buffers */
#define BLUETOOTH_BUFF_SIZE  128
static struct circular_buffer_t rx_circ_buff;
static struct circular_buffer_t tx_circ_buff;

static uint8_t rx_buff[BLUETOOTH_BUFF_SIZE];
static uint8_t tx_buff[BLUETOOTH_BUFF_SIZE];

/**@brief Function for the GAP initialization.
 *
 * @details This function will set up all the necessary GAP (Generic Access Profile) parameters of
 *          the device. It also sets the permissions and appearance.
 */
static void gap_params_init(void)
{
    uint32_t                err_code;
    ble_gap_conn_params_t   gap_conn_params;
    ble_gap_conn_sec_mode_t sec_mode;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

    err_code = sd_ble_gap_device_name_set(&sec_mode,
                                          (const uint8_t *) DEVICE_NAME,
                                          strlen(DEVICE_NAME));
    APP_ERROR_CHECK(err_code);

    memset(&gap_conn_params, 0, sizeof(gap_conn_params));

    gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
    gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
    gap_conn_params.slave_latency     = SLAVE_LATENCY;
    gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

    err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for handling Queued Write Module errors.
 *
 * @details A pointer to this function will be passed to each service which may need to inform the
 *          application about an error.
 *
 * @param[in]   nrf_error   Error code containing information about what went wrong.
 */
static void nrf_qwr_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}

/**@brief Function for handling the data from the Nordic UART Service.
 *
 * @details This function will process the data received from the Nordic UART BLE Service and send
 *          it to the UART module.
 *
 * @param[in] p_evt       Nordic UART Service event.
 */
static void nus_data_handler(ble_nus_evt_t * p_evt)
{
    if (p_evt->type == BLE_NUS_EVT_RX_DATA)
    {
        for (uint32_t i = 0; i < p_evt->params.rx_data.length; i++)
        {
            uint8_t data = p_evt->params.rx_data.p_data[i];
            uint8_t full = 0;

            if (!iscntrl(data) || (data == '\n')) 
            {
                // Should add byte to input buffer
                full = circular_buffer_try_push(&rx_circ_buff, data);

                if (!full && isprint(data)) //Don't need this for the nordic uart app
                {
                    // Echo
                    bluetooth_put_char((char)data);
                } else if (!full && (data == '\n'))
                {
                    //Echo newline
                    bluetooth_put_char('\n');
                }
            }
            else if (data == 127) //Don't need this for the nordic uart app
            {
                // Backspace
                uint8_t empty = circular_buffer_unpush(&rx_circ_buff);

                if (!empty)
                {
                    bluetooth_put_string("\x1B[1D\x1B[K");
                }
            }

            //Echo everything back, bypass CLI
            /*do
            {
                err_code = ble_nus_data_send(&m_nus, p_evt->params.rx_data.p_data, &p_evt->params.rx_data.length, m_conn_handle);
                if ((err_code != NRF_ERROR_INVALID_STATE) &&
                    (err_code != NRF_ERROR_RESOURCES) &&
                    (err_code != NRF_ERROR_NOT_FOUND))
                {
                    APP_ERROR_CHECK(err_code);
                }
            } while (err_code == NRF_ERROR_RESOURCES);*/
        }
    }
    else if (p_evt->type == BLE_NUS_EVT_TX_RDY)
    {
        ble_uart_busy = false;
        bluetooth_service();
    }
}

/**@brief Function for initializing services that will be used by the application. */
static void services_init(void)
{
    uint32_t           err_code;
    ble_nus_init_t     nus_init;
    nrf_ble_qwr_init_t qwr_init = {0};

    // Initialize Queued Write Module.
    qwr_init.error_handler = nrf_qwr_error_handler;

    err_code = nrf_ble_qwr_init(&m_qwr, &qwr_init);
    APP_ERROR_CHECK(err_code);

    // Initialize NUS.
    memset(&nus_init, 0, sizeof(nus_init));

    nus_init.data_handler = nus_data_handler;

    err_code = ble_nus_init(&m_nus, &nus_init);
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for handling an event from the Connection Parameters Module.
 *
 * @details This function will be called for all events in the Connection Parameters Module
 *          which are passed to the application.
 *
 * @note All this function does is to disconnect. This could have been done by simply setting
 *       the disconnect_on_fail config parameter, but instead we use the event handler
 *       mechanism to demonstrate its use.
 *
 * @param[in] p_evt  Event received from the Connection Parameters Module.
 */
static void on_conn_params_evt(ble_conn_params_evt_t * p_evt)
{
    uint32_t err_code;

    if (p_evt->evt_type == BLE_CONN_PARAMS_EVT_FAILED)
    {
        err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
        APP_ERROR_CHECK(err_code);
    }
}

/**@brief Function for handling errors from the Connection Parameters module.
 *
 * @param[in] nrf_error  Error code containing information about what went wrong.
 */
static void conn_params_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}

/**@brief Function for initializing the Connection Parameters module. */
static void conn_params_init(void)
{
    uint32_t               err_code;
    ble_conn_params_init_t cp_init;

    memset(&cp_init, 0, sizeof(cp_init));

    cp_init.p_conn_params                  = NULL;
    cp_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
    cp_init.next_conn_params_update_delay  = NEXT_CONN_PARAMS_UPDATE_DELAY;
    cp_init.max_conn_params_update_count   = MAX_CONN_PARAMS_UPDATE_COUNT;
    cp_init.start_on_notify_cccd_handle    = BLE_GATT_HANDLE_INVALID;
    cp_init.disconnect_on_fail             = false;
    cp_init.evt_handler                    = on_conn_params_evt;
    cp_init.error_handler                  = conn_params_error_handler;

    err_code = ble_conn_params_init(&cp_init);
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for handling BLE events.
 *
 * @param[in]   p_ble_evt   Bluetooth stack event.
 * @param[in]   p_context   Unused.
 */
static void ble_evt_handler(ble_evt_t const * p_ble_evt, void * p_context)
{
    uint32_t err_code;

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
            err_code = nrf_ble_qwr_conn_handle_assign(&m_qwr, m_conn_handle);
            APP_ERROR_CHECK(err_code);
            if(err_code == NRF_SUCCESS && connected_callback != NULL)
            {
                connected_callback();
                
            }
            if(err_code == NRF_SUCCESS && ready_callback != NULL)
            {
                ready_callback(ready_callback_context);
            }
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            m_conn_handle = BLE_CONN_HANDLE_INVALID;
            if(disconnected_callback != NULL)
            {
                disconnected_callback();
            }
            break;

        case BLE_GAP_EVT_PHY_UPDATE_REQUEST:
        {
            ble_gap_phys_t const phys =
            {
                .rx_phys = BLE_GAP_PHY_AUTO,
                .tx_phys = BLE_GAP_PHY_AUTO,
            };
            err_code = sd_ble_gap_phy_update(p_ble_evt->evt.gap_evt.conn_handle, &phys);
            APP_ERROR_CHECK(err_code);
        } break;

        case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
            // Pairing not supported
            err_code = sd_ble_gap_sec_params_reply(m_conn_handle, BLE_GAP_SEC_STATUS_PAIRING_NOT_SUPP, NULL, NULL);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GATTS_EVT_SYS_ATTR_MISSING:
            // No system attributes have been stored.
            err_code = sd_ble_gatts_sys_attr_set(m_conn_handle, NULL, 0, 0);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GATTC_EVT_TIMEOUT:
            // Disconnect on GATT Client timeout event.
            err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gattc_evt.conn_handle,
                                             BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(err_code);
            if(disconnected_callback != NULL)
            {
                disconnected_callback();
            }
            break;

        case BLE_GATTS_EVT_TIMEOUT:
            // Disconnect on GATT Server timeout event.
            err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gatts_evt.conn_handle,
                                             BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(err_code);
            if(disconnected_callback != NULL)
            {
                disconnected_callback();
            }
            break;

        default:
            // No implementation needed.
            break;
    }

    ble_nus_on_ble_evt(p_ble_evt, p_context);
}

/**@brief Function for the SoftDevice initialization.
 *
 * @details This function initializes the SoftDevice and the BLE event interrupt.
 */
static void ble_stack_init(void)
{
    ret_code_t err_code;

    err_code = nrf_sdh_enable_request();
    APP_ERROR_CHECK(err_code);

    // Configure the BLE stack using the default settings.
    // Fetch the start address of the application RAM.
    uint32_t ram_start = 0;
    err_code = nrf_sdh_ble_default_cfg_set(APP_BLE_CONN_CFG_TAG, &ram_start);
    APP_ERROR_CHECK(err_code);

    // Enable BLE stack.
    err_code = nrf_sdh_ble_enable(&ram_start);
    APP_ERROR_CHECK(err_code);

    // Register a handler for BLE events.
    NRF_SDH_BLE_OBSERVER(m_ble_observer, APP_BLE_OBSERVER_PRIO, ble_evt_handler, NULL);
}

/**@brief Function for handling events from the GATT library. */
void gatt_evt_handler(nrf_ble_gatt_t * p_gatt, nrf_ble_gatt_evt_t const * p_evt)
{
    if ((m_conn_handle == p_evt->conn_handle) && (p_evt->evt_id == NRF_BLE_GATT_EVT_ATT_MTU_UPDATED))
    {
        m_ble_nus_max_data_len = p_evt->params.att_mtu_effective - OPCODE_LENGTH - HANDLE_LENGTH;
    }
}

/**@brief Function for initializing the GATT library. */
void gatt_init(void)
{
    ret_code_t err_code;

    err_code = nrf_ble_gatt_init(&m_gatt, gatt_evt_handler);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_ble_gatt_att_mtu_periph_set(&m_gatt, NRF_SDH_BLE_GATT_MAX_MTU_SIZE);
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for initializing the Advertising functionality. */
static void advertising_init(void)
{
    uint32_t               err_code;
    ble_advertising_init_t init;

    memset(&init, 0, sizeof(init));

    init.advdata.name_type          = BLE_ADVDATA_FULL_NAME;
    init.advdata.include_appearance = false;
    init.advdata.flags              = BLE_GAP_ADV_FLAGS_LE_ONLY_LIMITED_DISC_MODE;

    init.srdata.uuids_complete.uuid_cnt = sizeof(m_adv_uuids) / sizeof(m_adv_uuids[0]);
    init.srdata.uuids_complete.p_uuids  = m_adv_uuids;

    init.config.ble_adv_fast_enabled  = true;
    init.config.ble_adv_fast_interval = APP_ADV_INTERVAL;
    init.config.ble_adv_fast_timeout  = APP_ADV_DURATION;

    err_code = ble_advertising_init(&m_advertising, &init);
    APP_ERROR_CHECK(err_code);

    ble_advertising_conn_cfg_tag_set(&m_advertising, APP_BLE_CONN_CFG_TAG);
}

/**@brief Function for starting advertising. */
static void advertising_start(void)
{
    uint32_t err_code = ble_advertising_start(&m_advertising, BLE_ADV_MODE_FAST);
    APP_ERROR_CHECK(err_code);
}

void bluetooth_init(void)
{
    ble_uart_busy = false;
    ble_stack_init();
    gap_params_init();
    gatt_init();
    services_init();
    advertising_init();
    conn_params_init();
    advertising_start();

    /* Initialize circular buffers */
    init_circular_buffer(&rx_circ_buff, rx_buff, BLUETOOTH_BUFF_SIZE);
    init_circular_buffer(&tx_circ_buff, tx_buff, BLUETOOTH_BUFF_SIZE);
}

void bluetooth_service(void)
{
    // Find the head of the tx buffer and the number of contiguous bytes after
    // the head that we can send.
    uint8_t *head;
    uint16_t len = circular_buffer_get_head(&tx_circ_buff, &head);

    if (!len)
        return;

    if(len > BLE_NUS_MAX_DATA_LEN)
        len = BLE_NUS_MAX_DATA_LEN;

    // Start sending data
    uint32_t err_code;
    err_code = ble_nus_data_send(&m_nus, head, &len, m_conn_handle);
    if(err_code != NRF_SUCCESS)
    {
        if ((err_code != NRF_ERROR_INVALID_STATE) &&
            (err_code != NRF_ERROR_RESOURCES) &&
            (err_code != NRF_ERROR_NOT_FOUND))
        {
            APP_ERROR_CHECK(err_code);
        }
        return;
    }
    circular_buffer_move_head(&tx_circ_buff, len);
}

void bluetooth_set_connected_callback (void (*callback)(void*))
{
    connected_callback = callback;
}

void bluetooth_set_disconnected_callback (void (*callback)(void*))
{
    disconnected_callback = callback;
}

void bluetooth_set_ready_callback (void (*callback)(void*), void *context)
{
    ready_callback = callback;
    ready_callback_context = context;
}

uint16_t bluetooth_put_string(const char *str)
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
    bluetooth_service();

    return i;
}

void bluetooth_put_string_blocking(const char *str)
{
    uint8_t carriage_return = 0;

    for (const char *i = str; *i != '\0';) {
        // Wait for a character worth of space to become avaliable in the buffer
        while (circular_buffer_is_full(&tx_circ_buff)) {
            // Make sure that we aren't waiting for a transaction which is not
            // in progress.
            bluetooth_service();
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
    bluetooth_service();
}

uint16_t bluetooth_put_bytes(const uint8_t *bytes, uint16_t length)
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
    bluetooth_service();

    return i;
}

void bluetooth_put_bytes_blocking(const uint8_t *bytes, uint16_t length)
{
    for (uint16_t i = 0; i < length; i++) {
        // Wait for a character worth of space to become avaliable in the buffer
        while (circular_buffer_is_full(&tx_circ_buff)) {
            // Make sure that we aren't waiting for a transaction which is not
            // in progress.
            bluetooth_service();
        }

        circular_buffer_push(&tx_circ_buff, bytes[i]);
    }

    // Make sure that we start transmission right away if there is no
    // transmission already in progress.
    bluetooth_service();
}

void bluetooth_put_char (const char c)
{
    circular_buffer_push(&tx_circ_buff, (uint8_t)c);

    if (c == '\n') {
        // Add carriage return as some terminal emulators seem to think that
        // they are typewriters.
        circular_buffer_push(&tx_circ_buff, (uint8_t)'\r');
    }

    // Make sure that we start transmition right away if there is no transmition
    // already in progress.
    bluetooth_service();
}

void bluetooth_get_string (char *str, uint16_t len)
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

uint8_t bluetooth_has_delim (char delim)
{
    return circular_buffer_has_char(&rx_circ_buff, delim);
}

void bluetooth_get_line_delim (char delim, char *str, uint16_t len)
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

uint8_t bluetooth_has_line (void)
{
    return circular_buffer_has_line(&rx_circ_buff);
}

void bluetooth_get_line (char *str, uint16_t len)
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

char bluetooth_get_char (void)
{
    char c = '\0';
    circular_buffer_pop(&rx_circ_buff, (uint8_t*)&c);
    return c;
}

uint8_t bluetooth_out_buffer_empty (void)
{
    return circular_buffer_is_empty(&rx_circ_buff);
}

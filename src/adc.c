#include "adc.h"

#include <string.h>

#include <nrfx_saadc.h>

#include "global.h"

#define ADC_NUM_CHANNELS        4
#define ADC_SAMPLE_PERIOD       500

static nrf_saadc_value_t adc_buffers[2][ADC_NUM_CHANNELS];
static uint8_t adc_calibration_complete;
static uint8_t adc_sample_in_progress;
static uint8_t adc_samples_pending;
static uint32_t adc_last_sample_time;

static nrf_saadc_value_t adc_last_samples[ADC_NUM_CHANNELS];


struct adc_val_lut_entry {
    int32_t v;
    int16_t t;
};

static int16_t linear_interp(int32_t v, const struct adc_val_lut_entry *lut,
                             int num_lut_entries)
{
    // Check that voltage we measured is within the bounds of our LUT
    if (v <= lut[0].v) {
        return lut[0].t;
    } else if (v >= lut[num_lut_entries - 1].v) {
        return lut[num_lut_entries - 1].t;
    }

    // Find the first LUT entry with a voltage greater than the one we measured
    int upper_bound_index = 1;
    for (; upper_bound_index < (num_lut_entries - 1);
         upper_bound_index++) {
        if (lut[upper_bound_index].v >= v) {
            break;
        }
    }

    // Interpolate between the entry we found and the previous one
    // t = ((l.t * (u.v - v)) + (u.t * (v - l.v)))/(u.v - l.v)

    int32_t numerator = (lut[upper_bound_index - 1].t *
                         (lut[upper_bound_index].v - v)) +
                        (lut[upper_bound_index].t *
                         (v - lut[upper_bound_index - 1].v));
    int32_t denominator = (lut[upper_bound_index].v -
                           lut[upper_bound_index - 1].v);

    return (int16_t)(numerator / denominator);
}

static int16_t adc_sample_to_ir_temp(nrf_saadc_value_t sample)
{
    // Calculate sample voltage (in mV)
    int32_t sample_volts = ((int32_t)sample * 1000) / 6827;
    // (6827 = ((GAIN/REF) * 2^RES) = ((0.25/0.6V) * 2^14))

    // Remove reference voltage offset
    sample_volts -= 1024;

    // Note that the voltages below are multiplied byteh 80x gain of the amp
    static const struct adc_val_lut_entry ir_temp_lut[] = {
        { .v = -180, .t = 0 },
        { .v = 0, .t = 2500 },
        { .v = 160, .t = 4500 },
        { .v = 240, .t = 5500 },
        { .v = 320, .t = 6250 },
        { .v = 480, .t = 7500 },
        { .v = 800, .t = 10000 }
    };

    return linear_interp(sample_volts, ir_temp_lut,
                         sizeof(ir_temp_lut)/sizeof(ir_temp_lut[0]));
}

static int16_t adc_sample_to_ambient_temp(nrf_saadc_value_t sample)
{
    // Calculate sample voltage (in mV)
    const int32_t sample_volts = ((int32_t)sample * 1000) / 13653;
    // (13653 = ((GAIN/REF) * 2^RES) = ((0.5/0.6V) * 2^14))

    static const struct adc_val_lut_entry ambient_temp_lut[] = {
        { .v = 0, .t = -2000 },
        { .v = 18, .t = -2000 },
        { .v = 24, .t = -1500 },
        { .v = 32, .t = -1000 },
        { .v = 42, .t = -500 },
        { .v = 53, .t = 0 },
        { .v = 68, .t = 500  },
        { .v = 86, .t = 1000 },
        { .v = 107, .t = 1500 },
        { .v = 133, .t = 2000 },
        { .v = 200, .t = 2500 },
        { .v = 198, .t = 3000 },
        { .v = 239, .t = 3500 },
        { .v = 285, .t = 4000 },
        { .v = 340, .t = 4500 },
        { .v = 393, .t = 5000 },
        { .v = 454, .t = 5550 },
        { .v = 521, .t = 6000 },
        { .v = 590, .t = 6500 },
        { .v = 662, .t = 7000 },
        { .v = 736, .t = 7500 },
        { .v = 811, .t = 8000 },
        { .v = 885, .t = 8500 },
        { .v = 958, .t = 9000 },
        { .v = 1028, .t = 9500 },
        { .v = 1096, .t = 10000 }
    };

    return linear_interp(sample_volts, ambient_temp_lut,
                         sizeof(ambient_temp_lut)/sizeof(ambient_temp_lut[0]));
}

static int16_t adc_sample_to_battery_voltage(nrf_saadc_value_t sample)
{
    // Calculate sample voltage (in mV)
    const int32_t sample_volts = ((int32_t)sample * 11000) / 27307;
    // (27307 = ((GAIN/REF) * 2^RES) = ((1/0.6V) * 2^14))
    // Battery voltage is sampled through a voltage divider with a 100 KΩ
    // resistor and a 10 KΩ resistor. So the actual voltage is 11 times more
    // than the measured voltage, that's why we use 11000 instead of 1000 in the
    // line above

    return (int16_t)sample_volts;
}

static int16_t adc_sample_to_supply_voltage(nrf_saadc_value_t sample)
{
    // Calculate sample voltage (in mV)
    const int32_t sample_volts = ((int32_t)sample * 1000) / 6827;
    // (6827 = ((GAIN/REF) * 2^RES) = ((0.25/0.6V) * 2^14))

    return (int16_t)sample_volts;
}






static void saadc_callback(nrfx_saadc_evt_t const * p_event)
{
    ret_code_t err_code;

    if (p_event->type == NRFX_SAADC_EVT_CALIBRATEDONE) {
        // Calibration complete, set flag
        adc_calibration_complete = 1;

        // Queue sample buffers
        err_code = nrfx_saadc_buffer_convert(adc_buffers[0], ADC_NUM_CHANNELS);
        if (err_code != NRFX_SUCCESS) {
            return;
        }
        err_code = nrfx_saadc_buffer_convert(adc_buffers[1], ADC_NUM_CHANNELS);
        if (err_code != NRFX_SUCCESS) {
            return;
        }
    } else if (p_event->type == NRFX_SAADC_EVT_DONE) {
        memcpy(adc_last_samples, p_event->data.done.p_buffer,
               sizeof(nrf_saadc_value_t) * ADC_NUM_CHANNELS);
        adc_last_sample_time = millis;
        adc_samples_pending = 0;
        adc_sample_in_progress = 0;

        err_code = nrfx_saadc_buffer_convert(p_event->data.done.p_buffer,
                                             ADC_NUM_CHANNELS);
        if (err_code != NRFX_SUCCESS) {
            return;
        }
    }
}





int init_adc(void)
{
    ret_code_t err_code;

    nrfx_saadc_config_t config = NRFX_SAADC_DEFAULT_CONFIG;
    config.resolution = NRF_SAADC_RESOLUTION_14BIT;
    err_code = nrfx_saadc_init(&config, saadc_callback);
    if (err_code != NRFX_SUCCESS) {
        return -1;
    }

    // Channel 0: AIN 0 - IR Temp
    nrf_saadc_channel_config_t channel_0_config = (nrf_saadc_channel_config_t)NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN0);
    channel_0_config.gain = NRF_SAADC_GAIN1_4;
    channel_0_config.acq_time = NRF_SAADC_ACQTIME_40US;
    err_code = nrfx_saadc_channel_init(0, &channel_0_config);
    if (err_code != NRFX_SUCCESS) {
        return -1;
    }

    // Channel 1: AIN 1 - Ambient Temp
    nrf_saadc_channel_config_t channel_1_config  = (nrf_saadc_channel_config_t)NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN1);
    channel_1_config.gain = NRF_SAADC_GAIN1_2;
    channel_1_config.acq_time = NRF_SAADC_ACQTIME_40US;
    err_code = nrfx_saadc_channel_init(1, &channel_1_config);
    if (err_code != NRFX_SUCCESS) {
        return -1;
    }

    // Channel 2: AIN 2 - Battery Voltage
    nrf_saadc_channel_config_t channel_2_config  = (nrf_saadc_channel_config_t)NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN2);
    channel_2_config.gain = NRF_SAADC_GAIN1;
    channel_2_config.acq_time = NRF_SAADC_ACQTIME_40US;
    err_code = nrfx_saadc_channel_init(2, &channel_2_config);
    if (err_code != NRFX_SUCCESS) {
        return -1;
    }

    // Channel 3: VDD - Supply Voltage
    nrf_saadc_channel_config_t channel_3_config  = (nrf_saadc_channel_config_t)NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_VDD);
    channel_3_config.gain = NRF_SAADC_GAIN1_4;
    err_code = nrfx_saadc_channel_init(3, &channel_3_config);
    if (err_code != NRFX_SUCCESS) {
        return -1;
    }

    // Start callibration
    adc_calibration_complete = 0;
    err_code = nrfx_saadc_calibrate_offset();
    if (err_code != NRFX_SUCCESS) {
        return -1;
    }

    adc_last_sample_time = 0;

    return 0;
}

void adc_service(void)
{
    if (adc_calibration_complete && !adc_sample_in_progress &&
            ((millis - adc_last_sample_time) > ADC_SAMPLE_PERIOD)) {
        adc_sample_in_progress = 1;
        // Start a conversion
        nrfx_saadc_sample();
    }

    if (adc_samples_pending) {
        // Calculate values from samples
        uint16_t ir_temp = adc_sample_to_ir_temp(adc_last_samples[0]);
        uint16_t ambient_temp = adc_sample_to_ambient_temp(adc_last_samples[1]);
        uint16_t v_bat = adc_sample_to_battery_voltage(adc_last_samples[2]);
        uint16_t vdd = adc_sample_to_supply_voltage(adc_last_samples[3]);

        // Do something with the samples
        (void)ir_temp;
        (void)ambient_temp;
        (void)v_bat;
        (void)vdd;

        adc_samples_pending = 0;
    }
}

int16_t adc_most_recent_ir_temp(void)
{
    return adc_sample_to_ir_temp(adc_last_samples[0]);
}

int16_t adc_most_recent_ambient_temp(void)
{
    return adc_sample_to_ambient_temp(adc_last_samples[1]);
}

int16_t adc_most_recent_battery_voltage(void)
{
    return adc_sample_to_battery_voltage(adc_last_samples[2]);
}

int16_t adc_most_recent_supply_voltage(void)
{
    return adc_sample_to_supply_voltage(adc_last_samples[3]);
}

uint32_t adc_get_last_measurment_time(void)
{
    return adc_last_sample_time;
}

int32_t adc_get_channel_0_volts(void)
{
    return (((int32_t)adc_last_samples[0] * 1000) / 6827);
}

int32_t adc_get_channel_1_volts(void)
{
    return (((int32_t)adc_last_samples[1] * 1000) / 13653);
}

int32_t adc_get_channel_2_volts(void)
{
    return (((int32_t)adc_last_samples[2] * 1000) / 27307);
}

int32_t adc_get_channel_3_volts(void)
{
    return (((int32_t)adc_last_samples[3] * 1000) / 6827);
}

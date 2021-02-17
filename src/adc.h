#ifndef adc_h
#define adc_h

#include <stdint.h>

extern int init_adc(void);

extern void adc_service(void);

extern int16_t adc_most_recent_ir_temp(void);

extern int16_t adc_most_recent_ambient_temp(void);

extern int16_t adc_most_recent_battery_voltage(void);

extern int16_t adc_most_recent_supply_voltage(void);

extern uint32_t adc_get_last_measurment_time(void);

extern int32_t adc_get_channel_0_volts(void);

extern int32_t adc_get_channel_1_volts(void);

extern int32_t adc_get_channel_2_volts(void);

extern int32_t adc_get_channel_3_volts(void);

#endif /* adc_h */

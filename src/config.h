#ifndef config_h
#define config_h

#include "global.h"
#include "icm_20948.h"

//
//
//  SD Card
//
//
#define ENABLE_SD_CARD

#define SDC_SCK_PIN     19
#define SDC_MOSI_PIN    21
#define SDC_MISO_PIN    20
#define SDC_CS_PIN      17


//
//
//  Bluetooth
//
//
#define ENABLE_BLUETOOTH


//
//
//  IMU
//
//
#define ENABLE_IMU

#define IMU_PERIOD MS_TO_MILLIS(100)

#ifdef ENABLE_IMU
// Sensor driver instances go here so that we can use them in CLI commands
extern struct icm_20948_desc imu;
#endif


//
//
//  ADC
//
//
#define ENABLE_ADC

#define ADC_PERIOD MS_TO_MILLIS(500)


#endif /* config_h */

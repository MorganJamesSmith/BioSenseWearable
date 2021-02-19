/*
 * Driver for the Maxim MAX86150 ECG and PPG integrated sensor
 *
 *
 * Based off of the following Arduino library:
 *
 *   Written by Ashwin Whitchurch, ProtoCentral Electronics (www.protocentral.com)
 *
 *   https://github.com/protocentral/protocentral_max86150
 *
 * Based on code written by Peter Jansen and Nathan Seidle (SparkFun) for the
 * MAX30105 sensor BSD license, all text above must be included in any
 * redistribution.
 */

#ifndef max86150_h
#define max86150_h

#include <stdint.h>
#include <stdbool.h>

#define MAX86150_ADDRESS          0x5E //7-bit I2C Address
//Note that MAX30102 has the same I2C address and Part ID

bool max86150_begin(TwoWire &wirePort = Wire, uint32_t i2cSpeed = I2C_SPEED_STANDARD, uint8_t i2caddr = MAX86150_ADDRESS);
void init_max86150(const nrfx_spim_t *spi);

uint32_t max86150_getRed(void); //Returns immediate red value
uint32_t max86150_getIR(void);  //Returns immediate IR value
int32_t max86150_getECG(void);  //Returns immediate ECG value
bool max86150_safeCheck(uint8_t maxTimeToCheck); //Given a max amount of time, check for new data

// Configuration
void max86150_softReset();
void max86150_shutDown();
void max86150_wakeUp();

void max86150_setLEDMode(uint8_t mode);

void max86150_setADCRange(uint8_t adcRange);
void max86150_setSampleRate(uint8_t sampleRate);
void max86150_setPulseWidth(uint8_t pulseWidth);

void max86150_setPulseAmplitudeRed(uint8_t value);
void max86150_setPulseAmplitudeIR(uint8_t value);
void max86150_setPulseAmplitudeProximity(uint8_t value);

void max86150_setProximityThreshold(uint8_t threshMSB);

//Multi-led configuration mode (page 22)
void max86150_enableSlot(uint8_t slotNumber, uint8_t device); //Given slot number, assign a device to slot
void max86150_disableSlots(void);

// Data Collection

//Interrupts (page 13, 14)
uint8_t max86150_getINT1(void); //Returns the main interrupt group
uint8_t max86150_getINT2(void); //Returns the temp ready interrupt
void max86150_enableAFULL(void); //Enable/disable individual interrupts
void max86150_disableAFULL(void);
void max86150_enableDATARDY(void);
void max86150_disableDATARDY(void);
void max86150_enableALCOVF(void);
void max86150_disableALCOVF(void);
void max86150_enablePROXINT(void);
void max86150_disablePROXINT(void);
void max86150_enableDIETEMPRDY(void);
void max86150_disableDIETEMPRDY(void);

//FIFO Configuration (page 18)
void max86150_setFIFOAverage(uint8_t samples);
void max86150_enableFIFORollover();
void max86150_disableFIFORollover();
void max86150_setFIFOAlmostFull(uint8_t samples);

//FIFO Reading
uint16_t max86150_check(void); //Checks for new data and fills FIFO
uint8_t max86150_available(void); //Tells caller how many new samples are available (head - tail)
void max86150_nextSample(void); //Advances the tail of the sense array
uint32_t max86150_getFIFORed(void); //Returns the FIFO sample pointed to by tail
uint32_t max86150_getFIFOIR(void); //Returns the FIFO sample pointed to by tail
int32_t max86150_getFIFOECG(void); //Returns the FIFO sample pointed to by tail

uint8_t max86150_getWritePointer(void);
uint8_t max86150_getReadPointer(void);
void max86150_clearFIFO(void); //Sets the read/write pointers to zero

//Proximity Mode Interrupt Threshold
void max86150_setPROXINTTHRESH(uint8_t val);

// Die Temperature
float max86150_readTemperature();
float max86150_readTemperatureF();

// Detecting ID/Revision
uint8_t max86150_getRevisionID();
uint8_t max86150_readPartID();
uint8_t max86150_readRegLED();

// Setup the IC with user selectable settings
void max86150_setup(uint8_t powerLevel = 0x1F, uint8_t sampleAverage = 4, uint8_t ledMode = 3, int sampleRate = 400, int pulseWidth = 411, int adcRange = 4096);

// Low-level I2C communication
uint8_t max86150_readRegister8(uint8_t address, uint8_t reg);
void max86150_writeRegister8(uint8_t address, uint8_t reg, uint8_t value);

private:
TwoWire *_i2cPort; //The generic connection to user's chosen I2C hardware
uint8_t _i2caddr;

//activeLEDs is the number of channels turned on, and can be 1 to 3. 2 is common for Red+IR.
uint8_t activeDevices; //Gets set during setup. Allows check() to calculate how many bytes to read from FIFO

uint8_t revisionID;

void max86150_readRevisionID();

void max86150_bitMask(uint8_t reg, uint8_t mask, uint8_t thing);

#define STORAGE_SIZE 4 //Each long is 4 bytes so limit this to fit on your micro
typedef struct Record
{
    uint32_t red[STORAGE_SIZE];
    uint32_t IR[STORAGE_SIZE];
    int32_t ecg[STORAGE_SIZE];
    uint8_t head;
    uint8_t tail;
} sense_struct; //This is our circular buffer of readings from the sensor

sense_struct sense;

#endif /* max86150_h */

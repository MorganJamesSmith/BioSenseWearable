/*
UV light sensor Si1133 partial work.
*/
/*
5.3 I2C Command Register Operation
Writing the codes shown below in the command summary table signals the sensor 
to undertake one of several complex operations.

These operations take time and all commands should be followed by 
 - a read of the RESPONSE0 register to confirm the operation is complete by examining the counter and 
 - to check for an error in the error bit. The error bit is set in the RESPONSE0 register’s command counter if there is an error in the previous command 
   (e.g., attempt to write to an illegal address beyond the parameter table, or a chan- nel and /or burst configuration that exceeds the size of the output field (26 bytes)). 
   If there is no such error, then the counter portion of the command counter will be incremented.

*/
enum I2C_registers {
    /** returns DEVID which is 0x33 */
    PART_ID, 
    /** returns hardware id */
    HW_ID,
    /** hardware revision 0xMN where M is major rev and N is minor */
    REV_ID,
    /** contains 0 after a hard reset or a RESET command */
    INFO0, INFO1,
    // address 5,6 undefined
    /** seemingly not used */
    HOSTIN3=7, HOSTIN2, HOSTIN1,
    /** IN (readable by host) data for parameter table on PARAM_SET command */
    HOSTIN0,
    /** specify command to take */
    COMMAND,
    // address 0x0C to 0x0E unused
    /** least 6 significant bits enable interrupt behaviour */
    IRQ_ENABLE = 0x0F,
    /** the sensor mirrors data byte written to parameter here to verify write was successful */
    RESPONSE1, 
    /** bit mapping
     * 7   RUNNING indicator
     * 6   SUSPEND indicator
     * 5   SLEEP   indicator
     * 4   CMD_ERR cleared by reset, set on bad command
     * 3:0 CMMND_CTR
     *  - if CMD_ERR = 0
     *      a counter that increments on every good command
     *      reset to 0 by the RESET_CMD_CTR command
     *  - if CMD_ERR = 1
     *   This is how it is written in the manual but 0x10 is 5 bits so it can't possibly be correct
     *    0x10 - invalid command
     *    0x11 - parameter access to invalid location
     *    0x12 saturation of the ADC or overflow of accumulation
     *    0x13 output buffer overflow can happen when burst mode is enabled and configured for greater than 26 bytes of output
     */
    RESPONSE0, 
    /**bit mapping:
     * 7:6 UNUSED, read = 00b, write = don't care
     * 5:0 IRQ bit mask for channels 5:0 results being ready.
     */
    IRQ_STATUS,
    /** captured sensor data */
    HOSTOUT0, /* fill in hosts 1-24 here. */ HOSTOUT25 = 0x2c,
};
enum ParameterTable {
    I2C_ADDR,
    CHAN_LIST,
    /** bit map
     * 7   RSRVD
     * 6:5 DECIM_RATE
     * 4:0 ADCMUX
     */
    ADC_CONFIG0,  
    /** bit map
     * 7   HSIG
     * 6:4 SW_GAIN
     * 3:0 HW_GAIN
     */
    ADC_SENS0, 
    /** bit map
     * 7   RSRVD
     * 6   24BIT_OUT
     * 5:3 POSTSHIFT
     * 2   UNUSED
     * 1:0 THRESH_SEL - selects which threshold specified at THRESHOLD#_L and THRESHOLD#_H
     */
    ADC_POST0,  
    /**
     * 7:6 COUNTER_INDEX - indicates which MEAS_COUNT# is used
     * 5:0 RSRVD
     */
    MEAS_CONFIG0,
    /** see description of item 0 */
    ADC_CONFIG1,  ADC_SENS1,  ADC_POST1,  MEAS_CONFIG1,
    /** see description of item 0 */
    ADC_CONFIG2,  ADC_SENS2,  ADC_POST2,  MEAS_CONFIG2,
    /** see description of item 0 */
    ADC_CONFIG3,  ADC_SENS3,  ADC_POST3,  MEAS_CONFIG3,
    /** see description of item 0 */
    ADC_CONFIG4,  ADC_SENS4,  ADC_POST4,  MEAS_CONFIG4,
    /** see description of item 0 */
    ADC_CONFIG5,  ADC_SENS5,  ADC_POST5,  MEAS_CONFIG5,

    MEAS_RATE_H,  MEAS_RATE_L,
    MEAS_COUNT0,  MEAS_COUNT1, MEAS_COUNT2,

    // 0x1F to 0x24 unused
    THRESHOLD0_H = 0x25, THRESHOLD0_L,
    THRESHOLD1_H, THRESHOLD1_L,
    THRESHOLD2_H, THRESHOLD2_L,
    BURST
};
/**
 * commands to be written to the COMMAND register to perform a command
 * note that sucessfully completing any command except RESET ones cause
 * an increment of the CMD_CTR field of the RESPONSE0 register (bits [3:0])
 */
enum Commands {
    /** Resets RESPONSE0 CMMND_CTR field to 0. */
    RESET_CMD_CTR   = 0x00,
    /** Forces a Reset, Resets RESPONSE0 CMMND_CTR field to 0xXXX01111 */
    RESET_SW        = 0x01,
    /** 
     * Initiates a set of measurements specified in CHAN_LIST parameter. 
     * A FORCE command will only execute the measurements which do not have 
     * a meas counter index configured in MEAS- CONFIGx.
     */
    FORCE           = 0x11,
    /** Pauses autonomous measurements specified in CHAN_LIST. */
    PAUSE           = 0x12,
    /** 
     * Starts autonomous measurements specified in CHAN_LIST. 
     * A START autonomous command will only start the measurements 
     * which has a counter index selected in MEASCONFIGx.
     */
    START           = 0x13,
    /**
     * Must be OR-ed (|) with parameter table entry for valid access
     * Reads Parameter and store results in RESPONSE1
     */
    PARAM_QUERY     = 0b01000000,
    /**
     * writes INPUT0 to the parameter specified in 6 bit address
     * must be OR-ed (|) with parameter table entry for valid access
     */
    PARAM_SET       = 0b10000000,
};
/**
 * indicates it listens to address 0x55 or alternate 0x52, not sure what condition 
 * changes the port it listens to, it can be changed by writing to the I2C_ADDR register.
 */
char OWN_ADDRESS = 0x55;
char _I2Cread(char device, char address);
void _I2Cwrite(char device, char address, char value);
/**
 * verifies that command executed correctly, by ensuring RESPONSE0 incremented compared to status passed
 * if the error bit is set this returns non 0 value
 * TODO: fill in actual logic
 */
char verify_resp(char initial_resp_value){
    while(1){
        char resp = _I2Cread(OWN_ADDRESS, RESPONSE0);
        if(_HAS_INCREMENTED(initial_resp_value, resp)){
            return 0;
        }
        else if (_IS_ERROR(resp)){
            return 1;
        }
    }
}
/**
 * reads a parameter and returns it.
 * if there was an error this returns 255 (all 1s)
 * param2read should be a value from the ParameterTable enum
 */
char read_param(enum ParameterTable param2read){
    char resp0 = _I2Cread(OWN_ADDRESS, RESPONSE0);
    _I2Cwrite(OWN_ADDRESS, COMMAND, PARAM_QUERY | param2read);
    if(verify_resp(resp0)){return 0xFF;}
    // it mentions specifically that the read for RESPONSE0 and RESPONSE1 should not be in the same I2C transaction
    return _I2Cread(OWN_ADDRESS, RESPONSE1);
}
/**
 * writes parameter to register, 
 * param2write should be a value from the ParameterTable enum
 * returns 0 for success, 1 for failure
 */
char write_param(enum ParameterTable param2write, char value){
    char resp0 = _I2Cread(OWN_ADDRESS, RESPONSE0);
    _I2Cwrite(OWN_ADDRESS, HOSTIN0, value);
    _I2Cwrite(OWN_ADDRESS, COMMAND, PARAM_SET | param2write);
    // it indicates these 2 writes CAN be done in the same I2C transaction
    return verify_resp(resp0);
}

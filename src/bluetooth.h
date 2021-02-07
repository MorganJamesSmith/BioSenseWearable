#ifndef bluetooth_h
#define bluetooth_h

#include <stdint.h>

void bluetooth_init(void);

void bluetooth_service(void);

/**
 *  Configure a callback to be called when the a Bluetooth connection is established.
 *
 *  @param callback The function to be called
 */
void bluetooth_set_connected_callback (void (*callback)());

/**
 *  Configure a callback to be called when the a Bluetooth connection is disconnected.
 *
 *  @param callback The function to be called
 */
void bluetooth_set_disconnected_callback (void (*callback)());

/**
 *  Configure a callback to be called when the Bluetooth serial interface is ready.
 *
 *  @param callback The function to be called
 *  @param context Pointer to be provided to callback
 */
void bluetooth_set_ready_callback (void (*callback)(void*), void *context);

/**
 *  Queue a string to be written to a Bluetooth interface.
 *
 *  @param str The string to be written.
 *
 *  @return The number of characters which could be queued for transmission.
 */
uint16_t bluetooth_put_string(const char *str);

/**
 *  Queue a string to be written to a Bluetooth serial port. If there is not enough
 *  space for the string in the buffer, block until there is.
 *
 *  @param str The string to be written.
 */
void bluetooth_put_string_blocking(const char *str);

/**
 *  Queue a byte array to be written to a Bluetooth interface.
 *
 *  @param bytes The array to be written
 *  @param length The number of bytes to be written
 *
 *  @return The number of bytes which could be added to the queue.
 */
uint16_t bluetooth_put_bytes(const uint8_t *bytes, uint16_t length);

/**
 *  Queue a string to be written to a Bluetooth interface. If there is not enough
 *  space for the string in the buffer, block until there is.
 *
 *  @param bytes The array to be written
 *  @param length The number of bytes to be written
 */
void bluetooth_put_bytes_blocking(const uint8_t *bytes, uint16_t length);

/**
 *  Write a character to a Bluetooth serial port.
 *
 *  @param c The character to be written
 */
void bluetooth_put_char(const char c);

/**
 *  Get string from a Bluetooth serial port input buffer.
 *
 *  @param str The string in which the data should be stored.
 *  @param len The maximum number of chars to be read from the buffer.
 */
void bluetooth_get_string(char *str, uint16_t len);

/**
 *  Determine if there is a full line, as delimited by the provided char,
 *  available to be read from a Bluetooth serial port buffer.
 *
 *  @param delim The delimiter for new lines (ie. '\n').
 *
 *  @return 0 if there is no line available, 1 if a line is available.
 */
uint8_t bluetooth_has_delim(char delim);

/**
 *  Read a string from the input buffer up to the next occurrence of a
 *  delimiter.
 *
 *  @param delim The delimiter for new lines (ie. '\n').
 *  @param str The string in which the data should be stored.
 *  @param len The maximum number of chars to be read from the input buffer.
 */
void bluetooth_get_line_delim(char delim, char *str, uint16_t len);

/**
 *  Determine if there is a full line, delimited by the sequence "\r\n",
 *  available to be read from a Bluetooth serial port buffer.
 *
 *  @return 0 if there is no line available, 1 if a line is available.
 */
uint8_t bluetooth_has_line(void);

/**
 *  Read a string from the input buffer up to the next occurrence of a
 *  the sequence "\r\n".
 *
 *  @param str The string in which the data should be stored.
 *  @param len The maximum number of chars to be read from the input buffer.
 */
void bluetooth_get_line(char *str, uint16_t len);

/**
 *  Get a character from a Bluetooth serial port input buffer.
 *
 *  @return The least recently received character in the input buffer
 */
char bluetooth_get_char(void);

/**
 *  Determine if the out buffer of a Bluetooth serial port is empty.
 */
uint8_t bluetooth_out_buffer_empty(void);

#endif /* bluetooth_h */
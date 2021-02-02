#ifndef usb_serial_h
#define usb_serial_h

#include <stdint.h>

void init_usb_cdc(void);

void usb_cdc_service(void);

/**
 *  Configure a callback to be called when the USB serial interface is ready.
 *
 *  @param callback The function to be called
 *  @param context Pointer to be provided to callback
 */
extern void usb_cdc_set_ready_callback (void (*callback)(void*), void *context);

/**
 *  Queue a string to be written to a CDC interface.
 *
 *  @param str The string to be written.
 *
 *  @return The number of characters which could be queued for transmission.
 */
extern uint16_t usb_cdc_put_string(const char *str);

/**
 *  Queue a string to be written to a CDC port. If there is not enough
 *  space for the string in the buffer, block until there is.
 *
 *  @param str The string to be written.
 */
extern void usb_cdc_put_string_blocking(const char *str);

/**
 *  Queue a byte array to be written to a CDC interface.
 *
 *  @param bytes The array to be written
 *  @param length The number of bytes to be written
 *
 *  @return The number of bytes which could be added to the queue.
 */
extern uint16_t usb_cdc_put_bytes(const uint8_t *bytes, uint16_t length);

/**
 *  Queue a string to be written to a CDC interface. If there is not enough
 *  space for the string in the buffer, block until there is.
 *
 *  @param bytes The array to be written
 *  @param length The number of bytes to be written
 */
extern void usb_cdc_put_bytes_blocking(const uint8_t *bytes, uint16_t length);

/**
 *  Write a character to a CDC port.
 *
 *  @param c The character to be written
 */
extern void usb_cdc_put_char(const char c);

/**
 *  Get string from a CDC port input buffer.
 *
 *  @param str The string in which the data should be stored.
 *  @param len The maximum number of chars to be read from the buffer.
 */
extern void usb_cdc_get_string(char *str, uint16_t len);

/**
 *  Determine if there is a full line, as delimited by the provided char,
 *  available to be read from a CDC port buffer.
 *
 *  @param delim The delimiter for new lines (ie. '\n').
 *
 *  @return 0 if there is no line available, 1 if a line is available.
 */
extern uint8_t usb_cdc_has_delim(char delim);

/**
 *  Read a string from the input buffer up to the next occurrence of a
 *  delimiter.
 *
 *  @param delim The delimiter for new lines (ie. '\n').
 *  @param str The string in which the data should be stored.
 *  @param len The maximum number of chars to be read from the input buffer.
 */
extern void usb_cdc_get_line_delim(char delim, char *str, uint16_t len);

/**
 *  Determine if there is a full line, delimited by the sequence "\r\n",
 *  available to be read from a CDC port buffer.
 *
 *  @return 0 if there is no line available, 1 if a line is available.
 */
extern uint8_t usb_cdc_has_line(void);

/**
 *  Read a string from the input buffer up to the next occurrence of a
 *  the sequence "\r\n".
 *
 *  @param str The string in which the data should be stored.
 *  @param len The maximum number of chars to be read from the input buffer.
 */
extern void usb_cdc_get_line(char *str, uint16_t len);

/**
 *  Get a character from a CDC port input buffer.
 *
 *  @return The least recently received character in the input buffer
 */
extern char usb_cdc_get_char(void);

/**
 *  Determine if the out buffer of a CDC port is empty.
 */
extern uint8_t usb_cdc_out_buffer_empty(void);

#endif /* circular_buffer_h */

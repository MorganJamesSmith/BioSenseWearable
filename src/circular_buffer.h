/**
 * @file circular_buffer.h
 * @desc Fixed length circular buffer implementation
 * @author Samuel Dewan
 * @date 2018-12-27
 * Last Author: Samuel Dewan
 * Last Edited On: 2019-11-30
 */

#ifndef circular_buffer_h
#define circular_buffer_h

#include <stdint.h>

/**
 *  Instance of an arbitrary length circular buffer.
 */
struct circular_buffer_t {
    uint8_t *buffer;
    uint16_t capacity;
    
    uint16_t head;
    uint16_t tail;
    uint16_t length;
};


/**
 *  Initialize a new circular buffer from an existing array.
 *
 *  @param buffer The buffer descriptor to be initialized.
 *  @param memory The underlying array for the new buffer.
 *  @param capacity The size of the buffer.
 */
static inline void init_circular_buffer(struct circular_buffer_t *buffer,
                                        uint8_t *memory, uint16_t capacity)
{
    buffer->buffer = memory;
    buffer->capacity = capacity;
    
    buffer->head = 0;
    buffer->tail = 0;
    buffer->length = 0;
}

/**
 *  Determine if a circular buffer is empty.
 *
 *  @param buffer The circular buffer for which the empty-ness should be
 *                determined.
 *
 *  @return A non-zero value if the buffer is empty, 0 otherwise
 */
static inline int circular_buffer_is_empty(struct circular_buffer_t *buffer)
{
    return buffer->length == 0;
}

/**
 *  Determine if a circular buffer is full.
 *
 *  @param buffer The circular buffer for which the full-ness should be
 *                determined.
 *
 *  @return A non-zero value if the buffer is full, 0 otherwise
 */
static inline int circular_buffer_is_full(struct circular_buffer_t *buffer)
{
    return buffer->length == buffer->capacity;
}

/**
 *  Determine the amount of unused space in the buffer
 *
 *  @param buffer The circular buffer for which the amount of free space should
 *                be found.
 *
 *  @return The number of free bytes in the buffer.
 */
static inline uint16_t circular_buffer_unused(struct circular_buffer_t *buffer)
{
    return buffer->capacity - buffer->length;
}

/**
 *  Get the capacity of a circular buffer.
 *
 *  @param buffer The circular buffer for which the capacity should be
 *                determined.
 *
 *  @return The capacity of the buffer.
 */
static inline uint16_t circular_buffer_capacity(struct circular_buffer_t *buffer)
{
    return buffer->capacity;
}

/**
 *  Insert an item at the tail of a circular buffer.
 *
 *  @note If the buffer is full, the oldest data will be overwritten.
 *
 *  @param buffer The circular buffer into which data should be inserted.
 *  @param value The data to be inserted.
 */
static inline void circular_buffer_push(struct circular_buffer_t *buffer,
                                        uint8_t value)
{
    __disable_irq();
    
    // If the buffer is full, don't let the tail pass the head
    if (circular_buffer_is_full(buffer)) {
        buffer->head = (buffer->head + 1) % buffer->capacity;
    } else {
        buffer->length++;
    }
    
    buffer->buffer[buffer->tail] = value;
    buffer->tail = (buffer->tail + 1) % buffer->capacity;
    
    __enable_irq();
}

/**
 *  Insert an item at the tail of a circular buffer iff there is space
 *  available.
 *
 *  @param buffer The circular buffer into which data should be inserted.
 *  @param value The data to be inserted.
 *
 *  @return 0 on success, 1 if the buffer is full
 */
static inline int circular_buffer_try_push(struct circular_buffer_t *buffer,
                                           uint8_t value)
{
    if (circular_buffer_is_full(buffer)) {
        return 1;
    } else {
        circular_buffer_push(buffer, value);
        return 0;
    }
}

/**
 *  Get the item from the head of a circular buffer, if available, and remove
 *  it from the buffer.
 *
 *  @param buffer The circular buffer from which an item should be popped.
 *  @param value Pointer where the popped item will be stored.
 *
 *  @return 0 on success, 1 if the buffer is empty
 */
static inline int circular_buffer_pop(struct circular_buffer_t *buffer,
                                      uint8_t *value)
{
    if (circular_buffer_is_empty(buffer)) {
        return 1;
    } else {
        __disable_irq();
        
        *value = buffer->buffer[buffer->head];
        buffer->head = (buffer->head + 1) % buffer->capacity;
        buffer->length--;
        
        __enable_irq();
        return 0;
    }
}

/**
 *  Get a pointer to the head of the buffer and the number of contiguous bytes
 *  in the buffer following the pointer.
 *
 *  @param buffer The circular buffer for which the head should be found.
 *  @param head Pointer a pointer to the head will be placed.
 *
 *  @return The number of contiguous bytes in the buffer after the head.
 */
static inline uint16_t circular_buffer_get_head(struct circular_buffer_t *buffer,
                                                uint8_t **head)
{
    *head = buffer->buffer + buffer->head;
    
    if (circular_buffer_is_empty(buffer)) {
        return 0;
    } else if (buffer->head >= buffer->tail) {
        return buffer->capacity - buffer->head;
    } else {
        return buffer->tail - buffer->head;
    }
}

/**
 *  Move the head of the buffer forwards by a certain number of bytes. This has
 *  the effect of removing `length` bytes from the buffer. If the head of the
 *  buffer would be moved past the tail, the head will be moved up to match the
 *  tail.
 *
 *  @param buffer The circular buffer for which the head should be moved.
 *  @param length The distance which the head should be moved.
 */
static inline void circular_buffer_move_head(struct circular_buffer_t *buffer,
                                             uint16_t length)
{
    __disable_irq();
    
    if (circular_buffer_is_empty(buffer)) {
        __enable_irq();
        return;
    } else if (buffer->head < buffer->tail) {
        if (buffer->head + length < buffer->tail) {
            buffer->head += length;
            buffer->length -= length;
        } else {
            buffer->head = buffer->tail;
            buffer->length = 0;
        }
    } else {
        if (buffer->head + length < buffer->capacity) {
            buffer->head += length;
            buffer->length -= length;
        } else if (((buffer->head + length) % buffer->capacity) < buffer->tail) {
            buffer->head = (buffer->head + length) % buffer->capacity;
            buffer->length -= length;
        } else {
            buffer->head = buffer->tail;
            buffer->length = 0;
        }
    }
    
    __enable_irq();
}

/**
 *  Get the item from the head of a circular buffer, if available, without
 *  removing it from the buffer.
 *
 *  @param buffer The circular buffer from which an item should be gotten.
 *  @param value Pointer where the peaked item will be stored.
 *
 *  @return 0 on success, 1 if the buffer is empty
 */
static inline int circular_buffer_peak(struct circular_buffer_t *buffer,
                                       uint8_t *value)
{
    if (circular_buffer_is_empty(buffer)) {
        return 1;
    } else {
        *value = buffer->buffer[buffer->head];
        return 0;
    }
}

/**
 *  Remove an item from the tail of the buffer.
 *
 *  @param buffer The buffer from which the last inserted item should be removed
 *
 *  @return 0 on success, 1 if the buffer is empty
 */
static inline int circular_buffer_unpush(struct circular_buffer_t *buffer)
{
    if (circular_buffer_is_empty(buffer)) {
        return 1;
    } else {
        __disable_irq();

        if (buffer->tail == 0) {
            buffer->tail = buffer->capacity;
        }
        buffer->tail--;
        buffer->length--;
        
        __enable_irq();
        return 0;
    }
}

/**
 *  Determine if a character is present in a circular buffer.
 *
 *  @param buffer The buffer in which the presence of a character should be
 *                determined.
 *  @param c The character which should be searched for.
 *
 *  @return 1 if the character is found, 0 otherwise
 */
static inline int circular_buffer_has_char(struct circular_buffer_t *buffer,
                                           char c)
{
    if (circular_buffer_is_empty(buffer)) {
        return 0;
    }
    
    uint16_t i = buffer->head;
    do {
        if (buffer->buffer[i] == c) {
            return 1;
        }
        
        i = ((i + 1) % buffer->capacity);
    } while (i != buffer->tail);
    
    return 0;
}

/**
 *  Determine if the character sequence "\r\n" is present in the buffer.
 *
 *  @param buffer The buffer in which the presence of a line should be
 *                determined.
 *
 *  @return 1 if a line is found, 0 otherwise
 */
static inline int circular_buffer_has_line(struct circular_buffer_t *buffer)
{
    if (circular_buffer_is_empty(buffer)) {
        return 0;
    }
    
    uint16_t i = buffer->head;
    do {
        if (buffer->buffer[i] == '\r') {
            uint16_t next = (i + 1) % buffer->capacity;
            if ((next != buffer->tail) && (buffer->buffer[next] == '\n')) {
                return 1;
            }
        }
        
        i = ((i + 1) % buffer->capacity);
    } while (i != buffer->tail);
    
    return 0;
}

/**
 *  Resets a circular buffer to an empty state.
 *
 *  @param buffer The buffer to be cleared.
 */
static inline void circular_buffer_clear(struct circular_buffer_t *buffer)
{
    buffer->head = 0;
    buffer->tail = 0;
    buffer->length = 0;
}

#endif /* circular_buffer_h */

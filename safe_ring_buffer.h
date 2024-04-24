#ifndef RNG_BUFF_H
#define RNG_BUFF_H

#include <stdint.h>
#define DEFAULT_BUFFER_SIZE 32

struct ring_buffer {
        uint32_t *buffer;
        uint8_t tail;
        uint8_t head;
        uint8_t size;
};

uint32_t ring_buffer_peek(const struct ring_buffer *ring);
uint32_t ring_buffer_get(struct ring_buffer *ring);
uint8_t ring_buffer_put(struct ring_buffer *ring, uint32_t data);
uint8_t ring_buffer_full(const struct ring_buffer *ring);
uint8_t ring_buffer_empty(const struct ring_buffer *ring);
struct ring_buffer create_ring_buffer(unsigned int size);
#endif

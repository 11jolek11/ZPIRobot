#include "./safe_ring_buffer.h"
#include <string.h>
#include <stdio.h>

static uint32_t last_action_buffer = 0x0;

ring_buffer create_ring_buffer(unsigned int size) {
        if (size == 0) {
                size = DEFAULT_BUFFER_SIZE;
        }

        uint32_t buff[size];

        for (int i = 0; i <= size-1; i++) {
                buff[i] = 0;
                printf("%d\n", buff[i]);
        }

        ring_buffer command_buffer  = { .buffer=buff, .size=size };
        return command_buffer;
}

uint32_t ring_buffer_peek(const ring_buffer *ring){
        return ring->buffer[ring->tail];
};

uint32_t ring_buffer_get(ring_buffer *ring) {
        uint8_t next_tail = ring->tail + 1;

        const uint32_t requested_data = ring->buffer[ring->tail];

        if (ring->tail == ring->head) {
                return last_action_buffer;
                // return 0;
        }
        ring->tail++;

        if (ring->tail == ring->size) {
                ring->tail = 0;
        }

        last_action_buffer = requested_data;
        return requested_data;
};

uint8_t ring_buffer_put(ring_buffer *ring, uint32_t data) {
        if (ring_buffer_full(ring)) {
                return 0;
        }
        ring->buffer[ring->head] = data;
        ring->head++;

        if (ring->size == ring->head) {
                ring->head = 0;
        }

        return 1;
};

uint8_t ring_buffer_full(const ring_buffer *ring) {
        uint8_t index_after_head = ring->head + 1;

        if (index_after_head == ring->size) {
                index_after_head = 0;
        }

        return index_after_head == ring->tail;

};

uint8_t ring_buffer_empty(const ring_buffer *ring) {
        return ring->tail == ring->head;
};

uint8_t ring_buffer_not_empty(const ring_buffer *ring) {
        return !(ring->tail == ring->head);
};


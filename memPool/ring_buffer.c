#include "ring_buffer.h"
#include <stdio.h>

uint8_t RB_Init(ring_buffer *rb_handle, uint64_t *buffer_addr ,uint64_t buffer_size) {
    if (buffer_size < 2 || buffer_size == 0xFFFFFFFF) return RING_BUFFER_ERROR;
    rb_handle->head = 0;
    rb_handle->tail = 0;
    rb_handle->Length = 0;
    rb_handle->array_addr = buffer_addr;
    rb_handle->max_Length = buffer_size;
    return RING_BUFFER_SUCCESS;
}

uint8_t RB_Write_Addr(ring_buffer *rb_handle, uint64_t data) {
    if (rb_handle->Length == rb_handle->max_Length) {
        printf("WARNNING: RB FULL\n");
        return RING_BUFFER_ERROR;
    }
    rb_handle->array_addr[rb_handle->tail] = data;
    rb_handle->Length += 1;
    rb_handle->tail += 1;
    if (rb_handle->tail > rb_handle->max_Length - 1) rb_handle->tail = 0;
    return RING_BUFFER_SUCCESS;
}

uint8_t RB_Read_Addr(ring_buffer *rb_handle, uint64_t *output_addr) {
    if (rb_handle->Length != 0) {
        *output_addr = rb_handle->array_addr[rb_handle->head];
        rb_handle->head += 1;
        rb_handle->Length -= 1;
        if (rb_handle->head > rb_handle->max_Length - 1) rb_handle->head = 0;
        return RING_BUFFER_SUCCESS;
    }
    return RING_BUFFER_ERROR;
}

uint64_t RB_Get_Length(ring_buffer *rb_handle) { return rb_handle->Length; }
uint64_t RB_Get_FreeSize(ring_buffer *rb_handle) { return rb_handle->max_Length - rb_handle->Length; }

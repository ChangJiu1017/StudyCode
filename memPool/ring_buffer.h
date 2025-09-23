#ifndef _RING_BUFFER_H_
#define _RING_BUFFER_H_
#include <stdint.h>

#define RING_BUFFER_SUCCESS     0x01
#define RING_BUFFER_ERROR       0x00
#define ADDR_LEN sizeof(void*)

typedef struct {
    uint64_t head;
    uint64_t tail;
    uint64_t Length;
    uint64_t* array_addr;
    uint64_t max_Length;
} ring_buffer;

uint8_t RB_Init(ring_buffer *rb_handle, uint64_t* buffer_addr ,uint64_t buffer_size);
uint8_t RB_Write_Addr(ring_buffer *rb_handle, uint64_t data);
uint8_t RB_Read_Addr(ring_buffer *rb_handle, uint64_t *output_addr);
uint64_t RB_Get_Length(ring_buffer *rb_handle);
uint64_t RB_Get_FreeSize(ring_buffer *rb_handle);

#endif

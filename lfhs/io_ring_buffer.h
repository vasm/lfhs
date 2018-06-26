//
//  io_ring_buffer.h
//
//  lfhs: lightning-fast http server
//
//  Created by Vasiliy Sabadazh on 12.12.2017.
//  Copyleft 2017 Vasiliy Sabadazh. All rights are granted.
//

#ifndef io_ring_buffer_h
#define io_ring_buffer_h

#include <pthread.h>
#include <stddef.h>

typedef struct _io_ring_buffer
{
    pthread_mutex_t mutex;
    size_t size;
    size_t data_start;
    size_t data_length;
    void* data;
} io_ring_buffer;

int io_ring_buffer_init(io_ring_buffer* buf);
int io_ring_buffer_free(io_ring_buffer* buf);

// copy data from some memory block into the buffer
int io_ring_buffer_write_data(io_ring_buffer* buf, void* data, size_t length);

// copy data from buffer
int io_ring_buffer_read_data(io_ring_buffer* buf, size_t data_size, void* dest, size_t* bytes_written);

#endif /* io_ring_buffer_h */

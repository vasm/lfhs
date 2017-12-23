//
//  io_buffer.h
//
//  lfhs: lightning-fast http server
//
//  Created by Vasiliy Sabadazh on 12.12.2017.
//  Copyleft 2017 Vasiliy Sabadazh. All rights are granted.
//

#ifndef io_buffer_h
#define io_buffer_h

#include <pthread.h>
#include <stddef.h>


typedef struct _io_buffer
{
    pthread_mutex_t mutex;
    size_t size;
    size_t data_start;
    size_t data_length;
    void* data;
} io_buffer;

int io_buffer_init(io_buffer* buf);

// copy data from some memory block into the buffer
int io_buffer_write_data(io_buffer* buf, void* data, size_t length);

// copy data from buffer
int io_buffer_read_data(io_buffer* buf, size_t data_size, void* dest, size_t* bytes_written);

typedef enum
{
    e_io_buffer_no_error = 0,
    e_io_buffer_too_small = 1,
    e_io_buffer_alloc_failed = 2,
    e_io_buffer_not_enough_data = 3,
    e_io_buffer_cannot_shrink = 4
} io_buffer_error;

char* io_buffer_error_str(io_buffer_error);

#endif /* io_buffer_h */

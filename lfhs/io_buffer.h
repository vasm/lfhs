//
//  io_buffer.h
//  lfhs
//
//  Created by Vasiliy Sabadazh on 05/05/2018.
//  Copyright © 2018 Vasiliy Sabadazh. All rights reserved.
//

#ifndef io_buffer_h
#define io_buffer_h

#include <pthread.h>
#include <stddef.h>

typedef struct _io_buffer
{
    pthread_mutex_t mutex;
    size_t size;
    size_t data_length; // data always starts at 0th byte, but may end before buffer ends
    void* data;
} io_buffer;

int io_buffer_init(io_buffer* buf);
int io_buffer_free(io_buffer* buf);

// copy data from some memory block into the buffer
int io_buffer_write_data(io_buffer* buf, void* data, size_t length);

// copy data from buffer
int io_buffer_read_data(io_buffer* buf, size_t data_size, void* dest, size_t* bytes_written);


#endif /* io_buffer_h */

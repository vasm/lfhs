//
//  io_buffer.c
//
//  lfhs: lightning-fast http server
//
//  Created by Vasiliy Sabadazh on 12.12.2017.
//  Copyleft 2017 Vasiliy Sabadazh. All rights are granted.
//

#include "io_buffer.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static const size_t c_io_buffer_default_size = 1024;

/* NOTE:
 io_buffer::data_begin points at the first element of the data
 io_buffer::data_length denotes the number of bytes occupied by actual data
 
 at the init time, data_begin = 0, data_length = 0
 */

#define safe_enter assert(buf); pthread_mutex_lock(&buf->mutex);
#define safe_return(val) {pthread_mutex_unlock(&buf->mutex); return val;}


bool is_wrapped(io_buffer *buf) {
    return buf->data_start + buf->data_length > buf->size;
}


int io_buffer_init(io_buffer* buf)
{
    assert(buf);
    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&buf->mutex, &mutex_attr);
    pthread_mutex_lock(&buf->mutex);
    
    buf->size = c_io_buffer_default_size;
    buf->data = malloc(c_io_buffer_default_size);
    if (buf->data == NULL)
        safe_return(e_io_buffer_alloc_failed);
    
    buf->data_start = 0;
    buf->data_length = 0;
    memset(buf->data, 0, c_io_buffer_default_size);
    
    safe_return(e_io_buffer_no_error);
}


int io_buffer_resize(io_buffer* buf, size_t new_size)
{
    safe_enter;
    assert(new_size > 0);
    
    if (new_size < buf->data_length)
        safe_return(e_io_buffer_cannot_shrink);
    
    void* new_data = malloc(new_size);
    memset(new_data, 0, new_size);
    size_t bytes_written;
    io_buffer_read_data(buf, buf->data_length, new_data, &bytes_written);
    buf->data_start = 0;
    buf->size = new_size;
    
    free(buf->data);
    buf->data = new_data;
    
    
    safe_return(e_io_buffer_no_error);
}


int io_buffer_read_data(io_buffer* buf, size_t data_size, void* dest, size_t* bytes_written)
{
    safe_enter;
    assert(dest);
    
    size_t _bytes_written = buf->data_length < data_size ? buf->data_length : data_size; //MIN(buf_len, data_size);
    *bytes_written = _bytes_written;
    
    if (buf->data_start + _bytes_written > buf->size) {
        memcpy(dest, buf->data + buf->data_start, buf->size - _bytes_written);
        memcpy(dest + buf->size - buf->data_start, buf->data, _bytes_written - (buf->size - buf->data_start));
        buf->data_start = _bytes_written - (buf->size - buf->data_start);
    } else {
        memcpy(dest, buf->data + buf->data_start, _bytes_written);
    }
    
    buf->data_length -= _bytes_written;
    
    safe_return(e_io_buffer_no_error);
}


int io_buffer_write_data(io_buffer* buf, void* data, size_t length)
{
    safe_enter;
    
    size_t new_datasize = length + buf->data_length;
    if (new_datasize > buf->size) {
        int retcode = io_buffer_resize(buf, new_datasize);
        if (retcode != e_io_buffer_no_error)
            safe_return(retcode);
    }
    
    safe_return(e_io_buffer_no_error);
}


//
//  io_ring_buffer.c
//
//  lfhs: lightning-fast http server
//  Copyleft 2017 Vasiliy Sabadazh. All rights are granted.
//

#include "io_ring_buffer.h"
#include "error.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static const size_t c_io_ring_buffer_default_size = 1024;

/* NOTE:
 io_ring_buffer::data_begin points at the first element of the data
 io_ring_buffer::data_length denotes the number of bytes occupied by actual data
 
 at the init time, data_begin = 0, data_length = 0
 */

#define safe_enter assert(buf); pthread_mutex_lock(&buf->mutex);
#define safe_return(val) {pthread_mutex_unlock(&buf->mutex); return val;}


bool is_wrapped(io_ring_buffer *buf) {
    return buf->data_start + buf->data_length > buf->size;
}


int io_ring_buffer_init(io_ring_buffer* buf)
{
    assert(buf);
    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&buf->mutex, &mutex_attr);
    pthread_mutex_lock(&buf->mutex);
    
    buf->size = c_io_ring_buffer_default_size;
    buf->data = malloc(c_io_ring_buffer_default_size);
    if (buf->data == NULL)
        safe_return(e_io_buffer_alloc_failed);
    
    buf->data_start = 0;
    buf->data_length = 0;
    memset(buf->data, 0, c_io_ring_buffer_default_size);
    
    safe_return(e_io_buffer_no_error);
}


int io_ring_buffer_free(io_ring_buffer* buf)
{
    safe_enter;
    free(buf->data);
    pthread_mutex_unlock(&buf->mutex);
    pthread_mutex_destroy(&buf->mutex);
    return e_io_buffer_no_error;
}


int io_ring_buffer_resize(io_ring_buffer* buf, size_t new_size)
{
    safe_enter;
    assert(new_size > 0);
    
    if (new_size < buf->data_length)
        safe_return(e_io_buffer_cannot_shrink);
    
    void* new_data = malloc(new_size);
    memset(new_data, 0, new_size);
    size_t bytes_written;
    size_t data_length = buf->data_length;
    io_ring_buffer_read_data(buf, buf->data_length, new_data, &bytes_written);
    buf->data_start = 0;
    buf->size = new_size;
    buf->data_length = data_length;
    
    free(buf->data);
    buf->data = new_data;
    
    safe_return(e_io_buffer_no_error);
}


int io_ring_buffer_read_data(io_ring_buffer* buf, size_t data_size, void* dest, size_t* bytes_written)
{
    safe_enter;
    assert(dest);
    
    size_t actual_bytes_written = buf->data_length < data_size ? buf->data_length : data_size;  // MIN(buf_len, data_size);
    
    if (bytes_written != NULL)
        *bytes_written = actual_bytes_written;
    
    // should we read the data as two blocks?
    if (buf->data_start + actual_bytes_written > buf->size) {
        size_t first_block_size = buf->size - buf->data_start;
        memcpy(dest,
               buf->data + buf->data_start,
               first_block_size);
        
        memcpy(dest + first_block_size,
               buf->data,
               actual_bytes_written - first_block_size);
    } else {
        memcpy(dest, buf->data + buf->data_start, actual_bytes_written);
    }
    
    buf->data_length -= actual_bytes_written;
    
    if (buf->data_length == 0)
        buf->data_start = 0;
    else
        buf->data_start = (buf->data_start + actual_bytes_written) % buf->size;
    
    safe_return(e_io_buffer_no_error);
}


int io_ring_buffer_write_data(io_ring_buffer* buf, void* data, size_t length)
{
    safe_enter;
    
    size_t new_datasize = length + buf->data_length;
    if (new_datasize > buf->size) {
        int retcode = io_ring_buffer_resize(buf, new_datasize);
        if (retcode != e_io_buffer_no_error)
            safe_return(retcode);
    }
    
    // should we write the data as two blocks?
    if (buf->data_start + buf->data_length + length > buf->size) {
        size_t first_block_size = buf->size - buf->data_start - buf->data_length;
        
        memcpy(buf->data + buf->data_start + buf->data_length,
               data,
               first_block_size);
        
        memcpy(buf->data,
               data + first_block_size,
               length - first_block_size);
    } else {
        memcpy(buf->data + buf->data_start + buf->data_length,
               data,
               length);
    }
    
    buf->data_length += length;
    
    safe_return(e_io_buffer_no_error);
}

size_t io_ring_buffer_get_data_length(io_ring_buffer* buf)
{
    safe_enter;
    size_t res = buf->data_length;
    safe_return(res);
}

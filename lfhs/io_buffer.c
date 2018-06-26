//
//  io_buffer.c
//  lfhs: lightning-fast http server
//
//  Created by Vasiliy Sabadazh on 11/05/2018.
//  Copyleft 2018 Vasiliy Sabadazh. All rights reserved.
//

#include "io_buffer.h"
#include "error.h"

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


int io_buffer_init(io_buffer* buf)
{
    assert(buf);
    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&buf->mutex, &mutex_attr);
    pthread_mutex_lock(&buf->mutex);
    
    if (buf->size != 0)
        free(buf->data);
    
    buf->size = c_io_buffer_default_size;
    buf->data = malloc(c_io_buffer_default_size);
    if (buf->data == NULL)
        safe_return(e_io_buffer_alloc_failed);
    
    buf->data_length = 0;
    memset(buf->data, 0, c_io_buffer_default_size);
    
    safe_return(e_io_buffer_no_error);
}


int io_buffer_free(io_buffer* buf)
{
    safe_enter;
    free(buf->data);
    pthread_mutex_unlock(&buf->mutex);
    pthread_mutex_destroy(&buf->mutex);
    buf->data_length = 0;
    buf->size = 0;
    return e_io_buffer_no_error;
}


int io_buffer_resize(io_buffer* buf, size_t new_size)
{
    safe_enter;
    assert(new_size > 0);
    
    if (new_size < buf->data_length)
        safe_return(e_io_buffer_cannot_shrink);
    
    assert(!"TODO");
    
    safe_return(e_io_buffer_no_error);
}


int io_buffer_read_data(io_buffer* buf, size_t data_size, void* dest, size_t* bytes_written)
{
    safe_enter;
    assert(dest);
    assert(!"TODO");
    safe_return(e_io_buffer_no_error);
}


int io_buffer_write_data(io_buffer* buf, void* data, size_t length)
{
    safe_enter;
    assert(data);
    assert(!"TODO");
    safe_return(e_io_buffer_no_error);
}

size_t io_buffer_get_data_length(io_buffer* buf)
{
    safe_enter;
    size_t res = buf->data_length;
    safe_return(res);
}

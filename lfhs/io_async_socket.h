//
//  io_async_socket.h
//
//  lfhs: lightning-fast http server
//  Copyleft 2017 Vasiliy Sabadazh. All rights are granted.
//

#ifndef io_async_socket_h
#define io_async_socket_h

#include "io_buffer.h"

typedef enum {
    io_socket_no_error = 0,
    io_socket_invalid_fd = 1
} io_socket_status;

typedef struct _io_connection_state {
    int fd;
    int state;
    io_buffer read_buffer;
    io_buffer write_buffer;
} io_connection_state;

typedef void (*async_handler) (io_buffer* _Nonnull data, int status);

int async_read(io_connection_state* _Nonnull socket,
               size_t bytes,
               async_handler _Nonnull handler);

int async_read_until(io_connection_state* _Nonnull socket,
                     const char* _Nonnull delimiter,
                     async_handler _Nonnull handler);

#endif /* io_async_socket_h */

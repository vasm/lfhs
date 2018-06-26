//
//  io_async_socket.c
//
//  lfhs: lightning-fast http server
//  Copyleft 2017 Vasiliy Sabadazh. All rights are granted.
//

#include "io_async_socket.h"
#include "io_buffer.h"
#include <stdbool.h>


enum io_socket_cmd {
    io_socket_cmd_read = 0,
    io_socket_cmd_read_until = 1,
    io_socket_cmd_write = 2
};


static bool _io_thread_running = false;

void io_thread(void)
{
    _io_thread_running = true;
}

int async_read(io_connection_state* _Nonnull socket, size_t bytes, async_handler _Nonnull handler)
{
    return io_socket_no_error;
}


int async_read_until(io_connection_state* _Nonnull socket,
                     const char* _Nonnull delimiter,
                     async_handler _Nonnull handler)
{
    return io_socket_no_error;
}

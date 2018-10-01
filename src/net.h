#ifndef NET_H
#define NET_H

/*
 * Platform-dependent includes
 */


#if (defined (__unix__) || defined(__APPLE__)) // we're good
    #define POSIX
    #include <sys/ioctl.h>
    #include <sys/socket.h>
    #include <sys/time.h>
    #include <netinet/in.h>
    #include <unistd.h>


    typedef int socket_t;
    typedef int ioctlarg_t;

    inline bool is_bad_socket(socket_t socket_handle) { return socket_handle < 0; }


#elif defined(_MSC_VER)
    #include <WinSock2.h>
    //#include <ws2tcpip.h>
    #include <stdio.h>
    #pragma comment(lib, "Ws2_32.lib")

    typedef SOCKET socket_t;
    typedef u_long ioctlarg_t;

    int close(socket_t s) { return closesocket(s); }
    int ioctl(socket_t s, long cmd, u_long* argp) { return ioctlsocket(s, cmd, argp); }

    inline bool is_bad_socket(socket_t socket_handle) { return socket_handle == ~0ul; }
#elif true
    #error "Compiling on unknown platform"
#endif

enum net_bool { FALSE = 0, TRUE = 1 };

#endif // NET_H

//
// Created by kovacs on 2022. 04. 14..
//

#ifndef CROSS_FOX_SERVER_H
#define CROSS_FOX_SERVER_H

#ifdef _WIN32
    #include <WinSock2.h>
    #include <Ws2tcpip.h>
    #pragma comment(lib, "Ws2_32.lib")
#else
    // Assume that any non-Windows platform uses POSIX-style sockets instead.
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <netdb.h>  // Needed for getaddrinfo() and freeaddrinfo()
    #include <unistd.h> // Needed for close()
#endif

#include <string.h>

struct RabbitServer{
    int domain;
    int service;
    int protocol;
    u_long _interface;
    char* port;
    int backlog;

    struct sockaddr_in address;

    SOCKET socket;

    void (*rbt_launch)(struct RabbitServer *server);
};

//this is the declaration of the constructor
struct RabbitServer rbt_server_constructor(int domain, int service, int protocol, u_long _interface, char* port, int backlog, void(*rbt_launch)(struct RabbitServer *server));


#endif //CROSS_FOX_SERVER_H

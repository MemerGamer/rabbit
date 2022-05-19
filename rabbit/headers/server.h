//
// Server implementation
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

#include "../headers/hashtable.h"
#include "errors.h"

typedef struct RabbitServer{
    int address_family;    // default: AF_INET
    int protocol;          // default: IPPROTO_TCP
    int port;              // default: 80
    char* address;         // default: "127.0.0.1"
    int backlog;           // default: 1

    SOCKET listen_socket;  // is created with the data given above
    SOCKET accept_socket;  // is created on every request

    RabbitEndpointHashTable* endpoints;
} RabbitServer;

/**
 * GET /tutorials/other/top-20-mysql-best-practices/ HTTP/1.1
Host: code.tutsplus.com
User-Agent: Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.5) Gecko/20091102 Firefox/3.5.5 (.NET CLR 3.5.30729)
Accept: text/html,application/xhtml+xml,application/xml;q=0.9;q=0.8
Accept-Language: en-us,en;q=0.5
Accept-Encoding: gzip,deflate
        Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7
Keep-Alive: 300
Connection: keep-alive
        Cookie: PHPSESSID=r2t5uvjq435r4q7ib3vtdjq120
Pragma: no-cache
        Cache-Control: no-cache
 * * */
typedef struct RabbitRequest{
    char* method;
    char* path;
}RabbitRequest;

char* rbt_get_method_str(RabbitMethod method);

/**
 * Creates a RabbitServer
 * @param address_family
 * @param protocol
 * @param port
 * @param address
 * @param backlog
 * @return a RabbitServer on success, and NULL on error
 */
RabbitServer* rbt_create_server(int address_family, int protocol, int port, char* address, int backlog);

/**
 * Deletes a RabbitServer
 * @param pserver
 * @return RabbitError
 */
RabbitError rbt_delete_server(RabbitServer** pserver);

RabbitError rbt_end();

#endif //CROSS_FOX_SERVER_H

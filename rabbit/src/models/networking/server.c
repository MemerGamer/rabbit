//
// Created by kovacs on 2022. 04. 14..
//

#include "../../../headers/models/networking/server.h"
#include <stdio.h>
#include <stdlib.h>

//this is the implementation of the constructor, this function initializes the constructor with input data
struct RabbitServer rbt_server_constructor(int domain, int service, int protocol, u_long _interface, char* port, int backlog,
                                           void (*rbt_launch)(struct RabbitServer *server)) {
    #ifdef _WIN32
        WSADATA wsa_data;
        WSAStartup(MAKEWORD(1,1), &wsa_data);
    #endif

    struct RabbitServer server;

    struct addrinfo *result = NULL, hints;
    ZeroMemory(&hints, sizeof (hints));
    server.domain = hints.ai_family = domain;
    server.service = hints.ai_socktype = service;
    server.protocol = hints.ai_protocol = protocol;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the local address and port to be used by the server
    int iResult = getaddrinfo(NULL, port, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
    }

    server.socket = INVALID_SOCKET;
    server.socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (server.socket == INVALID_SOCKET) {
        printf("Error at socket(): %d\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
    }

    printf("Created server.\n");

    return server;
}

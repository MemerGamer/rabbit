//
// Server implementation
//

#include "../headers/server.h"
#include "../headers/defaults.h"

char* rbt_get_method_str(RabbitMethod method){
    switch (method) {
        case RBT_M_GET:
            return "GET";
        case RBT_M_POST:
            return "POST";
        case RBT_M_PUT:
            return "PUT";
        case RBT_M_DELETE:
            return "DELETE";
        default: return "GET";
    }
}

RabbitServer* rbt_create_server(int address_family, int protocol, int port, char* address, int backlog){
    RabbitServer* server = (RabbitServer*) calloc(1, sizeof(RabbitServer));
    server->address_family = address_family;
    server->protocol = protocol;
    server->port = port;
    server->address = (char*)calloc(10, sizeof(char));
    strcpy(server->address, address);
    server->backlog = backlog;
    server->endpoints = rbt_create_hash_table();
    !server->endpoints && RBT_SHOW_LOG && rbt_log("Unable to create hash table", RBT_LOG_FILE);

    server->listen_socket = socket(address_family, SOCK_STREAM, protocol);
    if (server->listen_socket == INVALID_SOCKET) {
        free(server->address);
        rbt_end();
        return NULL;
    }

    return server;
}

RabbitError rbt_delete_server(RabbitServer** pserver){
    free((*pserver)->address);
    closesocket((*pserver)->listen_socket);
    rbt_delete_hash_table(&(*pserver)->endpoints);
    free(*pserver);

    return RBT_ERR_NO_ERROR;
}

RabbitError rbt_end(){
    WSACleanup();
    return RBT_ERR_NO_ERROR;
}
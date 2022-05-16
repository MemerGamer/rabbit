//
// Server implementation
//

#include "../headers/server.h"
#include "../headers/defaults.h"

RabbitServer* rbt_create_server(int address_family, int protocol, int port, char* address, int backlog){
    RabbitServer* server = (RabbitServer*) calloc(1, sizeof(RabbitServer));
    server->address_family = address_family;
    server->protocol = protocol;
    server->port = port;
    strcpy_s(server->address, strlen(address), address);
    server->backlog = backlog;
    server->endpoints = rbt_create_hash_table();
//     !server->endpoints && RBT_LOG_ERRORS && rbt_log("Unable to create hash table", RBT_ERROR_LOG_FILE)

    return server;
}

RabbitError rbt_delete_server(RabbitServer **pserver) {
    return RBT_ERR_KEY_USED;
}

//
// Server implementation
//

#include "../headers/server.h"

RabbitServer *rbt_create_server(int address_family, int protocol, int port, char *address, int backlog) {
    return NULL;
}

RabbitError rbt_delete_server(RabbitServer **pserver) {
    return RBT_ERR_KEY_USED;
}

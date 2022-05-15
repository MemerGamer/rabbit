//
// All the functions that the framework offers
//

#include "../headers/actions.h"

RabbitError rbt_add_static(RabbitServer **pserver, char *resource_path) {
    return RBT_ERR_KEY_USED;
}

RabbitError rbt_add_api(RabbitServer **pserver, char *endpoint, void (*function)(const char *, ...)) {
    return RBT_ERR_KEY_USED;
}

RabbitError rbt_start_server(RabbitServer **pserver) {
    return RBT_ERR_KEY_USED;
}

RabbitError rbt_handle_request(RabbitServer **pserver) {
    return RBT_ERR_KEY_USED;
}

RabbitError rbt_stop_server(RabbitServer **pserver) {
    return RBT_ERR_KEY_USED;
}

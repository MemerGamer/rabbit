//
// Hash table implementation used to manage endpoints
//

#include "../headers/hashtable.h"

RabbitEndpointHashTable *rbt_create_hash_table() {
    return NULL;
}

int rbt_hash(char *key) {
    return 0;
}

RabbitEndpoint *rbt_get_from_hash_table(RabbitEndpointHashTable *hash_table, char *key) {
    return NULL;
}

char **rbt_get_hash_keys(RabbitEndpointHashTable *hash_table) {
    return NULL;
}

RabbitError rbt_put_to_hash_table(RabbitEndpointHashTable **phash_table, char *key, RabbitEndpoint *endpoint) {
    return RBT_ERR_KEY_USED;
}

RabbitError rbt_remove_from_hash_table(RabbitEndpointHashTable **phash_table, char *key) {
    return RBT_ERR_KEY_USED;
}

RabbitError rbt_delete_hash_table(RabbitEndpointHashTable **phash_table) {
    return RBT_ERR_KEY_USED;
}

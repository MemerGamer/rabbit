//
// Hash table implementation used to manage endpoints
//

#include "../headers/hashtable.h"
#include "../headers/defaults.h"
#include <stdlib.h>
#include <string.h>

RabbitEndpointHashTable *rbt_create_hash_table() {
    RabbitEndpointHashTable* hash_table = (RabbitEndpointHashTable*)calloc(1, sizeof(RabbitEndpointHashTable));
    hash_table->endpoints = (RabbitEndpoint**)calloc(RBT_HASH_TABLE_MAX_BUCKETS, sizeof(RabbitEndpoint*));
    hash_table->hash_keys = (char**)calloc(RBT_HASH_TABLE_MAX_ENDPOINTS, sizeof(char*));

    return hash_table;
}

int rbt_hash(char *key) {
    return 0;
}

RabbitEndpoint* rbt_get_from_hash_table(RabbitEndpointHashTable *hash_table, char *key) {
    unsigned int index = rbt_hash(key);
    return hash_table->endpoints[index];
}

char** rbt_get_hash_keys(RabbitEndpointHashTable *hash_table) {
    return NULL;
}

RabbitError rbt_put_to_hash_table(RabbitEndpointHashTable **phash_table, char *key, RabbitEndpoint *endpoint) {
    unsigned int index = rbt_hash(key);

    if ((*phash_table)->endpoints[index] != NULL){
        return RBT_ERR_KEY_USED;
    }

    memcpy((*phash_table)->endpoints[index], endpoint, sizeof(RabbitEndpoint));

    strcpy((*phash_table)->hash_keys[(*phash_table)->num_of_endpoints], key);
    (*phash_table)->num_of_endpoints++;

    return RBT_ERR_NO_ERROR;
}

RabbitError rbt_remove_from_hash_table(RabbitEndpointHashTable **phash_table, char *key) {
    unsigned int index = rbt_hash(key);

    if ((*phash_table)->endpoints[index] == NULL){
        return RBT_ERR_KEY_NULL_VALUE;
    }

    rbt_delete_endpoint(&(*phash_table)->endpoints[index]);

    strcpy((*phash_table)->hash_keys[(*phash_table)->num_of_endpoints], key);
    (*phash_table)->num_of_endpoints--;

    return RBT_ERR_NO_ERROR;
}

RabbitError rbt_delete_hash_table(RabbitEndpointHashTable **phash_table) {
    free((*phash_table)->endpoints);

    for (int i = 0; i < (*phash_table)->num_of_endpoints; ++i) {
        free((*phash_table)->hash_keys[i]);
    }
    free((*phash_table)->hash_keys);

    free(*phash_table);

    return RBT_ERR_NO_ERROR;
}

RabbitEndpoint* rbt_create_endpoint(char* endpoint, RabbitMethod method, void (*function)(const char* fmt, ...)){
    RabbitEndpoint* _endpoint = (RabbitEndpoint*)calloc(1, sizeof(RabbitEndpoint));
    strcpy(_endpoint->endpoint, endpoint);
    _endpoint->method = method;
    _endpoint->function = function;

    return _endpoint;
}

RabbitError rbt_delete_endpoint(RabbitEndpoint** p_endpoint){
    free((*p_endpoint)->endpoint);
    free(*p_endpoint);

    return RBT_ERR_NO_ERROR;
}

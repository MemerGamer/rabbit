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

    return hash_table;
}

int rbt_hash(char *key) {
    unsigned int hash = 5381;
    unsigned int c;

    while ((c = (unsigned char)*key++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash % RBT_HASH_TABLE_MAX_BUCKETS;
}

RabbitEndpoint* rbt_get_from_hash_table(RabbitEndpointHashTable *hash_table, char *key) {
    unsigned int index = rbt_hash(key);
    RabbitEndpoint* node = hash_table->endpoints[index];

    if (node == NULL){
        return NULL;    // the key is not in the table
    }

    while(!rbt_str_equals(node->endpoint, key) && node->next != NULL){
        node = node->next;
    }

    // creating new object to not pass next item in list
    return rbt_str_equals(node->endpoint, key) ? rbt_create_endpoint(node->endpoint, node->static_resource_path, node->method, node->function) : NULL;
}

RabbitError rbt_put_to_hash_table(RabbitEndpointHashTable **phash_table, char *key, RabbitEndpoint *endpoint) {
    if ((*phash_table)->num_of_endpoints + 1 > RBT_HASH_TABLE_MAX_ENDPOINTS){
        return RBT_HASH_TABLE_FULL;
    }
    if (rbt_get_from_hash_table(*phash_table, key) != NULL){
        return RBT_ERR_KEY_USED;
    }

    unsigned int index = rbt_hash(key);
    RabbitEndpoint* node = (*phash_table)->endpoints[index];

    if (node == NULL){  // if this is the first node in the bucket
        (*phash_table)->endpoints[index] = rbt_create_endpoint(endpoint->endpoint, endpoint->static_resource_path, endpoint->method, endpoint->function);
    }
    else { // if there are already nodes in the bucket, insert as first node
        RabbitEndpoint* tmpNode = rbt_create_endpoint(endpoint->endpoint,endpoint->static_resource_path,endpoint->method, endpoint->function);
        tmpNode->next = node;
        (*phash_table)->endpoints[index] = tmpNode;
    }

    (*phash_table)->num_of_endpoints++;
    return RBT_ERR_NO_ERROR;
}

RabbitError rbt_remove_from_hash_table(RabbitEndpointHashTable **phash_table, char *key) {
    unsigned int index = rbt_hash(key);
    RabbitEndpoint* node = (*phash_table)->endpoints[index];

    if (node == NULL){
        return RBT_ERR_KEY_NULL_VALUE;    // the key is not in the table
    }

    // remove first node
    if (rbt_str_equals(node->endpoint, key)){
        RabbitEndpoint* tmpNode = node;
        (*phash_table)->endpoints[index] = node->next;

        rbt_delete_endpoint(&tmpNode);
        (*phash_table)->num_of_endpoints--;
        return RBT_ERR_NO_ERROR;
    }

    while(!rbt_str_equals(node->next->endpoint, key) && node->next->next != NULL){
        node = node->next;
    }

    if (rbt_str_equals(node->next->endpoint, key)){
        RabbitEndpoint* tmpNode = node->next;
        node->next = node->next->next;

        rbt_delete_endpoint(&tmpNode);
        (*phash_table)->num_of_endpoints--;
        return RBT_ERR_NO_ERROR;
    }

    return RBT_ERR_KEY_NULL_VALUE;
}

RabbitError rbt_delete_hash_table(RabbitEndpointHashTable **phash_table) {
    for (int i = 0; i < RBT_HASH_TABLE_MAX_BUCKETS; ++i) {
        RabbitEndpoint* node = (*phash_table)->endpoints[i];

        while (node != NULL && node->next != NULL){
            RabbitEndpoint* tmpNode = node->next;
            node->next = node->next->next;
            free(tmpNode);
        }

        free(node);
    }

    free((*phash_table)->endpoints);
    free(*phash_table);

    return RBT_ERR_NO_ERROR;
}

RabbitEndpoint* rbt_create_endpoint(char* endpoint, char* static_resource_path, RabbitMethod method, void (*function)(const char* fmt, ...)){
    RabbitEndpoint* _endpoint = (RabbitEndpoint*)calloc(1, sizeof(RabbitEndpoint));
    _endpoint->endpoint = (char*)calloc(strlen(endpoint)+1, sizeof(char));
    if (static_resource_path != NULL){
        _endpoint->static_resource_path = (char*)calloc(strlen(static_resource_path)+1, sizeof(char));
        strcpy(_endpoint->static_resource_path, static_resource_path);
    }
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

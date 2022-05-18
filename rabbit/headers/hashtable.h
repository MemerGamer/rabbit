//
// Hash table implementation used to manage endpoints
//

#ifndef RABBIT_HASHTABLE_H
#define RABBIT_HASHTABLE_H

#include "../headers/constants.h"
#include "../headers/errors.h"
#include "../headers/utils.h"

typedef struct RabbitEndpoint{
    char* endpoint;                // the endpoint is used as a key
    char* static_resource_path;    // only applicable for static resources
    RabbitMethod method;
    void (*function)(const char* fmt, ...);  // has to be researched. Variadic functions
    struct RabbitEndpoint* next;
} RabbitEndpoint;

typedef struct{
    unsigned int num_of_endpoints;
    RabbitEndpoint** endpoints;
} RabbitEndpointHashTable;

/**
 *  Creates a RabbitEndpointHashTable
 * @return a Rabbit hash table
 */
RabbitEndpointHashTable* rbt_create_hash_table();

/**
 * Hashing function used by Rabbit
 * @param key
 * @return index
 */
int rbt_hash(char* key);

/**
 * Returns the value at the given key from a RabbitEndpointHashTable
 * @param hash_table
 * @param key
 * @return
 */
RabbitEndpoint* rbt_get_from_hash_table(RabbitEndpointHashTable* hash_table, char* key);

/**
 * Puts a RabbitEndpoint in a RabbitEndpointHashTable, at the given key <br>
 * <b>Note:</b> if the key has a RabbitEndpoint already, the function returns RBT_ERR_KEY_USED.
 * Please remove the RabbitEndpoint from the key before inserting a new value
 * @param phash_table
 * @param key
 * @param endpoint
 * @return RabbitError
 */
RabbitError rbt_put_to_hash_table(RabbitEndpointHashTable** phash_table, char* key, RabbitEndpoint* endpoint);

/**
 * Removes the RabbitEndpoint at the given key from the RabbitEndpointHashTable <br>
 * <b>Note:</b> the function returns RBT_ERR_NULL_POINTER if the RabbitEndpointHashTable is NULL
 * @param phash_table
 * @param key
 * @return RabbitError
 */
RabbitError rbt_remove_from_hash_table(RabbitEndpointHashTable** phash_table, char* key);

/**
 * Deletes a RabbitEndpointHashTable <br>
 * <b>Note:</b> the function returns RBT_ERR_NULL_POINTER is the RabbitEndpointHashTable is NULL
 * @param phash_table
 * @return RabbitError
 */
RabbitError rbt_delete_hash_table(RabbitEndpointHashTable** phash_table);

/**
 * Creates a RabbitEndpoint
 * @param endpoint
 * @param method
 * @param function
 * @return
 */
RabbitEndpoint* rbt_create_endpoint(char* endpoint, char* static_resource_path, RabbitMethod method, void (*function)(const char* fmt, ...));

/**
 * Deletes a RabbitEndpoint
 * @param p_endpoint
 * @return
 */
RabbitError rbt_delete_endpoint(RabbitEndpoint** p_endpoint);

#endif //RABBIT_HASHTABLE_H

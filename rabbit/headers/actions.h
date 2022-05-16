//
// All the functions that the framework offers
//

#ifndef RABBIT_ACTIONS_H
#define RABBIT_ACTIONS_H

#include "server.h"

/**
 * Initializes system specific socket implementation <br>
 * <b>Note:</b> this function needs to be called before any other
 * @return RabbitError
 */
RabbitError rbt_init();

/**
 * Adds a static resource to the server <br>
 * Static resources can be html pages, images, scripts, etc.
 * @param pserver
 * @param resource_path a full path to the resource
 * @return RabbitError
 */
RabbitError rbt_add_static(RabbitServer** pserver, char* resource_path);

/**
 * Adds a REST API endpoint to the server
 * @param pserver
 * @param endpoint
 * @param function function to execute
 * @return RabbitError
 */
RabbitError rbt_add_api(RabbitServer** pserver, char* endpoint, void (*function)(const char* fmt, ...));

/**
 * Starts the given server
 * @param server
 * @return RabbitError
 */
RabbitError rbt_start_server(RabbitServer* server);

/**
 * Handles the next incoming request <br>
 * <b>Note:</b> in order to handle all requests, call the function inside a loop
 * @param pserver
 * @return RabbitError
 */
RabbitError rbt_handle_request(RabbitServer** pserver);

/**
 * Stops the server
 * @param pserver
 * @return RabbitError
 */
RabbitError rbt_stop_server(RabbitServer** pserver);

#endif //RABBIT_ACTIONS_H

//
// All the functions that the framework offers
//

#include <stdio.h>
#include "../headers/actions.h"
#include "../headers/errors.h"
#include "../headers/defaults.h"
#include "../headers/arg_array.h"

RabbitError rbt_init(){
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (iResult != NO_ERROR) {
        return RBT_ERR_CANT_START_SERVER;
    }

    return RBT_ERR_NO_ERROR;
}

RabbitError rbt_add_static(RabbitServer **pserver, char* endpoint, char *resource_path) {
    RabbitEndpoint* rbt_endpoint = rbt_create_endpoint(endpoint, resource_path, RBT_M_GET, NULL, NULL);
    if (!rbt_endpoint){
        return RBT_ERR_NULL_POINTER;
    }

    if (rbt_put_to_hash_table(&(*pserver)->endpoints, endpoint, rbt_endpoint) == RBT_ERR_KEY_USED){
        if (RBT_SHOW_LOG){
            char tmp_buff[100];
            sprintf(tmp_buff, "Endpoint \"%s\" already used", endpoint);
            rbt_log(tmp_buff, RBT_LOG_FILE);
        }
        return RBT_ERR_CANT_ADD_STATIC;
    }
    rbt_delete_endpoint(&rbt_endpoint);

    return RBT_ERR_NO_ERROR;
}

RabbitError rbt_add_api(RabbitServer **pserver, char *endpoint, void (*function)(RabbitArgArray*), ...) {
    va_list arg_ptr;    // variable argument list
    va_start(arg_ptr, function);    // start of variable list
    RabbitArgArray* arg_array = rbt_get_arg_array(arg_ptr);

    RabbitEndpoint* rbt_endpoint = rbt_create_endpoint(endpoint, NULL, RBT_M_GET, function, arg_array);
    if (!rbt_endpoint){
        return RBT_ERR_NULL_POINTER;
    }

    if (rbt_put_to_hash_table(&(*pserver)->endpoints, endpoint, rbt_endpoint) == RBT_ERR_KEY_USED){
        if (RBT_SHOW_LOG){
            char tmp_buff[100];
            sprintf(tmp_buff, "Endpoint \"%s\" already used", endpoint);
            rbt_log(tmp_buff, RBT_LOG_FILE);
        }
        return RBT_ERR_CANT_ADD_STATIC;
    }
    rbt_delete_endpoint(&rbt_endpoint);

    return RBT_ERR_NO_ERROR;}

RabbitError rbt_start_server(RabbitServer* server) {
    struct sockaddr_in service;
    service.sin_family = server->address_family;
    service.sin_addr.s_addr = inet_addr(server->address);
    service.sin_port = htons(server->port);

    if (bind(server->listen_socket,(SOCKADDR *) & service, sizeof (service)) == SOCKET_ERROR) {
        closesocket(server->listen_socket);
        rbt_end();
        return RBT_ERR_CANT_START_SERVER;
    }

    if (listen(server->listen_socket, 1) == SOCKET_ERROR) {
        closesocket(server->listen_socket);
        rbt_end();
        return RBT_ERR_CANT_START_SERVER;
    }

    RBT_SHOW_LOG && rbt_log("Rabbit Server started at http://"RBT_HOST"!", RBT_LOG_FILE);
    RBT_SHOW_LOG && rbt_log("To disable these messages check \"defaults.h\". For more info check out our GitHub repository", RBT_LOG_FILE);

    return RBT_ERR_NO_ERROR;
}

RabbitError rbt_handle_request(RabbitServer **pserver) {
    SOCKET AcceptSocket;

    // Accept the connection.
    AcceptSocket = accept((*pserver)->listen_socket, NULL, NULL);
    if (AcceptSocket == INVALID_SOCKET) {
        closesocket((*pserver)->listen_socket);
        rbt_end();
        return RBT_ERR_SOCKET_ERROR;
    } else{
        /**
         * NOTE:
         * szoval itt lenyegeben a kovetkezo tortenik:
         *  - a request_buffer-be belekerul a request, annak az elejen van a metodus es a path
         *  - lekered az endpointok kozott a path-et
         *  - ha megkapta, es talal a metodus, akkor megnezed, hogy statikus vagy api resource
         *  - statikus ha a function == NULL, es ha != akkor api resource
         *  - ha statikus kiolvasod egy bufferbe a lekert adatot, a static_resource_path -rol, es visszakuldod
         *  - ha meg api akkor ki kell talaljuk, hogy hogyan legyen azzal a variadic functionnal
         */

        RabbitEndpoint* endpoint = rbt_get_from_hash_table((*pserver)->endpoints, "/");
        char request_buffer[RBT_MAX_REQUEST_BUFFER_SIZE+1];
        recv(AcceptSocket, request_buffer, RBT_MAX_REQUEST_BUFFER_SIZE, 0);
        char response_buffer[RBT_MAX_RESPONSE_BUFFER_SIZE];

        if (endpoint != NULL && rbt_str_equals(rbt_get_method_str(endpoint->method),"GET")){
            if (endpoint->function == NULL && endpoint->static_resource_path != NULL){
                FILE *file_stream = fopen(endpoint->static_resource_path, "rb");
                fseek(file_stream, 0, SEEK_END);
                long fsize = ftell(file_stream);
                fseek(file_stream, 0, SEEK_SET);

                char* file_content = (char*)calloc(fsize + 2, sizeof(char));
                strcat(file_content, "\n");  //IMPORTANT: it needs a separating \n
                fread(file_content, fsize, 1, file_stream);
                fclose(file_stream);

                file_content[fsize] = '\0';

                char* content = (char*) calloc(strlen(file_content), sizeof(char));
                memcpy(content, file_content, strlen(file_content) + 1);

                // user content as body
                RabbitResponse* response = rbt_create_response("200 OK",
                                                               rbt_get_file_content_type(endpoint->static_resource_path), content);
                strcpy(response_buffer, rbt_get_response_str(response));
                rbt_delete_response(&response);
            }
            else if (endpoint->function != NULL && endpoint->static_resource_path == NULL){
                endpoint->function(endpoint->arg_array);
            }
        }
        else {
            // endpoint not specified by user => 404 response
        }

        RBT_SHOW_LOG && rbt_log("Sending response", RBT_LOG_FILE);

        send(AcceptSocket, response_buffer, (int)strlen(response_buffer), 0 );
    }
    closesocket(AcceptSocket);

    return RBT_ERR_NO_ERROR;
}

RabbitError rbt_stop_server(RabbitServer **pserver) {
    rbt_end();
    return RBT_ERR_NO_ERROR;
}

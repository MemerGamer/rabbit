//
// All the functions that the framework offers
//

#include <stdio.h>
#include "../headers/actions.h"

RabbitError rbt_init(){
    #ifdef _WIN32
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (iResult != NO_ERROR) {
        return RBT_ERR_CANT_START_SERVER;
    }
    #endif

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


#ifdef _WIN32
    if (bind(server->listen_socket,(SOCKADDR *) & service, sizeof (service)) == SOCKET_ERROR) {

        closesocket(server->listen_socket);

        rbt_end();
        return RBT_ERR_CANT_START_SERVER;
    }
#else
    if (bind(server->listen_socket,(struct sockaddr *) & service, sizeof (service)) == SOCKET_ERROR) {


        close(server->listen_socket);

        rbt_end();
        return RBT_ERR_CANT_START_SERVER;
    }
#endif


    if (listen(server->listen_socket, 1) == SOCKET_ERROR) {
        #ifdef _WIN32
            closesocket(server->listen_socket);
        #else
            close(server->listen_socket);
        #endif
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
#ifdef _WIN32
        closesocket((*pserver)->listen_socket);
#else
        close((*pserver)->listen_socket);
#endif
        rbt_end();
        return RBT_ERR_SOCKET_ERROR;
    } else{
        char request_buffer[RBT_MAX_REQUEST_BUFFER_SIZE+1];
        recv(AcceptSocket, request_buffer, RBT_MAX_REQUEST_BUFFER_SIZE, 0);
        RabbitRequest* request = rbt_parse_request(request_buffer);
        RabbitEndpoint *endpoint = NULL;
        if (request->path != NULL){
            rbt_str_replace(request->path, "%20", " ");
            endpoint = rbt_get_from_hash_table((*pserver)->endpoints, request->path);
        }
        else {
            request = NULL;
        }

        char response_buffer[RBT_MAX_RESPONSE_BUFFER_SIZE];

        if (request != NULL && endpoint != NULL && rbt_str_equals(rbt_get_method_str(endpoint->method),request->method)){
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

                if (rbt_str_equals(rbt_get_file_content_type(endpoint->static_resource_path), "image/png")){
                    send(AcceptSocket, file_content, fsize, 0 );
#ifdef _WIN32
                    closesocket(AcceptSocket);
#else
                    close(AcceptSocket);
#endif

                    return RBT_ERR_NO_ERROR;
                }

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
                RabbitResponse* response = rbt_create_response("200 OK","", "");
                strcpy(response_buffer, rbt_get_response_str(response));
                rbt_delete_response(&response);
            }
        }
        else {
            RabbitResponse* response = rbt_create_response("404 NOT FOUND","", "");
            strcpy(response_buffer, rbt_get_response_str(response));
            rbt_delete_response(&response);
        }

        RBT_SHOW_LOG && rbt_log("Sending response", RBT_LOG_FILE);

        send(AcceptSocket, response_buffer, (int)strlen(response_buffer), 0 );
    }
#ifdef _WIN32
    closesocket(AcceptSocket);
#else
    close(AcceptSocket);
#endif

    return RBT_ERR_NO_ERROR;
}

RabbitError rbt_stop_server(RabbitServer **pserver) {
    rbt_end();
    return RBT_ERR_NO_ERROR;
}

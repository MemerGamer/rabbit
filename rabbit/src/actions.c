//
// All the functions that the framework offers
//

#include <stdio.h>
#include "../headers/actions.h"
#include "../headers/errors.h"
#include "../headers/defaults.h"

RabbitError rbt_init(){
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (iResult != NO_ERROR) {
        return RBT_ERR_CANT_START_SERVER;
    }

    return RBT_ERR_NO_ERROR;
}

RabbitError rbt_add_static(RabbitServer **pserver, char* endpoint, char *resource_path) {
    RabbitEndpoint* rbt_endpoint = rbt_create_endpoint(endpoint, resource_path, RBT_M_GET, NULL);
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

RabbitError rbt_add_api(RabbitServer **pserver, char *endpoint, void (*function)(const char *, ...)) {
    RabbitEndpoint* rbt_endpoint = rbt_create_endpoint(endpoint, NULL, RBT_M_GET, function);
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
    /**
     * NOTE:
     * szoval itt lenyegeben a kovetkezo tortenik:
     *  - a buffer-be belekerul a request, annak az elejen van a metodus es a path
     *  - lekered az endpointok kozott a path-et
     *  - ha megkapta, es talal a metodus, akkor megnezed, hogy statikus vagy api resource
     *  - statikus ha a function == NULL, es ha != akkor api resource
     *  - ha statikus kiolvasod egy bufferbe a lekert adatot, a static_resource_path -rol, es visszakuldod
     *  - ha meg api akkor ki kell talaljuk, hogy hogyan legyen azzal a variadic functionnal
     */

    RabbitEndpoint* endpoint = rbt_get_from_hash_table((*pserver)->endpoints, "ide_a_request_path");
    if (endpoint != NULL){ // ide meg kell, hogy && endpoint->method == beolvasott endpoint - erre kellene egy fgv ami visszateriti az enumot string formaba, es hasznalhatod utana a rbt_str_equals() fgv-t a utils.h-bol
        if (endpoint->function == NULL && endpoint->static_resource_path != NULL){
            // static resource => serve static resource

            // egyelore probald meg, hogy csak siman kiolvasod a file tartalmat egy bufferbe (lent van erre pelda)
            // es csak visszakuldod
            // lehet megy, lehet nem, idk, meglatjuk XD
        }
        else if (endpoint->function != NULL && endpoint->static_resource_path == NULL){
            // api resource => call function
        }
    }
    else {
        // endpoint not specified by user => 404 response
    }

    SOCKET AcceptSocket;

    //----------------------
    // Accept the connection.
    AcceptSocket = accept((*pserver)->listen_socket, NULL, NULL);
    if (AcceptSocket == INVALID_SOCKET) {
        closesocket((*pserver)->listen_socket);
        rbt_end();
        return -10;
    } else{
        static int numberOfConnections = 0;
        char buffer[30001];
        recv(AcceptSocket,buffer,30000,0);
        char hello[20000];

        //setting up data about version
        char* version = (char*) calloc(strlen("HTTP/1.1 200 OK GMT\n"), sizeof(char));
        memcpy(version, "HTTP/1.1 200 OK GMT\n", strlen("HTTP/1.1 200 OK GMT\n") + 1);

        //setting up data about server type
        char* server_type = (char*) calloc(strlen("Server: Apache/2.2.14 (Win32)\n"), sizeof(char));
        memcpy(server_type, "Server: Apache/2.2.14 (Win32)\n", strlen("Server: Apache/2.2.14 (Win32)\n") + 1);

        //setting up data about date
        char* date= (char*) calloc(strlen("Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT\n"), sizeof(char));
        memcpy(date, "Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT\n", strlen("Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT\n") + 1);

        //setting up data about content type
        char* content_type = (char*) calloc(strlen("Content-Type: text/html\n"), sizeof(char));
        memcpy(content_type, "Content-Type: text/html\n", strlen("Content-Type: text/html\n") + 1);

        //setting up data about connection type
        char* connection_type = (char*) calloc(strlen("Connection: Closed\n"), sizeof(char));
        memcpy(connection_type, "Connection: Closed\n", strlen("Connection: Closed\n") + 1);

        //setting up data about content
        FILE *f = fopen("../demo/src/index.html", "rb");
        fseek(f, 0, SEEK_END);
        long fsize = ftell(f);
        fseek(f, 0, SEEK_SET);

        char* contentFromInput = (char*)calloc(fsize + 2,sizeof(char));
        strcat(contentFromInput, "\n");  //IMPORTANT: it needs a separating \n
        fread(contentFromInput, fsize, 1, f);
        fclose(f);

        contentFromInput[fsize] = 0;

        char* content = (char*) calloc(strlen(contentFromInput), sizeof(char));
        memcpy(content, contentFromInput, strlen(contentFromInput) + 1);

        //allocating memory for full response
        char* full_response = (char*)calloc((
                                                              strlen(version) + strlen(server_type) +
                                                              strlen(date) + strlen(content_type) +
                                                              strlen(connection_type) +
                                                              strlen(content) + 1
                                                      ), sizeof(char));

        //creating full response string
        strcat(full_response, version);
        strcat(full_response, server_type);
        strcat(full_response, date);
        strcat(full_response, content_type);
        strcat(full_response, connection_type);
        strcat(full_response, content);
        //sprintf(hello,"HTTP/1.1 200 OK GMT\nServer: Apache/2.2.14 (Win32)\nLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT\nContent-Type: text/html\nConnection: Closed\n\n<html><body><h1>Hello, Fox %i!</h1></body></html>", numberOfConnections++);
//        sprintf(hello, full_response, numberOfConnections++); idk it broke :c
        strcpy(hello, full_response);
        RBT_SHOW_LOG && rbt_log("Sending response", RBT_LOG_FILE);
        send(AcceptSocket, hello, (int)strlen(hello), 0 );
    }
    closesocket(AcceptSocket);

    return RBT_ERR_NO_ERROR;
}

RabbitError rbt_stop_server(RabbitServer **pserver) {
    rbt_end();
    return RBT_ERR_NO_ERROR;
}

//
// Created by szabi on 5/20/2022.
//

#include "../headers/response.h"

RabbitResponse* rbt_create_response(char* status, char* content_type, char* body){
    RabbitResponse* response = (RabbitResponse*)calloc(1, sizeof(RabbitResponse));

    response->version = (char*)calloc(strlen("HTTP/1.1"), sizeof(char));
    strcpy(response->version, "HTTP/1.1");
    response->version[strlen("HTTP/1.1")] = '\0';

    response->status = (char*)calloc(strlen(status), sizeof(char));
    strcpy(response->status, status);
    response->status[strlen(status)] = '\0';

    response->server = (char*)calloc(strlen("Rabbit"), sizeof(char));
    strcpy(response->server, "Rabbit");
    response->server[strlen("Rabbit")] = '\0';

    // more info at https://zetcode.com/articles/cdatetime/
    time_t now;
    time(&now);
    struct tm *local = localtime(&now);
    response->date = (char*)calloc(30, sizeof(char));
    strftime(response->date, 30, "%a, %e %b %G %T GMT", local);
    response->date[30] = '\0';

    response->content_type = (char*)calloc(strlen(content_type), sizeof(char));
    strcpy(response->content_type, content_type);

    response->connection_type = (char*)calloc(strlen("Closed"), sizeof(char));
    strcpy(response->connection_type, "Closed");

    response->body = (char*)calloc(strlen(body), sizeof(char));
    strcpy(response->body, body);

    return response;
}
char* rbt_get_response_str(RabbitResponse* response){
    char* response_str = (char*)calloc(RBT_MAX_RESPONSE_BUFFER_SIZE, sizeof(char));
    strcpy(response_str, response->version);
    strcat(response_str, " ");
    strcat(response_str, response->status);
    strcat(response_str, "\nServer: ");
    strcat(response_str, response->server);
    strcat(response_str, "\nLast-Modified: ");
    strcat(response_str, response->date);
    strcat(response_str, "\nContent-Type: ");
    strcat(response_str, response->content_type);
    strcat(response_str, "\nConnection: ");
    strcat(response_str, response->connection_type);
    strcat(response_str, "\n\n");
    strcat(response_str, response->body);

    return response_str;
}

void rbt_delete_response(RabbitResponse** p_response){
//    free((*p_response)->version);
//    free((*p_response)->status);
//    free((*p_response)->server);
//    free((*p_response)->date);
//    free((*p_response)->content_type);
//    free((*p_response)->connection_type);
//    free((*p_response)->body);
    free((*p_response));
}
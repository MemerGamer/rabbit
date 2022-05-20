//
// Created by szabi on 5/20/2022.
//

#include "../headers/request.h"

RabbitRequest* rbt_parse_request(char* request_buffer){
    RabbitRequest* request = (RabbitRequest*)calloc(1, sizeof(RabbitRequest));
    char* token = strtok(request_buffer, " ");
    request->method = token;
    token = strtok(NULL, " ");
    request->path = token;

    return request;
}

char* rbt_get_method_str(RabbitMethod method){
    switch (method) {
        case RBT_M_GET:
            return "GET";
        case RBT_M_POST:
            return "POST";
        case RBT_M_PUT:
            return "PUT";
        case RBT_M_DELETE:
            return "DELETE";
        default: return "GET";
    }
}
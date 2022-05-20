//
// Created by szabi on 5/20/2022.
//

#ifndef RABBIT_REQUEST_H
#define RABBIT_REQUEST_H

#include <stdlib.h>
#include <string.h>

typedef enum RabbitMethod{
    RBT_M_GET,
    RBT_M_POST,
    RBT_M_PUT,
    RBT_M_DELETE
} RabbitMethod;

typedef struct RabbitRequest{
    char* method;
    char* path;
}RabbitRequest;

RabbitRequest* rbt_parse_request(char* request_buffer);
char* rbt_get_method_str(RabbitMethod method);

#endif //RABBIT_REQUEST_H

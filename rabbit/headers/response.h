//
// Created by szabi on 5/20/2022.
//

#ifndef RABBIT_RESPONSE_H
#define RABBIT_RESPONSE_H

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../headers/defaults.h"

typedef struct {
    char* version;          // HTTP/1.1
    char* status;           // 200 OK
    char* server;           // Server: Rabbit
    char* date;             // Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT
    char* content_type;     // Content-Type: text/html
    char* connection_type;  // Connection: Closed
    char* body;             // <body></body>
}RabbitResponse;

RabbitResponse* rbt_create_response(char* status, char* content_type, char* body);
char* rbt_get_response_str(RabbitResponse* response);
void rbt_delete_response(RabbitResponse** p_response);

#endif //RABBIT_RESPONSE_H

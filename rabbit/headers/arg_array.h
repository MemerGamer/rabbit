//
// Created by szabi on 5/19/2022.
//

#ifndef RABBIT_ARG_ARRAY_H
#define RABBIT_ARG_ARRAY_H

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/utils.h"

typedef union{
    char* string;
    char* pointer;
    int integer;
    float decimal;
}RabbitArg;

typedef struct{
    RabbitArg** values;
    unsigned int num_of_args;
}RabbitArgArray;

RabbitArgArray* rbt_create_arg_array(unsigned int num_of_args);
RabbitArgArray* rbt_get_arg_array(va_list arg_ptr);
void rbt_delete_arg_array(RabbitArgArray** p_arg_array);
void rbt_copy_arg_array(RabbitArgArray* from, RabbitArgArray** to);

#endif //RABBIT_ARG_ARRAY_H

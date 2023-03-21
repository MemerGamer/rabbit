//
// Created by szabi on 5/19/2022.
//

#include "../headers/arg_array.h"
#include <stdio.h>
RabbitArgArray* rbt_get_arg_array(va_list arg_ptr){

    char* format_string = va_arg(arg_ptr, char*);  // get format string
    RabbitArgArray* arg_array = NULL;

    printf("format_string=%s\n", format_string);

    // fixed possible memory leak issue
    if (format_string != NULL && format_string != (char*)0xcccccccc){ // hacky way to check if there are no arguments
        arg_array = rbt_create_arg_array(rbt_get_num_of_chars(format_string, '%'));

        // here was a memory leak because of strtok
        char* token;
        char* next_token;
        token = strtok_r(format_string, " ", &next_token);

        while( token != NULL ) {
            printf("token=%s\n", token);
            arg_array->values[arg_array->num_of_args] = (RabbitArg*)calloc(1, sizeof(RabbitArg));

            if (rbt_str_equals(token, "%s")){
                char* string = va_arg(arg_ptr, char*);
                arg_array->values[arg_array->num_of_args]->string = string;
            }
            else if (rbt_str_equals(token, "%i")){
                int integer = va_arg(arg_ptr, int);
                arg_array->values[arg_array->num_of_args]->integer = integer;
            }
            else if (rbt_str_equals(token, "%p")){
                char* pointer = va_arg(arg_ptr, void*);
                arg_array->values[arg_array->num_of_args]->pointer = pointer;
            }
            else if(rbt_str_equals(token, "%f")){
                float decimal = va_arg(arg_ptr, double);
                arg_array->values[arg_array->num_of_args]->decimal = decimal;
            }

            arg_array->num_of_args++;
            token = strtok(NULL, " ");
        }
    }
    va_end(arg_ptr);

    return arg_array;
}

RabbitArgArray* rbt_create_arg_array(unsigned int num_of_args){
    RabbitArgArray* arg_array;
    arg_array = (RabbitArgArray*)calloc(1, sizeof(RabbitArgArray));
    arg_array->values = (RabbitArg**)calloc(num_of_args, sizeof(RabbitArg*));

    return arg_array;
}

void rbt_delete_arg_array(RabbitArgArray** p_arg_array){
    free((*p_arg_array)->values); // todo: remove with type checking
    (*p_arg_array)->values = NULL;
    free((*p_arg_array));
    (*p_arg_array) = NULL;
}

void rbt_copy_arg_array(RabbitArgArray* from, RabbitArgArray** to){
    (*to)->num_of_args = from->num_of_args;
    for (int i = 0; i < from->num_of_args; ++i) {
        (*to)->values[i] = from->values[i];
    }
}
#include <stdio.h>
#include "../rabbit/headers/include.h"

void test(RabbitArgArray* arg_array);
void count(RabbitArgArray* arg_array);

int main() {
    rbt_init();

    RabbitServer* server = rbt_create_server(AF_INET, IPPROTO_TCP, 80, RBT_HOST, 20);
    rbt_start_server(server);

    rbt_add_static(&server, "/", "../demo/src/index.html");
    rbt_add_static(&server, "/test.txt", "../demo/src/test.txt");
    rbt_add_static(&server, "/Rabbit icon white.png", "../demo/src/Rabbit icon white.png");
    rbt_add_static(&server, "/background.png", "../demo/src/background.png");
    // Note: the server on unix based systems are only working for static files
    // rbt_add_api(&server, "/api/test", test, "%s %s %i %p", "alma", "korte", 1, server);
    rbt_add_static(&server, "/api/count", "../demo/src/count.txt");
    // rbt_add_api(&server, "/api/count/up", count);

    while(1){
        rbt_handle_request(&server);
    }
}

void test(RabbitArgArray* arg_array){
    if (arg_array == NULL){
        printf("No arguments passed with function\n");
        return;
    }

    printf("\nTesting function calling.\n");
    printf("A string: %s\n", arg_array->values[0]->string);
    printf("Another string: %s\n", arg_array->values[1]->string);
    printf("An integer: %i\n", arg_array->values[2]->integer);
    printf("A pointer: %p, which is a RabbitServer* pointer.\n", arg_array->values[3]->pointer);
    printf("And I can cast it, and get the address for example: %s\n", ((RabbitServer*)arg_array->values[3]->pointer)->address);
}

void count(RabbitArgArray* arg_array){
    static int counter = 0;
    freopen("../demo/src/count.txt", "w", stdout);
    printf("%i", counter);
    freopen("CON", "w", stdout);
    counter++;
}

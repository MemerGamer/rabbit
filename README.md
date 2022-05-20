<!-- ![Rabbit logo](./icons/svg/Rabbit%20logo%20white.svg) -->
<p align=center>
 <img src="./icons/svg/Rabbit%20logo%20white.svg#gh-dark-mode-only" alt="Rabbit logo" style="width:300px;"/>
 <img src="./icons/svg/Rabbit%20logo%20black.svg#gh-light-mode-only" alt="Rabbit logo" style="width:300px;"/>
</p>
<p align=center>
  Rabbit is a versatile and easy to use webserver and REST API making framework written in C
</p>

#### SIDE NOTE: This project originates from [cross-fox](https://github.com/MemerGamer/cross-fox)

## Datatypes:

### RabbitServer:

```c
typedef struct RabbitServer{
    int address_family;    // default: AF_INET
    int protocol;          // default: IPPROTO_TCP
    int port;              // default: 80
    char* address;         // default: "127.0.0.1"
    int backlog;           // default: 1

    SOCKET listen_socket;  // is created with the data given above
    SOCKET accept_socket;  // is created on every request

    RabbitEndpointHashTable* endpoints;
} RabbitServer;
```

### RabbitEndpoint:

```c
typedef struct RabbitEndpoint{
    char* endpoint;                // the endpoint is used as a key
    char* static_resource_path;    // only applicable for static resources
    RabbitMethod method;
    void (*function)(RabbitArgArray*);
    RabbitArgArray* arg_array;
    struct RabbitEndpoint* next;
} RabbitEndpoint;
```

### RabbitEndpointHashTable:

```c
typedef struct{
    unsigned int num_of_endpoints;
    RabbitEndpoint** endpoints;
} RabbitEndpointHashTable;
```

### RabbitMethod:

```c
typedef enum RabbitMethod{
    RBT_M_GET,
    RBT_M_POST,
    RBT_M_PUT,
    RBT_M_DELETE
} RabbitMethod;
```

### RabbitReuqest:

```c
typedef struct RabbitRequest{
    char* method;
    char* path;
}RabbitRequest;
```

## Defines:

```c
// constants.h
// constants that we use across the framework. These should not be changed.
```

```c
// defaults.h
// constants that hold the default values in the framework. These can be changed.
// region General settings
#define RBT_SHOW_LOG true
#define RBT_LOG_FILE "CON"
// endregion

// region Server defaults
#define RBT_HOST "127.0.0.1"

#define RBT_MIN_RESPONSE_BUFFER_SIZE 5000
#define RBT_MED_RESPONSE_BUFFER_SIZE 10000
#define RBT_MAX_RESPONSE_BUFFER_SIZE 20000

#define RBT_MAX_REQUEST_BUFFER_SIZE 5000

#define RBT_HASH_TABLE_MAX_BUCKETS 30
#define RBT_HASH_TABLE_MAX_ENDPOINTS 10

// endregion
```

## UML Drawing: [drawing](https://miro.com/app/board/uXjVO1xVzOQ=/)

## Make a simple http server:

1. Call `rbt_init()` before calling any other functions.
2. Use `rbt_create_server()` to create a RabbitServer struct.
3. Add static files using `rbt_add_static()`.

   **Note:** to serve all the files inside a folder, use `rbt_add_static_folder()`.

4. Use `rbt_start_server()` to start the previously created RabbitServer.
5. Call `rbt_handle_request()` inside an infinite loop.
   **Note:** if you call this function once, only your first request will be handled.
6. Implement a way of stopping the server, where you need to call `rbt_stop_server()`.
   You can see an example in the **demo** folder.

N**ote:** The last two steps are made as such, to not interfere with the program flow.

## Make a simple REST API:

1. Call `rbt_init()` before calling any other functions.
2. Use `rbt_create_server()` to create a RabbitServer struct.
3. Add REST API endpoints using `rbt_add_api()`.
4. Use `rbt_start_server()` to start the previously created RabbitServer.
5. Call `rbt_handle_request()` inside an infinite loop.
   **Note:** if you call this function once, only your first request will be handled.
6. Implement a way of stopping the server, where you need to call `rbt_stop_server()`.
   You can see an example in the **demo** folder.

N**ote:** The last two steps are made as such, to not interfere with the program flow.

## Test program 
### (implemented in **demo\main.c**):

```c
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
    rbt_add_api(&server, "/api/test", test, "%s %s %i %p", "alma", "korte", 1, server);
    rbt_add_static(&server, "/api/count", "../demo/src/count.txt");
    rbt_add_api(&server, "/api/count/up", count);

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
```

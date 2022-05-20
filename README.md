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

		char* root_path;       // default: "/"
		RabbitEndpoints* endpoints;
} RabbitServer;
```

### RabbitEndpointHashTable:

```c
typedef struct{
    unsigned int num_of_endpoints;
		RabbitEndpoint* endpoints;
		char** hashKeys;
} RabbitEndpointHashTable;
```

### RabbitEndpoint:

```c
typedef struct RabbitEndpoint{
		char* endpoint;                // the endpoint is used as a key
		RabbitMethod method;
    void (*function)(const char* fmt, ...);  // has to be researched. Variadic functions
} RabbitEndpoint;
```

### RabbitMethod:

```c
typedef enum{
	RBT_M_GET,
	RBT_M_POST
	RBT_M_PUT,
	RBT_M_DELETE
}RabbitMethod;
```

### RabbitReuqest:

```c
my_add_user(...RabbitServer...){
	// get somehow request body
	// do stuff
	// give us a response body and send response
}
```

## Defines:

```c
// constants.h
// constants that we use across the framework. These should not be changed.
```

```c
// defaults.h
// constants that hold the default values in the framework. These can be changed.
#define RBT_LOCALHOST "127.0.0.1"
#define RBT_MIN_RESPONSE_BUFFER_SIZE 5000
#define RBT_MED_RESPONSE_BUFFER_SIZE 10000
#define RBT_MAX_RESPONSE_BUFFER_SIZE 20000
#define RBT_MAX_REQUEST_BUFFER_SIZE 5000
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

## Tests:

```c
// HASHTABLE
RabbitEndpointHashTable* hash_table = rbt_create_hash_table();
RabbitEndpoint* endpoint = rbt_create_endpoint("alma", RBT_M_GET, NULL);
rbt_put_to_hash_table(&hash_table, endpoint->endpoint, endpoint);
rbt_delete_endpoint(&endpoint);

endpoint = rbt_create_endpoint("korte", RBT_M_GET, NULL);
rbt_put_to_hash_table(&hash_table, endpoint->endpoint, endpoint);
rbt_delete_endpoint(&endpoint);

endpoint = rbt_create_endpoint("dio", RBT_M_GET, NULL);
rbt_put_to_hash_table(&hash_table, endpoint->endpoint, endpoint);
rbt_delete_endpoint(&endpoint);

endpoint = rbt_create_endpoint("asdas", RBT_M_GET, NULL);
rbt_put_to_hash_table(&hash_table, endpoint->endpoint, endpoint);
rbt_delete_endpoint(&endpoint);

endpoint = rbt_create_endpoint("2133", RBT_M_GET, NULL);
rbt_put_to_hash_table(&hash_table, endpoint->endpoint, endpoint);
rbt_delete_endpoint(&endpoint);

endpoint = rbt_create_endpoint("d//o", RBT_M_GET, NULL);
rbt_put_to_hash_table(&hash_table, endpoint->endpoint, endpoint);
rbt_delete_endpoint(&endpoint);

endpoint = rbt_get_from_hash_table(hash_table, "d//o");

rbt_delete_hash_table(&hash_table);

```

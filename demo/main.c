#include <stdio.h>
#include "../rabbit/headers/actions.h"
#include "../rabbit/headers/defaults.h"

int main() {
    rbt_init();

    RabbitServer* server = rbt_create_server(AF_INET, IPPROTO_TCP, 80, RBT_HOST, 1);
    rbt_start_server(server);

    rbt_add_static(&server, "/", "../demo/src/index.html");
    // we'll have some issues with spaces :))
    rbt_add_static(&server, "/Rabbit icon white.png", "../demo/src/fox-logo.gif");
    rbt_add_api(&server, "/api/test", NULL);

    while(1){
        rbt_handle_request(&server);
    }
}

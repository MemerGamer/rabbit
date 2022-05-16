#include <stdio.h>
#include "../rabbit/headers/actions.h"
#include "../rabbit/headers/defaults.h"

int main() {
    rbt_init();

    RabbitServer* server = rbt_create_server(AF_INET, IPPROTO_TCP, 80, RBT_LOCALHOST, 1);

    rbt_start_server(server);

    while(1){
        rbt_handle_request(&server);
    }
}

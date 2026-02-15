#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include "comm.c"
#include <assert.h>

int main(int argc, char *argv[]){
    int sd = UDP_Open(10000);
    assert(sd > -1);
    while (1){
        struct sockaddr_in addrRcv;
        char* msg = server_receive(sd, &addrRcv);
        if (msg) {
            printf("Received message: %s\n", msg);
        }
        free(msg);
    }
    return 0;


}
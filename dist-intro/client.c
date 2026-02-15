#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include "comm.c"

#define BUFFER_SIZE 1024

char* read_file(char* filename){
    struct stat st;
    stat(filename, &st);
    int size = st.st_size;

    char* buffer = malloc(size + 1);
    FILE* f = fopen(filename, "r");
    fread(buffer, 1, size, f);
    buffer[size] = '\0';
    fclose(f);
    return buffer;
}

int main(int argc, char *argv[]){
    char* message = read_file(argv[1]);

    int sd = UDP_Open(20000);
    struct sockaddr_in addrSnd, addrRcv;
    int rc = UDP_FillSockAddr(&addrSnd, argv[2], 10000); // fill the addrSnd with the server's IP and port

    rc = client_send(sd, &addrSnd, message, strlen(message)); 

    free(message);
    /*
    if (rc > 0) {
        rc = receives(sd, &addrRcv, message, BUFFER_SIZE); // the OS after receiving the message from the server will fill the addrRcv with the server's IP and port
        // will need to use the addrRcv to see which server sent the message back to us (if there are multiple servers)


        printf("Received message: %s\n", message);

    }
    */
    return 0;


}
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <string.h>
#include "udp.c"

#define PACKET_SIZE 3
#define WINDOW_SIZE 10

typedef struct {
    int seq_num;        // Which piece of the file is this? (0, 1, 2...)
    int total_packets;  // Useful so the server knows when to stop
    int data_size;      // How many bytes are in the payload (last packet might be small)
    int type;           // 0 for Data, 1 for ACK
} packet_header_t;

int client_send(int sd, struct sockaddr_in *addr, char *msg, int n) {
    int num_packets = n / PACKET_SIZE + (n % PACKET_SIZE != 0);

    for (int i = 0; i < num_packets; i++) {
        int offset = i * PACKET_SIZE;
        int bytes_to_send = (i == num_packets - 1) ? (n - offset) : PACKET_SIZE;
        char buffer[sizeof(packet_header_t) + bytes_to_send + 1];

        packet_header_t* header = (packet_header_t*) buffer; // point the start of the buffer to the header
        header->seq_num = i;
        header->total_packets = num_packets;
        header->data_size = bytes_to_send;
        header->type = 0; 


        memcpy(buffer + sizeof(packet_header_t), msg + offset, bytes_to_send);
        buffer[sizeof(packet_header_t) + bytes_to_send + 1] = '\0'; 


        int tries = 5;
        while (tries > 0) {
            UDP_Write(sd, addr, buffer, bytes_to_send + sizeof(packet_header_t)); // send the header + payload

            struct timeval tv;
            tv.tv_sec = 2;
            tv.tv_usec = 0;

            fd_set readfds;
            FD_ZERO(&readfds);
            FD_SET(sd, &readfds);
            int sel = select(sd+1, &readfds, NULL, NULL, &tv); // listen to the socket for 2 seconds
            if (sel > 0){
                char ack[4];
                struct sockaddr_in addrRcv;
                int rc = UDP_Read(sd, &addrRcv, ack, 4);
                if (rc > 0 && strcmp(ack, "ACK") == 0) {
                    printf("Sent message %d successfully and received ACK\n", i);
                }
                break;
            }
            else {
                printf("Failed, have %d tries left\n", --tries);
                fflush(stdout);
            }
        }
    }
    
    return 0;

}


char* server_receive(int sd, struct sockaddr_in *addr) {
    char* big_buffer = NULL;
    int cursize = 0;
    int end = 0;

    while (1){
        char buffer[PACKET_SIZE + sizeof(packet_header_t) + 1];
        int rc = UDP_Read(sd, addr, buffer, sizeof(buffer));
        if (rc > 0) {
            buffer[rc] = '\0';

            packet_header_t* header = (packet_header_t*) buffer;
            if (header->seq_num == header->total_packets - 1) {
                end = 1;
            }
            
            big_buffer = realloc(big_buffer, cursize + header->data_size + 1);
            
            
            memcpy(big_buffer + cursize, buffer + sizeof(packet_header_t), header->data_size);
            cursize += header->data_size;
            big_buffer[cursize] = '\0';


            // send ack back to client
            char ack[sizeof(packet_header_t) + 1];
            packet_header_t* ack_header = (packet_header_t*) ack;
            ack_header->seq_num = header->seq_num;
            ack_header->total_packets = header->total_packets;
            ack_header->data_size = 0;
            ack_header->type = 1; // ACK type

            UDP_Write(sd, addr, ack, sizeof(ack));

            if (end) break;
        }
    }
    return big_buffer;
}
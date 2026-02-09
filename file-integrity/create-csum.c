#include <stdio.h>
#include <unistd.h> 
#include <fcntl.h>
#include <bits/getopt_core.h>

#define BLOCK_SIZE 4096

int main(int argc, char* argv[]){
    int fd = open(argv[1], O_RDONLY);

    char buffer[BLOCK_SIZE];
    ssize_t bytesRead;
    char* output_file = "csum.out";

    int fd1 = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);

    while ((bytesRead = read(fd, buffer, BLOCK_SIZE)) > 0) {
        unsigned char csum = 0;
        for (ssize_t i = 0; i < bytesRead; i++) {
            csum ^= (unsigned char)buffer[i];
        }
        write(fd1, &csum, 1);
        
    }
    close(fd1);
    close(fd);
}
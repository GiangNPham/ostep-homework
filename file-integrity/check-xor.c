#include <stdio.h>
#include <unistd.h> 
#include <fcntl.h>

int main(int argc, char* argv[]){
    int fd = open(argv[1], O_RDONLY);

    char ch;
    ssize_t bytesRead;
    unsigned char xorResult = 0;

    while ((bytesRead = read(fd, &ch, 1)) > 0) {
        xorResult ^= (unsigned char)ch;
    }
    printf("XOR Result: %x\n", xorResult);
    close(fd);
}
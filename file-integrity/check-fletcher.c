#include <stdio.h>
#include <unistd.h> 
#include <fcntl.h>
#include <inttypes.h>

int main(int argc, char* argv[]){
    int fd = open(argv[1], O_RDONLY);

    char ch;
    ssize_t bytesRead;
    uint16_t s1 = 0, s2 = 0;

    while ((bytesRead = read(fd, &ch, 1)) > 0) {
        s1 = (s1 + (unsigned char)ch) % 255;
        s2 = (s2 + s1) % 255;
    }

    printf("Result: %x\n", (s2 << 8) | s1);
    close(fd);
}
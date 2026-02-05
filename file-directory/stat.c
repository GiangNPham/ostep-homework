#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char* argv[]){
    struct stat buffer;
    int status = stat(argv[1], &buffer);
    if (status == 0) {
        printf("File size: %lld bytes; Blocks allocated: %ld, Link count: %lld\n", (long long)buffer.st_size, buffer.st_blocks, (long long) buffer.st_nlink);
    }
}
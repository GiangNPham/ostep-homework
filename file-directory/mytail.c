#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]){
    int lines = atoi(argv[2]);

    int fd = open(argv[3], O_RDONLY);

    // off_t file_size = lseek(fd, 0, SEEK_END);
    off_t curpos = lseek(fd, 0, SEEK_END);
    int nlcount = 0;

    while (curpos > 0 && nlcount < lines) {
        curpos--;
        lseek(fd, curpos, SEEK_SET);
        char c;
        read(fd, &c, 1);
        if (c == '\n') {
            nlcount++;
        }
    }
    if (curpos > 0) {
        lseek(fd, curpos+1, SEEK_SET);
    } else {
        lseek(fd, 0, SEEK_SET);
    }
    char c;
    while (read(fd, &c, 1) == 1) {
        putchar(c);
    }
    return 0;
    
}
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>

int main(int argc, char* argv[]){
    // Check if -l flag is present
    if (argc > 1 && argv[1][0] == '-' && argv[1][1] == 'l') {
        DIR* dir;
        struct dirent *de;
        
        if (argc == 2) {
            dir = opendir(".");
        } else {
            dir = opendir(argv[2]);
        }

        while ((de = readdir(dir)) != NULL) {
            struct stat buffer;
            stat(de->d_name, &buffer);
            printf("File: %s; Size: %lld bytes; Links: %lld; Permissions: %o\n", de->d_name, (long long)buffer.st_size, (long long)buffer.st_nlink, buffer.st_mode & 07777);
        }
        closedir(dir);
    } else {
        DIR* dir;
        struct dirent *de;

        if (argc == 1) {
            dir = opendir(".");
        } else {
            dir = opendir(argv[1]);
        }

        while ((de = readdir(dir)) != NULL) {
            printf("%s\n", de->d_name);
        }
        closedir(dir);
    }
    return 0;

}
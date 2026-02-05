#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// stat does care about which directory we are in => use chdir to navigate directories

void printDir(DIR* dir){
    struct dirent *de;
    while ((de = readdir(dir)) != NULL) {
        struct stat buffer;
        if (stat(de->d_name, &buffer) == -1) {
            perror("stat");
            continue;
        }
        if (S_ISREG(buffer.st_mode)) printf("File: %s\n", de->d_name);
        else if (!strcmp(de->d_name, ".")) continue;
        else if (!strcmp(de->d_name, "..")) continue;
        else {
            DIR* subdir = opendir(de->d_name);
            if (subdir != NULL) {
                chdir(de->d_name);
                printDir(subdir);
                chdir("..");
                closedir(subdir);
            }
        }
    }
}

int main(int argc, char* argv[]){
    DIR* dir;
    
    if (argc == 1) {
        dir = opendir(".");
    } else {
        dir = opendir(argv[1]);
    }

    char* target = (argc == 1) ? "." : argv[1];
    chdir(target);

    printDir(dir);
    closedir(dir);
    return 0;
}
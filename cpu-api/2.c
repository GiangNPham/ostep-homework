#include <stdio.h>
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

int main(){
    int fd = open("a.txt", O_RDWR, O_TRUNC);

    if (!fork()){
        const char *message = "Hello from parent\n";
        for (int i = 0; i < strlen(message); i++){
            write(fd, &message[i], 1);
        }
    } else {
        const char *message = "Hello from child\n";
        for (int i = 0; i < strlen(message); i++){
            write(fd, &message[i], 1);
        }
    }
    close(fd);
    return 0;
}
#include <stdio.h>
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <stdio.h>

int main(){
    pid_t pid = fork();
    int x = 100;

    if (pid < 0){
        perror("fork failed");
        return 1;
    }
    if (pid == 0){
        x = 111;
        printf("child: %d\n", x);
    }
    else {
        wait(&pid);
        x = 121;
        printf("parent: %d\n", x);
    }
    return 0;
}
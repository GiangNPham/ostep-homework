#include <stdio.h>
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <stdio.h>

int main(){
    pid_t pid = fork();

    if (!pid){
        close(STDOUT_FILENO);
        printf("Hello\n");
    }
}
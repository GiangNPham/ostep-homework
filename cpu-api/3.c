#include <stdio.h>
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <stdio.h>

int main(){
    pid_t pid = fork();
    if (!pid){
        printf("hello\n");
    } else {
        int *status;
        waitpid(pid, status, 0);

        printf("%d\n", *status);
        printf("goodbye\n");
    }
    return 0;
}
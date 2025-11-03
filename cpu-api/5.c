#include <stdio.h>
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <stdio.h>
#include <string.h>

int main(){
    int p[2];
    pipe(p);

    pid_t pid1 = fork();
    pid_t pid2;

    if (pid1) {
        pid2 = fork();

        if (!pid2){
            wait(&pid1);
            close(p[1]);
            dup2(p[0], STDIN_FILENO);

            char buffer[10];
            read(p[0], buffer, 10);
            printf("%s", buffer);
        }
    }
    else {
        close(p[0]);
        dup2(p[1], STDOUT_FILENO);
        char *buffer = "Hello\n";
        write(p[1], buffer, strlen(buffer));
    }
}
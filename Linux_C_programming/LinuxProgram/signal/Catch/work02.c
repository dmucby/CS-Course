#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    pid_t pid;

    signal(SIGCHLD, SIG_IGN);

    pid = fork();
    if(pid == 0){
        printf("child : ppid=%d\n", getppid());
        sleep(3);
        exit(1);
    }    
    printf("parent : pid=%d\n", getpid());
    
    return 0;
}

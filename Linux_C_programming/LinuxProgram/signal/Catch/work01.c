#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void sig_chld(int signo){
    /* signal handler */
    printf("catch the signal SIGCHLD!");
    wait();
}

int main(int argc, char const *argv[])
{
    //signal(SIGCHLD, sig_chld);
    struct sigaction newact, oldact;
    unsigned int     sig_chlld;

    newact.sa_handler = sig_chld;
    sigemptyset(&newact.sa_mask);
    newact.sa_flags = 0;
    sigaction(SIGCHLD, &newact, &oldact);

    pid_t pid;
    pid = fork();
    if(pid < 0){
        perror("fork error!\n");
        exit(1);
    }
    if(pid == 0){
        printf("1\n");
        sleep(3);
        exit(3);
    }

    while (1)
    {
        sleep(1);
        printf("2\n");
    }

    sigaction(SIGCHLD, &oldact, NULL);
    
    return 0;
}


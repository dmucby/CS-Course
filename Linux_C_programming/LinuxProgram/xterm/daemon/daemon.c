#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

void daemonize(void){
    pid_t pid;
    if((pid = fork()) < 0){
        perror("fork");
        exit(1);
    }else if(pid != 0)
        exit;

    setsid();
    // change the current working directory
    if(chdir("/") < 0){
        perror("chdir");
        exit(1);
    }

    // 将文件描述符赋予/dev/null 
    close(0);
    open("/dev/null", O_RDWR);
    dup2(0, 1);
    dup2(0, 2);
}

int main(void){
    daemonize();
    while(1);
}
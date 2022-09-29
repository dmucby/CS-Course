#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    // ps 显示当shell正在运行的进程
    execlp("ps","ps","-o","pid,ppid,pgrp,session,tpgid,comm", NULL);
    perror("exec ps");
    exit(1);
}
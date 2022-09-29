#include <unistd.h>
#include <signal.h>
#include <stdio.h>

void sig_alarm(int signo){

}

unsigned int mysleep(unsigned int nsecs){
    struct sigaction newact, oldact;
    sigset_t         newmask, oldmask, suspmask;
    unsigned int     unslept;

    /* set handler function */
    newact.sa_handler = sig_alarm;
    // 无可屏蔽信号
    sigemptyset(&newact.sa_mask);
    newact.sa_flags = 0;
    sigaction(SIGALRM, &newact, &oldact);

    /*block SIGALRM save current mask*/
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGALRM);
    // 此时屏蔽信号SIGALRM 第一次修改信号掩码 
    // 将原来的信号屏蔽字放到oldmask中
    sigprocmask(SIG_BLOCK, &newmask, &oldmask);

    // 屏蔽两秒
    alarm(nsecs);

    suspmask = oldmask;
    /* SIGALRM isn't blocked */
    // 确保旧的屏蔽字被删除
    sigdelset(&suspmask, SIGALRM);
    /* wait for any signal to be caught*/
    // 解除新的屏蔽信号，并等待挂起 
    // 等待suspend返回，自动恢复原来的屏蔽字，再次屏蔽SIGALRM
    sigsuspend(&suspmask);

    unslept = alarm(0);
    // 修改为原来的信号处理动作
    sigaction(SIGALRM, &oldact, NULL);

    /* reset signal mask unblock*/
    // 再次解除对SIGALRM的屏蔽
    sigprocmask(SIG_SETMASK, &oldmask, NULL);
    return unslept;
}

int main(void){
    while(1){
        mysleep(2);
        printf("two seconds passed\n");
    }
    return 0;
}
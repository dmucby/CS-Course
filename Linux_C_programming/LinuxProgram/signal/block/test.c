#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void printsigset(const sigset_t *set)
{
    int i;
    for(i = 1; i < 32; i++){
        // 如果包含信号
        if(sigismember(set, i) == 1){
            putchar('1');
        }else{
            putchar('0');
        }
    }
    // 此处动作？
    puts("");
}

int main(void){
    sigset_t s,p;
    // 初始化信号集
    sigemptyset(&s);
    // 将SIGINT加入信号集
    sigaddset(&s, SIGINT);
    // 更改屏蔽字
    // SIG_BLOCK mask|set
    sigprocmask(SIG_BLOCK, &s, NULL);
    while(1){
        // 读取当前的未决信号
        sigpending(&p);
        printsigset(&p);
        sleep(1);
    }
    return 0;
}
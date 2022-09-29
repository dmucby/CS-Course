#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>


#define MAX_SIZE 1024

void my_pipe(){
    
}

int main(int argc, char const *argv[])
{
    while (1)
    {
        printf("Name@host:path#");
        char* buf = (char*)malloc(MAX_SIZE);
        
        // 刷新
        fflush(stdout);

        fgets(buf, 1023, stdin);

        buf[strlen(buf)-1] = '\0';

        // 开始处理命令
        int m_argc = 0;
        char *ptr = buf, *m_argv[32] = {NULL};

        while(*ptr != '\0'){
            if( *ptr!=' ' ){
                m_argv[m_argc] = ptr;
                m_argc++;

                // 提取每个参数
                while(*ptr != '\0' && *ptr != ' ') ptr++;
                *ptr = '\0';
            }
            ptr++;
        }

        m_argv[m_argc] = NULL;

        // 处理自建命令
        

        // fork子进程
        pid_t pid = fork();
        // 子进程执行命令 父进程来清理子进程
        if(pid < 0){
            perror("fork faild");
            exit(1);
        }
        if(pid == 0){
            execvp(m_argv[0],m_argv);
            perror("exec error");
            exit(1);
        }else{
            wait(NULL);
        }

    }
    
    return 0;
}

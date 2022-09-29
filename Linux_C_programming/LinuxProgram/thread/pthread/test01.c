#include <pthread.h>
#include <unistd.h>

#include <stdio.h>

void* func(void* arg)
{
    // 强转型指针 static_cast
    // 将pthread_t 类型 转为 *pthread_t
    pthread_t main_tid = *static_cast<pthread_t*>(arg);
    pthread_cancel(main_tid);
    //pthread_cancel(arg);
    while (1)
    {
        printf("child loops\n");
    }
    return NULL;
}

int main(int argc, char* argv[])
{
    pthread_t main_tid = pthread_self();
    pthread_t tid = 0;
    pthread_create(&tid, NULL, func, &main_tid);
    while (1)
    {
        printf("main loops\n");
    }
    //sleep(1);
    printf("main exit\n");
    return 0;
}
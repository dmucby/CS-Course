#include "mutex.hpp"
#include <unistd.h>
#include <iostream>

#define NUM_THREADS 10000

int num = 0;
Mutex mutex;// 全局信号锁

void *count(void *args){
    // 加锁
    MutexLock lock(&mutex);
    num++;
    //return;
}

int main(int argc, char const *argv[])
{
    pthread_t thread[NUM_THREADS];

    for (size_t i = 0; i < NUM_THREADS; i++)
    {
        int ret = pthread_create(&thread[i], NULL, count, NULL);
        if(ret){
            return -1;
        }
    }

    for (size_t i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(thread[i], NULL);
    }

    std::cout << num << std::endl;
    return 0;
}

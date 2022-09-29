#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

#define NUM 5
int queue[NUM];
sem_t blank_number, product_number;

void *producer(void *arg){
    int p = 0;
    while(1){
        // 消耗需求
        sem_wait(&blank_number);
        queue[p] = rand() % 1000 + 1;
        printf("Produce %d\n", queue[p]);
        // 生产产品
        sem_post(&product_number);
        // 环形队列
        p = (p+1)% NUM;
        sleep(rand()%5);
    }
}

void *consumer(void *arg){
    int c = 0;
    while(1){
        // 消耗等待产品
        sem_wait(&product_number);
        printf("Consume: %d\n",queue[c]);
        queue[c] = 0;
        // 发布需求
        sem_post(&blank_number);
        c = (c+1) % NUM;
        sleep(rand()%5);
    }
}

int main(int argc, char const *argv[])
{
    pthread_t pid, cid;

    // 0表示同一线程同步
    sem_init(&blank_number, 0, NUM);
    sem_init(&product_number,0, 0);

    pthread_create(&pid, NULL, producer, NULL);
    pthread_create(&cid, NULL, consumer, NULL);

    pthread_join(pid, NULL);
    pthread_join(cid, NULL);

    sem_destroy(&blank_number);
    sem_destroy(&product_number);

    return 0;
}

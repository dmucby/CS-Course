// 验证消费者-生产者

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

struct msg{
    struct msg *next;
    int num;
};

struct msg *head;
struct msg *tail;
pthread_cond_t has_product = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *consumer(void *p){
    struct msg *mp;

    for(;;){
        pthread_mutex_lock(&lock);
        while(head == NULL){
            pthread_cond_wait(&has_product, &lock);
        }
        // 消费一个
        mp = head;
        head = mp->next;
        if(head == NULL){
            tail = NULL;
        }
        pthread_mutex_unlock(&lock);
        printf("Consume %d\n",mp->num);
        free(mp);
        sleep(rand() % 2);
    }

}

void *producer(void *p){
    struct msg *mp;

    for(;;){
        mp = malloc(sizeof(struct msg));
        mp->num = rand() % 1000 +1;
        printf("Produce %d\n", mp->num);
        pthread_mutex_lock(&lock);
        // 生产一个
        // LIFO last in first out
        //mp->next = head;
        //head = mp;
        // FIFO first in first out
        if(head == NULL){
            head = mp;
            tail = mp;
        }else{
            tail->next = mp;
            tail = mp;
        }
        pthread_mutex_unlock(&lock);
        pthread_cond_signal(&has_product);
        sleep(rand() % 5);
    }
}

int main(int argc, char const *argv[])
{
    pthread_t pid, cid, cid2;

    srand(time(NULL));
    // pthread_t 指针取引用
    pthread_create(&cid,NULL,consumer,NULL);
    pthread_create(&pid,NULL,producer,NULL);
    pthread_create(&cid2,NULL,consumer,NULL);

    //等待返回
    pthread_join(pid,NULL);
    pthread_join(cid,NULL);
    pthread_join(cid2, NULL);
    return 0;
}

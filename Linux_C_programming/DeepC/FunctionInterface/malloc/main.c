#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int number;
    char *msg;
} unit_t;

int main(int argc, char const *argv[])
{
    // 隐式转换
    unit_t *p = malloc(sizeof(unit_t));
    // 内存分配后需要判断
    if(p == NULL){
        printf("out of memory\n");
        exit(1);
    }
    p->number = 3;
    p->msg = malloc(20);
    strcpy(p->msg, "Hello world!");
    printf("number: %d\nmsg: %s\n", p->number, p->msg);
    // 注意顺序不能变，否则p就变成了野指针 msg就成了内存碎片
    free(p->msg);
    // 将p的空间归还，但是值并没有变化
    // free只能够清除空间
    free(p);
    p = NULL;

    return 0;
}

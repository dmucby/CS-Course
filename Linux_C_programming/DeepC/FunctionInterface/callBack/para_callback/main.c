#include <stdio.h>
#include "para_callback.h"

void say_hello(void *str)
{
    printf("Hello %s\n", (const char *)str);
}

void count_numbers(void *num)
{
    int i;
    for(i=1; i<=(int)num; i++)
        printf("%d ",i);
    // 注意putchar只输出单个字符
    putchar('\n');
}

int main(int argc, char const *argv[])
{
    repeat_three_times(say_hello, "Guys");
    repeat_three_times(count_numbers, (void *)5);
    return 0;
}

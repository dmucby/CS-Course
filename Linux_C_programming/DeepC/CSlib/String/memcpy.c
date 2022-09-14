#include <stdio.h>
#include <string.h>

int main(void)
{
    // 重叠拷贝的问题？？？
    char buf[20] = "hello world\n";
    // 此处都有错误！
    //memcpy(buf + 1, buf , 13);
    //memmove(buf + 1, buf , 13);
    memcpy(buf + 1, buf , 13);
    printf(buf);
    return 0;
}
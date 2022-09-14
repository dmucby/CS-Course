#include <stdio.h>
#include "populator.h"

int main(int argc, char const *argv[])
{
    unit_t u;

    // 初始化
    set_unit(&u);

    printf("number:%d\nmsg:%s\n",u.number, u.msg);
    return 0;
}

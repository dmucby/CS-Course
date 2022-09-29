#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    // 创建文件需要追加第三个参数，表示访问权限
    // 注意不能使用读写权限
    int i = open("test1.txt",O_CREAT,0644);
    printf("%d\n",i);
    return 0;
}

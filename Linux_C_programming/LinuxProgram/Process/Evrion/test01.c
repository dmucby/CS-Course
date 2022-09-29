#include <stdio.h>

int main(void)
{
    // 获得外部变量
    extern char **environ;
    for (size_t i = 0; environ[i]!=NULL ; i++)
    {
        printf("%s\n", environ[i]);
    }
    return 0;
}
#include <stdio.h>

int main(void)
{
    int ch;
    /* Ctrl-D 表示EOF */
    while((ch = getchar()) != EOF){
        putchar(toupper(ch));
    }
    return 0;
}
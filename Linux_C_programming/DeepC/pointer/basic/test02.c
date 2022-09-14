#include <stdio.h>

void func(void *pv)
{
    char  *pchar = pv;
    *pchar = 'A';
}

int main(int argc, char const *argv[])
{
    char c;
    func(&c);
    printf("%c\n", c);
    return 0;
}

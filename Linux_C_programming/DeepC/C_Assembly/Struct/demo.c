#include <stdio.h>

int main(int argc, char const *argv[])
{
    struct {
        char a;
        short b;
        int c;
        char d;
    } __attribute__((packed)) s;

    s.a = 1;
    s.b = 2;
    s.c = 3;
    s.d = 4;

    printf("%u\n", sizeof(s));

    return 0;
}

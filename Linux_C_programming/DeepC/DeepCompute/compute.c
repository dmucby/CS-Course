#include <stdio.h>

int main(int argc, char const *argv[])
{
    // unsigned char c = 0xfc;
    // unsigned int i = ~c;

    // printf("%d %d\n",c,i);

    // int i = 0xcffffff3;
    // printf("%x\n", 0xcffffff3>>2);
    // printf("%x\n", i>>2);
    
    // unsigned int a, b, mask = 1U << 6;
    // a = 0x12345678;
    // b = a ^ mask; /* flip the 6th bit */
    // printf("%d %d\n",a,b);

    int t = 5;
    int c = (t=3, t+2);
    printf("%d\n", c);

    return 0;
}

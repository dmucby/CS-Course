#include <stdio.h>

int main(int argc, char const *argv[])
{
    int a[12];
    printf("%d\n",sizeof a/sizeof a[0]);

    typedef unsigned long int size_t;

    
    return 0;
}

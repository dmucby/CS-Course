#include <stdio.h>

int sum(int n, int a, ...) {
    int s = 0;
    int *p = &a;
    for (size_t i = 0; i < n; i++)
    {
        s += p[i];
    }
    return s;
}

int main(int argc, char const *argv[])
{
    printf("%d\n", sum(5,1,2,3,4,5));
    return 0;
}

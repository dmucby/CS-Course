#include <stdio.h>

void foo();

int main(void)
{
	char c = 60; 
    //int c = 60; 
    //同样的结果
	//double c = 60;
    foo(c);
	return 0;
}

void foo(double d)
{
	printf("%f\n", d);
}
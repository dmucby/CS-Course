#include <stdio.h>

// 防止隐式调用警告
extern void push(char);
extern char pop(void);
extern int  is_empty(void);

int a, b = 1;

int main(void){
    push('a');
    push('b');
    push('c');

    while(!is_empty())
        putchar(pop());
    putchar('\n');

    return 0;
}


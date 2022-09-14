#include <stdio.h>

int main(){
    int a = 10, b;
    __asm__("movl %1, %%eax\n\t" //将%1的值加入寄存器eax
            "movl %%eax, %0\n\t" //将寄存器的值传给%0的寄存器
            :"=r"(b)  // 把%0寄存器的值给b
            :"r"(a)   // 编译器分配一个寄存器保存a的值
            :"%eax"   // 不保存寄存器eax的值
            );
    printf("Result: %d, %d\n",a, b);
    return 0;
}
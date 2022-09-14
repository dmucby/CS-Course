## 实际的CPU

上述栈顶`top`也是由一个寄存器来记录，这个寄存器位`esp`(stack pointer)

### eip入栈过程
```Assembly
sub esp, 4
mov dword ptr[esp], eip
```
翻译为C语言为
```c
esp = (void*) (((unsigned int*) esp) - 4);
*((unsigned int*) esp ) = (unsigned int) eip;
```
将`eip`写入`esp`指向的内存中，eip寄存器长度为32位，即4个字节

## 函数调用过程
```bash
(gdb) disas
Dump of assembler code for function eax_plus_1s:
=> 0x080483f0 <+0>: add    $0x1,%eax
   0x080483f3 <+3>: ret    
End of assembler dump.
# 查看当前函数的执行情况

(gdb) info register esp
esp            0xffffd6e8   0xffffd6e8
# 查看栈顶指针的值，每调用一次函数，值-4

(gdb) p/x *(unsigned int*)$esp
$2 = 0x80483fe
# 查看栈顶指向的内容
# 通常为调用其函数的下一条语句或者指令
```
1. 实现函数跳转，跳转之前CPU保存当前所在位置(返回地址)
2. 函数返回时，从调用的位置恢复

这个地址保存在**堆栈**，借助于栈的进出逻辑实现函数嵌套和递归调用。

## C语言与汇编翻译区别
### 作用域
   由于汇编语言的寄存器是全局可见的，所以需要使用使用局部变量来实现每个函数自己的堆栈。

### 入栈与出栈
```Assembly
push eax # 将eax的值保存到堆栈中
pop ebx # 将栈顶的值取出并存放到ebx中
```

### 函数参数的传递
一般使用**堆栈**传递

### 恐怖代码分析
```C
#include<stdio.h>

int foo();
int bar();

int main(void)
{
	foo(2, 3, 4);
	return 0;
}

int foo(int a, int b)
{
	return bar(a);
}

int bar(int c, int d)
{
	int e = c + d;
	return e;
}
```
#### gdb调试
```bash
(gdb) s
8               foo(2, 3, 4);
(gdb) disassemble
Dump of assembler code for function main:
   0x0000555555555129 <+0>:     endbr64 
   0x000055555555512d <+4>:     push   %rbp
   0x000055555555512e <+5>:     mov    %rsp,%rbp
=> 0x0000555555555131 <+8>:     mov    $0x4,%edx
   0x0000555555555136 <+13>:    mov    $0x3,%esi
   0x000055555555513b <+18>:    mov    $0x2,%edi
   0x0000555555555140 <+23>:    mov    $0x0,%eax
   0x0000555555555145 <+28>:    callq  0x555555555151 <foo>
   0x000055555555514a <+33>:    mov    $0x0,%eax
   0x000055555555514f <+38>:    pop    %rbp
   0x0000555555555150 <+39>:    retq   
End of assembler dump.
```
* 可看出从右向左依次加入栈中
* 结果不变？？？是因为什么？为什么`bar`函数中d赋值为3，而`foo`函数中没有传递d的值
* 4的值保存到哪去了？

```bash
# 在foo函数中的值
(gdb) s
foo (a=21845, b=1431654464) at test04.c:13
(gdb) p a
$1 = 21845
(gdb) p b
$2 = 1431654464
(gdb) s # 此时到达 return bar(a)  
(gdb) p a
$3 = 2
(gdb) p b
$4 = 3 # 传值正常
0x000055555555515d <+12>:    mov    %edi,-0x4(%rbp)
   0x0000555555555160 <+15>:    mov    %esi,-0x8(%rbp)
=> 0x0000555555555163 <+18>:    mov    -0x4(%rbp),%eax
# a -> edi 寄存器中
# b -> esi 寄存器
```
   在`bar`函数中
```
Dump of assembler code for function bar:
   0x0000555555555174 <+0>:     endbr64
   0x0000555555555178 <+4>:     push   %rbp
   0x0000555555555179 <+5>:     mov    %rsp,%rbp
   0x000055555555517c <+8>:     mov    %edi,-0x14(%rbp)
   0x000055555555517f <+11>:    mov    %esi,-0x18(%rbp)
=> 0x0000555555555182 <+14>:    mov    -0x14(%rbp),%edx
   0x0000555555555185 <+17>:    mov    -0x18(%rbp),%eax
# 可以看到 c,d的值均从edi,esi中取得
```

**多余的参数去哪了？**
```bash
0x0000555555555131 <+8>:     mov    $0x4,%edx
# 可以看到 4 保存在了edx中，而且main函数初始化一个0eax来保存返回值的结果
0x0000555555555136 <+13>:    mov    $0x3,%esi
0x000055555555513b <+18>:    mov    $0x2,%edi
0x0000555555555140 <+23>:    mov    $0x0,%eax
```
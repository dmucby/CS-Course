## 指令寄存器

* 记录指令执行的位置(CPU指令执行到哪了)

有时候需要流程控制能力，例如选择和循环，这时需要跳转。

```Assembly
global main

main:
    
```

```bash
0x080483f9 <+12>:    jle    0x8048402 <main+21>
   0x080483fb <+14>:    mov    ebx,0x1
   0x08048400 <+19>:    jmp    0x804841f <main+50>
   0x08048402 <+21>:    cmp    ebx,0x45
```
* <main+21> 对应 <+21>

## 状态寄存器
    由于是先比较后跳转，需要保存临时的比较结果。

这个寄存器是
```bash
eflags
```
叫做标志位寄存器：
* 结果正负
* 有没有进位
* 结果是不是0
后续指令的跳转根绝`eflags`中的状态来决定，其相当于保存`cmp`结果的寄存器。

# 循环
    跳转和比较指令的集合，或者叫做程序往回跳转

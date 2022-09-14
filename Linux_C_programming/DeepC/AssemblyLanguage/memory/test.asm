global main

# 程序会挂掉
# 不可以随意读写内存

main:
    mov ebx, 1
    mov ecx, 2
    add ebx, ecx

    mov [sui_bian_xie], ebx # 将值保存到寄存器中
    mov eax, [sui_bian_xie] # 将内存的值读到寄存器中

    ret

section .data
sui_bian_xie    dw  0
# 开辟一块4字节的空间，并且全部用0填充
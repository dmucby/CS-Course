# mov 类似赋值命令
# 用于数据传输

# add 
# ret 类似 return

global main

main:
    mov eax, 1
    mov ebx, 2
    mov ecx, 3
    mov edx, 4
    mov ebx, 5
    add eax, ebx
    add eax, ecx
    add eax, edx
    add eax, ebx
    ret


# 更多的寄存器
# eax, ebx, ecx,edx,esi,edi,ebp
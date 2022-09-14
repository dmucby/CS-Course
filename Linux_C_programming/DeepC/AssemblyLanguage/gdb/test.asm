# 反汇编


global main

main:
    mov eax, 1
    mov ebx, 2
    add eax, ebx
    ret


# gdb 调试
# set disassembly-flavor intel 
# 将反汇编的格式调整为intel格式
# 反汇编 将可执行文件 翻译为汇编代码


# stepi 汇编单步调试
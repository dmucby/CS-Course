global main

eax_plus_1s:
    add eax, 1
    ret

ebx_plus_ls:
    add ebx, 1
    ret

main:
    mov eax, 0
    mov ebx, 0
    # 函数调用 jmp升级版
    # 函数栈帧
    call eax_plus_1s
    call eax_plus_1s
    call ebx_plus_ls
    call ebx_plus_ls
    add  eax,ebx
    ret
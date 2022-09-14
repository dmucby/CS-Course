fibo:
global main

fibo:
    cmp eax, 1
    je _get_out
    cmp eax, 2
    je _get_out

    push ebx
    push ecx
    push edx

    mov edx, eax
    sub eax, 1
    call fibo
    mov ebx, eax

    mov eax, edx
    sub eax, 2
    call fibo
    mov ecx, eax

    # 将两个数累加，并累计到eax寄存器中
    mov eax, ebx
    add eax, ecx

    pop edx
    pop ecx
    pop ebx

    ret

_get_out:
    mov eax, 1
    ret

main:
    mov eax, 7
    call fibo
    ret
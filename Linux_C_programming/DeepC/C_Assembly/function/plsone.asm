global main

eax_plus_ls:
    add eax, 1
    ret

main:
    mov eax, 0
    call eax_plus_ls
    ret

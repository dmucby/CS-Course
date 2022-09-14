global main

main:
    mov eax, 10

    cmp  eax, 100
    jle  lower_or_equal
    sub  eax, 20

lower_or_equal:
    cmp eax, 10
    jg  more_ten
    add eax, 10

more_ten:
    add eax, 1
    ret
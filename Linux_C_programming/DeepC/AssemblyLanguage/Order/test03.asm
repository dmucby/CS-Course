global main

main:
    mov eax, 1
    mov ebx, 2
    
    # 跳跃指令
    jmp gun_kai
    
    add eax, ebx

gun_kai:
    ret
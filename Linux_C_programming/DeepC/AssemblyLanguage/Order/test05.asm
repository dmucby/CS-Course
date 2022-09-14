global main

main:
    mov eax, 50
    cmp eax, 10  # 两个数进行比较
    jle less_ten 
    # 条件跳转指令，当前面的结果为小于等于的时候跳转
    sub eax, 10

less_ten:
    ret 

# 跳转指令都是以j开头
# 助记单词
# a: above
# e: equal
# b: below
# n: not
# g: greater
# l: lower
# s: signed
# z: zero
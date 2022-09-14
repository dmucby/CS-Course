nasm -f elf nmb.asm -o nmb.o
gcc -m32 nmb.o -o nmb
./nmb ; echo $?
# echo $?打印最后的返回值
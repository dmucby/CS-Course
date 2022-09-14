.section .rodata

msg:
     .ascii  "Hello, world!\n"
 
.section  .text

.globl  _start

_start:
     # syscall number sys_write
     movl $4, %eax
     # can_shu write(1, nei_rong, length)
     movl $1, %ebx
     movl $msg, %ecx
     movl $14, %edx
     int  $0x80

     # exit
     movl $1, %eax
     movl $0, %ebx
     int  $0x80
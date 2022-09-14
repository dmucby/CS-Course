# Linux C编程一站式学习

## 第14章. 计算机中数的表示

### 1. 为什么计算机用二进制计数

![1-bit Full Adder](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/number.digitallogic.png)





## 第15章 数据类型详解

### 1.整形

char型符号依据平台来确定，C标准：**优先考虑效率，而可移植性尚在其次**。

C语言与平台和编译器是密不可分的，离开了具体的平台和编译器讨论C语言，就只能讨论到本书第一部分的程度了。注意，ASCII码的取值范围是0~127，所以不管`char`型是有符号的还是无符号的，存一个ASCII码都没有问题，一般来说，如果用`char`型存ASCII码字符，就不必明确写是`signed`还是`unsigned`，如果用`char`型表示8位的整数，为了可移植性就必须写明是`signed`还是`unsigned`。

> ###  Implementation-defined、Unspecified和Undefined
>
> * Implementation-defined
>
>   C语言标准未明确该规定，需要依据编译器作出规定。
>
> * Unspecified
>
>   往往有几种可选的处理方式，C标准没有明确规定按哪种方式处理，编译器可以自己决定，这样即便用同一个编译器的不同版本来编译也可能得到不同的结果，因为编译器没有在文档中明确写它会怎么处理，那么不同版本的编译器就可以选择不同的处理方式
>
> * Undefined
>
>   C标准没规定怎么处理，编译器很可能也没规定，甚至也没做出错处理，有很多Undefined的情况编译器是检查不出来的，最终会导致运行时错误，比如数组访问越界就是Undefined的。

#### 打印`int`的下界

```c
printf("%d\n",-2147483648);   
```

```bash
gcc Int.c -std=c99
Int.c: In function ‘main’:
Int.c:5:14: warning: format ‘%d’ expects argument of type ‘int’, but argument 2 has type ‘long int’ [-Wformat=]
    5 |     printf("%d\n",-2147483648);
      |             ~^    ~~~~~~~~~~~
      |              |    |
      |              int  long int
      |             %ld
```

C编译器把它当成一个整数常量`2147483648`和一个负号运算符表达式，而整数常量2147483648 > 2147483647超过了`int`型的取值范围，

但是改为

```c
printf("%d\n", -2147483647-1);
```

编译器仍然会警告，此处有BUG！！！

#### 类型转换

```c
long long i = 1234567890 * 1234567890;
```

```
warning: integer overflow in expression of type ‘int’ results in ‘304084036’ [-Woverflow]
    6 |     long long i = 1234567890 * 1234567890;
      |                              ^
```

两个`int`型，而乘积超过了整形范围，因此提示结果溢出。改为如下即可

```c
long long i = 1234567890LL * 1234567890;
```

一个为long long,另一个也会先转换为Long long再进行运算。



### 2. 浮点型

与平台相关性很大：

* 浮点运算单元：FPU （Floating Point Unit）
* 整数模拟浮点运算

#### 浮点数写法：

* 科学计数法：3.e-1、.987
* 后缀：f\F(float 型)，l\L(Long double)，没有后缀为double



## 第 16 章 运算符详解

### 1. 位运算

> 操作数必须是整形

```c
unsigned char c = 0xfc;
unsigned int i = ~c;
```

#### 运算过程：

1. 常量0xfc为整型
2. 转换为 unsigned char 赋值给C
3. 计算~C,先转换为整形，然后取反
4. 得到ffffff03，赋值给i

#### 1.2 移位运算

在一定范围内，左移一位相当于乘2，移位操作对负数计算消耗有点高，因为要保持最高位1



#### 1.4 异或

1. 一个数和自己做异或的结果是0。做异或运算生成的0比读取一个0更快，因为无寄存器读取操作

2. 从异或的真值表可以看出，不管是0还是1，和0做异或保持原值不变，和1做异或得到原值的相反值。配合掩码实现某些位的翻转

3. 如果a1 ^ a2 ^ a3 ^ ... ^ an的结果是1，则表示a1、a2、a3...an之中1的个数为奇数个，否则为偶数个。这条性质可用于奇偶校验（Parity Check），比如在串口通信过程中，每个字节的数据都计算一个校验位，数据和校验位一起发送出去，这样接收方可以根据校验位粗略地判断接收到的数据是否有误。

4. x ^ x ^ y == y，因为x ^ x == 0，0 ^ y == y。这个性质有什么用呢？我们来看这样一个问题：交换两个变量的值，不得借助额外的存储空间，所以就不能采用`temp = a; a = b; b = temp;`的办法了。利用位运算可以这样做交换：

   ```c
   a = a ^ b;
   b = b ^ a;
   a = a ^ b;
   ```

   1. a = a0 ^ b0
   2. b = b0 ^ a0 ^ b0
   3. a = a0 ^ b0 ^ a0



### 2. 其他运算符

#### 2.1 复合运算符

> Compound Assignment Operator

a += 1 与 a = a + 1

* 前者求值一次(寄存器读取一次)
* 后者求值两次
* 在效率上和副作用上前者都较好



#### 2.3 逗号运算符

```c
int t = 5;
int c = (t=3, t+2);
printf("%d\n", c);
```

1. t 赋值为3
2. c 赋值为 t+2



#### 2.4 sizeof and typedef

`sizeof`返回值类型为size_t

```c
int a[12];
printf("%d\n",sizeof a/sizeof a[0]);
```

```bash
size.c:6:14: warning: format ‘%d’ expects argument of type ‘int’, but argument 2 has type ‘long unsigned int’ [-Wformat=]
    6 |     printf("%d\n",sizeof a/sizeof a[0]);
      |             ~^    ~~~~~~~~~~~~~~~~~~~~
      |              |    |
      |              int  long unsigned int
      |             %ld
```

可得出size_t 类型为 `long unsigned int`

typedef 表示为类型取一个新名字

```c
typedef unsigned long int size_t;
```

* 将`unsigned long int` 取名为 `size_t`
* 如果不加上typedef，size_t表示变量名



## 第 17 章 计算机体系结构基础

### 2. CPU

> 从内存取指令，然后解释执行，然会取下一条指令



### 3. 设备

#### 架构：

* 内存映射IO：CPU直接连接设备寄存器类似直接访问内存
* 端口IO：引出额外的总线来接设备（专用访问）

#### 执行过程

在x86平台上，硬盘是挂在IDE、SATA或SCSI总线上的设备，保存在硬盘上的程序是不能被CPU直接取指令执行的，操作系统在执行程序时会把它从硬盘拷贝到内存，这样CPU才能取指令执行，这个过程称为加载（Load）。程序加载到内存之后，成为操作系统调度执行的一个任务，就称为进程（Process）。进程和程序不是一一对应的。一个程序可以多次加载到内存，成为同时运行的多个进程，例如可以同时开多个终端窗口，每个窗口都运行一个Shell进程，而它们对应的程序都是磁盘上的`/bin/bash`文件。

#### 操作系统启动

操作系统（Operating System）本身也是一段保存在磁盘上的程序，计算机在启动时执行一段固定的启动代码（称为Bootloader）首先把操作系统从磁盘加载到内存，然后执行操作系统中的代码把用户需要的其它程序加载到内存。操作系统和其它用户程序的不同之处在于：操作系统是常驻内存的，而其它用户程序则不一定，用户需要运行哪个程序，操作系统就把它加载到内存，用户不需要哪个程序，操作系统就把它终止掉，释放它所占的内存。操作系统最核心的功能是管理进程调度、管理内存的分配使用和管理各种设备，做这些工作的程序称为内核（Kernel），在我的系统上内核程序是`/boot/vmlinuz-2.6.28-13-generic`文件，它在计算机启动时加载到内存并常驻内存。广义上操作系统的概念还包括一些必不可少的用户程序，比如Shell是每个Linux系统必不可少的，而Office办公套件则是可有可无的，所以前者也属于广义上操作系统的范畴，而后者属于应用软件。

#### 中断

访问设备还有一点和访问内存不同。内存只是保存数据而不会产生新的数据，如果CPU不去读它，它也不需要主动提供数据给CPU，所以内存总是被动地等待被读或者被写。而设备往往会自己产生数据，并且需要主动通知CPU来读这些数据，例如敲键盘产生一个输入字符，用户希望计算机马上响应自己的输入，这就要求键盘设备主动通知CPU来读这个字符并做相应处理，给用户响应。这是由中断（Interrupt）机制实现的，每个设备都有一条中断线，通过中断控制器连接到CPU，当设备需要主动通知CPU时就引发一个中断信号，CPU正在执行的指令将被打断，程序计数器会指向某个固定的地址（这个地址由体系结构定义），于是CPU从这个地址开始取指令（或者说跳转到这个地址），执行中断服务程序（ISR，Interrupt Service Routine），完成中断处理之后再返回先前被打断的地方执行后续指令。比如某种体系结构规定发生中断时跳转到地址0x00000010执行，那么就要事先把一段ISR程序加载到这个地址，ISR程序是内核代码的一部分，在这段代码中首先判断是哪个设备引发了中断，然后调用该设备的中断处理函数做进一步处理。

由于各种设备的操作方法各不相同，每种设备都需要专门的设备驱动程序（Device Driver），一个操作系统为了支持广泛的设备就需要有大量的设备驱动程序，事实上Linux内核源代码中绝大部分是设备驱动程序。设备驱动程序通常是内核里的一组函数，通过读写设备寄存器实现对设备的初始化、读、写等操作，有些设备还要提供一个中断处理函数供ISR调用。



### 3. MMU

操作系统采用 虚拟内存管理（Virtual Memory Management）机制，需要

> MMU（Memory Management Unit，内存管理单元）

#### 物理内存和虚拟内存

物理内存：CPU执行单元将内存地址直接传输到内存芯片引脚上接受

![物理地址](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/arch.pabox.png)

CPU执行单元发出的内存地址将被MMU截获，从CPU到MMU的地址称为虚拟地址（Virtual Address，以下简称VA），而MMU将这个地址翻译成另一个地址发到CPU芯片的外部地址引脚上

![虚拟地址](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/arch.vabox.png)

虚拟地址空间和物理地址空间是独立的，32位处理器的虚拟地址空间是4GB，而物理地址空间既可以大于也可以小于4GB。

MMU将VA映射到PA是以页（Page）为单位的，32位处理器的页尺寸通常是4KB。例如，MMU可以通过一个映射项将VA的一页0xb7001000~0xb7001fff映射到PA的一页0x2000~0x2fff，如果CPU执行单元要访问虚拟地址0xb7001008，则实际访问到的物理地址是0x2008。物理内存中的页称为物理页面或者页帧（Page Frame）。虚拟内存的哪个页面映射到物理内存的哪个页帧是通过页表（Page Table）来描述的，页表保存在物理内存中，MMU会查找页表来确定一个VA应该映射到什么PA。（字典与索引）



#### 操作系统与MMU

1. 启动时，在物理内存中填表
2. 用指令让MMU知道页表在物理内存的哪个地方
3. CPU访问内存都会先让MMU查表和地址转换
4. 地址转换可以有硬件自动完成



#### 为什么需要多一层

> All problems in computer science can be solved by another level of indirection.



#### MMU功能

* 保护内存：设置页面访问权限

![处理器模式](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/arch.cpumode.png)

#### 虚拟地址空间划分

* 内核空间
* 用户空间

例如x86平台的Linux系统虚拟地址空间是0x00000000~0xffffffff，前3GB（0x00000000~0xbfffffff）是用户空间，后1GB（0xc0000000~0xffffffff）是内核空间。

##### 空间切换

用户程序加载到用户空间，在用户模式下执行，不能访问内核中的数据，也不能跳转到内核代码中执行。这样可以保护内核，如果一个进程访问了非法地址，顶多这一个进程崩溃，而不会影响到内核和整个系统的稳定性。CPU在产生中断或异常时不仅会跳转到中断或异常服务程序，还会自动切换模式，从用户模式切换到特权模式，因此从中断或异常服务程序可以跳转到内核代码中执行。事实上，整个内核就是由各种中断和异常处理程序组成的。总结一下：在正常情况下处理器在用户模式执行用户程序，在中断或异常情况下处理器切换到特权模式执行内核程序，处理完中断或异常之后再返回用户模式继续执行用户程序。



#### 段错误的产生

1. 用户程序访问虚拟内存，无权查看
2. MMU产生异常，CPU从用户模式切换到特权模式，跳转到内核代码执行异常服务程序
3. 内核把这个异常解释为段错误，把引发异常的进程中止。



### 5. Memory Hierarchy

#### Cache:

和MMU一样位于CPU核中。

Cache缓存最近访问过的内存数据，由于Cache的访问速度是内存的几十倍，所以有效利用Cache可以大大提高计算机的整体性能。一级Cache是这样工作的：CPU执行单元要访问内存时首先发出VA，Cache利用VA查找相应的数据有没有被缓存，如果Cache中有就不需要访问物理内存了，如果是读操作就直接将Cache中的数据传给CPU寄存器，如果是写操作就直接改写到Cache中；如果Cache没有缓存该数据，**就去物理内存中取数据，但并不是要哪个字节就取哪个字节，而是把相邻的几十个字节都取上来缓存着，以备下次用到，这称为一个Cache Line，典型的Cache Line大小是32~256字节。**

如果计算机还配置了二级缓存，则在访问物理内存之前先用PA去二级缓存中查找。一级缓存是用VA寻址的，二级缓存是用PA寻址的，这是它们的区别。Cache所做的工作是由硬件自动完成的，而不是像寄存器一样由指令决定先做什么后做什么。

Cache从内存取数据时会预取一个Cache Line缓存起来，操作系统从硬盘读数据时会预读几个页面缓存起来，都是希望这些数据以后会被程序访问到。

大多数程序的行为都具有局部性（Locality）的特点：它们会花费大量的时间反复执行一小段代码（例如循环），或者反复访问一个很小的地址范围中的数据（例如访问一个数组）。所以预读缓存的办法是很有效的：CPU取一条指令，我把和它相邻的指令也都缓存起来，CPU很可能马上就会取到；CPU访问一个数据，我把和它相邻的数据也都缓存起来，CPU很可能马上就会访问到。

设想有两台计算机，一台有256KB的Cache，另一台没有Cache，两台计算机的内存都是512MB的，硬盘都是100GB的，虽然多出来256KB的Cache与内存、硬盘的容量相比微不足道，但访问Cache比访问内存、硬盘快几个数量级，由于局部性原理，CPU大部分时间是在和Cache打交道，有Cache的计算机明显会快很多。高速存储器的容量只能做得很小，却能显著提升计算机的性能，这就是Memory Hierarchy的意义所在。



## 第 18 章 x86汇编程序基础

### 1.最简单的汇编程序

```assembly
#PURPOSE: Simple program that exits and returns a  #代表单行注释
#	  status code back to the Linux kernel
#
#INPUT:   none
#
#OUTPUT:  returns a status code. This can be viewed
#	  by typing
#
#	  echo $?
#
#	  after running the program
#
#VARIABLES:
#	  %eax holds the system call number
#	  %ebx holds the return status
#

# 以.开头  表示特殊指令
# .sectio表示把代码分成若干段 不同段有不同程序和不同权限
# .data段保存程序的数据 类似C语言全局变量
 .section .data

# .text保存代码 只读可执行 后续指令都属于.text段
 .section .text
 
 # _start表示一个符号，符号在汇编程序中代表一个地址
 .globl _start
_start:
 movl $1, %eax	# this is the linux kernel command
		# number (system call) for exiting
		# a program

 movl $4, %ebx	# this is the status number we will
		# return to the operating system.
		# Change this around and it will
		# return different things to
		# echo $?

 int $0x80	# this wakes up the kernel to run
		# the exit command
```



#### 汇编程序运行流程

1. 汇编器as把汇编程序的主给付翻译成机器，生成目标文件

   ```bash
   $ as hello.s -o hello.o
   ```

2. 链接器（Linker, 或Link Editor）`ld` 把目标文件`hello.o`链接成可执行文件`hello`：

   ```bash
   $ ld hello.o -o hello
   ```

   连接的作用：

   * 修改目标文件信息，对地址做重定向
   * 多个目标文件合并成一个可执行文件

3. 得到上一条命令的退出状态

   ```bash
   echo $?
   0
   ```



#### 变量和函数本质

在C语言中我们通过变量名访问一个变量，其实就是读写某个地址的内存单元，我们通过函数名调用一个函数，其实就是跳转到该函数第一条指令所在的地址，所以变量名和函数名都是符号，本质上是代表内存地址的。



```assembly
.gloabl _start
```

`globl`指示告诉汇编器，`_start`这个符号要被链接器用到，所以要在目标文件的符号表中标记它是一个全局符号（在[第 5.1 节 “目标文件”](https://docs.huihoo.com/c/linux-c-programming/ch18s05.html#asm.relocatable)详细解释）。`_start`就像C程序的`main`函数一样特殊，是整个程序的入口，链接器在链接时会查找目标文件中的`_start`符号代表的地址，把它设置为整个程序的入口地址，所以每个汇编程序都要提供一个`_start`符号并且用`.globl`声明。如果一个符号没有用`.globl`声明，就表示这个符号不会被链接器用到。

```assembly
_start:
```

两个_start: 

* 一个代表实际地址  🏠
* 一个代表开门的钥匙 🔑_

而`_start`这个符号又比较特殊，它所代表的地址是整个程序的入口地址，所以下一条指令`movl $1, %eax`就成了程序中第一条被执行的指令。

```assembly
movl $1, %eax
```

`mov`的后缀l表示long，说明是32位的传送指令。这条指令不要求CPU读内存，1这个数是在CPU内部产生的，称为立即数（Immediate）。在汇编程序中，立即数前面要加$，寄存器名前面要加%，以便跟符号名区分开。以后我们会看到`mov`指令还有另外几种形式，但数据传送方向都是一样的，第一个操作数总是源操作数，第二个操作数总是目标操作数。

```assembly
int $0x80
```

系统调用，软中断处理，产生异常，切换用户状态，类似产生一个函数

传递两个参数：一个系统调用号，一个参数，类似函数的函数名和函数参数。



```assembly
_exit
```

* 直接终止当前进程



### 4.寻址方式

* 直接寻址

  ```assembly
  movl ADDRESS, %eax
  ```

* 变址寻址

  ```assembly
  movl data_items(,%edi,4), %eax
  ```

  用于访问数组

* 间接寻址，把某个地址存放在寄存器中

  ```assembly
  movl (%eax), %ebx
  ```

* 基址寻址

  ```assembly
  movl 4(%eax), %ebx
  ```

  4为偏移量

* 立即寻址

  ```assembly
  movl $12, %eax $12
  ```

  在CPU内产生数字

* 寄存器寻址

  ```assembly
  movl $12, %eax
  ```

​		指令中有个操作数是寄存器



### 5.FLE 文件格式

* 可重定位的文件
* 可执行的文件
* 共享文件

现在详细解释一下这个程序的汇编、链接、运行过程：

1. 写一个汇编程序保存成文本文件`max.s`。
2. 汇编器读取这个文本文件转换成目标文件`max.o`，目标文件由若干个Section组成，我们在汇编程序中声明的`.section`会成为目标文件中的Section，此外汇编器还会自动添加一些Section（比如符号表）。
3. 然后链接器把目标文件中的Section合并成几个Segment[[28](https://docs.huihoo.com/c/linux-c-programming/ch18s05.html#ftn.id2770769)]，生成可执行文件`max`。
4. 最后加载器（Loader）根据可执行文件中的Segment信息加载运行这个程序。



#### ELF文件

![ELF文件](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/asm.elfoverview.png)



* ELF Header：体系结构和操作系统的信息，并指出文件开始和结束的位置（program\section header table）
* section header table 保存所有 section的描述信息



#### 目标文件

`readelf` 读出ELF Header和Section Header Table

```bash
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00 
  Class:                             ELF64
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V # 系统
  ABI Version:                       0
  Type:                              REL (Relocatable file)
  Machine:                           Advanced Micro Devices X86-64 #体系结构
  Version:                           0x1
  Entry point address:               0x0
  Start of program headers:          0 (bytes into file)
  Start of section headers:          504 (bytes into file) #从这个地址开始
  Flags:                             0x0
  Size of this header:               64 (bytes) 
  Size of program headers:           0 (bytes)
  Number of program headers:         0
  Size of section headers:           64 (bytes) # 每个section header 占40自己字节
  Number of section headers:         8 
  Section header string table index: 7
```



##### Section Header信息

```bash
Section Headers:
  [Nr] Name              Type             Address(虚拟地址，用于链接)   Offset(之处section文件地址)
       Size              EntSize          Flags  Link  Info  Align
  [ 0]                   NULL             0000000000000000  00000000
       0000000000000000  0000000000000000           0     0     0
  [ 1] .text             PROGBITS         0000000000000000  00000040
       000000000000002d  0000000000000000  AX       0     0     1
  [ 2] .rela.text        RELA             0000000000000000  00000190
       0000000000000030  0000000000000018   I       5     1     8
  [ 3] .data             PROGBITS         0000000000000000  0000006d
       0000000000000038  0000000000000000  WA       0     0     1
  [ 4] .bss              NOBITS           0000000000000000  000000a5
       0000000000000000  0000000000000000  WA       0     0     1
  [ 5] .symtab           SYMTAB           0000000000000000  000000a8
       00000000000000c0  0000000000000018           6     7     8
  [ 6] .strtab           STRTAB           0000000000000000  00000168
       0000000000000028  0000000000000000           0     0     1
  [ 7] .shstrtab         STRTAB           0000000000000000  000001c0
       0000000000000031  0000000000000000           0     0     1
Key to Flags:
  W (write), A (alloc), X (execute), M (merge), S (strings), I (info),
  L (link order), O (extra OS processing required), G (group), T (TLS),
  C (compressed), x (unknown), o (OS specific), E (exclude),
  l (large), p (processor specific)

There are no section groups in this file.

There are no program headers in this file.

There is no dynamic section in this file.
```

左边一列是文件地址，中间是每个字节的十六进制表示，右边是把这些字节解释成ASCII码所对应的字符。中间有一个*号表示省略的部分全是0。`.data`段对应的是这一块：

```bash
00000000  7f 45 4c 46 02 01 01 00  00 00 00 00 00 00 00 00  |.ELF............|
00000010  01 00 3e 00 01 00 00 00  00 00 00 00 00 00 00 00  |..>.............|
00000020  00 00 00 00 00 00 00 00  f8 01 00 00 00 00 00 00  |................|
00000030  00 00 00 00 40 00 00 00  00 00 40 00 08 00 07 00  |....@.....@.....|
```



```bash
Relocation section '.rela.text' at offset 0x190 contains 2 entries:
  Offset          Info           Type           Sym. Value    Sym. Name + Addend
000000000009  00020000000a R_X86_64_32       0000000000000000 .data + 0
00000000001a  00020000000a R_X86_64_32       0000000000000000 .data + 0
# 在哪些地方做重定向

The decoding of unwind sections for machine type Advanced Micro Devices X86-64 is not currently supported.

# 符号表
Symbol table '.symtab' contains 8 entries:
   Num:    Value          Size Type    Bind   Vis      Ndx Name #所在section编号
     0: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT  UND 
     1: 0000000000000000     0 SECTION LOCAL  DEFAULT    1 
     2: 0000000000000000     0 SECTION LOCAL  DEFAULT    3 
     3: 0000000000000000     0 SECTION LOCAL  DEFAULT    4 
     4: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT    3 data_items
     5: 000000000000000f     0 NOTYPE  LOCAL  DEFAULT    1 start_loop
     6: 0000000000000026     0 NOTYPE  LOCAL  DEFAULT    1 loop_exit
     7: 0000000000000000     0 NOTYPE  GLOBAL DEFAULT    1 _start

No version information found in this file.
```





## 第 19 章 汇编与C的关系

### 2.main函数与启动例程

1. `ctrl.o`中的_start初始化（Startup Routine）
2. 调用C代码中的main函数



#### 分布编译

1. 将C语言编译为.o文件

   ```bash
   gcc -o mian.o main.c
   ```

2. 将.o文件编译为可执行文件

   ```bash
   gcc -o main main.o
   ```

   这条命令本质是调用`ld`命令去链接

   ```bash
   ld /usr/lib/crt1.o /usr/lib/crti.o main.o -o main -lc -dynamic-linker /lib/ld-linux.so.2
   ```

   -lc：调用动态链接库

#### 

#### `ctrl.o` 如何调用 main

```bash
$ nm /usr/lib/crt1.o 
00000000 R _IO_stdin_used
00000000 D __data_start
         U __libc_csu_fini
         U __libc_csu_init
         U __libc_start_main
00000000 R _fp_hw
00000000 T _start
00000000 W data_start
         U main
$ nm /usr/lib/crti.o
         U _GLOBAL_OFFSET_TABLE_
         w __gmon_start__
00000000 T _fini
00000000 T _init
```

**U main**，表示尚未定义，需要指定文件将其与`crtl.o`连接来提供这个地址，在此时`ld`类似一个指针或者变基

详细编译过程

```bash
gcc -v
Using built-in specs.
COLLECT_GCC=gcc
COLLECT_LTO_WRAPPER=/usr/lib/gcc/x86_64-linux-gnu/9/lto-wrapper
OFFLOAD_TARGET_NAMES=nvptx-none:hsa
OFFLOAD_TARGET_DEFAULT=1
Target: x86_64-linux-gnu
Configured with: ../src/configure -v --with-pkgversion='Ubuntu 9.4.0-1ubuntu1~20.04.1' --with-bugurl=file:///usr/share/doc/gcc-9/README.Bugs --enable-languages=c,ada,c++,go,brig,d,fortran,objc,obj-c++,gm2 --prefix=/usr --with-gcc-major-version-only --program-suffix=-9 --program-prefix=x86_64-linux-gnu- --enable-shared --enable-linker-build-id --libexecdir=/usr/lib --without-included-gettext --enable-threads=posix --libdir=/usr/lib --enable-nls --enable-clocale=gnu --enable-libstdcxx-debug --enable-libstdcxx-time=yes --with-default-libstdcxx-abi=new --enable-gnu-unique-object --disable-vtable-verify --enable-plugin --enable-default-pie --with-system-zlib --with-target-system-zlib=auto --enable-objc-gc=auto --enable-multiarch --disable-werror --with-arch-32=i686 --with-abi=m64 --with-multilib-list=m32,m64,mx32 --enable-multilib --with-tune=generic --enable-offload-targets=nvptx-none=/build/gcc-9-Av3uEd/gcc-9-9.4.0/debian/tmp-nvptx/usr,hsa --without-cuda-driver --enable-checking=release --build=x86_64-linux-gnu --host=x86_64-linux-gnu --target=x86_64-linux-gnu
Thread model: posix
gcc version 9.4.0 (Ubuntu 9.4.0-1ubuntu1~20.04.1) 
```

观察`main`文件的汇编代码

```
0000000000001190 <__libc_csu_init>:
0000000000001200 <__libc_csu_fini>:
```

在`crtl.o`文件中未定义的文件，都链接了相关地址，而`__libc_start_main`未定义，此库做运行时的动态链接。（用户扩展链接）

```bash
 1061:       48 8d 3d 02 01 00 00    lea    0x102(%rip),%rdi        # 116a <main>
    1068:       ff 15 72 2f 00 00       callq  *0x2f72(%rip)        # 3fe0 <__libc_start_main@GLIBC_2.2.5>
```

取出main函数的地址传给寄存器，然后执行该地址下的main函数

实际上，Startup Routine调用main函数的形式为

```bash
exit(main(argc, argv));
```

`ctrl.o`调用main函数获取main函数的返回值，传递给系统调用_exit，成为退出的进程。

有意思的C语言程序

```c
#include <stdlib.h>

int main(void){
    exit(-1);
}
```

```bash
gcc test01.c ;./a.out; echo $?
255
```

可看到异常退出为-1，且系统调用的返回值为255



### 3. 变量的存储布局

#### 例子1

```c
#include <stdio.h>

const int A = 10;
int a = 20;
static int b = 30;
int c;

int main(void){
    static int a = 40;
    char b[] = "Hello world";
    register int c = 50;

    printf("Heloo World %d\n", c);

    return 0;
}
```

* `const`
* static
* register

```bash
$ readelf -a a.out
```

查看符号表

```bash
66: 0000000000002004     4 OBJECT  GLOBAL DEFAULT   18 A
69: 0000000000004020     4 OBJECT  GLOBAL DEFAULT   26 c
70: 0000000000004010     4 OBJECT  GLOBAL DEFAULT   25 a
42: 0000000000004014     4 OBJECT  LOCAL  DEFAULT   25 b 
# 由于 b 被static修饰这个符号为local
# local表示这个符号只能在某一个文件中使用，而不能被全局链接
```

##### 全局变量和局部变量的区分

> 符号表

* global：全局变量
* local：

具体观察变量

```bash
 43: 0000000000004018     4 OBJECT  LOCAL  DEFAULT   25 a.2320
 70: 0000000000004010     4 OBJECT  GLOBAL DEFAULT   25 a
```

可以看到，函数中的static变量不同于局部变量：**函数调用时分配，返回时释放**，而是像全局变量一样静态分配，由于a变量还是只在局部变量起作用，在其他函数中就不是指他，所以有一个后缀与全局变量区分



##### 函数中的数组

```bash
char b[] = "Hello world";
1185:       48 b8 48 65 6c 6c 6f    movabs $0x6f77206f6c6c6548,%rax
118c:       20 77 6f 
118f:       48 89 45 dc             mov    %rax,-0x24(%rbp)
1193:       c7 45 e4 72 6c 64 00    movl   $0x646c72,-0x1c(%rbp)
register int c = 50;
119a:       bb 32 00 00 00          mov    $0x32,%ebx
```

可知这个数组直接由指令写进寄存器

![数组的存储布局2](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/asmc.array.png)

数组的地址由第一个基地址决定。



register修饰的关键字，直接由寄存器存储，且函数使用该变量时，只将首地址压入函数栈帧中。



#### 全局变量和局部变量

> Scope

* 函数作用域

* 文件作用域

  * main
  * 全局变量
  * 库文件函数(printf)

* 块作用域

  * {}中，类似Java中的代码块

* 函数原型作用域

  * 函数声明而非定义

    ```c
    int foo(int a, int b);
    ```

    此中的a和b



#### 同一`namespace`的重名标识符

> 内层覆盖外层



#### 可修饰标识符的修饰符

链接类：

* external：类似文件中的全局变量
* internal
* no link

存储类修饰符

* static：存储空间静态分配，internal link属性
* auto：自动分配存储空间
* register：分配一个专门的寄存器储存，分配不开寄存器当作auto
* extern：多次声明同一个变量



#### 变量的生存期

* 静态生存期：static修饰的变量
* 自动生存期：被修饰的变量
* 动态生存期：malloc\free



### 4.结构体与联合体

```c
#include <stdio.h>

int main(int argc, char const *argv[])
{
    struct {
        char a;
        short b;
        int c;
        char d;
    } s;

    s.a = 1;
    s.b = 2;
    s.c = 3;
    s.d = 4;

    printf("%u\n", sizeof(s));
    
    return 0;
}

```

结构体汇编代码

```assembly
 s.a = 1;
 115c:       c6 45 f4 01             movb   $0x1,-0xc(%rbp)
 s.b = 2;
 1160:       66 c7 45 f6 02 00       movw   $0x2,-0xa(%rbp)
 s.c = 3;
 1166:       c7 45 f8 03 00 00 00    movl   $0x3,-0x8(%rbp)
 s.d = 4;
 116d:       c6 45 fc 04             movb   $0x4,-0x4(%rbp)
```

从低向高排列，类似数组，但会有空隙。

![image-20220825085153131](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/image-20220825085153131.png)



#### 对齐

> 对齐的指令执行效率比较高

* `movl`：访问4字节，所访问内存地址是4的整数倍

* `movw`：访问两字节指令

* `movb`：访问两字节

a占一个字节，而b占两个字节，为了对齐，a进行填充占两个字节，同样d因为c也做了字节填充。

gcc 扩展语法避免填充

```c
struct {
        char a;
        short b;
        int c;
        char d;
    } __attribute__((packed)) s;
```

  

### 5.C内联汇编

基本格式`__asm__("asm code\n\t")`





### 2.main函数与启动例程

> _start 与 main函数的区别和联系



### 6.Volatitle 限定符

```bash
buf[0] = recv;
1131:       0f b6 05 d9 2e 00 00    movzbl 0x2ed9(%rip),%eax #4011<recv>
1138:       88 05 d4 2e 00 00       mov    %al,0x2ed4(%rip)  #4012 <buf>
buf[1] = recv;
113e:       0f b6 05 cc 2e 00 00    movzbl 0x2ecc(%rip),%eax #4011<recv>
1145:       88 05 c8 2e 00 00       mov    %al,0x2ec8(%rip)  
#4013 <buf+0x1>
buf[2] = recv;
114b:       0f b6 05 bf 2e 00 00    movzbl 0x2ebf(%rip),%eax #4011<recv>
1152:       88 05 bc 2e 00 00       mov    %al,0x2ebc(%rip)  
# 4014 <buf+0x2>
send = ~buf[0];
1158:       0f b6 05 b3 2e 00 00    movzbl 0x2eb3(%rip),%eax # 4012<buf>
115f:       f7 d0                   not    %eax
1161:       88 05 ae 2e 00 00       mov    %al,0x2eae(%rip) # 4015<send>
send = ~buf[1];
1167:       0f b6 05 a5 2e 00 00    movzbl 0x2ea5(%rip),%eax        
# 4013 <buf+0x1>
116e:       f7 d0                   not    %eax
1170:       88 05 9f 2e 00 00       mov    %al,0x2e9f(%rip) #4015 <send>
send = ~buf[2];
1176:       0f b6 05 97 2e 00 00    movzbl 0x2e97(%rip),%eax        
# 4014 <buf+0x2>
117d:       f7 d0                   not    %eax
117f:       88 05 90 2e 00 00       mov    %al,0x2e90(%rip) #4015 <send>
```

#### 汇编解读

```assembly
movzbl 0x2ed9, %eax
```

movz：将字长短移动到字长长的寄存器中，高位用0填充

* b(byte)：单子节
* w(word)：两字节
* l(long)：四字节

在该汇编代码中，意思为将地址0x2ed9出的一个字节移动到四位字节eax寄存器中。

```assembly
mov	%al, 0x804a01a
```

`al`寄存器是`eax`寄存器的低字节，把这个字节移动到`0x804a01a`的一个字节中。该操作反映了可以用不同名称来访问寄存器的低8位或者16位字节。

![eax寄存器](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/asmc.eax.png)

在`eax`寄存器中，`al`表示低8位，`ax`表示低16位。



#### 尝试编译优化

添加`-o`编译优化项

```bash
buf[0] = recv;
112d:       0f b6 05 dd 2e 00 00    movzbl 0x2edd(%rip),%eax#4011 <recv>
1134:       88 05 d8 2e 00 00       mov    %al,0x2ed8(%rip) #4012 <buf>
buf[1] = recv;
113a:       88 05 d3 2e 00 00       mov    %al,0x2ed3(%rip)        
# 4013 <buf+0x1>
buf[2] = recv;
1140:       88 05 ce 2e 00 00       mov    %al,0x2ece(%rip)        
# 4014 <buf+0x2>
send = ~buf[0];
1146:       f7 d0                   not    %eax
1148:       88 05 c7 2e 00 00       mov    %al,0x2ec7(%rip) #4015 <send>
send = ~buf[1];
send = ~buf[2];
```

可看到，只有第一条指令从内存中读取到了读取一个字节到寄存器`eax`中，然后将寄存器的值`al`保存到`buf[0]`中，后续指令直接把`al`到值直接保存到`buf[1]\buf[2]`中。

后三条指令也只有把第一次的值取反，然后保存到内存地址中。



#### volatile的使用

> 防止编译器自作聪明，把不该优化的优化了

```c
//artificial device registers
volatile unsigned char recv;
volatile unsigned char send;
```

反编译的结果

```bash
buf[0] = recv;
    112d:       0f b6 0d dd 2e 00 00    movzbl 0x2edd(%rip),%ecx        # 4011 <recv>
    1134:       88 0d d8 2e 00 00       mov    %cl,0x2ed8(%rip)        # 4012 <buf>
    buf[1] = recv;
    113a:       0f b6 15 d0 2e 00 00    movzbl 0x2ed0(%rip),%edx        # 4011 <recv>
    1141:       88 15 cc 2e 00 00       mov    %dl,0x2ecc(%rip)        # 4013 <buf+0x1>
    buf[2] = recv;
    1147:       0f b6 05 c3 2e 00 00    movzbl 0x2ec3(%rip),%eax        # 4011 <recv>
    114e:       88 05 c0 2e 00 00       mov    %al,0x2ec0(%rip)        # 4014 <buf+0x2>
    send = ~buf[0];
    1154:       f7 d1                   not    %ecx
    1156:       88 0d b9 2e 00 00       mov    %cl,0x2eb9(%rip)        # 4015 <send>
    send = ~buf[1];
    115c:       f7 d2                   not    %edx
    115e:       88 15 b1 2e 00 00       mov    %dl,0x2eb1(%rip)        # 4015 <send>
    send = ~buf[2];
    1164:       f7 d0                   not    %eax
    1166:       88 05 a9 2e 00 00       mov    %al,0x2ea9(%rip)        # 4015 <send>
```

可以看到并没有被编译优化，优化选项`01\02\03\0s`编译选项逐渐增多。



#### Cache的透明问题

> 读写内存时可能从Cache中直接读写，而不从内存中读写。

![串口发送和接收寄存器被Cache缓存会有什么问题](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/asmc.nocache.png)

* 如果CPU同时写入三个字节，如果被优化，那么只有一个字节被写入Cache中
* 那么前两个字节就被丢失

##### 解决办法

* 在某一段范围禁用Cache，在页表中设置哪些Cache页面可以缓存
* 信号处理和多线程情况下也需要使用`volatile`



## 第20章 链接详解

### 1.多目标文件的链接

```bash
warning: implicit declaration of function ‘push’ [-Wimplicit-function-declaration]
```

-隐式函数声明 警告

```c
/* main.c */
#include <stdio.h>

int a, b = 1;

int main(void)
{
	push('a');
	push('b');
	push('c');
	
	while(!is_empty())
		putchar(pop());
	putchar('\n');

	return 0;
}
```

nm查看符号表

```bash
# nm main.o
0000000000000004 C a
0000000000000000 D b
                 U _GLOBAL_OFFSET_TABLE_
                 U is_empty
0000000000000000 T main
                 U pop
                 U push
                 U putchar
```

可以看到`stack.c`与`putchar`中的函数均为定义，`putchar`为`libc`的库函数，在可执行文件`main`中未定义。

#### 具体链接文件

![多目标文件的链接](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/link.multiobj.png)

编译程序将各个段暴力合并，方便调用。

``` bash
gcc stack.o main.o -o main
```

根据链接程序的先后来确定最后各个段中符号的顺序。



#### 链接脚本

```bash
$ ld --verbose
```

`ENTRY(_start)`说明`_start`是整个程序的入口点，因此`_start`是入口点并不是规定死的，是可以改用其它函数做入口点的。

```bash
PROVIDE (__executable_start = SEGMENT_START("text-segment", 0x400000)); . = SEGMENT_START("text-segment", 0x400000) + SIZEOF_HEADERS;
```

这是Text Segment的起始地址

```bash
 .interp         : { *(.interp) }
```

左边表示`.interp`最终生成的文件，表示由各个被链接文件的`.interp`段组成。

### 2.定义与声明

隐式函数声明：

* 返回值为：int
* 声明时带有`extern`就不会带有警告

> 使用`static`则表示该标识符`Internal Linkage`

**注意变量声明与函数声明`extern`区别很大**



#### 2.1 C语言封装 extern和static关键字

> Encapsulation

注意某些变量只能由内部进行访问，应该需要阻止外部的访问，需要声明为`Internal Linkage`：

```c
/* stack.c */
static char stack[512];
static int top = -1;
```

这样在`main.c`中声明了`extern`变量也访问不到`top` 和`stack`，类似一种封装的思想。



#### 2.2 头文件

> 防止重复的声明，引入了头文件

```c
/*stack.h*/
#ifndef STACK_H
#define STACK_H
extern void push(char);
extern char pop(void);
extern int  is_empty(void);
#endif
```

此时使用`gcc -c main.c`编译，会直接找到目录中的`stack.h`。

```bash
$ tree
.
|-- main.c
|-- stack.c
`-- stack.h

0 directories, 3 files
```

如果将`stack.c`与`stack.h`移动到同一个字目录下，则需要使用`gcc -c main.c -Istack`编译。

```bash
$ tree
.
|-- main.c
`-- stack
    |-- stack.c
    `-- stack.h

1 directory, 3 files
```



##### 预定义宏

```c
#ifndef STACK_H
#define STACK_H
```

如果这个宏定义过，那么`ifndef`与`endif`之间的代码就包含在预处理的输出结果中，否则就不包含在预处理的结果中。这样就避免了包含了重复的内容。



##### 包含重复的头文件

1. 预处理速度变慢
2. 互相包含头文件，陷入死循环
3. 有些头文件的定义不允许重复出现



##### 包含`.h`而不是`.c`

文件项目过大，在文件内部有重复定义

![为什么要包含头文件而不是.c文件](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/link.includeh.png)

此时链接脚本就不知道该如何组装了。

> 智能组装程序？？？



#### 2.3 定义和声明的详细规则

* extern : previous linkage
* Static : internal linkage



### 3.静态库

> 在多个项目中都需要用到

静态库打包

```bash
$ ar rs libstack.a stack.o push.o pop.o is_empty.o
ar: creating libstack.a
```

注意库名以`lib`开头，以`.a`为后缀，表示`Archive`，`ar`类似`tar`命令，打包，s用于生成静态库，表示建立索引。

`ranlib`命令可以为静态库创建索引

然后将`libstack.a`和`main.c`编译链接在一起：

```bash
$ gcc main.c -L. -lstack -Istack -o main
```

-L：表示编译去哪里找库文件

编译器会首先去寻找共享库`libstack.so`，没有继续寻找`libstack.a`



### 4.共享库

#### 4.1 编译运行

```bash
$ gcc -c -fPIC stack/stack.c stack/push.c stack/pop.c stack/is_empty.c
```

用到外部的函数和变量时，会在链接过程进行地址的重定向



### 5.虚拟内存管理

`/proc`目录中的文件并不是真正的磁盘文件，而是由内核虚拟出来的文件系统，当前系统中运行的每个进程在`/proc`下都有一个子目录，目录名就是进程的id，查看目录下的文件可以得到该进程的相关信息。



#### 作用

1. 保护物理内存
2. 让每个进程都有自己的独立空间

![进程地址空间是独立的](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/link.sepva-20220902115318718.png)

* 可读部分共享（共享库的来源）

3. 虚拟内存的连续映射不连续的物理内存（防止需要一大块连续内存的情况）

![不连续的PA可以映射为连续的VA](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/link.discontpa-20220902115318805.png)

4. 分配更大的内存（部分内容可临时保存到磁盘中：交换设备）



## 第21章 预处理的步骤

* Icloud丢弃没必要的空白和换行符



### 2.宏定义

#### 2.1函数式宏

> Function-like Macro

```c
#define MAX(a, b) ((a)>(b)?(a):(b))
k = MAX(i&0x0f, j&0x0f)
```

```bash
gcc -E 
```

查看预处理结果

```bash
# 1 "test01.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 31 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 32 "<command-line>" 2
# 1 "test01.c"


int main(int argc, char const *argv[])
{
    int k;
    k = ((i&0x0f) > (j&0x0f)?(i&0x0f):(j&0x0f));
    return 0;
}
```

与函数调用类似，替换了宏的参数

##### 宏函数和函数区别

* 参数没有类型，只负责做形式上的替换，不做参数类型检查
* 宏编译生成的指令都类似一个函数体
* 优先级问题
* 宏的代码执行效率低（但是省略了栈帧、传参、返回）

频繁调用的函数经常使用宏来定义

##### 例子1

```c
#define device_init_wakeup(dev,val) \
        do { \
                device_can_wakeup(dev) = !!(val); \
                device_set_wakeup_enable(dev,val); \
        } while(0)

#define device_init_wakeup(dev,val) \
                device_can_wakeup(dev) = !!(val); \
                device_set_wakeup_enable(dev,val);

if (n > 0)
	device_init_wakeup(d, v);
```

if…else语句被12行的；结束了，因此写成do{ … } 的形式

取消宏定义

```c
#define X 3
... /* X is 3 */
#undef X
... /* X has no definition */
#define X 2
... /* X is 2 */
```



#### 2.2 内联函数

关键字：`inline`

该关键字告诉内核这个好书调用尽可能快

无编译优化：此时作为正常函数调用

```bash
80483a0:       e8 9f ff ff ff          call   8048344 <MAX>
```

编译优化后：

```bash
int max(int n)
{
 8048355:       55                      push   %ebp
 8048356:       89 e5                   mov    %esp,%ebp
 8048358:       53                      push   %ebx
 8048359:       83 ec 04                sub    $0x4,%esp
 804835c:       8b 5d 08                mov    0x8(%ebp),%ebx
        return n == 0 ? a[0] : MAX(a[n], max(n-1));
 804835f:       85 db                   test   %ebx,%ebx
 8048361:       75 07                   jne    804836a <max+0x15>
 8048363:       a1 a0 95 04 08          mov    0x80495a0,%eax
 8048368:       eb 18                   jmp    8048382 <max+0x2d>
 804836a:       8d 43 ff                lea    -0x1(%ebx),%eax
 804836d:       89 04 24                mov    %eax,(%esp)
 8048370:       e8 e0 ff ff ff          call   8048355 <max>
inline int MAX(int a, int b)
{
        return a > b ? a : b;
 8048375:       8b 14 9d a0 95 04 08    mov    0x80495a0(,%ebx,4),%edx
 804837c:       39 d0                   cmp    %edx,%eax
 804837e:       7d 02                   jge    8048382 <max+0x2d>
 8048380:       89 d0                   mov    %edx,%eax
int a[] = { 9, 3, 5, 2, 1, 0, 8, 7, 6, 4 };
```

可发现并没有调用MAX函数，而是直接内联在`max`函数中

#### 2.4 宏展开

```c
#define x 3
#define f(a) f(x * (a))
#undef x
#define x 2
#define g f
#define t(a) a

t(t(g)(0) + t)(1);
```

展开步骤：

1. 把g展开成分f，转换为`t(f(0) + t)(1);`
2. 根据2号定义展开，得到`t(f(x * (0)) + t)(1)`
3. 把x替换为2，得到



### 3.条件预处理

头文件处理

```c
#ifndef HEADER_FILENAME
#define HEADER_FILENAME
/* body of header */
#endif
```



## 第 23 章 指针

### 1.基础

间接寻址 + 数组索引（本质上是一个整数）

![指针的基本概念](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/pointer.pointer0.png)

```c
int main(int argc, char const *argv[])
{
    int i;
    // 内存单元的地址保存在另外一个内存单元
    int *pi = &i;
    char c;
    char *pc = &c;
    return 0;
}
```

i的地址是在编译链接时确定的，不需要到运行时才知道

`*`指针间接寻址运算符（Indirection Operator）,`*pi`表示取指针pi所指向的变量的值，也称为Dereference,变量的引用。

野指针：不确定地址的指针，防止野指针，在定义指针需要初始化指针为具体值

```c
int main(void)
{
  int *p = NULL;
  *p = 0;
}
```

NULL在C标准库的头文件`stddef.h`中定义：

```c
#define NULL ((void *)0)
```

把地址0转换为指针类型，OS不会把任何数据保存在地址0附近，`void *`指针与其他类型的指针可以隐式转换为通用指针，常用于函数接口。



### 2.指针类型的参数和返回值

```c
int *swap(int *px, int *py)
{
    int temp;
    temp = *px;
    *px = *py;
    *py = temp;
    return px;
}
```

此处将只是将px，py地址指向的值进行了交换，而没有交换i，j的值。

```c
int i = 10, j = 20;
int *p = swap(&i, &j);
```



### 3.指针与数组

数组明做右值时自动转换为指向首元素的指针。意思为`a[2]`等价于`*(a+2)`，所以产生了以下写法。

```c
int *pa = a;
```

此时pa指向a的首地址

#### 比较运算符

只是比较同一数组中元素中的指针才有意义，且比较指针间的地址。

例如，指针相减表示元素的个数，指针相加无意义，所以不允许相加.

> Rule of Least Surprise原则，你的功能不能让客户感到惊讶！

**注意：**

数组做左值时不代表元素首地址，只代表元素空间。

#### 函数传参

指针写法

```c
void func(int *a)
{
	...
}
```

数组写法

```c
void func(int a[])
{
	...
}
```

这两种写法对编译器看来没有区别，都代表指针，如果参数指向第一个元素通常写成指针形式，指向一串元素的首元素，则经常写成数组的形式。



### 4.指针与const限定符

> *号可单独看作一个变量，这样方便理解

常见写法：

```c
const int *a;
int const *a;
```

* 指向`const int`型，a指向内存单元不可改写，即`(*a)++`不允许，但是a可以改写

```c
int * const a;
```

* a是一个指向int型的const指针，此时const修饰a，*a可以改写

```c
int const * const a;
```

* a是一个指向const int型的const指针，*a和a都不可以改写

**注意:指向`const`的变量不可以指向非`const`变量**

这样做是为了防止所指向的内存单元被更改，反之则可以。

#### `const`的作用

1. 写代码时不必担心所指向的内存单元被修改
2. 把不该变化的尽可能都写成`const`
3. `const`对编译器优化很有帮助



### 5.指针与结构体

```c
struct unit {
    char c;
    int num;
};

struct unit u;
struct unit *p = &u;
```

定义结构体类型的指针和变量，访问时可以写成`(*p).c`、`(*p).num`，代表的意思为取p地址下的这个字段，也可以写成`p->c`、`p->num`，这样更加简洁。



### 6. 指向指针的指针与指针数组

指针数组：

```c
int *a[10];
```

这里每个元素都是`int *`类型的，他是等价于

```c
int **pa;
```

如果让pa指向a的首元素，则有：

```c
int * a[10];
int **pa = &a[0];
```

对比正常数组

```c
int *pa = a;
```

a[0]代表第一个指针变量，取地址表示一个指针符号指向指针变量的地址。

#### 深入main的参数

```c
int main(int argc, char const *argv[])
{
    /* code */
    return 0;
}
```

此处使用的argv表示一连串的指针变量等价于`**argv`，写成这种形式是为了表示指向一个数组的首元素，每个指针都为`char *`类型

![argv指针数组1](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/pointer.argv-20220907210027357.png)

由于内存模型为这，所以也可以这样写

```c
for(i=0; argv[i] != NULL; i++)
```



### 7.指向数组的指针与多维数组

指向数组的指针

```c
int (*a)[10];
```

相比于指向指针的指针，多了一个`()`，认为`[]`比`*`优先级更高，如果a先和`*`结合表示这是个指针，如果和`[]`则表示是一个数组。

因此可以拆成两个：

`int *a[10]`表示为：

```c
typedef int *t;
t a[10];
```

t代表`int *`类型的指针，a表示这种类型组成的数组。

`int (*a)[10]`则可以拆成：

```c
typedef int t[10];
t *a;
```

t代表由10个int组成的数组类型，a代表指向这种类型的数组，类似结构体。

#### 数组指针的使用

```c
int a[10];
int (*pa)[10] = &a[0];
```

a是一个数组，&a中，数组名为左值，取数组的首地址复制给指针pa，注意：&a表示取数组a的首地址，&a[0]表示取数组a第一个元素的首地址，这两个数值相同，&a表示指针int (*)[10]，&a[0]表示int *。



### 8.函数类型和函数指针类型

函数指针存放函数的入口地址

```c
#include <stdio.h>

void say_hello(const char *str){
    printf("Hello %s\n", str);
}

int main(int argc, char const *argv[])
{
    void (*f)(const char *) = say_hello;
    f("Guys");
    return 0;
}
```

f的类型声明：`void(*f)(const char *)`，*后面，所以f是一个指针。返回值，参数

函数类型与数组类型相似，做右值时自动转换为指针类型，这里做了一次类型转换。当然可以直接写成`void (*f)(const char *) = &say_hello`，直接取地址再赋值给f，就不需要自动转换类型了。

#### 函数调用的区别

* `f(“guays”)`：直接写法，要求操作数是函数指针
* `(*f)(“guys”)`：把函数类型转换为函数指针然后做的函数调用



#### 指针与直接函数调用区别

* 当函数中有多层if..else时，使用函数指针更好
* 封装多个函数，每个函数只做一件事，调用时函数指针取出相应的函数



### 9.不完全类型和复杂声明

![C语言类型总结](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/pointer.type.gif)

* 函数类型
* 对象类型
  * 标量类型
    * 指针类型
    * 数学类型
  * 非标量类型
* 不完全类型

> 取自Standard C

#### 不完全类型

> 不知道该种类型占几个字节

```c
struct s;
union u;
char str[];
```

不完全类型可以通过多次声明变为完全类型

```c
char str[];
char str[10];
```

#### 结构体

```c
struct s {
  struct t *pt;
}

struct t {
  struct s *ps;
}
```

由于编译器从前往后编译，看到s是一个不完全类型，t也是一个不完全类型，但pt是一个指针类型，指针的存储大小确定。接下俩，编译器看到t，此时t有了完整的定义，因为s定义完整，先是pt的类型有完整定义，然后再是ps有了完整定义。

结论：c语言中可以递归定义指针成员，但不能递归定义变量成员。

链表的由来：

```c
struct s {
  char data[6];
  struct s* next;
}
```

#### 分解函数声明

1. 先看指针
2. 逐步替换

```c
int (*(*fp) (void *)) [10];
```

1. *fp是一个函数指针，指向T1类型

```c
int (*T1 (void *))[10];
typedef T1 *fp;
```

2. T1为函数类型，参数为void，返回值为T2类型

```c
typedef int (*T2)[10];
typedef T2 T1(void *);
typedef T1 *fp;
```

3. T2和*组合在一起也为指针，指向T3的指针

```c
typedef int T3[10];
typedef T3 *T2;
typedef T2 T1(void *);
typedef T1 *fp;
```

显然，T3是一个int数组，由十个元素组成。

最后来看：

1. 最外层为一个int数组（T3）
2. 第一内层为一个函数指针（T2）
3. 最内层为一个函数类型（T1）



## 第24章 函数接口

### 1.本章预备

#### 1.1 `strcpy`与`strncpy`

**Man Page很重要**

strcpy的man page

![image-20220908122056565](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/image-20220908122056565.png)

SYNOPSIS(简介)：表示函数原型

**注意：**src为const，证明所在的内存空间不能被改写

strcpy只知道`src`的首地址，不知道长度，他会一直拷贝到`\0`，所以需要注意数组是否会越界，此外还不应该有重叠。

strncpy除了可以指定长度外，还可以实现缺省补`\0`

#### 1.2 malloc与free

分配一块全局内存空间，其他不可访问

malloc在底层通过`brk`系统调用向操作系统申请内存，函数返回通用`void *`，动态分配的内存用完之后可以用`free`释放掉吧。

##### 内存泄漏

内存分配后而不释放

malloc(0)也是合法的，但是不能通过这指针访问内存

##### malloc与free的实现



##### 例子：

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int number;
    char *msg;
} unit_t;

int main(int argc, char const *argv[])
{
    // 隐式转换
    unit_t *p = malloc(sizeof(unit_t));
    // 内存分配后需要判断
    if(p == NULL){
        printf("out of memory\n");
        exit(1);
    }
    p->number = 3;
    p->msg = malloc(20);
    strcpy(p->msg, "Hello world!");
    printf("number: %d\nmsg: %s\n", p->number, p->msg);
    // 注意顺序不能变，否则p就变成了野指针 msg就成了内存碎片
    free(p->msg);
    // 将p的空间归还，但是值并没有变化
    // free只能够清除空间
    free(p);
    p = NULL;

    return 0;
}
```



### 2.传入参数与传出参数

传入参数

```c
unit_t u; 
set_unit(&u);
```

传出参数

```c
void set_unit(unit_t *p)
```



### 3.两层指针的参数

```c
void get_a_day(const char **);
```

这里表示传出参数

传入参数表示：

```c
const char *firstday = NULL;
get_a_day(&firstday);
```

也可以在函数中分配内存，函数返回指向该内存的指针，所以一般构造函数和析构造函数

#### 调用者：

1. 分配空间
2. 调用函数分配内存
3. 调用函数释放内存

#### 实现者：

1. 规定指针类型
2. 分配内存并完成初始化
3. 释放内存

#### 为什么不在`main`函数中释放内存？



### 4.返回值为指针

* 传出静态内存或者已分配的动态内存的指针
* 在函数中动态分配内存并传出指向这块内存的指针

嵌套指针时，需要多层分配内存，返回时只返回最高层，同时释放也一样。



### 5. 回调函数

参数为函数指针，调用者传递一个函数的地址给实现者，让实现者调用它。

```c
// 定义函数指针
typedef void (*callback_t)(void *);
// 只定义了参数类型
extern void repeat_three_times(callback_t, void *);
```

* 注意这里的函数返回值为void，参数为万能指针（参数类型由实现者去规定）

```c
void repeat_three_times(callback_t f, void *para)
{
    f(para);
    f(para);
    f(para);
}
```

回调函数典型应用为：C++范型

调用者一般会间接调用自己的函数：

1. 调用者将回调函数传给实现者
2. 实现者记住这个函数，**注册**回调函数
3. 然后某个事件发生时，调用注册过的函数

#### 常用框架

```c
/* registry.h */
#ifndef REGISTRY_H
#define REGISTRY_H

typedef void (*registry_t)(void);
extern void register_func(registry_t);

#endif
```

```c
/* registry.c */
#include <unistd.h>
#include "registry.h"

static registry_t func;

void register_func(registry_t f)
{
     func = f;
}

static void on_some_event(void)
{
     ...
     func();
     ...
}
```



### 6.可变参数

```c
int printf(const char *format, ...);
```

C到标准库的`va_list`类型和`va_start`、`va_arg`、`va_end`宏

#### LEA指令

> load effective address（把某个内存地址放在目标位置）

```assembly
lea dest, src
```

例子：

```assembly
 my_printf("c\ts\n","1","hello");
    1355:       48 8d 15 a8 0c 00 00    lea    0xca8(%rip),%rdx        # 2004 <_IO_stdin_used+0x4>
    135c:       48 8d 35 a7 0c 00 00    lea    0xca7(%rip),%rsi        # 200a <_IO_stdin_used+0xa>
    1363:       48 8d 3d a2 0c 00 00    lea    0xca2(%rip),%rdi        # 200c <_IO_stdin_used+0xc>
    136a:       b8 00 00 00 00          mov    $0x0,%eax
    136f:       e8 15 fe ff ff          callq  1189 <my_printf>
```

从右向左依次压入栈，第一个参数靠近栈顶，第三个靠近栈底。

![myprintf函数的参数布局](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/interface.vaarg.png)

但为了使第三个参数对齐到4字节边界，所以第二个参数也占4个字节。



#### 实现原理

`stdarg.h`的一种实现

```c
/* stdarg.h standard header */
#ifndef _STDARG
#define _STDARG

/* type definitions */
typedef char *va_list;
/* macros */
#define va_arg(ap, T) \
	(* (T *)(((ap) += _Bnd(T, 3U)) - _Bnd(T, 3U)))
#define va_end(ap) (void)0
#define va_start(ap, A) \
	(void)((ap) = (char *)&(A) + _Bnd(A, 3U))
#define _Bnd(X, bnd) (sizeof (X) + (bnd) & ~(bnd))
#endif
```

* `Bnd`宏：将变量X的长度对齐到bnd+1字节的整数倍
* `va_list`：指针变量
* `va_start(ap, format)`宏：让ap指向format参数的下一个参数
* `va_arg(ap, int)`宏：把第二个参数的值按int取出来，使ap指向第三个参数



#### `printf`原理

* 根据第一个参数来确定后面有几个参数以及类型
* 调用者需要保证个数确定

另一种方法：

**根据Sengtinel判断参数的个数**

例如，末尾变量传入一个NULL，从栈上取数据到此参数时，默认结束。

最后，需要注意，C语言标准规定需要定义一个有名字的参数，`va_start`中要用参数列表最后一个有名字的参数。



## 第25章 C标准库

Man Page 永远的神！

### 1. 字符串操作函数

库文件：

```c
#include <string.h>
```

#### 1.1初始化字符串

```c
void *memset(void *s, int c, size_t n);
返回值：s指向哪，返回的指针就指向哪
```

* 全局和静态变量，自动初始化为0
* 函数的局部变量，初始值不确定，用该函数初始化，而且`malloc`初始化的值也是不确定的

#### 1.2 取字符串的长度

```c
size_t strlen(const char *s)
```

返回的长度不包括`\0`，实现原理是一直寻找`\0`

#### 1.3 拷贝字符串

* strcpy、strncpy拷贝以`\0`结束的字符串
* memcpy、memmove函数

```c
void *memcpy(void *dest, const void *src, size_t n);
void *memmove(void *dest, const void *src, size_t n);
返回值：dest指向哪，返回的指针就指向哪
```

mem命名的函数并不关心`\0`，**说明并不把参数当作字符**，因此返回值为`void *`

区别为，move为memcpy的升级版，内存重叠时也可以拷贝正确。

#### 1.4 连接字符串

```c
char *strcat(char *dest, const char *src);
char *strncat(char *dest, const char *src, size_t n);
返回值：dest指向哪，返回的指针就指向哪
```

#### 1.5 比较字符串

```c
#include <string.h>

int memcmp(const void *s1, const void *s2, size_t n);
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);
返回值：负值表示s1小于s2，0表示s1等于s2，正值表示s1大于s2
```

* memcmp：比较前n个字节
* strcpy：当其中一个`\0`时结束比较
* Strlcpy：有两个选择，要么在其中一个字符遇到`\0`结束，要么比较晚n个字符

#### 1.6 搜索字符串

```c
#include <string.h>

char *strchr(const char *s, int c);
char *strrchr(const char *s, int c);
返回值：如果找到字符c，返回字符串s中指向字符c的指针，如果找不到就返回NULL
```

`strchr`在字符串`s`中从前到后查找字符`c`，找到字符`c`第一次出现的位置时就返回，返回值指向这个位置，如果找不到字符`c`就返回`NULL`。`strrchr`和`strchr`类似，但是从右向左找字符`c`，找到字符`c`第一次出现的位置就返回，函数名中间多了一个字母r可以理解为Right-to-left。

```c
#include <string.h>

char *strstr(const char *haystack, const char *needle);
返回值：如果找到子串，返回值指向子串的开头，如果找不到就返回NULL
```

`strstr`在一个长字符串中从前到后找一个子串（Substring），找到子串第一次出现的位置就返回，返回值指向子串的开头，如果找不到就返回NULL。这两个参数名很形象，在干草堆`haystack`中找一根针`needle`，按中文的说法叫大海捞针，显然`haystack`是长字符串，`needle`是要找的子串。

### 2. 标准I/O库函数

调用者只是将文件指针在哭函数接口之间传来传去，FILE结构体的成员在库函数内部维护，有体会一下封装。

```java
public     private 
```

#### 2.2 fopen/fclose

fopen错误时，返回NULL，并设置errno，通常这样写

```c
if ( (fp = fopen("/tmp/file1","r")) == NULL){
  printf("error open file /tmp/file1!\n");
  exit(1);
}
```

fclose调用出错时，返回EOF

```c
/* End of file character.
   Some things throughout the library rely on this being -1.  */
#ifndef EOF
# define EOF (-1)
#endif
```

### 2.3 stdin/stdout/stderr





#### 2.10 C标准库的I/O缓冲区

* C标准库为每个打开的文件分配一个I/O缓冲区，通过文件FILE结构体可以找到缓冲区
* 用户读写函数多数都在缓冲区，少部分请求内核
* 用户每次第一调用读写函数会进入内核，然后把数据放到缓冲区，用户在缓冲区进行读写

##### C标准库缓冲区与用户缓冲区

![C标准库的I/O缓冲区](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/stdlib.buffer.png)



* 全缓冲：写满后就返回内核
* 行缓冲：有换行就写回内核，或者写满，标准输入和输出通常为行缓冲
* 无缓冲：尽快通过系统调用写入内核

##### 为什么没有输出？

`printf("hello world");`打印的字符串中没有换行符，所以只把字符串写到标准输出的I/O缓冲区中而没有写回内核（写到终端设备），如果敲Ctrl-C，进程是异常终止的，并没有调用`exit`，也就没有机会Flush I/O缓冲区，因此字符串最终没有打印到屏幕上。

去掉while(1);也可以打印，程序退出时会调用exit Flush所有I/O缓冲区

```c
#include <stdio.h>

int main()
{
	printf("hello world");
	fflush(stdout);
	while(1);
}
```

* 用户调用`fflush`强制写回内核

* 该函数确保数据写回了内核，以免进程异常终止丢失数据

  


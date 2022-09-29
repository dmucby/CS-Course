# Linux C 一站式编程

## 28. 文件与I/O

### 1.汇编程序的Hello world

> C标准库函数通过系统调用把I/O操作从用户传给内核

```assembly
.data					# section declaration

msg:
	.ascii	"Hello, world!\n"	# our dear string
	len = . - msg			# length of our dear string

.text					# section declaration

			# we must export the entry point to the ELF linker or
    .global _start	# loader. They conventionally recognize _start as their
			# entry point. Use ld -e foo to override the default.

_start:

# write our string to stdout

	movl	$len,%edx	# third argument: message length
	movl	$msg,%ecx	# second argument: pointer to message to write
	movl	$1,%ebx		# first argument: file handle (stdout)
	movl	$4,%eax		# system call number (sys_write)
	int	$0x80		# call kernel

# and exit

	movl	$0,%ebx		# first argument: exit code
	movl	$1,%eax		# system call number (sys_exit)
	int	$0x80		# call kernel
```

* .data段
  * msg相当于一个全局变量，代表字符串的首地址
  * len表示当前地址-msg的地址：即表示字符串的长度
  * `.`的含义：汇编器维护地址计数器，按顺序加计数器，使用`.`表示取出当前地址计数器的值
* _statrt
  * write系统调用：系统调用号4由eax寄存器保存，其余寄存器保存系统调用参数。（如何用宏定义写？？？）
    * ebx保存文件描述符，1表示标准输出，相当于C语言的stdout
    * edx保存着输出的字节数
    * ecx保存输出的缓冲区首地址
  * _exit系统调用

#### 初识宏写法

`sys_open()`

```c
SYSCALL_DEFINE3(open, const char __user *, filename, int, flags, umode_t, mode)
```

函数声明：

```c
asmlinkage long sys_open(const char __user *filename, int flags, umode_t mode);
```

定义

```c
#define SYSCALL_DEFINE3(name, ...)              \
    SYSCALL_DEFINEx(3, _##name, __VA_ARGS__)
```

* ##：分隔连接方式，强制链接

```c
#define VAR(type, name) type name##_##type
VAR(int, var1);
展开之后就是：
int var1_int;
```

1. 首先根据`SYSCALL_DEFINEx(3, _open, __VA_ARGS__)`展开
2. 然后根据

```c
#define SYSCALL_DEFINEx(x, sname, ...)              \
    __SYSCALL_DEFINEx(x, sname, __VA_ARGS__)
```

展开为：

```c
__SYSCALL_DEFINEx(3, _open, __VA_ARGS__)
```

3. 根据宏定义

```c
#define __SYSCALL_DEFINEx(x, name, ...)                 \
    asmlinkage long sys##name(__SC_DECL##x(__VA_ARGS__))
```

展开：

```c
asmlinkage long sys_name(__SC_DECL3(__VA_ARGS__))
```

其中，这里的`__VA_AGRS__`为：

```c
const char __user *, filename, int, flags, umode_t, mode
```

而且`__SC_DECL3`为宏定义：

```c
#define __SC_DECL1(t1, a1)  t1 a1
#define __SC_DECL2(t2, a2, ...) t2 a2, __SC_DECL1(__VA_ARGS__)
#define __SC_DECL3(t3, a3, ...) t3 a3, __SC_DECL2(__VA_ARGS__)
```

最终展开为：

```c
SYSCALL_DEFINE3(open, const char __user *, filename, int, flags, umode_t, mode)
宏定义展开之后就成为：
asmlinkage long sys_open(const char __user *filename, int flags, umode_t mode);
```



### 2. C标准I/O库函数与Unbuffered I/O函数

* fopen：返回一个文件描述符（编号）和一个FILE结构体
* fgetc：通过FILE*找到文件，判断能否从I/O缓冲区读下一个字符

> Open、read、write、close被称作无缓冲I/O

#### 区别：

* 无缓冲I/O每次读写都要进入内核
* C标准函数需要注意内核和缓冲区可能不一致，需要及时fflush
* 需要注意的是对网络设备和终端的读写一般需要第一时间通知内核

![库函数与系统调用的层次关系](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/io.syscall-20220912235539687.png)



#### UNIX标准

> POSIX（Portable Operating System Interface）



#### 文件描述符

task_struct进程描述符，称为进程控制快（PCB），而在其中有一个指针指向`file_struct`结构，称为文件描述符表，其中每个表包含一个指向已经打开的文件的指针。

![文件描述符表](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/io.fd.png)

用户不能直接访问文件描述符表，而只能使用描述符的索引，这些索引就是文件描述符，（0，1，2这些数字），用int类型变量保存。

用户访问时，内核通过文件描述符找到相应的表项，再通过表中的指针找到相应的文件。

程序启动时会自动打开三个文件：

* 标准输入
* 标准输出
* 标准错误输出

在C语言中分别用：

* stdin
* stdout
* stderr

所对应的文件描述符为：0，1，2，保存在相应的FILE结构体中。

在头文件`unistd.h`中有如下定义

```c
#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2
```

### 3.Open/close

```c
int open(const char *pathname, int flags, ...);
```

flags参数有一些列常数值可供选择，可以同时选择多个常数用按位或运算符连接起来，这些宏都以o_开头，表示or：

必须选项

* O_RDONLY：只读
* O_WRONLY：只写
* O_RDWR：可读写

可选选项：

* 追加
* 不存在则创建

#### open与fopen区别

* fopen会自动创建文件（如果文件不存在）



#### close

```c
#include <unistd.h>

int close(int fd);
返回值：成功返回0，出错返回-1并设置errno
```

* 程序终止时，内核会自动关闭它打开的所有文件



#### O_CREAT

调用open创建文件时，不能追加读写权限

```c
int main(int argc, char const *argv[])
{
    // 创建文件需要追加第三个参数，表示访问权限
    // 注意不能使用读写权限
    int i = open("test1.txt",O_CREAT,0644);
    printf("%d\n",i);
    return 0;
}
```

### 4. Read/write

#### read函数

`read`可以从设备和文件中读取数据

```c
#include <unistd.h>

ssize_t read(int fd, void *buf, size_t count);
返回值：成功返回读取的字节数，出错返回-1并设置errno，如果在调read之前已到达文件末尾，则这次read返回0
```

> * Count：请求读取的字节数
> * 数据保存在缓冲区buf中
>
> 这个对写的位置标记在内核中，而C标准I/O库时的读写位置是用户空间的I/O缓存区。

* 返回值：ssize_t
  * 可以返回正的字节数
  * 可以返回-1、0

读取字节数返回值小于count参数分类：

* 读到末尾时
* 设别终端，换行
* 网络：传输协议和缓存机制

#### write函数

write向打开的设备或文件中写数据

```c
#include <unistd.h>

ssize_t write(int fd, const void *buf, size_t count);
返回值：成功返回写入的字节数，出错返回-1并设置errno
```



#### 阻塞

> Block

当进程调用一个阻塞的系统函数时，进程会处于睡眠状态。

进程运行状态：

* 正在被调度执行。CPU、eip、寄存器计算
* 就绪状态：随时可执行，等待被调度

```bash
./a.out 
hello
read STDIN_FILENO: Success
 ./a.out
hello world
hello # d
d: command not found
```



阻塞分析：

1. Shell创建a.out进程，a.out开始执行，而shell进程水面等待a.out进程退出
2. a.out调用read水面等待，直到换行符，从read返回，read只读取10个字符，其余字符保存在内核终端的缓存区
3. a.out进程打印并退出，shell恢复运行，shell继续从终端读取用户输入命令，于是读走了终端设备输入缓冲区剩下的字符d和换行符，把它当成了一条命令。



#### 非阻塞I/O

第一次数据没有达到，数据发生阻塞，然而实际并没有阻塞而是直接返回错误，内部调用者应再读一次。（轮询模式 Poll）

缺点：所有设备都一直没有数据到达，轮询做无用功，此时就浪费了不少资源，因此诞生了间隔轮询（sleep）

其实还会有缺点，即数据得不到及时处理，而且还会进行反复轮询。（select 解决方案）

##### 非阻塞例子：

```c
#define MSG_TRY "try again\n"
fd = open("/dev/tty", O_RDONLY|O_NONBLOCK);
n = read(fd, buf, 10);
if (n < 0) {
		if (errno == EAGAIN) {
			// 产生间隔
      sleep(1);
			write(STDOUT_FILENO, MSG_TRY, strlen(MSG_TRY));
      // 轮询
			goto tryagain;
		}	
		perror("read /dev/tty");
		exit(1);
	}
	write(STDOUT_FILENO, buf, n);
	close(fd);
```

##### 符阻塞等待超时退出

利用`i`记录等待次数

```c
for(i=0; i<5; i++) {
		n = read(fd, buf, 10);
		if(n>=0)
			break;
		if(errno!=EAGAIN) {
			perror("read /dev/tty");
			exit(1);
		}
		sleep(1);
		write(STDOUT_FILENO, MSG_TRY, strlen(MSG_TRY));
	}
	if(i==5)
		write(STDOUT_FILENO, MSG_TIMEOUT, strlen(MSG_TIMEOUT));
	else
		write(STDOUT_FILENO, buf, n);
```

### 5.lseek

与C标准`fseek`类似，可以增加偏移量

```c
#include <sys/types.h>
#include <unistd.h>

off_t lseek(int fd, off_t offset, int whence);

off_t currpos;
currpos = lseek(fd, 0, SEEK_CUR);
```

### 6.fcntl

改变已打开的文件的属性，可以重新设置标志(flags)，不用重新open。

```c
#include <unistd.h>
#include <fcntl.h>

int fcntl(int fd, int cmd);
int fcntl(int fd, int cmd, long arg);
int fcntl(int fd, int cmd, struct flock *lock);
```

函数后参数的个数由`cmd`决定



#### SHELL重定向

```bash
$ ./a.out 0 < /dev/tty
read only
```

执行a.out时将标准输入重定向到`/dev/tty`，并且只读，其中`argv[1]`是0。

**注意：**

* shell的重定向语法不属于成的命令行参数
* 此命令行只有两个参数 `argv[0]`和`argv[1]`
* 重定向由shell结束，在启动程序时就已经生效

在重定向<、>、>>、<>前加一个数字，该数字就表示在哪个文件描述符打开文件。

例如`2>>temp.foo`表示将标准错误输出重定向到文件`temp.foo`，并且以追加的方式写入。

```bash
$ command > /dev/null 2>&1
```

将标准输出和错误(2代表标准错误，1代表标准输出)都重定向到`/dev/null`，文件描述符写在重定向符号右边需要加`&`号，否则会被解释为文件名。

### 7.ioctl

向设备发送控制和配置命令，有一些数据不能用read/write读写，`out-of-band`数据。

ioctl命令传送的是控制信息，数据为辅助数据，串口收发数据。

### 8.mmap

把磁盘文件的一部符直接映射到内存中，文件中的位置就有了对应的内存地址，对文件读写直接用指针，而不用read/write。

```c
#include <sys/mman.h>

 void *mmap(void *addr, size_t len, int prot, int flag, int filedes, off_t off);
 int munmap(void *addr, size_t len);
```

![mmap函数](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/io.mmap-20220914095512752.png)

* addr：如果为空，内核会在进程地址空间中选择合适的地址建立映射，不为NULL，内核会给出提示
* len：需要映射的文件的长度
* off：是文件从什么位置开始映射，必须是页的大小
* filedes：文件描述符

port取值：PORT_

* EXEC：可执行
* READ：可读
* WRITE：可写
* NONE：不可访问

flag参数取值：

* MAP_SHARED：多个进程对同一个文件的映射共享，一个做出变化，都可以看到
* MAP_PRIVATE：不共享，一个进程修改，其他不会看到，也不会写到真实文件中

```bash
strace ./a.out
execve("./a.out", ["./a.out"], 0x7ffee670ed10 /* 32 vars */) = 0
brk(NULL)                               = 0x558de6264000
arch_prctl(0x3001 /* ARCH_??? */, 0x7ffdf8ac88c0) = -1 EINVAL (Invalid argument)
access("/etc/ld.so.preload", R_OK)      = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
fstat(3, {st_mode=S_IFREG|0644, st_size=48044, ...}) = 0
mmap(NULL, 48044, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7fc090d1a000
close(3)                                = 0
# 共享库的地址
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\300A\2\0\0\0\0\0"..., 832) = 832
pread64(3, "\6\0\0\0\4\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0"..., 784, 64) = 784
pread64(3, "\4\0\0\0\20\0\0\0\5\0\0\0GNU\0\2\0\0\300\4\0\0\0\3\0\0\0\0\0\0\0", 32, 848) = 32
pread64(3, "\4\0\0\0\24\0\0\0\3\0\0\0GNU\0\30x\346\264ur\f|Q\226\236i\253-'o"..., 68, 880) = 68
fstat(3, {st_mode=S_IFREG|0755, st_size=2029592, ...}) = 0
mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7fc090d18000
pread64(3, "\6\0\0\0\4\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0"..., 784, 64) = 784
pread64(3, "\4\0\0\0\20\0\0\0\5\0\0\0GNU\0\2\0\0\300\4\0\0\0\3\0\0\0\0\0\0\0", 32, 848) = 32
pread64(3, "\4\0\0\0\24\0\0\0\3\0\0\0GNU\0\30x\346\264ur\f|Q\226\236i\253-'o"..., 68, 880) = 68
mmap(NULL, 2037344, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7fc090b26000
mmap(0x7fc090b48000, 1540096, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x22000) = 0x7fc090b48000
mmap(0x7fc090cc0000, 319488, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x19a000) = 0x7fc090cc0000
mmap(0x7fc090d0e000, 24576, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1e7000) = 0x7fc090d0e000
mmap(0x7fc090d14000, 13920, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7fc090d14000
close(3)                                = 0
arch_prctl(ARCH_SET_FS, 0x7fc090d19540) = 0
mprotect(0x7fc090d0e000, 16384, PROT_READ) = 0
mprotect(0x558de49fd000, 4096, PROT_READ) = 0
mprotect(0x7fc090d53000, 4096, PROT_READ) = 0
munmap(0x7fc090d1a000, 48044)           = 0
fstat(1, {st_mode=S_IFCHR|0620, st_rdev=makedev(0x88, 0), ...}) = 0
brk(NULL)                               = 0x558de6264000
brk(0x558de6285000)                     = 0x558de6285000
# 底层最后为write
write(1, "hello world\n", 12hello world
)           = 12
exit_group(0)                           = ?
+++ exited with 0 +++
```

#### 修改样例：

```bash
$ od -tx1 -tc hello 
0000000 68 65 6c 6c 6f 0a
          h   e   l   l   o  \n
0000006
```

程序：

```c
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>

int main(void)
{
	int *p;
	int fd = open("hello", O_RDWR);
	if (fd < 0) {
		perror("open hello");
		exit(1);
	}
	p = mmap(NULL, 6, PROT_WRITE, MAP_SHARED, fd, 0);
	if (p == MAP_FAILED) {
		perror("mmap");
		exit(1);
	}
	close(fd);
	p[0] = 0x30313233;
  // 修改了映射地址
  // p[1] = 0x34353637;
	munmap(p, 6);
	return 0;
}
```

修改后的内容：

```bash
$ od -tx1 -tc hello
 0000000 33 32 31 30 6f 0a
           3   2   1   0   o  \n
 0000006
```

本来地址为，修改为映射的地址，在此处指修改了前四个字节，表示int型为每位为4个字节，如果修改后四位需要加上：

```c
  // p[1] = 0x34353637;
```

且修改映射后，内容也会发生变化。

```bash
od -tx1 -tc hello 
0000000  33  32  31  30  37  36  35  34
          3   2   1   0   7   6   5   4
0000010
```



## 第29章 文件系统

### 2. ext2 文件系统

#### 2.1 总体存储布局

分区存储文件需要先格式化，格式化过程在磁盘上写一些管理存储布局的信息。

![ext2文件系统的总体存储布局](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/fs.ext2layout-20220914171529611.png)

最小单位为块（Block），在格式化时确定。

Boot Block（启动块）由PC标准确定，用来存储磁盘分区信息和启动信息。

每个块组由以下部分组成：

* 超级块：整个块组的信息
* 块组描述符(GDT, Group Descriptor Table)
  * 位置信息
  * 在每个块组的开头有一份拷贝
* 块位图（Block Bitmap）：
  * 描述整个块组中空闲，每个bit代表一个块，1表示占用
  * df命令统计整个磁盘的已用空间
    * 本质查看每个块位图，不用查看每个的分区
    * 格式化时多少个块组？
* inode位图(inode Table)
  * 与块位图类似
* inode表
  * 文件描述信息（文件类型、权限，文件大小，时间）
* 数据块：
  * 常规文件，数据存储在数据块中
  * 对于目录，目录的所有文件名和目录名存储在数据块中，目录也是一种特殊文件
  * 符号链接：路径名较长保存在数据块，较短直接保存在inode中
  * 设备文件、FIFO和socket保存在inode中

#### 实验1

```bash
ls -l /dev | grep zero
crw-rw-rw- 1 root root       1,   5 Mar 17  2022 zero
```

* 文件类型为：c，表示设备文件
* 文件大小：1，5 表示主设备号和次设备号

#### 实验2

```bash
lrwxrwxrwx 1 root root  7 Sep 17 17:09 halo -> ./hello
-rw-r--r-- 1 root root  0 Sep 17 17:08 hello
```

* hello：字节0
* 符号链接文件halo：字节7
  * `./hello`有7个字节，保存着一个路径名
* 第一个数字表示：硬链接数，有几个名字，保存在inode中
* 目录的硬链接`.` `..`也算

#### 实验3

格式化分区研究

首先，创建一个1MB的文件并清零：

```bash
$ dd if=/dev/zero of=fs count=256 bs=4K
```

* cp：拷贝文件
* dd：把文件的一部分拷贝成另外一个文件
* 把`/dev/zero`文件开头的1M(256*4K)字节拷贝成文件名为fs的文件

* if：输入文件
* of：输出文件
* count：拷贝多少次
* bs：拷贝多少字节

然后对fs进行格式化，把这个文件的数据块合起来看成一个1MB磁盘的分区，然后在这个分区上再划分出块组。

```bash
mke2fs fs
```

查看fs块组描述信息：

```bash
dumpe2fs fs
dumpe2fs 1.45.5 (07-Jan-2020)
Filesystem volume name:   <none>
Last mounted on:          <not available>
Filesystem UUID:          368ab1f4-28b5-483e-ad8b-4c139299d627
Filesystem magic number:  0xEF53
Filesystem revision #:    1 (dynamic)
Filesystem features:      ext_attr resize_inode dir_index filetype sparse_super large_file
Filesystem flags:         signed_directory_hash 
Default mount options:    user_xattr acl
Filesystem state:         clean
Errors behavior:          Continue
Filesystem OS type:       Linux
Inode count:              128
Block count:              256
Reserved block count:     12
Free blocks:              242
Free inodes:              117
First block:              0
Block size:               4096
Fragment size:            4096
Blocks per group:         32768
Fragments per group:      32768
Inodes per group:         128
Inode blocks per group:   4
Filesystem created:       Sat Sep 17 17:44:44 2022
Last mount time:          n/a
Last write time:          Sat Sep 17 17:44:44 2022
Mount count:              0
Maximum mount count:      -1
Last checked:             Sat Sep 17 17:44:44 2022
Check interval:           0 (<none>)
Reserved blocks uid:      0 (user root)
Reserved blocks gid:      0 (group root)
First inode:              11
Inode size:               128
Default directory hash:   half_md4
Directory Hash Seed:      a4b13e25-5a02-46cd-ab2c-b13e06a9c9c1


Group 0: (Blocks 0-255)
  Primary superblock at 0, Group descriptors at 1-1
  Block bitmap at 2 (+2)
  Inode bitmap at 3 (+3)
  Inode table at 4-7 (+4)
  242 free blocks, 117 free inodes, 2 directories
  Free blocks: 14-255
  Free inodes: 12-128
```

* Group0占到了0～255块
* 为何第一组直接占了256块？？

```bash
root@iZwz95y0pmhiy0p2rgjs8eZ:~# sudo mount -o loop fs /mnt
root@iZwz95y0pmhiy0p2rgjs8eZ:~# cd /mnt/
root@iZwz95y0pmhiy0p2rgjs8eZ:/mnt# ls -la
total 24
drwxr-xr-x  3 root root  4096 Sep 17 17:44 .
drwxr-xr-x 18 root root  4096 Jul  4 15:19 ..
drwx------  2 root root 16384 Sep 17 17:44 lost+found
```

常规文件挂载到目录下：

* -o loop：表示是一个常规文件
* Mount：会把数据块中的数据当作分区格式来解释
* 生成了三个目录：lost+found目录由e2fsck工具使用，类似“失物招领”

添加删除文件，会自动保存在fs中，取消挂载

```bash
$ sudo umount /mnt
```

二进制工具查看文件系统的所有字节

```bash
od -tx1 -Ax fs
000000 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
# 开始的1KB为启动块 内容全部为0
*
000400 80 00 00 00 00 01 00 00 0c 00 00 00 f2 00 00 00
000410 75 00 00 00 00 00 00 00 02 00 00 00 02 00 00 00
000420 00 80 00 00 00 80 00 00 80 00 00 00 53 9b 25 63
000430 a7 9c 25 63 01 00 ff ff 53 ef 01 00 01 00 00 00
000440 0c 97 25 63 00 00 00 00 00 00 00 00 01 00 00 00
000450 00 00 00 00 0b 00 00 00 80 00 00 00 38 00 00 00
000460 02 00 00 00 03 00 00 00 36 8a b1 f4 28 b5 48 3e
000470 ad 8b 4c 13 92 99 d6 27 00 00 00 00 00 00 00 00
000480 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
# 超级块
*
# 块组描述符 文件较小，只有一个块组描述符
0004e0 00 00 00 00 00 00 00 00 00 00 00 00 a4 b1 3e 25
0004f0 5a 02 46 cd ab 2c b1 3e 06 a9 c9 c1 01 00 00 00
000500 0c 00 00 00 00 00 00 00 0c 97 25 63 00 00 00 00
000510 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
*
000560 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
000570 00 00 00 00 00 00 00 00 08 00 00 00 00 00 00 00
000580 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
*
001000 02 00 00 00 03 00 00 00 04 00 00 00 f2 00 75 00
001010 02 00 04 00 00 00 00 00 00 00 00 00 00 00 00 00
001020 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
*
002000 ff 3f 00 00 00 00 00 00 00 00 00 00 00 00 00 00
002010 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
002020 ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff
*
003000 ff 07 00 00 00 00 00 00 00 00 00 00 00 00 00 00
003010 ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff
*
004000 00 00 00 00 00 00 00 00 0c 97 25 63 0c 97 25 63
004010 0c 97 25 63 00 00 00 00 00 00 00 00 00 00 00 00
004020 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
*
004080 ed 41 00 00 00 10 00 00 5a 9b 25 63 0c 97 25 63
004090 0c 97 25 63 00 00 00 00 00 00 03 00 08 00 00 00
0040a0 00 00 00 00 00 00 00 00 08 00 00 00 00 00 00 00
0040b0 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
*
004300 80 81 00 00 00 c0 40 00 0c 97 25 63 0c 97 25 63
004310 0c 97 25 63 00 00 00 00 00 00 01 00 08 00 00 00
004320 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
*
004350 00 00 00 00 00 00 00 00 00 00 00 00 0d 00 00 00
004360 00 00 00 00 00 00 00 00 00 00 00 00 01 00 00 00
004370 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
*
004500 c0 41 00 00 00 40 00 00 0c 97 25 63 0c 97 25 63
004510 0c 97 25 63 00 00 00 00 00 00 02 00 20 00 00 00
004520 00 00 00 00 00 00 00 00 09 00 00 00 0a 00 00 00
004530 0b 00 00 00 0c 00 00 00 00 00 00 00 00 00 00 00
004540 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
*

008000 02 00 00 00 0c 00 01 02 2e 00 00 00 02 00 00 00
008010 0c 00 02 02 2e 2e 00 00 0b 00 00 00 e8 0f 0a 02
008020 6c 6f 73 74 2b 66 6f 75 6e 64 00 00 00 00 00 00
008030 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
*
009000 0b 00 00 00 0c 00 01 02 2e 00 00 00 02 00 00 00
009010 f4 0f 02 02 2e 2e 00 00 00 00 00 00 00 00 00 00
009020 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
*
00a000 00 00 00 00 00 10 00 00 00 00 00 00 00 00 00 00
00a010 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
*
00b000 00 00 00 00 00 10 00 00 00 00 00 00 00 00 00 00
00b010 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
*
00c000 00 00 00 00 00 10 00 00 00 00 00 00 00 00 00 00
00c010 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
*
100000
```

debugfs工具

```bash
$ debugfs fs
stat/
Inode: 2   Type: directory    Mode:  0755   Flags: 0x0
Generation: 0    Version: 0x00000000
User:     0   Group:     0   Size: 4096
File ACL: 0
Links: 3   Blockcount: 8
Fragment:  Address: 0    Number: 0    Size: 0
ctime: 0x6325970c -- Sat Sep 17 17:44:44 2022
atime: 0x63259b5a -- Sat Sep 17 18:03:06 2022
mtime: 0x6325970c -- Sat Sep 17 17:44:44 2022
BLOCKS:
(0):8
TOTAL: 1
```

`stat/`展示根目录inode的信息

对应信息：

![根目录的inode](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/fs.rootinode-20220917182105218.png)

* Links为3表示三个硬链接
* Blockcount：以512字节来算 4096/512=8，物理块数量

文件类型编码：

![image-20220917182504334](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/image-20220917182504334.png)

#### 2.3 数据块寻址

![数据块的寻址](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/fs.datablockaddr.png)

1. 读索引(inode)
2. 读数据块（此处可能有间接寻址块）

#### 2.4 文件和目录操作的系统函数

**stat函数：**

1. 读取文件的inode
2. 把inode中的各种属性填入结构体`struct stat`结构体中

**chomd函数：**

改变文件的访问权限

1. 更改inode中的st_mode字段

**link函数**

创建硬链接

在数据块中添加一条新记录，inode和原文件相同

**rename函数**

更改数据块中文件名的记录，如果原文件和新文件名不在一个目录下，需要从原目录数据块中清除一条数据，再添加一条新目录的数据。mv命令是基于rename函数实现的，因此移动文件并不需要复制和删除文件，只需要改名。

但是如果不同分区之间移动文件，就必须复制和删除inode和数据块。

mkdir函数：在符目录数据块中添加记录，分配新的inode和数据块

Rmdir函数：需要保证目录为空才能删除，释放它的inode和数据块，清除父目录的数据，符目录的硬链接数量-1.

opendir函数：遍历数据块中的记录，返回一个DIR的指针来代表着目录，类似FILE*



## 第30章 进程

进程控制块掌握每一个进程的信息：

* 进程id
* 进程运行状态
* 进程切换要保存的CPU寄存器
* 虚拟地址的空间信息
* 描述控制端的信息
* 当前工作目录
* 文件描述符
* 信号相关的
* 用户id和组id
* 控制终端、Session、进程组
* 资源上限

进程从一个个复制出来的，Shell下输入命令可以运行程序，因为Shell进程在读取用户输入命令之后会调用fork复制出一个新的Shell进程，然后新的Shell调用exec执行新的程序。

一个程序可以多次加载到内存，成为多个进程，一个进程在调用exec前后也可以分别执行两个不同的程序。

例如ls执行过程：

![fork/exec](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/process.forkexec.png)

1. fork创建子进程
2. 父进程仍然执行/bin/bash程序
3. 子进程调用exec执行新的程序/bin/ls

### 2.环境变量

exec系统调用执行新程序时会把命令行参数和环境变量表传递给`main`函数，进程的地址空间：

![进程地址空间](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/process.addressspace.png)

和命令行参数类似argv，环境变量表也是一组字符串：

![环境变量](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/process.environ.png)

打印环境变量：

```c
#include <stdio.h>

int main(void)
{
    // 获得外部变量
    extern char **environ;
    for (size_t i = 0; environ[i]!=NULL ; i++)
    {
        printf("%s\n", environ[i]);
    }
    return 0;
}
```

由于父进程Shell在调用fork时，会把自身的环境变量赋值给子进程程。

环境变量设置：`name=value`，name大多数由大写字母加下划线组成。

#### 相关函数

```c
#include <stdlib.h>
char *getenv(const char *name);
int setenv(const char *name, const char *value, int rewrite);
void unsetenv(const char *name);
```

`setenv`：如果已存在变量name

* 若rewrite非0，则覆盖原来的定义
* 若rewrite为0，则不覆盖原来的定义，也不返回错误。

**注意：**

父进程在创建子进程时会复制环境变量给子进程，但彼此不会相互影响。

### 3. 进程控制

#### 3.1 fork函数

```c
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	pid_t pid;
	char *message;
	int n;
	pid = fork();
	if (pid < 0) {
		perror("fork failed");
		exit(1);
	}
	if (pid == 0) {
		message = "This is the child\n";
		n = 6;
	} else {
		message = "This is the parent\n";
		n = 3;
	}
	for(; n > 0; n--) {
		printf(message);
		sleep(1);
	}
	return 0;
}
```

1. 父进程调用fork，进入内核
2. 内核根据父进程复制出一个子进程，父进程和子进程的PCB信息相同，用户态代码和数据也相同，子进程也是调用了fork进入内核，但是还没返回。
3. 父子进程等待从内核返回（fork只调用一次），首先返回谁取决于系统调度。
4. fork之后的父进程和子进程的变量message和n互相不影响。
5. Shell进程是父进程的父进程，父进程运行时Shell进程处于等待状态。

父进程得到子进程的id，只有将fork的返回值记录下来，进程能通过函数得到自己的id和pid。

#### 3.2 exec函数

当进程调用一种exec函数时，该进程的用户空间代码和数据完全被新程序替换，从新程序的启动例程开始执行，而且调用前后exec前后该进程的id并未改变。

```c
#include <unistd.h>

int execl(const char *path, const char *arg, ...);
int execlp(const char *file, const char *arg, ...);
int execle(const char *path, const char *arg, ..., char *const envp[]);
int execv(const char *path, char *const argv[]);
int execvp(const char *file, char *const argv[]);
int execve(const char *path, char *const argv[], char *const envp[]);
```

exec执行成功不返回，失败才返回-1

不带p的第一个参数是path，带p的函数：

* 参数中包含/，则将其视为路径名
* 不带路径名，在PATH环境变量的目录列表中搜索这个程序

字母中带`l(list)`的exec函数要求将新程序的每个命令行参数当做一个参数给他，最后一个为NULL，起sentinel的作用。

字母中带有`v(vector)`的函数，则应该先构造一个指向各个参数的指针函数，然后将首地址传给他，类似argv

字母中e结尾的函数，可以传递一份新的环境变量。

```c
char *const ps_argv[] ={"ps", "-o", "pid,ppid,pgrp,session,tpgid,comm", NULL};
char *const ps_envp[] ={"PATH=/bin:/usr/bin", "TERM=console", NULL};
execl("/bin/ps", "ps", "-o", "pid,ppid,pgrp,session,tpgid,comm", NULL);
execv("/bin/ps", ps_argv);
execle("/bin/ps", "ps", "-o", "pid,ppid,pgrp,session,tpgid,comm", NULL, ps_envp);
execve("/bin/ps", ps_argv, ps_envp);
execlp("ps", "ps", "-o", "pid,ppid,pgrp,session,tpgid,comm", NULL);
execvp("ps", ps_argv);
```

事实上，`execve`是真正的系统调用函数。

![exec函数族](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/process.exec.png)

思想是封装函数的思想，将一组参数字符串，封装到一个数组中。

```c
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    // ps 显示当shell正在运行的进程
    execlp("ps","ps","-o","pid,ppid,pgrp,session,tpgid,comm", NULL);
    perror("exec ps");
    exit(1);
}
```

* 第一个ps是程序名
* 第二个ps是第一个命令行参数，不关心他的值，只讲它传给ps程序，通过argv[0]取到这个参数

调用exec时，原来的文件描述符仍然是打开的？父进程和子进程有相同的文件描述符，所以子进程能在exec时仍然使用原来的文件，能够实现I/O重定向。

```c
#include <stdio.h>

int main(void)
{
    int ch;
    /* Ctrl-D 表示EOF */
    while((ch = getchar()) != EOF){
        putchar(toupper(ch));
    }
    return 0;
}
```

封装参数，不借助于输入重定向，直接利用现有功能：

```c
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    int fd;
    if(argc != 2){
        fputs("usage:wrapper file\n", stderr);
        exit(1);
    }
    fd = open(argv[1], O_RDONLY);
    if(fd<0) {
        perror("open");
        exit(1);
    }
    dup2(fd, STDIN_FILENO);
    close(fd);
    execl("./a.out","a.out", NULL);
    perror("exec ./a.out");
    exit(1);
}
```

#### exec程序的加载和执行

* 当exec程加载成功时，成功用相关进程替换当前进程，执行完成后直接退出。且一般会在父级Shell中创建子Shell来执行
* 加载失败时，



#### 3.3 wait和waitpid函数

进程终止时会关闭所有的文件描述符，释放用户空间分配的内存，但它的PCB还保存。

内核保存的信息：

* 正常终止，保存着退出的状态
* 异常终止，保存着导致该进程终止的信号是哪个。

查看进程退出状态`$?`，因为Shell为他的父进程，当他终止时，Shell调用wait或waitpid得到它的推出状态，同时彻底清除这个进程。

一个进程已经终止，但是父进程未调用wait对他进行清理。

```c
./a.out &
```

& 表示后台运行，Shell不等待进程终止就立刻打印提示符并等待用户输入命令。

```bash
$ ps u
USER       PID %CPU %MEM    VSZ   RSS TTY      STAT START   TIME COMMAND
akaedu    6016  0.0  0.3   5724  3140 pts/0    Ss   08:41   0:00 bash
akaedu    6130 97.2  0.0   1536   284 pts/0    R    08:44  14:33 ./a.out
akaedu    6131  0.0  0.0      0     0 pts/0    Z    08:44   0:00 [a.out] <defunct>
```

如果一个进程的父进程终止，而他的子进程还在，这些子进程的父进程改为init进程，通常程序文件是/sbin/init，只要有子进程就会调用wait函数清理它。

僵尸进程不能用kill清除，kill只是用来终止进程。

```c
#include <sys/types.h>
#include <sys/wait.h>

pid_t wait(int *status);
pid_t waitpid(pid_t pid, int *status, int options);
```

调用成功则返回子进程id，否则返回-1.

调用wait时的情况：

* 阻塞：如果它的所有子进程都还运行
* 子进程已经终止，正等待父进程读取其终止信息。
* 出错立即返回

这两个函数的区别是：

* 如果父进程的所有子进程都还在运行，调用wait将使父进程阻塞，而调用waitpid指定option参数则可以立即返回。
* wait等待第一个终止的子进程，waitpid可以指定等待哪个子进程。

使父进程阻塞等待子进程终止，起到进程间同步的作用。如果参数status不是空指针，则子进程的终止信息通过这个参数传出，不关心则可设置为NULL。

```c
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    pid_t pid;
    pid = fork();
    if (pid < 0){
        perror("fork failed\n");
        exit(1);
    }
    if(pid == 0) {
        int i;
        for(i = 3;i > 0 ;i--){
            printf("This is the child\n");
          	// 使进程异常终止
            abort();
            sleep(1);
        }
        exit(3);
    }else {
        int stat_val;
        waitpid(pid, &stat_val, 0);
      	// 取出字段值 
        if(WIFEXITED(stat_val))
          // 取出子进程推出的状态
            printf("Child exited with code %d\n", WEXITSTATUS(stat_val));
        else if (WIFSIGNALED(stat_val))
            printf("Child terminated abnormally, signal %d\n", WTERMSIG(stat_val));
    }
    return 0;
}
```



### 4. 进程间的通信

每个进程各自有不同的用户地址空间，任何一个进程的全局变量在另一个进程都看不到，所以进程之间要交换数据必须通过内核，且会在内核中开辟缓冲区进行数据读写。

进程间通信（IPC, InterProcess Communication）

![进程间通信](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/process.ipc.png)

#### 4.1 管道（pipe）

```c
#include <unistd.h>

int pipe(int filedes[2]);
```

管道通信

调用pipe函数时在内核中开辟一块缓冲区(称为管道)，有一端读，另一端写，然后通过filedes参数传出给用户两个文件描述符，`fileds[0]`指向读，`fileds[1]`指向写端，与文件描述符一样。

管道就是一个代开的文件，在读写内核的缓冲区。

![管道](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/process.pipe.png)

1. 父进程调用pipe开辟管道，得到两个文件描述符指向管道的两端。
2. 父进程调用fork，子进程也指向同一管道。
3. 父进程关闭读端，紫金陈关闭写端。父进程可以往管道里写，子进程可以从管道里读。

```c
#include <stdlib.h>
#include <unistd.h>
#define MAXLINE 80

int main(void)
{
    int n;
    // 管道两端
    int fd[2];
    pid_t pid;
    char line[MAXLINE];

    if(pipe(fd) < 0){
        perror("pipe\n");
        exit(1);
    }
    if((pid = fork()) < 0){
        perror("fork");
        exit(1);
    }
    if(pid > 0){
        close(fd[0]);
      	// 1写端 
        write(fd[1], "hello world\n",12);
        wait(NULL);
    }else {
        close(fd[1]);
        n = read(fd[0], line, MAXLINE);
      	// 	读取数据并写入标准输出
        write(STDOUT_FILENO, line, n);
    }
    return 0;
}
```

* 管道的单向通信和双向通信？
* fork进程后继承文件描述符，然后可以实现fork的子进程进行通信

管道的特殊情况：

假设阻塞I/O操作，并没有设置O_NONBLOCK标志

1. 如果管道写端的文件描述符都关闭了，但是仍然从管道读端的数据，当所有数据被读完后，后续会读到0.
2. 如果有指向管道写端的文件描述符没有关闭，但是写端的进程没有向管道中写数据，读端此时如果读完所有数据后，再次read会阻塞。
3. 如果所有指向管道读端的文件描述符都关闭了，这时向管道写端write，该进程会收到信号`SIGPIPE`，会导致进程终止。
4. 读端的文件描述符没有关闭，而持有管道读端的进程没有读数据，这时进程从写端写数据，写满时再次`write`会阻塞，指导管道中有空位置才写入数据并返回。

#### 4.2 其他IPC机制

进程间通信需要经过内核，如果通信的进程没有从公共组先那继承文件描述符，如何去通信？内核如何标识可以通信的通道？因为路径名是全局的，一般用文件系统中的路径名来标识一个IPC通道

FILFO和UNIX Domain Socket这两种IPC机制都是利用文件系统中的特殊标识符标识。可以用mkfido命令创建一个FIFO文件。

FIFO文件仅用来标识通道，各个进程都可以进行读写。

#### 4.3消息传递机制总结

- 父进程通过`fork`可以将打开文件的描述符传递给子进程
- 子进程结束时，父进程调用`wait`可以得到子进程的终止信息
- 几个进程可以在文件系统中读写某个共享文件，也可以通过给文件加锁来实现进程间同步
- 进程之间互发信号，一般使用`SIGUSR1`和`SIGUSR2`实现用户自定义功能
- 管道
- FIFO
- mmap函数，几个进程可以映射同一内存区
- SYS V IPC，以前的SYS V UNIX系统实现的IPC机制，包括消息队列、信号量和共享内存，现在已经基本废弃
- UNIX Domain Socket，目前最广泛使用的IPC机制

共享内存？

#### 4.4 进程间8种通信方式

* 匿名管道：亲缘进程通信（pipe函数）
* 高级管道通信：将新的程序当作一个新的进程在程序中启动（类似子进程）
* 有名管道通信：允许无亲缘痛惜
* 消息队列通信：链表，存放在内核中
* 信号量通信：信号量是一个计数器，控制对多个进程对共享资源的访问，锁机制
* 信号：接受某个事件是否发生
* 共享内存通信：映射一段能被其他进程访问的内存



## 第31章 Shell脚本

### 2. Shell如何执行命令

#### 2.1 执行交互式命令

Shell一般执行命令，先fork然后再exec执行该命令。内建命令例外

内建命令：调用Shell进程内中的一个函数，并不创建新的进程。

#### 2.2.执行脚本

```bash
$ chmod +x test.sh
$ ./test.sh
```

1. 赋予执行权限
2. 执行

##### exec的执行机制

1. 发现为文本文件，且第一行制定了解释器
2. 用解释器程序的代码段替换当前进程，从解释器的_start开始执行
3. 类似`/bin/bash ./test.sh`

**Shell的执行步骤**

![Shell脚本的执行过程](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/shellscript.shellexec.png)

1. fork子进程，子进程exec执行脚本
2. 父进程等待子进程执行完毕
3. sh读取脚本，执行内建函数命令
4. 读取脚本执行`exec`命令，sh等待`exec终止`,`sh`终止。（父进程结束）
5. `sh`终止后，bash继续执行，打印提示符等待用户输入

#### 习题：

![image-20220924172345456](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/image-20220924172345456.png)

1. fork子进程执行(exit 2)
2. ()继续fork一个子进程
3. 执行 exit 2内建函数 ，退出()子进程，并返回2
4. echo $?打印返回值
5. 读取脚本完毕，`sh`执行完毕
6. 脚本执行完毕，执行bash，打印命令提示符。

### 3. Shell的基本语法

#### 3.1 变量

* 打印环境变量

```bash
$ printenv
```

本地变量：存在于当前shell进程中

`export`可以把本地变量变为环境变量

```bash
$ export name=value
```

`unset`可以删除已定义的环境变量和本地变量

引用变量的值`$name`，或者使用`$(name)`，事实上shell的变量的值都是字符串

#### 3.2 文件名替换符

匹配的字符，通配符(wildcard)

| *              | 匹配0个或多个任意字符                  |
| -------------- | -------------------------------------- |
| **?**          | **匹配一个任意字符**                   |
| **[若干字符]** | **匹配方括号中任意一个字符的一次出现** |

* 注意匹配的字符串在SHell中就已经展开，执行命令时就是实际的文件名

#### 3.3 命令代换：`或$()

类似函数指针

```bash
(base) ➜  test DATE=`date`
(base) ➜  test echo $DATE
2022年 9月24日 星期六 19时27分08秒 CST
(base) ➜  test DATE=$(date)
(base) ➜  test echo $DATE
2022年 9月24日 星期六 19时27分32秒 CST
```

#### 3.5 转义字符\

除去单个字符的特殊意义，创建一个名为“$ $”的文件

```bash
$ touch \$ \$
```

#### 3.6 单引号

字符串的界定符，需要一对

#### 3.7 双引号



### 4. bash启动脚本

bash启动时自动执行的脚本，设置环境变量。

以子进程还是`source`启动的？-> `source`

#### 4.1 交互式登陆Shell

1. 执行/etc/profile，系统中每个用户都要执行这个脚本，系统管理员可以操作
2. 依次执行当前用户主目录的`.bashrc`  \ `.bash_login`  \ `.profile`三个文件，当前用户可以重新设置一些设置

#### 4.2 非交互式登录Shell，图形界面打卡一个终端

mac\ubuntu下直接打开终端，此时Shell自动执行～/.bashrc脚本

为了使登录Shell时也能执行~./bashrc，一般配置

```bash
if [-f ~/.bashrc ];then
		. ~/bashrc
fi
```

如果bashrc文件存在，则执行他

#### 4.3 非交互启动

为fork子进程而设置



### 5. Shell脚本语法

#### 5.1 条件测试：test / [

* 如果为真，返回0
* 如果为假，则退出的状态为1

[为一个命令的名字，传递他的参数需要空格隔开

* -d：如果存在且为一个目录
* -f：如果FILE存在且是一个普通文件
* [! -a -o ] -a逻辑与(and)，-o逻辑或(o r)
* 注意把字符串放到 “ ” 中

#### 5.2 if/then/elif/else/fi

```bash
if [ -f ~/.bashrc ]; then
    . ~/.bashrc
fi
```

* 多条命令写在一行需要;来分割
* Shell自动续行，如上述的then

**为真：eixt 0，为假：exit 1**

```bash
#! /bin/sh

if [ -f /bin/bash ]
then echo "/bin/bash is a file"
else echo "/bin/bash is NOT a file"
fi
if :; then echo "always true"; fi
```

:是一个特殊的命令，不做任何事，exit Status总是真，等价于`/bin/true`或者`!/bin/false`

```bash
read YES_OR_NO
```

读取一行字符串，将该字符串存入Shell变量中

#### 5.3 case/esac

类似C语言`switch/case`

* 执行完一句后直接跳出
* 结尾以`;;`

```shell
#! /bin/sh

echo "Is it morning? Please answer yes or no."
read YES_OR_NO
case "$YES_OR_NO" in
yes|y|Yes|YES)
  echo "Good Morning!";;
[nN]*)
  echo "Good Afternoon!";;
*)
  echo "Sorry, $YES_OR_NO not recognized. Enter yes or no."
  exit 1;;
esac
exit 0
```

* $1 表示第一个命令行参数

#### 5.4 for/do/done

类似`foreach`

```shell
#! /bin/bash

for FRUIT in apple banana pear; do
	echo "I like $FRUIT!"
done
```

FRUIT是一个循环变量，类似pyhon里的for

类似修改文件名`chap`

```shell
$ for FILENAME in `ls chap?`; do mv $FILENAME $FILENAME~; done
```

#### 5.5 while/do/done

密码验证脚本,如果用户输错五次密码就报错退出。

```shell
  1 #! /bin/bash
  2
  3 COUNT=1
  4 echo "Enter password:"
  5 read TRY
  6 while [ "$TRY" != "secret" ]; do
  7   if [ $COUNT = 5 ]; then
  8     echo "five times error!exit."
  9     exit 1
 10   fi
 11   echo "Sorry, try again"
 12   read TRY
 13   COUNT=$(($COUNT+1))
 14 done
```

#### 5.6 位置参数和特殊变量

| `$0`          | 相当于C语言`main`函数的`argv[0]`                             |
| ------------- | ------------------------------------------------------------ |
| `$1`、`$2`... | 这些称为位置参数（Positional Parameter），相当于C语言`main`函数的`argv[1]`、`argv[2]`... |
| `$#`          | 相当于C语言`main`函数的`argc - 1`，注意这里的`#`后面不表示注释 |
| `$@`          | 表示参数列表`"$1" "$2" ...`，例如可以用在`for`循环中的`in`后面。 |
| `$?`          | 上一条命令的Exit Status                                      |
| `$$`          | 当前Shell的进程号                                            |

```shell
#! /bin/sh

echo "The program $0 is now running"
echo "The first parameter is $1"
echo "The second parameter is $2"
echo "The parameter list is $@"
shift
echo "The first parameter is $1"
echo "The second parameter is $2"
echo "The parameter list is $@"
```

* shift 代表向右移动多少个变量 

#### 5.7 函数

> 函数没有返回值和参数列表

```shell
#! /bin/bash

foo(){ echo "Function foo is called";}
echo "-=start=-"
foo
echo "-=end=-"
```

* {和后面的命令之间必须有空格或换行
* 如果将`}`和最后一条命令写在一行，必须要有`;`
* 函数类似一个Shell的内部脚本，可以用`$0`来表示参数，也可以有return类似`Exit Status`

### 6. Shell脚本的调用方法

* -n：读脚本，并不执行，检查语法错误
* -v：一边执行，一边打印错误
* -x：提供跟踪，将执行的每一条命令和结果依次打印出来

调用方法：

* 命令行参数
* 脚本开头
* 在脚本使用`set -X`



## 第32章 正则表达式

> Regular Expression

### 3.sed

流编辑器

```shell
sed option 'script' file1 file2 ...
sed option -f scriptfile file1 file2 ...
```

option的格式为：

```shell
/pattern/action
```

如果只输出结果，需要加上-n，删除时d不需要加上

sed不会更改原文件,&表示与之前模式匹配的字符串

```shell
 Sed sed 's/bc/-&-/' 1.txt
123
a-bc-
456
```

```shell
Sed sed 's/\([0-9]\)\([0-9]\)/-\1-~\2~/' 1.txt
-1-~2~3
abc
-4-~5~6
```

\1表示第一个匹配的字符，\2表示第二个

sed的贪心错误

```shell
Sed sed 's/<[^>]*>//g' 2.txt
Hello World
Welcome to the world of regexp!
```

* `[^>]`表示匹配除`>`之外的字符
* *表示多个字符

### 4.awk

awk可以以列为单位处理文件

```shell
ProductA  30
ProductB  76
ProductC  55
(base) ➜  awk awk '{print $2;}' 1.txt
30
76
55
```

* $2表示第二列
* $0表示当前所有

```shell
awk awk '$2<75 {printf "%s\t%s\n", $0, "REORDER";} $2>75 {print $0;}' 1.txt
ProductA  30	REORDER
ProductB  76
ProductC  55	REORDER
```

统计空行数量

`awk`命令的`condition`部分还可以是两个特殊的`condition`－`BEGIN`和`END`，对于每个待处理文件，`BEGIN`后面的`actions`在处理整个文件之前执行一次，`END`后面的`actions`在整个文件处理完之后执行一次。



## 第 33 章 信号

### 1. 信号的基本概念

前台进程可以被随时`ctrl-c`终止，收到`SIGINT`信号，信号相对于进程的控制流程来说是异步的。

系统定义的信号

```shell
# kill -l
 1) SIGHUP       2) SIGINT       3) SIGQUIT      4) SIGILL       5) SIGTRAP
 6) SIGABRT      7) SIGBUS       8) SIGFPE       9) SIGKILL     10) SIGUSR1
11) SIGSEGV     12) SIGUSR2     13) SIGPIPE     14) SIGALRM     15) SIGTERM
16) SIGSTKFLT   17) SIGCHLD     18) SIGCONT     19) SIGSTOP     20) SIGTSTP
21) SIGTTIN     22) SIGTTOU     23) SIGURG      24) SIGXCPU     25) SIGXFSZ
26) SIGVTALRM   27) SIGPROF     28) SIGWINCH    29) SIGIO       30) SIGPWR
31) SIGSYS      34) SIGRTMIN    35) SIGRTMIN+1  36) SIGRTMIN+2  37) SIGRTMIN+3
38) SIGRTMIN+4  39) SIGRTMIN+5  40) SIGRTMIN+6  41) SIGRTMIN+7  42) SIGRTMIN+8
43) SIGRTMIN+9  44) SIGRTMIN+10 45) SIGRTMIN+11 46) SIGRTMIN+12 47) SIGRTMIN+13
48) SIGRTMIN+14 49) SIGRTMIN+15 50) SIGRTMAX-14 51) SIGRTMAX-13 52) SIGRTMAX-12
53) SIGRTMAX-11 54) SIGRTMAX-10 55) SIGRTMAX-9  56) SIGRTMAX-8  57) SIGRTMAX-7
58) SIGRTMAX-6  59) SIGRTMAX-5  60) SIGRTMAX-4  61) SIGRTMAX-3  62) SIGRTMAX-2
63) SIGRTMAX-1  64) SIGRTMAX
```

* 由一个编号和一个宏定义名称组成(`signal.h`)

```bash
Signal     Value     Action   Comment
-------------------------------------------------------------------------
SIGHUP        1       Term    Hangup detected on controlling terminal
                              or death of controlling process
SIGINT        2       Term    Interrupt from keyboard
SIGQUIT       3       Core    Quit from keyboard
SIGILL        4       Core    Illegal Instruction
...
```

* Action表示默认处理动作：
  * Term：表示终止当前进程
  * Core：表示Core Dump

#### 产生信号的条件

* 用户按下某些键，硬中断
* 硬件异常，硬件检测并通知内核
* kill函数

如果不想按照默认动作处理，可以调用`signaction`函数处理信号

* 忽略信号
* 执行默认操作
* 提供一个信号处理函数，内核切换为用户时执行这个函数，类似捕获异常



### 2. 产生信号

#### 2.1通过终端按键产生信号

**Core Dump**

当一个进程异常终止时，可以选择把进程的用户空间内存数据保存到磁盘中，开发者可以使用开启产生Core文件。

```shell
ulimit -c 1024
```

#### 2.2 调用系统函数向进程发信号

kill函数

```c
#include <signal.h>

int kill(pid_t pid, int signo);
int raise(int signo);
```

成功返回1，失败返回0。

abort函数使当前进程收到SIGABRT信号而异常终止。

```c
#include <stdlib.h>

void abort(void);
```

#### 2.3 由软件条件产生信号

使用`alarm`函数和`SIGALRM`信号

```c
#include <unistd.h>

unsigned int alarm(unsigned int secondes);
```

调用`alarm`函数可以设定一个闹钟，默认处理操作是终止进程，返回值是0或者剩余的秒数。

```c
#include <unistd.h>
#include <stdio.h>

int main(void)
{
    int counter;
    alarm(1);
    for(counter=0; 1; counter++)
        printf("counter=%d ", counter);
    return 0;
}
```

1秒中不停的数数，1秒钟到了之后就被`SIGALARM`信号终止。

### 3.阻塞信号

#### 3.1 信号在内核中的表示

信号从产生到抵达之间的状态，称为未决状态，进程可以选择阻塞信号，被阻塞的信号处于未决状态。

每个信号有两个标志位和一个函数指针

* 阻塞标志位
* 未决标志位
* 函数处理指针

![信号在内核中的表示示意图](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/signal.internal.png)

1. `SIGHUP`信号未阻塞也未产生过，抵达时，执行默认处理动作。
2. `SIGINT`信号产生，正在被阻塞(block)，处理动作是忽略
3. `SIGUIT`信号未产生过，一旦产生将被阻塞，它的处理动作是用户自定义函数

**信号阻塞时，产生了多次该信号？**如何处理

1. Linux在信号抵达之前产生多次只记一次，而信号抵达之前会放在一个队列中。
2. 每个信号只有一个未决标志位

#### 3.2 信号集操作函数

`sigset_t`类型对于每种信号用一个bit来表示

```c
#include <signal.h>

int sigemptyset(sigset_t *set);
// 初始化信号集 所有bit清0，表示不含邮任何有效信号
int sigfillset(sigset_t *set);
// 信号置位，表示信号有效
int sigaddset(sigset_t *set, int signo);
// 添加
int sigdelset(sigset_t *set, int signo);
// 删除
int sigismember(const sigset_t *set, int signo);
// 判断信号集中是否包含某种函数
```

#### 3.3 sigprocmask

读取或更该进程的信号屏蔽字。

```c
#include <signal.h>
int sigprocmask(int how, const sigset_t *set, sigset_t *oset)
```

成功返回0，否则返回1.

* oset为非空指针，读取当前进程的当前信号屏蔽字，并用oset传出
* set为非空指针，根据参数how更改屏蔽字
* set和oset都为非空指针，先将原来的信号屏蔽字辈分到oset中

#### 3.4 sigpending

```c
#include <signal.h>

int sigpending(sigset_t *set)
```

sigpending读取当前进程的未决信号集，通过参数set传出，成功返回0，失败返回1



#### 实验

```c
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void printsigset(const sigset_t *set)
{
    int i;
    for(i = 1; i < 32; i++){
        // 如果包含信号
        if(sigismember(set, i) == 1){
            putchar('1');
        }else{
            putchar('0');
        }
    }
    // 此处动作？
    puts("");
}

int main(void){
    sigset_t s,p;
    // 初始化信号集
    sigemptyset(&s);
    // 将SIGINT加入信号集
    sigaddset(&s, SIGINT);
    // 更改屏蔽字
    // SIG_BLOCK mask|set
    sigprocmask(SIG_BLOCK, &s, NULL);
    while(1){
        // 读取当前的未决信号
        sigpending(&p);
        printsigset(&p);
        sleep(1);
    }
    return 0;
}
```

由于阻塞了SIGINT信号，导致`ctrl-c`不能终止程序。



### 4. 捕捉信号

#### 4.1 内核捕捉信号

信号的处理动作函数由用户自定义，处理过程

![信号的捕捉](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/signal.catch.png)



1. 用户注册了`SIGQUIT`信号的处理函数`sighandler`
2. 当前执行main函数，发生中断
3. 中断处理完毕后要返回用户态的main函数之前检查有信号`SGQUIT`抵达
4. 内核决定返回用户态，决定执行信号处理函数，该函数和`main`函数有不同的堆栈空间，是两个独立的控制流程
5. `sighandler`函数执行完之后，返回内核
6. 如果没有信号抵达，内核返回用户态，恢复`main`的上下文

#### 4.2 sigaction

```c
#include <signal.h>

int sigaction(int signo, const struct sigaction *act, struct sigaction *oact);
```

* 成功返回0，失败返回1
* `signo`指定信号的编号
* 如果`act`为非空，根据`act`修改信号的处理动作
* 如果`oact`为非空，则通过`oact`传出该信号原来处理动作

sigaction结构体：

```c
struct sigaction {
   void      (*sa_handler)(int);   /* addr of signal handler, */
                                       /* or SIG_IGN, or SIG_DFL */
   sigset_t sa_mask;               /* additional signals to block */
   int      sa_flags;              /* signal options, Figure 10.16 */

   /* alternate handler */
   void     (*sa_sigaction)(int, siginfo_t *, void *);
};

```

sa_handler赋值

* 为常数`SIG_IGN`传给`sigaction`表示忽略信号
* 为常数`SIG_DFL`表示执行系统默认动作
* 复制给一个函数指针，表示自定义函数捕捉信号

Sa_mask说明需要屏蔽的信号

#### 4.3 pause

```c
#include <unistd.h>

int pause(void);
```

使调用进程一直挂起制动有信号抵达：

* 如果信号的处理动作是忽略，则进程继续挂起
* 如果处理动作是捕捉，则调用信号处理函数后，pause返回-1，errno设置为EINTR

```c
#include <unistd.h>
#include <signal.h>
#include <stdio.h>

void sig_alarm(int signo){

}

unsigned int mysleep(unsigned int nsecs){
    struct sigaction newact, oldact;
    unsigned int unslept;

    newact.sa_handler = sig_alarm;
    // 无可屏蔽信号
    sigemptyset(&newact.sa_mask);
    newact.sa_flags = 0;
    sigaction(SIGALRM, &newact, &oldact);

    // 屏蔽两秒
    alarm(nsecs);
    pause();

    unslept = alarm(0);
    sigaction(SIGALRM, &oldact, NULL);

    return unslept;
}

int main(void){
    while(1){
        mysleep(2);
        printf("two seconds passed\n");
    }
    return 0;
}
```

1. main函数调用mysleep函数，调用`sigaction`注册了`SIGALRM`信号的处理函数
2. 调用`alarm(nsecs)`
3. 等待pause，内核调度到别的进程
4. 闹钟超市，内核将`SIGALRM`信号发给这个进程
5. 处理未决信号，调用信号处理函数
6. 进入`sig_alrm`函数时，`SIGALRM`信号被屏蔽，返回时，信号屏蔽解除，然后调用`sigreturn`返回内核态，内核切换回用户态执行进程的`main`函数
7. pause返回-1,调用`alarm(0)`取消闹钟，调用`sigactuon`恢复`SGALRM`信号以前的处理动作。



#### 4.4 可重入函数

不论进程的主控流程执行到哪，都会先跳到信号处理函数中执行，从信号处理函数返回后再执行住控制流程。它和主控程序异步。

引入信号处理函数使得一个进程具有多个控制流程。

可能出现的冲突：

![不可重入函数](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/signal.reentrancy.png)

`main`函数调用`insert`函数向链表插入节点，而信号处理函数也插入了节点，但最后只有一个节点被插入。由图可知，只有最后操作的函数完成了插入操作。

第一次调用还没返回时就再次进入该函数，称为重入。

不可重入的条件：

* 调用了`malloc`或者`free`
* 调用了标准`I/O`库函数



#### 4.5 sig_atomic_t类型与volatile限定符

插入操作出现错误的原因：**非原子操作**，即不被打断

为了在不同的平台问题，C标准定义了类型`sig_atomic_t`

对于程序多个执行流程访问同一全局变量的情况，volatile限定符能防止编译器错误的优化

* 内存单元不需要写操作，就可能导致每次读的值不同
* 对内存单元只写不读

映射到内存地址空间的硬件寄存器，而且sig_atomic_t变量总是需要加上volatile



#### 4.6 竞态条件与sigsuspend函数

在`mysleep`函数中，在还未执行`pause`之前，内核就已经先执行了信号处理函数，导致`pause`产生了空等待。

**关键：**时序没有得到保证，由于异步事件任何时候可能发生（调度更高优先级的进程），由于时序问题而导致错误，竞态条件

##### 屏蔽`signal`信号？

1. 屏蔽信号
2. `alarm(nsecs)`
3. 解除对`SIGALRM`信号的屏蔽
4. `pause()`

解除信号和调用`pause()`仍然存在间隙，将这两个操作合并为一个原子操作，导致产生了`sigsuspend`函数。

```c
#include <signal.h>

int sigsuspend(const sigset_t *sigmask);
```

进程的信号屏蔽字由`sigmask`决定，可以通过指定`sigmask`来临时解除对某个信号的屏蔽，然后挂起等待，当`sigsuspend`返回时，进程的信号屏蔽字恢复为原来的值。



#### 4.7 SIGCHLD信号

子进程在终止时会给父进程发送`SIGCHLD`信号，默认处理动作是忽略，父进程可以自定义信号的处理函数，这样父进程可以专心处理自己的工作，在自己的信号处理函数中调用wait即可。



#### 自定义信号处理

```c
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void sig_chld(int signo){
    /* signal handler */
    printf("catch the signal SIGCHLD!");
    wait();
}

int main(int argc, char const *argv[])
{
    //signal(SIGCHLD, sig_chld);
    struct sigaction newact, oldact;
    unsigned int     sig_chlld;

    newact.sa_handler = sig_chld;
    sigemptyset(&newact.sa_mask);
    newact.sa_flags = 0;
    sigaction(SIGCHLD, &newact, &oldact);

    pid_t pid;
    pid = fork();
    if(pid < 0){
        perror("fork error!\n");
        exit(1);
    }
    if(pid == 0){
        printf("1\n");
        sleep(3);
        exit(3);
    }

    while (1)
    {
        sleep(1);
        printf("2\n");
    }

    sigaction(SIGCHLD, &oldact, NULL);
    
    return 0;
}
```

```shell
1
2
2
catch the signal SIGCHLD!2
2
2
2
```

可以看到子进程的清理工作丝毫不影响父进程的工作。

将SIGCHLD设置为`SIG_IGN`，并看到僵尸进程的处理

```c
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    pid_t pid;

    signal(SIGCHLD, SIG_IGN);

    pid = fork();
    if(pid == 0){
        printf("child : ppid=%d\n", getppid());
        sleep(3);
        exit(1);
    }    
    printf("parent : pid=%d\n", getpid());
    
    return 0;
}
```

```shell
parent : pid=5797
child : ppid=1
```



## 第 34 章 终端、作业控制与守护进程

### 1. 终端

Shell进程的控制终端，因此由Shell进入其他进程会保存一份Shell进程的PCB。

默认情况下：

* 标准输入
* 标准输出
* 标注错误

每个进程的设备文件`/dev/tty`访问它的控制终端。事实上每个终端都对应一个不同的设备文件，设备既可以访问它的控制终端既可以`/dev/tty`，也可以访问它所对应的设备文件来访问。

查看设备对应的终端文件名。

```c
#include <unistd.h>
#include <stdio.h>

int main()
{
    printf("fd 0: %s\n", ttyname(0));
    printf("fd 1: %s\n", ttyname(1));
    printf("fd 2: %s\n", ttyname(2));
    return 0;
}
```

#### 1.2 终端登录过程

内核中处理终端设备的模块包括硬件驱动程序和线路规程（Line Discipline）

![终端设备模块](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/jobs.terminalmodule.png)

* 硬件驱动程序负责实际的硬件设备

* 线路规划：对于某些特殊字符并不让其直接通过，而是做特殊处理

  * 例如：ctrl-Z，对应的程序不会被程序的read读到，而是被线路规程截获，解释成`SIGTSTP`信号发给前台



##### 终端设备输入和输出队列缓冲区

![终端缓冲](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/jobs.terminalqueue.png)

输入队列为例：

1. 从键盘输入的字符经线路规程后进入输入队列
2. 用户程序先进先出读取
3. 输入队列读满时再输入会丢失自负



#### 1.3 网络登录过程

网络终端通过伪终端实现（Peseudo TTY）实现：主设备（Pseudo TTY）和从设备（PTY Slave）组成。

* 主设备：类似于键盘和显示器（一个内核的模块，一个进程操作他）

![伪终端](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/jobs.pseudotty.png)

1. 客户通过telnet连接服务器，则在服务器监听请求的是一个`talnet`进程，类似Shell，fork出子进程来服务

另一种为由系统服务程序inetd或xinetd监听请求，如果连接请求的端口号和`telnet`服务端口号一致。

2. `telnet`子进程打开一个伪终端，然后经过fork一分为二，父进程操作伪终端主设备，子进程将伪终端从设备作为它的控制终端，并且将文件描述符0、1、2指向控制终端，二者通过伪终端通信，父进程负责和`telnet`客户端通信，而子进程负责用户登录过程
3. 当用户输入，`telnet`将命令通过网络发给服务器，由服务器代替用户输入给伪终端，Shell进程并不知道自己连接的只是服务器，命令回显的数据发给`telnet`客户端，显示给用户。



### 2. 作业控制

#### 2.1 Session与进程组

事实上，Shell分前后台来控制的不是进程而逝作业（Job）或者进程组（Process Group）。

一个前台作业由多个进程组成，后台同理，Shell可以同时运行一个前台作业和任意多个后台作业，这叫做作业控制。

各进程、进程组、Session的关系如下图所示：

![Session与进程组](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/jobs.pg.png)

现在来看Session和进程组的角度重新来看登录和执行命令的进程：

1. `telnet`进程打开终端设备之前调用`setsid`创建一个新的Session，该进程被称为`Session Leader`，然后这个进程打开终端设备作为这个Session中所有进程的控制终端
2. 在登录过程中,`telnet`变成`login`再变成`Shell`，仍然是`Session Leader`
3. shell可以将作业中的某个子进程指定为一个新进程组的Leader，然后将它的子进程转移到该进程组中
4. 由于每个进程组只有一个前台进程

#### 2.2 与作业控制有关的信号

```shell
cat &
[1] 55081
[1]  + 55081 suspended (tty input)  cat
(base) ➜  ~ jobs
[1]  + suspended (tty input)  cat
(base) ➜  ~ kill -9 55081
[1]  + 55081 killed     cat
(base) ➜  ~ jobs
```

* cat需要读取标准输入，而后台进程不能读取终端输入，但为啥是`suspended`

`jobs`可以查看当前运行的作业,`fg`可以将某个作业提至前台运行，如果改作业正在后台运行则提至前台运行，如果改作业处于停止状态，则给进程组的每个进程发`SIGCONT`信号使它继续运行。

参数`%1`将第一个作业提至前台运行，cat挂到前台后等待终端输入,输入`ctrl-z`则向所有前台进程发`SIGTSTP`信号，该信号默认动作是使进程停止。

```shell
fg %1
[1]  + 55107 continued  cat
cat: stdin: Interrupted system call
```

`bg`命令可以让某个停止的作业在后台继续运行，也需要给进程组的每个进程发`SIGCONT`信号，然而`cat`需要读取终端输入，所以接收到信号又终止了。

```shell
bg %1
[1]  + 55120 continued  cat
cat: stdin: Interrupted system call
[1]  + 55120 exit 1     cat
```

`SIGKILL`与`SIGSTOP`信号，不能被阻塞和忽略，也不能被自定义函数捕捉，只能按系统默认的动作处理。



### 3. 守护进程

系统服务没有控制终端，不能直接和用户交互，不受用户登录注销的影响，一直运行，叫做**守护进程（Daemon）**

```shell
ps axj
```

查看系统中所有进程

* a：列出所有用户的进程
* x：列出有或者无控制终端的进程
* j：列出与作业控制相关的信息

`TPGID`为-1都表示无控制终端，COMAND一列用[]表示为内核线程,通常以`k`开头表示kernel。

```shell
USER               PID  PPID  PGID   SESS JOBC STAT   TT       TIME COMMAND
root                 1     0     1      0    0 Ss     ??   49:24.03 /sbin/launchd
root                87     1    87      0    0 Ss     ??    9:03.84 /usr/libexec/logd
root                88     1    88      0    0 Ss     ??    1:54.78 /usr/libexec/UserEventAgent (System)
```

守护进程通常以`d`结尾，表示`Daemon`

创建守护进程最关键一步调用`setsid`创建一个新的`Session`，并成为`Session Leader`

```c
#include <unistd.h>

pid_t setsid(void);
```

* 调用成功返回新创建的`Session`的`id`(其实就是pid)
* 但是需要保证当前进程不是进程组的leader就行（fork子进程再调用函数就行）

成功的结果：

* 一个新的`Session`
* 创建一个新的进程组
* 失去控制终端，原控制终端仍然是打开的，可以读写，但只是一个普通的打开文件。（修改了文件的权限？？或者标识？）

```c
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

void daemonize(void){
    pid_t pid;
    if((pid = fork()) < 0){
        perror("fork");
        exit(1);
    }else if(pid != 0)
        exit;

    setsid();
    // change the current working directory
    if(chdir("/") < 0){
        peeor("chdir");
        exit(1);
    }

    // 将文件描述符赋予/dev/null 
    close(0);
    open("/dev/null", O_RDWR);
    dup2(0, 1);
    dup2(0, 2);
}

int main(void){
    daemonize();
    while(1);
}
```

* 确保`setsid`的进程不是进程组的Leader
* 首先`fork`一个子进程，父进程退出，
* 然后子进程调用`setsid`创建新的`Session`，成为守护进程

守护进程的惯例：

* 将工作目录切换到根目录

* 将文件描述符0，1，2重定向到`/dev/null`



## 第 35 章 线程

### 1. 线程的概念

有些情况需要在一个进程中同时执行多个控制流，多线程程序的控制流可以长期共存，操作系统可以在县城之间调度和切换。由于多个线程共享同一地址空间，因此`Text Segment`、`Data Segment`都是共享的，如果定义一个函数，在各个线程中都可以调用，全局变量同理。

线程共享的资源：

* 文件描述符
* 信号处理方法
* 当前工作目录
* 用户id和组id

但有些资源是每个线程各有一份：

* 线程id
* 程序上下文，寄存器的值、程序计数器、栈指针
* 栈空间
* errno变量
* 信号屏蔽字
* 调度优先级

编译时需要加上`-lpthread`



### 2.线程控制

#### 2.1 创建线程

```c
#include <pthread.h>

int pthread_create(pthread_t *restrict thread,
                  const pthread_attr_t *restrict attr,
                  void *(*start_routine)(void*), void *restrict arg);
```

返回值：成功0，失败返回错误号。

pthread库的函数都是通过返回值返回错误好，每个线程也都有一个errno。

创建新的线程后，当前线程从`pthread_create()`返回后继续往下执行，新的线程的代码由函数指针`start_rountine`决定，该函数指针的参数由`arg`决定，返回值可以由`pthread_join`得到，类似于`wait`

新创建的线程的id被写到`thread`参数所指向的内存单元，类型为`pthread_t`，他在当前进程中保证唯一，可以为一个整数，也可以是一个结构体，也可以是一个地址。

attr：表示线程属性

```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_t ntid;

void printids(const char *s)
{
	pid_t      pid;
	pthread_t  tid;

	pid = getpid();
	tid = pthread_self();
	printf("%s pid %u tid %u (0x%x)\n", s, (unsigned int)pid,
	       (unsigned int)tid, (unsigned int)tid);
}

void *thr_fn(void *arg)
{
	printids(arg);
	return NULL;
}

int main(void)
{
	int err;

	err = pthread_create(&ntid, NULL, thr_fn, "new thread: ");
	if (err != 0) {
		fprintf(stderr, "can't create thread: %s\n", strerror(err));
		exit(1);
	}
	printids("main thread:");
	sleep(1);

	return 0;
}
```

由于`pthread_create`的错误码不保存在`errno`中，不能直接`perror`，需要先调用`strerror(err)`转换成错误信息再去打印。

`pthread_self`是线程库`pthread`提供的id，在系统级别没有作用。

由于任意线程调用`exit`，所有线程都会退出，所以主线程`return`前`sleep`一秒。



#### 2.2 终止线程

终止线程而不终止整个进程的三个方法：

* 线程函数`return`，对主线程不管用
* 一个线程可以调用`pthread_cancel`终止同一进程中的另一个线程
* 线程可以调用`pthread_exit`终止自己

pthread_cancel分为同步和异步两部分

```c
#include <pthread.h>

void pthread_exit(void *value_ptr);
```

需要注意，`pthread_exit`所指向的指针的内存单元必须是全局或者`malloc`分配的，不能在线程函数的栈上分配的，线程得到这个返回指针时线程函数已经退出了

```c
#include <pthread.h>

int pthread_join(pthread_t thread, void **value_ptr);
```

调用该函数的线程将挂起等待，直到id为`thread`的线程终止位置，pthread_join得到的终止状态是不同的：

* return返回，存放线程函数的返回值
* 调用pthread_cancel异常终止，存放常数`PTHREAD_CANCELED`
* 如果自己调用`pthread_exit`终止，存放的是传递个`pthread_exit`的参数



### 3. 线程间同步

#### 3.1 mutex

多个线程同时访问共享数据时可能会冲突，类似信号的原子性操作。

把某个全局变量+1的操作：

1. 从内存读变量值到寄存器
2. 寄存器的值+1
3. 将寄存器的值写回内存

多处理器上（1+1 ！= 2！！！！）

![并行访问冲突](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/thread.corrupt.png)

对于多线程程序访问内存冲突的问题，引入互斥锁(Mutex, Mutual Exclusive Lock)，获得锁的线程可以完成“读-修改-写”的操作，然后释放锁，没有锁的线程只能等待而不能访问公共数据，这样便组成了一个原子操作，要么执行，要么不执行。

```c
#include <pthread.h>

int pthread_mutex_destroy(pthread_mutex_t *mutex);
int pthread_mutex_int(pthread_mutex_t *restrict mutex,
                      const pthread_mutex_t *restrict attr);
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
```

* init 用来初始化线程的属性
* destroy 用来销毁线程属性
* 也可以用宏定义来初始化线程

```c
#include <pthread>

int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_trylock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);
```

* 线程调用`xx_lock`获取`Mutex`，如果锁被获取，则需要等待，等待线程释放锁
* 如果一个线程急需锁，调用`trylock`，如果不能获取，则线程不会挂起

#### lock与unlock

```c
lock:
	if(mutex > 0){
		mutex = 0;
		return 0;
	} else
		挂起等待;
	goto lock;

unlock:
	mutex = 1;
	唤醒等待Mutex的线程;
	return 0;
```

可以发现判断和修改并不是原子操作

大多数体系结构都提供了swap指令或excange指令，该指令把寄存器和内存单元的数据交换，由于只有一条指令，保证了原子性。一个处理器执行指令时，另一个处理器的交换指令只能等待总线周期。

```assembly
lock:
			movb $0, %al
      xchgb %al,mutex
      if(al寄存器的内容>0){
      	return 0; 
      }else
      		等待挂起
      goto lock;
unlock:
			movb $1, mutex
			唤醒等待Mutex的线程;
			return 0;
```

#### 等待实现：

每个Mutux都有一个等待队列，线程都处于睡眠状态，然后调用调度器函数切换到别的线程。

唤醒：状态改为就绪状态，加入就绪队列，调度有可能切换到被唤醒的线程。

#### 死锁：

资源永远得不到释放

* 自己等死自己
* A有了1，缺2等待，B有了2，缺1等待



#### 3.2 Condition Variable

线程同步有这样一个情况：

线程A需要某个条件成立才能继续执行，但是现在条件不成立，线程A就阻塞了，而线程B执行的过程中使这个条件成立了，唤醒A线程继续执行。

`pthread`库通过条件变量来阻塞一个等待条件，或者唤醒一个线程的条件,用`pthread_cond_t`来表示，初始化和销毁：

```c
#include <pthread.h>

int pthread_cond_timedwait(pthread_cond_t *restrict cond,
                           pthread_mutex_t *restrict mutex,
                           const struct timespec *restrict abstime);
int pthread_cond_wait(pthread_cond_t *restrict cond,
                     pthread_mutex_t *restrict mutex);
int pthread_cond_broadcast(pthread_cond_t *cond);
int pthread_cond_signal(pthread_cond_t *cond);
```

一个Condition Variable总是和一个Mutex搭配使用。一个线程可以调用`pthread_cond_wait`在一个`Condition Variable`阻塞等待，函数三个操作：

1. 释放Mutex
2. 阻塞等待
3. 当被唤醒时，重新获得Mutex并返回

如果设置超时参数，就返回`ETMEDOUT`，一个线程可以调用`pthread_cond_signal`唤醒在某个Condition Variable上等待的另一个线程，也可以唤醒在这个`Condition Variable`上等待的所有线程。

生产者与消费者：

FIFO队列实现

```c
// 验证消费者-生产者

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

struct msg{
    struct msg *next;
    int num;
};

struct msg *head;
struct msg *tail;
pthread_cond_t has_product = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *consumer(void *p){
    struct msg *mp;

    for(;;){
        pthread_mutex_lock(&lock);
        while(head == NULL){
            pthread_cond_wait(&has_product, &lock);
        }
        // 消费一个
        mp = head;
        head = mp->next;
        if(head == NULL){
            tail = NULL;
        }
        pthread_mutex_unlock(&lock);
        printf("Consume %d\n",mp->num);
        free(mp);
        sleep(rand() % 2);
    }

}

void *producer(void *p){
    struct msg *mp;

    for(;;){
        mp = malloc(sizeof(struct msg));
        mp->num = rand() % 1000 +1;
        printf("Produce %d\n", mp->num);
        pthread_mutex_lock(&lock);
        // 生产一个
        // LIFO last in first out
        //mp->next = head;
        //head = mp;
        // FIFO first in first out
        if(head == NULL){
            head = mp;
            tail = mp;
        }else{
            tail->next = mp;
            tail = mp;
        }
        pthread_mutex_unlock(&lock);
        pthread_cond_signal(&has_product);
        sleep(rand() % 5);
    }
}

int main(int argc, char const *argv[])
{
    pthread_t pid, cid, cid2;

    srand(time(NULL));
    // pthread_t 指针取引用
    pthread_create(&cid,NULL,consumer,NULL);
    pthread_create(&pid,NULL,producer,NULL);
    pthread_create(&cid2,NULL,consumer,NULL);

    //等待返回
    pthread_join(pid,NULL);
    pthread_join(cid,NULL);
    pthread_join(cid2, NULL);
    return 0;
}
```

#### 3.3 Semaphore

Mutex可以看成一种资源：1表示可用，0表示不可用，锁被释放时又可用

信号量（Semaphore）和Mutex类似，但是信号量可以大于1.

```c
#include <semaphore.h>

int sem_init(sem_t *sem, int pshared, unsigned int value);
int sem_wait(sem_t *sem);
int sem_trywait(sem_t *sem);
int sem_post(sem_t *sem);
int sem_destroy(sem_t *sem);
```

* Sem_init表示初始化，value表示可用资源的数量，pshared参数为0表示信号量用于同一进程的线程间同步。
* Sem_destroy释放资源
* Sem_wait可以获得资源，值-1，如果此时值为0，则挂起等待
* sem_post可以释放资源，使值+1，同时唤醒挂起等待的线程。



#### 3.4 其他线程间同步的机制

* 读写锁:Read-Writer Lock

Reader之间并不互斥，可以同时读共享数据，而Writer是独占的



#### 主线程退出对子线程的影响

主线程退出后，子线程的状态依赖于它所在的进程，如果进程没有退出的话子线程依然正常运转。如果进程退出，那么它的所有线程都会退出。

线程间的关系：对等（无主线程、子线程之分）。

Tast_struct中增加了一个`tgid`的概念，getpid通过系统调用返回的`task_struct`中的`tgid`，线程号字段`pid`由系统调用`syscall(SYS_gettid)`来获取。

实际的情况是主线程中的main函数执行完ruturn后弹栈，然后调用glibc库函数exit，exit进行相关清理工作后调用_exit系统调用退出该进程。所以，这种情况实际上是因为进程运行完毕退出导致所有的线程也都跟着退出了，并非是因为主线程的退出导致子线程也退出。

每个线程都有自己的信号屏蔽字，但是信号的处理是进程中所有线程共享的。这意味着尽管单个线程可以阻止某些信号，但当线程修改了与某个信号相关的处理行为以后，所有的线程都必须共享这个处理行为的改变。这样如果一个线程选择忽略某个信号，而其他的线程可以恢复信号的默认处理行为，或者是为信号设置一个新的处理程序，从而可以撤销上述线程的信号选择。

如果信号的默认处理动作是终止该进程，那么把信号传递给某个线程仍然会杀掉整个进程。

例如一个程序a.out创建了一个子线程，假设主线程的线程号为9601，子线程的线程号为9602(它们的tgid都是9601)，因为默认没有设置信号处理程序，所以如果运行命令kill 9602的话，是可以把9601和9602这个两个线程一起杀死的。如果不知道Linux线程背后的故事，可能就会觉得遇到灵异事件了。



### 线程与进程

* 进程是资源分配的基本单位，线程是调度的基本单位
* 进程是资源的集合，一个进程中多个线程共享这些资源
* CPU对作业进行调度，可调度的基本单位是线程
* 进程个体间是独立的存在



#### 内核与用户角度

**内核：**

只有pid，独立的调度单元

**用户角度：**

有两个线程，都属于同一个进程，同时包括`tid`与`pid`

只有用户视角才有`tid`

```txt
         								 USER VIEW
                         vvvv vvvv
              |          
<-- PID 43 -->|<----------------- PID 42 ----------------->
              |                           |
              |      +---------+          |
              |      | process |          |
              |     _| pid=42  |_         |
         __(fork) _/ | tgid=42 | \_ (new thread) _
        /     |      +---------+          |       \
+---------+   |                           |    +---------+
| process |   |                           |    | process |
| pid=43  |   |                           |    | pid=44  |
| tgid=43 |   |                           |    | tgid=42 |
+---------+   |                           |    +---------+
              |                           |
<-- PID 43 -->|<--------- PID 42 -------->|<--- PID 44 --->
              |                           |
                        ^^^^^^ ^^^^
                        KERNEL VIEW
```

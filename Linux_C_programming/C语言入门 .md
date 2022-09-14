# Linux C 一站式编程

## 10 .gdb

### 1. 单步执行和跟踪函数调用

单步执行：回车 、 n(next)

跟踪函数调用： s(step)

#### 在函数中查看状态：

* `bt`：查看函数调用栈帧

  ```bash
  (gdb) bt
  #0  add_range (low=1, high=10) at demo.c:6  栈帧编号为0 
  #1  0x00005555555551c3 in main () at demo.c:14 栈帧编号为1
  ```

  add_range 函数被main函数调用，以及调用参数(1,10)

* `i(info)` locals：产看局部变量的值

  ```bash
  (gdb) i locals
  i = 0
  sum = 0
  ```

* f(frame) n：查看栈帧局部变量的值

  ```bash
  (gdb) f 1  # 查看main函数局部变量
  #1  0x00005555555551c3 in main () at demo.c:14
  14              result[0] = add_range(1, 10);
  ```

* p(print) n:打印变量值

  ```bash
  (gdb) p sum # 打印变量值的名字
  $1 = 3
  ```

* finish：让当前函数运行到返回为止

  ```bash
  (gdb) finish
  Run till exit from #0  add_range (low=1, high=10)
      at demo.c:6
  main () at demo.c:14
  14              result[0] = add_range(1, 10);
  Value returned is $2 = 55
  ```

* `set var`  变量名=x

  ```bash
  (gdb) set var sum=0
  (gdb) finish
  Run till exit from #0  add_range (low=1, high=100)
      at demo.c:6
  main () at demo.c:15
  15              result[1] = add_range(1, 100);
  Value returned is $2 = 5050
  ```

#### bug 解决

```c
#include <stdio.h>

int add_range(int low, int high)
{
	int i, sum; // 局部变量没有初始化
	for (i = low; i <= high; i++)
		sum = sum + i;
	return sum;
}

int main(void)
{
	int result[100];
	result[0] = add_range(1, 10);
	result[1] = add_range(1, 100);
	printf("result[0]=%d\nresult[1]=%d\n", result[0], result[1]);
	return 0;
}
```

* 由于两次函数调用爱着，局部变量i、sum取上次调用的值



### 2.断点

#### 解锁新的命令

* display(跟踪)：持续关注某个变量，每次听下都会打印某个值

  ```bash
  (gdb) display sum
  1: sum = 1431654528
  ```

* break：设置断点 避免一步步走循环 (直接 c 连续运行到断点处)

  ```
  (gdb) b 9
  Breakpoint 2 at 0x555555555192: file demo.c, line 9.
  ```

* c(continue) ：连续运行至断点处

  ```bash
  (gdb) c
  Continuing.
  input=123
  
  Breakpoint 2, main () at demo.c:9
  9                       scanf("%s", input);
  1: sum = 123
  ```

* i(info) breakpoints

  ```bash
  (gdb) b 12
  Breakpoint 3 at 0x5555555551e8: file demo.c, line 12.
  (gdb) i breakpoints
  Num     Type           Disp Enb Address            What
  2       breakpoint     keep y   0x0000555555555192 in main at demo.c:9
          breakpoint already hit 1 time
  3       breakpoint     keep y   0x00005555555551e8 in main at demo.c:12
  ```

* 每个断点都有一个编号，可以用编号知道那个删除某个断点

  ```bash
  (gdb) delete breakpoints 2
  (gdb) i breakpoints
  Num     Type           Disp Enb Address            What
  3       breakpoint     keep y   0x00005555555551e8 in main at demo.c:12
  ```

* 禁用断点和重新启用断点

  ```bash
  (gdb) disable breakpoints 3
  (gdb) i breakpoints
  Num     Type           Disp Enb Address            What
  3       breakpoint     keep n   0x00005555555551e8 in main at demo.c:12
  (gdb) enable 3
  (gdb) i breakpoints
  Num     Type           Disp Enb Address            What
  3       breakpoint     keep y   0x00005555555551e8 in main at demo.c:12
  ```

* 断点还可以满足条件时启用

  ```
  break ... if ...
  run 从头开始连续运行程序
  ```

  

### 3.观察点

#### 解锁新命令

* x:打印指定存储单元的内容

  ```bash
  (gdb) p input  
  $3 = "12345"
  (gdb) x/7b input   # 7b 打印格式  b表示每个字节一组 7表示打印7组
  0x7fffffffe0b3: 49      50      51      52      53      0       -75
  ```

  前五个字符为ASCII码的‘1’到‘5’，第六个字节是写出界的‘\0’

* w(watchpoint) ：设置跟踪点

  ```bash
  (gdb) watch input[5]
  Hardware watchpoint 4: input[5]
  (gdb) i watchpoints # 查看当前设置了哪些观察点
  Num     Type           Disp Enb Address            What
  4       hw watchpoint  keep y                      input[5]
  ```

  如果不知道存储单元是在哪被改动的，设置观察点很有用



### 4. 段错误


































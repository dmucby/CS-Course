# CS 106L

## Hello word



```c++
int hello_as() {
    /* The assembly literally writes the hex representation
     * of as big a portion of the string as it can into the addresses
     * at range rsp to rsp + 0xd. That range is exactly 12 bytes long
     * as there are 12 characters in the "Hello, wordl!" string.
     */
    asm("sub    $0x20,%rsp\n\t"
        "movabs $0x77202c6f6c6c6548,%rax\n\t"  
        // moves "Hello, w" portion to mem at $rsp
        "mov    %rax,(%rsp)\n\t"
        "movl   $0x646c726f, 0x8(%rsp)\n\t"     
        // moves "orld"  portion to mem at $rsp + 8
        "movw   $0x21, 0xc(%rsp)\n\t"           
        // moves "!" portion to mem at $rsp + 12
        "movb   $0x0,0xd(%rsp)\n\t"             
        // moves string null terminator to mem at $rsp + 13
        "leaq    (%rsp),%rax\n\t"              
        // loads address of $rsp as first argument to puts
        "mov    %rax,%rdi\n\t"
        "call  __Z6myputsPc\n\t"               
        // calls puts
        "add    $0x20, %rsp\n\t"
    );
    return EXIT_SUCCESS;
}
```

* `asm`：在C++中嵌入汇编指令（`Assembler`）



### 编译代码

```bash
g++ -Wall -Werror -std=c++17 hello_world.cpp -o hello_world
```

> - -wall ：一般使用该选项，允许发出GCC能够提供的所有有用的警告。也可以用-W{warning}来标记指定的警告。
>
> - -werror：把所有警告转换为错误，以在警告发生时中止编译过程
> - -o: 制定目标名称,缺省的时候,gcc 编译出来的文件是a.out,很难听,如果你和我有同感
>   - .o是编译中间目标文件，相当于win上的.obj；
>   - .out是[可执行文件](https://so.csdn.net/so/search?q=可执行文件&spm=1001.2101.3001.7020)，相当于win上的exe；



## Quadratic

### Accessed through the namespace std::

CS 106B always uses the `using namespace std;` declaration, 

which automatically adds std:: for you



### C++ is a statically typed language

#### python Vs C++

* C++ 分门别类的管理
* Python 所有积木放在一个洞中，管理和取出很麻烦



## Struct

### std::pair

```c++
std::pair<int, string> numSuffix = {1,"st"};

cout << numSuffix.first << numSuffix.second;
//prints 1st
```

结构：

```c++
struct Pair {
	fill_in_type first;
	fill_in_type second;
};
```

```c++
std::pair<bool, Student> lookupStudent(string name) {
	Student blank;
	if (found(name)) return std::make_pair(false, blank);
	Student result = getStudentWithName(name);
	return std::make_pair(true, result);
}

int main(int argc, char const *argv[])
{
	std::pair<bool, Student> output = lookupStudent(“Keith”);
	std::cout << output.first << std::endl;
	std::cout << output.second << std.second;
	return 0;
}
```

To avoid specifying the types of a pair, use `std::make_pair(field1, field2)`

例如：

```c++
auto e = std::make_pair(3, “Hello”);
```



### Auto

> **auto** does not mean that the variable doesn’t have a type. 

It means that the type is deduced by the compiler.

```c++
auto a = 3;
auto b = 4.3;
auto c = ‘X’;
auto d = “Hello”;
auto e = std::make_pair(3, “Hello”);
```



## Stream

> an abstraction for input/output. Streams convert between data and the string representation of data.

输入输出流、管道的思想

Any primitive type + most from the STL work! 

For other types, you will have to write the << operator yourself!

std::cout is an output stream. It has type std::ostream.

```c++
std::cout << 5 << std::endl;
 // converts int value 5 to string “5”
 // sends “5” to the console output stream
```

* 将5转为字符串然后打印



### std::ofstream

Converts data of any type into a string and sends it to the file stream.

```c++
std::ofstream out(“out.txt”);
// out is now an ofstream that outputs to out.txt
 out << 5 << std::endl; // out.txt contains 5
```

* <<

* put：

  ```c++
  ostream out(&buf);
  char c = 'X';
  out.put('c').put('=').put(c).put('\n');
  ```

* write：

  ```c++
  out.write("aaa\n", 4);  
  ```



ofstream在往文件中写入数据时，数据实际上是先写到缓冲区中，并没有写到文件中去，所以需要调用一个flush，来确保数据会从缓冲区写到输出设备，也就是文件中去。



### Input Streams

std::cin is an input stream. It has type std::istream.

Can only receive strings using the >> operator,Receives a string from the stream and converts it to data.

Each >> **ONLY** reads until the next whitespace 

- Whitespace = tab, space, newline



#### getline

To read a whole line, use std::getline(istream& stream, string& line)

```c++
std::string line;
std::getline(cin, line); //now line has changed!
//say the user entered “Hello World 42!”
std::cout << line << std::endl;
//should print out“Hello World 42!”
```

getline reads up to the next delimiter (by default, ‘\n’), and does go past that delimiter.

**Don’t use >> with Stanford libraries, they use getline.**



#### Input File Streams

```c++
std::ifstream in(“out.txt”);
// in is now an ifstream that reads from out.txt
string str;
 in >> str; // first word in out.txt goes into str
```



#### Stringstreams

stream: an abstraction for input/output. Streams **convert** between data and the string representation of data.



#### HomeWork

1. C++ 拼接字符串
2. 文件读写（单词 + 行）
3. 通过stringstream将输入转化为字符串



C++ 抛出异常

```c++
throw std::domain_error(string("Unexpected character in input: ") + rem);
```





## Initialization and References

### Initialization

Pair初始化

```c++
    // make_pair initialize
    std::pair <int,string> demo = std::make_pair(3,"rd");
```

* curly bracket initialization.  {} 赋值

* auto:

  * 不过度使用
  * 常用于接受函数返回值
  * 不嵌套使用 auto<auto<auto>>

  ```c++
  auto [found, solutions] = quadratic(a, b, c);
  if (found) {
  	auto [x1, x2] = solutions;
  
  // 优化 绑定 auto中每个变量的类型清晰
  auto result = quadratic(a, b, c);
  auto found = result.first;
  if (found) {
  	auto solutions = result.second;
  
  ```

  

### References

#### & 操作符

> “=” automatically makes a copy! Must use & to avoid this.

* 传参和传址

#### l-value Vs. R-value

* l-values have names
*  l-values are not temporary - 
* r-values don’t have names 
* r-values are temporary

#### const

> indicates a variable can’t be modified!



#### 总结

by default, makes copies when we do variable assignment! We need to use & if we need references instead.



## Containers

> STL 赋值时不进行边界检查



### Vector

#### why push_front()?

* super slow
  * 将所有数组往后移动一个
  * C++ 注重效率

![image-2022071717215759](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/image-20220717172157509.png)



### vector dequeue list

* std::vector: use for almost everything 

* std::deque: use if you are frequently inserting/removing at front 

* std::list: use very rarely, and only if you need to split/join multiple lists

#### 插入时间的比较

![image-20220717172651789](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/image-20220717172651789.png)



### Ordered

#### maps Vs pairs

* const k or just k
* why not tuples
* pair make sence?



## Iterator

### Why ++iter and not iter++?

++iter returns the value after being incremented! 

iter++ returns the previous value and then increments it. 

(wastes just a bit of time)



### Pointers

* When variables are created, they're given an address in memory.

![image-20220717140520647](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/image-20220717140520647.png)

* Pointers are objects that store an address and type of a variable.

![image-20220717140644944](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/image-20220717140644944.png)

```c++
int *p = &x;
```



#### -> Vs ./*

```c++
int x = 5;
int* pointerToInt = &x; // creates pointer to int
cout << *pointerToInt << endl; // 5
std::pair<int, int> pair = {1, 2}; 
// creates pair
std::pair<int, int>* pointerToPair = &pair; 
// creates pointer to pair
cout << (*pair).first << endl; // 1
cout << pair->first << endl; // 1
```



### Pointers vs. Iterators

```c++
std::string lands = "Xadida";
auto iter = lands.begin();

char* first_char = &lands[0];
```

* Iterators is a form os the Pointers
* Pointers are more generic Iterators



#### 黑魔法

* 遍历和打印数组 
* 类似 Java8 stram玩法

```c++
std::copy(vec.begin(), vec.end(), std::ostream_iterator<int>(std::cout, ", "));
```



### Sumrize

* vector use for everything

* deque use for need fast insertion to front and back

  * **stack** \ **queue**

* set / map

  * inset、delete、access(write or for loop) `FAST`
  * Set ====> ordered but not guarantee order
  * Maps ===> consist of pairs

* unordered_ **more fast**
  * no index



## Class

### Why struct do this?

* struct : these are called fields
* permission：all datas are public to other
  *  should guard against nonsensical values 提防小人
  * 类比权限的praivate public proteced
* trouble：all need to initalize (都需要精确的初始化)
  * 类比Java的构造函数

> “A struct simply feels like an open pile of bits with very little in the way of encapsulation or functionality. A class feels like a living and responsible member of society with intelligent services, a strong encapsulation barrier, and a well defined interface.” 											- Bjarne Stroustrup “C++之父”





### QuickStart

* Defines **interface** for interacting with the private member variables!
  * 类似Java中的set\get方法

* class has own **namespace**, avoid name clashes

  * function name should use it

* function prototype:类似接口的概念
  
* this：类似的用法
  



### Array

* 分配 + 收回

  ```c++
  int *my_int_array;
  
  // allocate the memory
  my_int_array = new int[10];
  int one_element = my_int_array[0];
  cout << one_element << " " << my_int_array[0] << endl;
  
  // deallocate that memory
  delete [] my_int_array;
  ```

* 



### Destructor

* 与`Java` 最不同的地方，由于需要手动管理内存，所以类需要释放内存



## Template Classes

A Fun Story!

> Fundamental Theorem of Software Engineering: Any problem can be solved by adding enough layers of indirection.

没有什么是加几层（qian）解决不了的



### Vector problem

> vector should be able to contain any data!

### How to make a StudentVector?

* 为了不每写一个`class`都去写一个`vector`



#### Template Simple

* vector< xxx>

  ```c++
  vector <int>
  ```

* map <int ,int>

  ```c++
  map<int,string>
  ```

  

### Use Template

* 类似`Java`的泛型

#### MyPair

```c++
template<typename First,typename Second> class MyPair {
public:
    First getFirst();
    Second getSecond();

    void setFirst(First f);
    void setSecond(Second f);

private:
    First first;
    Second second;
};
```







## Interesting Problems?

### using namespace std?

* its effect is limited to the function or class scope(范围).
* 版本迭代再更改的问题，不方便自己修改
* 适配别人的修改和合作

![image-20220705161206556](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/image-20220705161206556.png)




















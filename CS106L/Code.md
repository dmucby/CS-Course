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



## Template Classes + Const Correctness

> theme : make classes
>
> * safe
> * general

### Iterator

```c++
std::vector a = {1, 2};
std::vector::iterator it = a.begin();
```

Iterator is the menber type of the vetcor.

```c++
//vector.h
template<typename T> class vector {
	using iterator = … // something internal
private:
	iterator front;
}
```

//iterator is a nested type(嵌套类型) in namespace vector::

Templates don’t emit code until instantiated(实例化), so #include the .cpp file in the .h file, not the other way around!

为什么引入它的头文件而不引入实现

* .h  [&#10004;]
* .cpp [x]



### Const

> const: keyword indicating a variable, function or parameter can’t be modified

* Can’t declare non-const reference to const variable!



#### Why Const

* 确保引用类型一致
* \- We need to promise that it doesn’t by defining them as const functions



### Simple StrVector

```c++
class StrVector{
    public:
        using iterator = std::string;
        const size_t kInitialSize = 2;

        size_t size() const;
        bool empty() const;
        std::string& at(size_t index);
        void insert(size_t pos,const std::string& elem);
        void push_back(const std::string& elem);

        iterator begin();
        iterator end();
}
```

#### Should **begin()** and **end()** be const?

* 如果`it`被人为改动，那么它将报错，为了满足，定义了`cbegin`



### Sumrize

*  auto will drop all const and &, so be sure to specify
* Make iterators and const_iterators for all your classes!





## Template Functions

How can we write methods that work on any data type?

> Writing reusable, unique code with no duplication!

### 模板

![image-20220730215018870](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/image-20220730215018870.png)

* typename is some generic type
  * class == typename

* 可以指定默认值

```c++
template <typename T=int> // 默认int
T myMin(const T a,const T b){
    return a < b ? a:b;
}
```

*  调用时指定类型

```c++
cout << myMin<float>(4.555,3.37888) << endl;
```

* Template functions can be called (instantiated) **implicitly** or **explicitly**
* Template parameters can be explicitly provided or **implicitly deduced**

template具体有智能，会自动返回相应的类型

```c++
template <typename T, typename U>
auto smartMyMin(T a, U b){
    return a < b? a:b;
}
```



### 编译template

* Normal functions are created during compile time, and used in runtime. 
* Template functions are not compiled until used by the code.
* 每个template函数被编译后,就像一个模板函数被实例化

#### 实例化函数template

* 显式实例化
  * 在可执行文件中创建函数模板
* 隐式实例化



**<重点 难>**

### Template Metaprogramming

> Writing code that runs during compilation (instead of run time)

在编译时运行代码

* make compiled code packages smaller
* speed up code when it's actually running



### Why write generic functions?

**一个计数程序的扩展**

* we can solve each of these problems with a single function

```c++
template <typename T>
int count_occurrences(const vector<T> vec, T target){
    int count = 0;
    for (size_t i = 0; i < vec.size(); i++)
    {
        if(vec[i] == target) count++;
    }
    return count; 
}

 cout << count_occurrences<std::string>({"Xadia", "Drakewood", 
    "Innean"}, "Xadia") << endl;
```

* 能不能不使用`vector`

```c++
  template <typename Collection, typename DataType>
  int count_occurrences(const Collection& arr, DataType target){
       int count = 0;
       for (size_t i = 0; i < arr.size(); ++i){
       	if (arr[i] == target) count++;
   	}
   	return count;
  }
  Usage: count_occurrences({"Xadia", "Drakewood", "Innean"}, "Xadia"); 
```

* The collection may not be indexable. How do we solve this?  
  * iterator


```c++
template <typename InputIt, typename DataType>
int count_occurrences(InputIt begin, InputIt end, DataType target){
    int count = 0;
    for (auto iter = begin; iter != end; iter++){
        const auto& elem = *iter;
        if(elem == target) count++;
    }
    return count;
}
```

  

## Functions and Lambdas

### Function Pointers and Lambdas

#### Predicate Functions

bool -> is -> 是或者不是 中文为谓语动词

```c++
bool isVowel(char c) {
    std::string vowels = "aeiou";
    return vowels.find(c) != std::string::npos;
}
```

计数函数又一次升级，定制化条件。（利用函数指针）

```c++
bool isVowel(char c){
    std::string vowels = "iaiyou";
    return vowels.find(c) != std::string::npos;
}

template <typename InputIt, typename UnPred>
int count_occurrences(InputIt begin, InputIt end, UnPred pred){
    int count = 0;
    for (auto iter = begin; iter != end; iter++){
        const auto& elem = *iter;
        if(pred(elem)) count++;
    }
    return count;
}

int main(){
    std::string str = "Xadia";
    cout << count_occurrences(str.begin(), str.end(), isVowel);
}
```



### Lambdas

Can we create functions with more ways to input information than just parameters?

#### 模板

```c++
auto var = [capture-clause] (auto param) -> bool{
	...
}
```

* auto var 类似变量的声明
* [   ]捕获条件：触发机制
* auto param ： 使用的参数
* bool : 返回值 

#### 计数程序超进化

```c++
template <typename InputIt, typename UnPred>
int count_occurrences(InputIt begin, InputIt end, UnPred pred){
    int count = 0;
    for (auto iter = begin; iter != end; iter++){
        const auto& elem = *iter;
        if(pred(elem)) count++;
    }
    return count;
}


int main(int argc, char const *argv[])
{
    // auto printNum = [] (int n) -> void {
    //     cout << n << endl;
    // };

    // printNum(5);

    int limit = 5;
    auto isMoreThan = [limit] (int n) -> bool{
        return n > limit;
    };
    std::vector<int> nums = {3,5,6,7,9,13};
    cout << count_occurrences(nums.begin(), nums.end(),isMoreThan) << endl; 

    return 0;
}

```

* Use lambdas when you need a short function, or one with read/write access to local variables!
* Use function pointers for longer logic and for overloading.(复杂逻辑和函数指针)



#### Operator()

another way to create function

```c++
class functor{
    public:
        int operator() (int arg) const{
            return num + arg;
        }
    private:
        int num;
};
```

A functor is any class that provides an implementation of operator().



### functors, lambdas, and function pointers relate

std::function is a complex, heavy, expensive, magical type that can hold any callable entity.



## Midquarter Review

### References

bug:

```txt
unable to deduce 'std::initializer_list<auto>' from '{{1, 1}}'
```

```c++
auto my_nums = {{1,1}};
// 直接初始化vector存在的问题，编译器可能不够高级
```





## Operator Overloading

### Time demo

Why are the arguments const?

```c++
bool before(const Time& a, const Time& b) {
    if (a.getHours() < b.getHours()) return true;
    if (b.getHours() < a.getHours()) return false;
    // otherwise, compare minutes
    if (a.getMinutes() < b.getMinutes()) return true;
    if (b.getMinutes() < a.getMinutes()) return false;
    // otherwise compare seconds...
}
```

const 引用传参

1.使用const可以避免无意中修改数据的编程错误

2.使用cosnt使函数能够处理const和非const实参，否则将只能接受非const数据，因为一个const数据是不能赋给一个非const数据的

3.使用const引用使函数能够正确生成并使用临时变量。



### Overload operators

* member functions
* non-member functions 



### 函数重载与运算符重载

C++ 允许在同一作用域中的某个**函数**和**运算符**指定多个定义，分别称为**函数重载**和**运算符重载**。

#### 函数重载

```c++
 public:
      void print(int i) {
        cout << "整数为: " << i << endl;
      }
 
      void print(double  f) {
        cout << "浮点数为: " << f << endl;
      }
 
      void print(char c[]) {
        cout << "字符串为: " << c << endl;
      }
```

#### 运算符重载

重载的运算符是带有特殊名称的函数，函数名是由关键字 operator 和其后要重载的运算符符号构成的。与其他函数一样，重载运算符有一个返回类型和一个参数列表。

可分为成员与非成员，成员函数可访问内部变量：

#### ++运算符的重载

 为了区分这两者，观察到以下规则：运算符的前缀形式与任何其他一元运算符的声明方式完全相同：后缀表单接受类型 **`int`**的额外参数。

表示递增或递减运算符的后缀形式的参数 **`int`** 通常不用于传递参数。 它通常包含值 0。 但是，可按以下方式使用它：

```c++
Int Int::operator++( int n )
{
    Int result = *this;
    if( n != 0 )    // Handle case where an argument is passed.
        _i += n;
    else
        _i++;       // Handle case where no argument is passed.
    return result;
}
```

#### friend

You may be wondering how non-member functions can access private member variables! 

让非成员函数访问私有变量。

#### << operator 重载

* 重新定义输入



## Special Member Function

covers the barebones 涵盖了基础的class

### Question

* What are special member functions? When are they called? 
* When should we declare a special member function? 
* When should we not declare a special member function? 
* What are modern standards for SMFs? How do they differ from classic, C++98 standards?



### What

These functions are generated only when they're called (and before any are explicitly defined by you)

联想单例设计模式：

* 懒汉：使用时完成初始化
* 饿汉：加载时完成初始化

构造器的更多玩法



### 组成

#### copy constructor

> 用Java里的话说 深拷贝 deep copy

####  copy assignment operator

=，创造对象，类似深拷贝

#### destructor

can be used for deallocating member variables and avoiding memory leaks



### Depth

### initializer list

> 创建即分配

```c++
template <typename T>
vector<T>::vector<T>() :  // 注意:
_size(0),_capacity(kInitialSiz),
_elems(new T[kInitialSize]){ }
```

* Faster
* 不可分配咋办？



#### Why aren’t sufficient?

* default:manually copying each member variable
* Moral of the story: in many cases, copying is not as simple as copying each member variable!
* Both copy and vec will point to the same underlying array!



#### How to fix?

1. initializer list 
2. 处理 edge cases;
3. return 自己的指针



#### 拷贝总结

1. Check for self-assignment
2. Make sure to free existing members
3. Copy assign （automatically assignable member）
4. Manually coy all members
5. Return a reference to *this



### Prevent copies

keep SMFs if you're overwriting them

```c++
PasswordManager(const PasswordManager& rhs) = delete;
PasswordManager& operator = (const PasswordManager& rhs) = delete;
```

告诉编译器自己不用他们的拷贝

如果是重写拷贝

```c++
Widget (const Widget& w) = default;
```

after a function prototype tells C++ to still generate the default SMF, even if you're defining other SMFs!



### When should we rewrite SMFs?

….



## Move Sematics in C++

> C++ 11 

A fancy way to say “how can we avoid making unnecessary copies of resources?”

数据移动而非拷贝！



### l-values vs R-values

* name
* not temporary

l-values live until the end of the scope , r-values live until the end of the line

#### r-value reference `&&`

让函数参数传递r-value变为可能

```c++
void change(int&& num){
    cout << "r-value:"<< num << " is changed" <<endl;
}

void change(int& num){
    cout << "l-value:"<< num << " is changed" <<endl;
}
```

![image-20220803223030760](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/image-20220803223030760.png)

### steal the array

* cast `l-value` to `r-value`

```c++
change(std::move(x));
```

![image-20220803230834696](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/image-20220803230834696.png)

### warning

look at this code:

```c++
int main() {
    vector<string> vec1 = {“hello”, “world”}
    vector<string> vec2 = std::move(vec1);
    vec1.push_back(“Sure hope vec2 doesn’t see this!”)
        // mistake 
}
```

* After a variable is moved via std::move, it should never be used until it is reassigned to a new variable! 

*  The C++ compiler might warn you about this mistake, but the code above compiles!



#### When

Don’t use std::move outside of **class definitions**, never use it in **application code**!





## Type Safety and std::optional

> How can we use c++’s type system to prevent errors at compile time?

const :

* 将大块数据传递给函数，来避免复制。
* 防止不经意间或者偷偷将数据改变 (accidentally or sneakily changes)



### Prevent type error

> How to prevent the  back when use {} ?

`std::vector back: {}`

Returns a reference to the last element in the container.

Calling `back` on an empty container causes [undefined behavior](https://en.cppreference.com/w/cpp/language/ub).

Undefined behavior: Function could crash, could give us garbage, could accidentally give us some actual value

#### first solution:

```c++
template <typename valuetype>
std::pair<bool, valuetype&> vector<valuetype>::back(){
    if(empty()){
        return {false, valuetype()};
    }
    return {true, *(begin() + size() -1)};
}
```

* 受返回值类型影响，如果是void中呢？



### Introducing std::optional

std::optional is a **template class** which will either contain a value of type T
or **contain nothing** (expressed as nullopt)



#### interface

```c++
std::optional<Student> lookupStudent(string name){//something}
std::optional<Student> output = lookupStudent(“Keith”);
    
if(student.has_value()){
	cout << output.value().name << “ is from “ <<
	output.value().state << endl;
} else {
	cout << “No student found” << endl;
}
```

* .value :
  * contained value or throws bad_optional_access
* .value_or(value type val) 
  * returns the contained value or default value, parameter val
* .has_value()
  * returns true if contained value exists, false otherwise



#### back use 进化

```c++
void removeOddsFromEnd(vector<int>& vec){
	while(vec.back().value() % 2 == 1){
		vec.pop_back();
	}
}
```

如果vetor是空值，将抛出错误 ` bad_optional_access`

如何改进？？？

```c++
while(vec.back().has_value() && vec.back().value() % 2 ==1) 
```

too long

```c++
while(vec.back() && vec.back().value() % 2 ==1)
```

hacky 写法

```c++
while(vec.back().value_or(2) % 2 == 1)
    // Totally hacky, but totally works ;) don’t do this ;)
```



## RAII, Smart Pointers, and C++ Project Building

### 课前预习

#### RALL:

> Resource Acquisition Is Initialization 获取资源立即初始化

C++语言局部对象自动销毁的特性来控制资源的生命周期

```c++
class  MutexLock {
 public:
  explicit MutexLock(Mutex *mu) // RALL
      : mu_(mu)  {
    this->mu_->Lock();
  }
  ~MutexLock() { this->mu_->Unlock(); }

 private:
  Mutex *const mu_;
  // No copying allowed
  MutexLock(const MutexLock&) = delete;
  void operator=(const MutexLock&) = delete;
};
```

#### Smart Pointer

* 智能释放空间

Since the destructor is automatically called when an object goes out of scope, the dynamically allocated memory would automatically be deleted (or reference count can be decremented).

* 模板类的实现

```c++
#include <iostream>

using std::cout; using std::endl;

template <class T>
class SmartPtr{
    T* ptr;
    public:
        explicit SmartPtr(T* p = NULL) : ptr(p){cout << "Initial ptr" <<endl; }
        ~SmartPtr(){ cout << "delete the ptr" << endl;delete (ptr);}
        T& operator*(){ return *ptr;}
        T* operator->() {return ptr;}
};   

int main(int argc, char const *argv[])
{
    SmartPtr<int> ptr(new int());
    *ptr = 20;
    cout << "*ptr: "<< *ptr << endl;
    return 0;
}

```

![image-20220804165937807](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/image-20220804165937807.png)



#### 真实的智能指针

##### **1.unique_ptr**

![image-20220804174021812](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/image-20220804174021812.png)

```c++
#include <iostream>
#include <memory>

using std::cout; using std::endl;

class Rectangle{
    int length;
    int breadth;

    public:
        Rectangle(const int &l,const int &b) 
            : length(l), breadth(b){ 
                cout << "Initial SPTR" << endl;
            }
        
        ~Rectangle(){
            cout << "destroy the SPTR" << endl;
        }
        
        const int area(){
            return length * breadth;
        }
};

int main(int argc, char const *argv[])
{
    std::unique_ptr<Rectangle> P1(new Rectangle(10,5));
    cout << P1->area() << endl;

    std::unique_ptr<Rectangle> P2;
    // 转移而不是拷贝
    P2 = std::move(P1);

    // 对比P1 和 P2
    cout << "P2: " << P2->area() << endl;
    // 段错误直接退出程序
    // cout << "P1: " <<P1->area() << endl;

    return 0;
}

```

![image-20220804174519536](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/image-20220804174519536.png)

可以看出，P2并未使用构造函数生成新的空间，而且P2完美复刻了P1。



##### 2.shared_ptr

![image-20220804174711101](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/image-20220804174711101.png)

```c++
std::shared_ptr<Rectangle> P1(new Rectangle(10, 5));
std::shared_ptr<Rectangle> P2 = P1;

cout << "P1:" << P1->area() << endl;
cout << "P2:" << P2->area() << endl;

cout << "count:" << P1.use_count() << endl;
```

![image-20220804175034761](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/image-20220804175034761.png)



### Problem

Takeaway: there are often more code paths than meets the eye!

#### What’s Wrong

```c++
string get_name_and_print_sweet_tooth(int id_number) {
     Person* p = new Person(id_number); // copy waste
        // assume the constructor fills in variables
    if (p->favorite_food() == "chocolate" ||
         p->favorite_drink() == "milkshake") {
            cout << p->first() << " "
             << p->last() << " has a sweet tooth!" << endl;
}
    auto result = p->first() + " " + p->last();
    delete p;
    return result;
}
```



### Smart Pointer

#### Another way

> * make_unique<> 
>
> * make_shared<>

**Always use std::make_unique()!**



### Program Progrcess

.cpp -> .s -> .o -> executable

1. cpp语言文件编译生成汇编代码

2. 汇编代码转换为二进制代码
3. 从多个对象文件发创建一个可执行文件
   1. 合并程序的各个部分  (combine)
   2. memory organizayion 创建新的内存结构
   3. 解决引用问题

![image-20220804182630167](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/image-20220804182630167.png)



## Interesting Problems?

### using namespace std?

* its effect is limited to the function or class scope(范围).
* 版本迭代再更改的问题，不方便自己修改
* 适配别人的修改和合作

![image-20220705161206556](https://personal-drawing-bed.oss-cn-beijing.aliyuncs.com/img/image-20220705161206556.png)




















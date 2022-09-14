// 定义函数类型
typedef int F(void);

F f, g;
//相当于声明
//int f(void);
//int g(void);

// 写法错误，函数不能返回函数类型
//F h(void);

//可以返回函数指针
F *e(void);

// * 放在（）中时，声明了一个函数指针，而不是返回了
int (*fp)(void);
// 声明了一个返回值为int，参数为void的函数指针
// 也可以这样声明
F *fp;


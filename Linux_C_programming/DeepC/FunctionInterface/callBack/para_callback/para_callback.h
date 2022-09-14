#ifndef PARA_CALLBACK_H
#define PARA_CALLBACK_H

// 定义函数指针
typedef void (*callback_t)(void *);
// 只定义了参数类型
extern void repeat_three_times(callback_t, void *);

#endif
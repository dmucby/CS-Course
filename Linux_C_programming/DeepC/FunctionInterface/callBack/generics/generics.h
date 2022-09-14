#ifndef GENERICS_H
#define GENERICS_H

// 此时*cmp_t为函数指针， int * cmp_t这时候则表示返回值为int *
typedef int (*cmp_t) (void *, void *);
extern void *max(void *data[], int num, cmp_t cmp);

#endif
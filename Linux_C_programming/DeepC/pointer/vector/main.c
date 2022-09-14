#include <stdio.h>

char a[4][3][2] = {
    {{'a','b'},{'c','d'},{'e','f'}},
    {{'g','h'},{'i','j'},{'k','l'}},
    {{'m','n'},{'o','p'},{'q','r'}},
    {{'s','t'},{'u','v'},{'w','x'}},
};

char (*pa)[2] = &a[1][0];
// 从g开始
char (*ppa)[3][2] = &a[1];
// [0]从g、h开始 [1]从2开始

int main(int argc, char const *argv[])
{
    char res1 = *((*pa)+11);
    printf("%c\n",res1);
    char res2 = *((*ppa)[1]+9);
    // *(*ppa)[0]+11) 也对
    printf("%c\n",res2);
    return 0;
}

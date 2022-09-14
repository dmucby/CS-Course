#include <stdio.h>
#include <assert.h>
#include <string.h>

// 防止内存重叠
char *my_memcpy(char *dest, const char* src, int cnt){
    assert(dest != NULL && src != NULL);
    char *ret = dest;

    // 解决办法为从后往前开始复制，这样不会保证/0丢失
    if(dest >= src && dest <= src+cnt-1){
        dest = dest + cnt - 1;
        src = src + cnt -1;
        while(cnt--)
            *dest-- = *src--;
    }else{
        while(cnt --)
            *dest++ = *src++;
    }
    return ret;
}

char *my_strcpy(char *dest, const char *src){
    assert(dest != NULL && src != NULL);
    char *ret = src;
    my_memcpy(dest, src, strlen(src)+1);
    return ret;
};

int main(int argc, char const *argv[])
{
    char buf[10];
    my_strcpy(buf,"abc");
    printf("%s\n", buf);    
    return 0;
}

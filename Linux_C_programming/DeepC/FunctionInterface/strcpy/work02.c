#include <stdio.h>
#include <string.h>
#include <assert.h>

// char *my_replace(char *src, char obj, size_t n){
//     //assert (src != NULL && obj != NULL);
//     char *ret = src;
//     while(*ret!=NULL) {
//         if (*ret == obj)
//         {
//             *ret = ' ';
//         }
//     }
//     return ret;
// }   

char *shrink_space(char *dest, const char *src, size_t n){
    assert (dest != NULL && src != NULL);
    char *ret = dest;
    while(*ret == '\t' ||*ret == '\n' || *ret == '\r')
        *ret = ' ';
    return ret;
}

int main(int argc, char const *argv[])
{
    char *obj = 
    "This Content hoho        is ok"
    "    ok?"
    ""
    "    file system"
    "uttered words   ok ok      ?"
    "end.";
    char res[100];
    printf("%s\n",shrink_space(obj,res,strlen(obj)));
    return 0;
}

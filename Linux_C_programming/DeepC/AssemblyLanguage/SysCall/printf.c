#include<string.h>
#include<unistd.h>

int main(int argc, char const *argv[])
{
    char *msg = "Hello, world!\n";
    write(1, msg, strlen(msg));

    return 0;
}

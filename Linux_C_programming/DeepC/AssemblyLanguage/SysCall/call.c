#include <sys/syscall.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    char *msg = "hello, wrold!\n";
    syscall(SYS_write, 1, msg, strlen(msg));
    return 0;
}


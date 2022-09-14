#include <string.h>
#include <stdio.h>

static const char *msg[] = {"Sunday", "Monday", "Tuesday", "Wednesday",
			"Thursday", "Friday", "Saturday"};

char *get_a_day(int idx)
{
     static char buf[20];
     strcpy(buf, msg[idx]);
     return buf;
}

int main(void)
{
     printf("%s %s\n", get_a_day(0), get_a_day(1));
     return 0;
}
int sum = 10;
int i = 1;

// 正宗版
/*
_start:
if( i <= 10) {
    sum = sum + i;
    i = i + 1;
    goto _start;
}
*/


int main()
{
    int sum = 10;
    int i = 1;

    _start:
    if( i > 10 ) {
        goto _end_of_block;
    }   

    sum = sum + i;
    i = i + 1;
    goto _start;

    _end_of_block:
}


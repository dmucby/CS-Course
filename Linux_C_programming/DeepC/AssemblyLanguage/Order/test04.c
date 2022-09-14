int main(int argc, char const *argv[])
{
    int a = 1;
    int b = 2;

    //实际上为汇编的jmp指令
    goto gun_kai;

    a = a + b;

gun_kai:
    return a;
}

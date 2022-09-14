#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#define MAX_LINE 128

//初始化buf用'\0'填充
void ainit( char *buf )
{
    int i;
    for ( i = 0; i< MAX_LINE; i++ ) {
        buf[i] = '\0';
    }
}
//将'\n'替换为'\0'
void adell( char *buf )
{
    int i;
    for ( i = 0; buf[i] != '\0'; i++ ) {
        if ( buf[i] =='\n' ) {
            buf[i] = '\0';
        }
    }
}
//去除数组中的'\t','空格'
void abdel( char *buf )
{
    int i = 0, j = 0;
    for ( ; buf[i] != '\0'; i++ ) {
        if ( (buf[i] == '\t') || (buf[i] == ' ') ) {
            j = i;
            while ( buf[j] != '\0' ) {
                buf[j] = buf[j + 1];
                j++;
            }
            i = i - 1;
        }
    }
}

int main(){
    FILE *fp_xml,*fp_ini;
    //定义缓冲区
    char buf[MAX_LINE] = {0};
    char dbuf[MAX_LINE] = {0};
    char ch = '\0';
    char *kv, *key, *val;
    int i = 0;

    fp_ini = fopen("test.ini","r+");
    fp_xml = fopen("test.xml","w+");

    if ( fp_ini == NULL ) {
            perror("config.ini is not exist\n");
            exit(1);
    }
    if ( fp_xml == NULL  ) {
        perror("xml is not exist\n");
        exit(1);
    }

    // 一个字符的从缓冲区读
    while((ch = fgetc(fp_ini)) != EOF){
        if(ch == ';'){
            //该语句控制</http>的打印和两个section之间的空一行
            if( buf[0] != '\0' )//只有，此时已经生成了<http>,由buf缓存，且并未flush时才成立
                                //所以该条件旨在生成第二个,即</http>
                fprintf( fp_xml, "</%s>\n\n", buf );//多空一行
                //由于这个语句包含在while中（;语句转换之前），所以最后一个</xxxx>无法生成
                //见line:105来生成
            //然后再进行;部分的转换
            fgets(buf,MAX_LINE,fp_ini);
            adell(buf);//将;Configuration of http\n后跟的\n去掉，后面我们自己控制换行
            fprintf(fp_xml,"<!-- %s -->\n",buf);
            
            fflush(NULL);//该语句是确保所有文件写操作从cache传给内核到磁盘，不写也不影响
            continue;
        }
        if(ch == '['){
            ainit(buf);//继续利用buf进行[http]的转换
            for(i = 0; (ch = fgetc(fp_ini)) != ']'; i++ ) {
                buf[i] = ch;//注意此处不能用fgets，我们只要[]内部的字符，不要括号和\n
            }
            fprintf(fp_xml,"<%s>\n",buf);
            fflush(NULL);
            continue;
        }
        if((ch != '\n') && (ch != '\t') && (ch != ' ')){
            ainit(dbuf);
            fgets(dbuf,MAX_LINE,fp_ini);//此处便可以直接获一行
            // for( i = 0; (ch = fgetc(fp_ini)) != '\n' ; i++ ) {
            //     dbuf[i] = ch;
            // }//如果用这个循环来给dbuf赋值会gdb提示索引越界,未找到原因
            adell(dbuf);//同上
            abdel(dbuf);//前面如果有\t，以及中间夹的空格等都去掉

            kv = dbuf;
            key = strtok(kv,"=");
            val = strtok(NULL,"=");//获取第一次截去后剩余的字符串
            fprintf(fp_xml,"   <%c%s>%s</%c%s>\n",ch,key,val,ch,key);//注意此处ch
            //当前fgets读取的一行少了第一个字符（他被while中ch = fgetc()读去了，所以要把它补上）
            fflush(NULL);
            continue;
        }
    }

    fprintf(fp_xml, "</%s>", buf);
    fclose(fp_xml);
    fclose(fp_ini);
    return 0;
}
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

int main(void)
{
    FILE *fp;
    if ( (fp = fopen("test.txt","a+")) == NULL){
        printf("error open file test.txt!\n");
        exit(1);
    }

    time_t t;
    struct tm *info;
    //char *infostr;
    long num;
    char lastLine[512];

    fseek(fp, 0, SEEK_SET);
    while (!feof(fp))
    {
        memset(lastLine, 0x00, 512);
        fscanf(fp, "%[^\n]\n",lastLine);
    }
    printf("lastLine:%s\n",lastLine);

    // atoi 转化为整型
    num = atoi(lastLine) > 0 ? atoi(lastLine+1) : 1;
    printf("last num: %ld\n", num);

    while(1) {
        // 获取当前时间
        t = time(NULL);
        // 将时间转换为本地时间 并转换为struct tm*类型
        info = localtime(&t);
        //infostr = asctime(info);
         fprintf(fp,"%ld\t %d-%d-%d  %d:%d:%d\n", num, 1900+info->tm_year, 1+info->tm_mon, info->tm_mday,info->tm_hour,info->tm_min,info->tm_sec);
        printf("[INFO]: %ld\t %d-%d-%d  %d:%d:%d\n", num, 1990+info->tm_year, 1+info->tm_mon, info->tm_mday,info->tm_hour,info->tm_min,info->tm_sec);   
        num++;
        fflush(fp);
        // 睡眠
        sleep(1);
    }

    fclose(fp);
    return 0;
}
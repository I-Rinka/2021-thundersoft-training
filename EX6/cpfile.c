#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
int main(int argc, char *argv[])
{
    FILE *source, *target;
    if (argc != 3)
    {
        printf("command line: cpfile.out <source> <target>\n");
        return -1;
    }
    if ((source = fopen(argv[1], "rb")) == NULL)
    {
        printf("file %s cannot open.\n", argv[1]);
        return -1;
    }
    //wb+:以二进制并且如果无则创建来打开文件
    if ((target = fopen(argv[2], "wb+")) == NULL)
    {
        printf("file %s cannot create.\n", argv[2]);
        return -1;
    }
    
    char c;
    while ((c=fgetc(source))!=EOF)
    {
        fputc(c,target);
    }
    fclose(source);
    fclose(target);
    return 0;
}
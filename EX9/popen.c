#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#define BUFSIZE 1024

int main()
{
    FILE *fp;
    char *cmd = "ls -l";
    char buf[BUFSIZE];
    if (NULL == (fp = popen(cmd, "r")))
    {
        printf("Popen error\n");
        exit(1);
    }

    while (NULL != (fgets(buf, BUFSIZE, fp)))
    {
        printf("%s", buf);
    }
    printf("\n");
    pclose(fp);
    exit(0);
}

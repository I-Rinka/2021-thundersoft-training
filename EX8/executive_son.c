#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int son_process(const char *command)
{
    execl("/bin/bash","bash","-c", command, NULL);
    return 0;
}

int main()
{
    char *command = (char *)malloc(sizeof(char) * 100);
    while (1)
    {
        printf("please input the command line:\n");
        fgets(command,100,stdin);
        
        int pid;
        pid = fork();

        if (pid == 0)
        {
            //son
            return son_process(command);
        }
        else
        {
            int rt_val = -1;
            wait(&rt_val);
            printf("Son ended, return value is:%d\n", rt_val);
        }
    }
    return 0;
}
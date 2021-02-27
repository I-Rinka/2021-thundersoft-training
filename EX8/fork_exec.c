#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
int main()
{
    int pid;
    pid = fork();
    switch (pid)
    {
    case -1:
        printf("fork fail!\n");
        exit(1);
    case 0:
        printf("Is son:\n");
        execl("/bin/ls", "ls", "-l", NULL);
        printf("exec fail\n");
        exit(1);
    default:
        printf("Is parent:\n");
        wait(NULL);
    }
    printf("The code that will be executed after son quites\n");
    return 0;
}
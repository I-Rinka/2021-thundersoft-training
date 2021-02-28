#include <signal.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <stdio.h>
void async_function()
{
    printf("开车了！\n");
    // signal(SIGRTMIN + 1, async_function);
}
int child_process()
{
    signal(SIGRTMIN + 1, async_function);
    while (1)
    {
    }
}

int main(int argc, char const *argv[])
{

    int pid;
    pid = fork();

    if (pid == 0)
    {
        child_process();
    }
    else
    {
        while (1)
        {
            getchar();
            kill(pid, SIGRTMIN + 1);
        }

        wait(NULL);
    }

    return 0;
}

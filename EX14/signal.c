#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
void waiting();
void stop();
int wait_mark;
int main()
{
    int p1, p2, stdout;
    while (-1 == (p1 = fork()))
        ;
    if (0 < p1)
    {
        while (-1 == (p2 = fork()))
            ;
        if (0 < p2)
        {
            wait_mark = 1;
            signal(SIGINT, stop);
            waiting();
            kill(p1, 16);
            kill(p2, 17);
            wait(0);
            wait(0);
            printf("Parent process is killed!\n");
            exit(0);
        }
        else
        {
            sigset_t sigs;
            sigemptyset(&sigs);
            sigaddset(&sigs, SIGINT);
            sigprocmask(SIG_BLOCK, &sigs, 0);
            wait_mark = 1;
            signal(17, stop);
            waiting();
            lockf(stdout, 1, 0);
            printf("Child process 2 is killed by parent!\n");
            lockf(stdout, 0, 0);
            exit(0);
        }
    }
    else
    {
        sigset_t sigs;
        sigemptyset(&sigs);
        sigaddset(&sigs, SIGINT);
        sigprocmask(SIG_BLOCK, &sigs, 0);
        wait_mark = 1;
        signal(16, stop);
        waiting();
        lockf(stdout, 1, 0);
        printf("Child process 1 is killed by parent!\n");
        lockf(stdout, 0, 0);
        exit(0);
    }
}
void waiting()
{
    while (0 != wait_mark)
        ;
}
void stop()
{
    wait_mark = 0;
}
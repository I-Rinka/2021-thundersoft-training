#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <syslog.h>
#include <signal.h>
#include <sys/param.h>
#include <sys/stat.h>

void init_daemon()
{
    int pid;
    int i;

    openlog("init_daemon log", LOG_PID, LOG_DAEMON);

    if (0 < (pid = fork()))
    {
        exit(0);
    }
    else if (0 > pid)
    {
        syslog(LOG_INFO, "create daemon fail.");
        exit(-1);
    }

    setsid();
    chdir("/tmp");
    umask(0);
    for (i = 0; i < NOFILE; i++)
    {
        close(i);
    }
}

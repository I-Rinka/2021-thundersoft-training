#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <syslog.h>
#include <signal.h>
#include <sys/param.h>
#include <sys/stat.h>
void init_daemon(void);
int main()
{
    FILE *fp;
    pid_t child;
    init_daemon();
    openlog("", LOG_PID, LOG_DAEMON);
    child = fork();
    if (0 > child)
    {
        syslog(LOG_INFO, "fail to create child process");
        exit(1);
    }
    else if (0 == child)
    {
        while (1)
        {
            sleep(5);
            if (0 < (fp = fopen("test.txt", "a+")))
            {
                fprintf(fp, "Test daemon.\n");
                fclose(fp);
            }
            else
            {
                syslog(LOG_INFO, "fail to open file");
            }
        }
    }
    else
    {
        waitpid(child, NULL, 0);
        while (1)
        {
            sleep(10);
        }
    }
    return 0;
}

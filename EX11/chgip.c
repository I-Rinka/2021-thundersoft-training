#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <syslog.h>
#include <signal.h>
#include <sys/param.h>
#include <sys/stat.h>
int main()
{
	FILE *fp;
	pid_t child;
	int i, lines, line, move;
	char str[20], command[100];
	child = fork();
	if (child > 0)
		exit(0);
	else if (child < 0)
	{
		perror("创建子进程失败");
		exit(1);
	}
	setsid();
	chdir("/tmp");
	umask(0);
	for (i = 0; i < NOFILE; ++i)
		close(i);
	fp = fopen("ip.txt", "r");
	if (fp <= 0)
	{
		perror("打开文件失败");
		exit(1);
	}
	lines = 0;
	while (fscanf(fp, "%s", &str) != EOF)
	{
		lines++;
	}
	while (1)
	{
		fseek(fp, 0, SEEK_SET);
		srand((int)time(0));
		line = rand() % lines + 1;
		move = 0;
		while (move < line)
		{
			fscanf(fp, "%s", &str);
			move++;
		}
		strcpy(command, "ifconfig eth0 ");
		strcat(command, str);
		strcat(command, " netmask 255.255.255.0");
		system(command);
		sleep(5 * 60);
	}
	fclose(fp);
	return 0;
}

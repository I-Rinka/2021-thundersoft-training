#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main()
{
	int fd[2], fd1[2];
	pid_t pid, pid1;
	char r_buf[100];
	char w_buf[100];
	char buf[100];
	char *p_wbuf;
	int r_num, i;

	memset(r_buf, 0, sizeof(r_buf));
	memset(w_buf, 0, sizeof(w_buf));

	p_wbuf = w_buf;

	if (pipe(fd) < 0)
	{
		printf("pipe create error!\n");
		return -1;
	}
	if (pipe(fd1) < 0)
	{
		printf("pipe fd1 create error!\n");
		return -1;
	}
	if ((pid = fork()) == 0)
	{
		close(fd[1]);
		close(fd1[0]);
		sleep(3);
		if ((r_num = read(fd[0], r_buf, 100)) > 0)
		{

			printf("%d num from to pipe is %s\n", r_num, r_buf);
		}
		for (i = 0; i < r_num; i++)
		{
			r_buf[i] -= 32;
		}
		if (write(fd1[1], r_buf, r_num) == -1)
		{
			printf("write fd1[1] successful!\n");
		}
		close(fd1[1]);
		close(fd[0]);
		exit(0);
	}
	else if (pid > 0)
	{
		close(fd[0]);
		close(fd1[1]);
		sleep(1);
		printf("input string:");
		scanf("%s", w_buf);
		write(fd[1], w_buf, strlen(w_buf));

		sleep(1);
		if (r_num = read(fd1[0], w_buf, 100) > 0)
			;
		{

			printf("from to pipe is %s\n", w_buf);
		}

		close(fd[1]);
		close(fd1[0]);
		waitpid(pid, NULL, 0);
	}
	return 0;
}

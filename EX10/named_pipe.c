#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#define FIFO_NAME "/tmp/myfifo"
#define FILEMODE (S_IRUSR | S_IWUSR)
int w_process()
{
    int fd;
    int w_num;
    if (access(FIFO_NAME, R_OK) < 0)
    {
        printf("cannot write fifo...\n");
        return 1;
    }
    fd = open(FIFO_NAME, O_WRONLY);
    if (fd == -1)
    {
        printf("open %s for write error\n", FIFO_NAME);
        return 1;
    }
    w_num = write(fd, "abcdg\0", 6);
    printf("Write %d bytes to %s\n", w_num, FIFO_NAME);
    return 0;
}

int r_process()
{
    char r_buf[50];
    int fd;
    int r_num;
    if (access(FIFO_NAME, R_OK) < 0)
    {
        printf("cannot read fifo...\n");
        return 1;
    }
    fd = open(FIFO_NAME, O_RDONLY);
    if (fd == -1)
    {
        printf("open %s for read error\n", FIFO_NAME);
        return 1;
    }
    r_num = read(fd, r_buf, 6);
    printf("Get %d bytes \"%s\" from %s\n", r_num, r_buf, FIFO_NAME);
    unlink(FIFO_NAME);
    return 0;
}

int main()
{
    char r_buf[50];
    int fd;
    int r_num;
    if ((mkfifo(FIFO_NAME, FILEMODE) < 0) && (errno != EEXIST))
    {
        printf("cannot create fifo...\n");
        return 1;
    }

    int pid = fork();

    if (pid == 0) //子进程
    {
        r_process();
    }
    else
    {
        w_process();
        wait(NULL);


        //检查是否tmp中是否没有myfifo了
        execl("/bin/ls", "ls", FIFO_NAME, "-l", NULL);
    }

    return 0;
}

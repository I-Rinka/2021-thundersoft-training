#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#define FIFO_NAME "/tmp/myfifo"
#define FILEMODE (S_IRUSR | S_IWUSR)
int main()
{
    int fd;
    int w_num;
    if ((mkfifo(FIFO_NAME, FILEMODE) < 0) && (errno != EEXIST))
    {
        printf("cannot create fifo...\n");
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

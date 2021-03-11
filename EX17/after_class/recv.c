#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stddef.h>
#include <time.h>
#define IPC_KEY 0x12345678

#define TYPE_S 1
#define TYPE_C 2
struct msgbuf
{
    long mtype;       /* message type, must be > 0 */
    char mtext[1024]; /* message data */
};

int main()
{
    time_t timer;
    struct tm *tblock;
    timer = time(NULL);
    tblock = localtime(&timer);
    int msgid = -1;

    msgid = msgget(IPC_KEY, IPC_CREAT | 0664);
    if (msgid < 0)
    {
        perror("massage get error");
        return -1;
    }
    while (1)
    {

        struct msgbuf buf;

        msgrcv(msgid, &buf, 1024, TYPE_C, 0);
        printf("client information:%s\n%s", buf.mtext, asctime(tblock));

        memset(&buf, 0x00, sizeof(struct msgbuf));
        buf.mtype = TYPE_S;
        scanf("%s", buf.mtext);
        msgsnd(msgid, &buf, 1024, 0);
    }
    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}

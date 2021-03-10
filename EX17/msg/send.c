#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/msg.h>
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
    int msgid = -1;

    //create queue
    msgid = msgget(IPC_KEY, IPC_CREAT | 0664);
    if (msgid < 0)
    {
        perror("msgget error");
        return -1;
    }
    while (1)
    {
        struct msgbuf buf;

        //send data
        memset(&buf, 0x00, sizeof(struct msgbuf));
        buf.mtype = TYPE_C;
        scanf("%s", buf.mtext);
        msgsnd(msgid, &buf, 1024, 0);

        msgrcv(msgid, &buf, 1024, TYPE_S, 0);
        printf("server say[%s]\n", buf.mtext);
    }
    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#define MSGKEY 75

struct msgform
{
    long mtype;
    char mtext[1000];
} msg;

int msgqid;

void client()
{
    int i;
    msgqid = msgget(MSGKEY, 0777);
    for (i = 10; i >= 1; i--)
    {
        msg.mtype = i;
        printf("(client)sent.\n");
        msgsnd(msgqid, &msg, 1024, 0);
    }
    exit(0);
}

int main()
{
    client();
    return 0;
}

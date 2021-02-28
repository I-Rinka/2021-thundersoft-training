#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/sem.h>
#define KEY (key_t)14010322
union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *arry;
};
static int sem_id = 0;
static int set_semvalue();
static void del_semvalue();
static void semaphore_p();
static void semaphore_v();
int main(int argc, char *argv[])
{
    int semid;
    int product = 1;
    int i;
    if (-1 == (semid = semget(KEY, 3, IPC_CREAT | 0660)))
    {
        printf("ERROR\n");
        return -1;
    }
    union semun arg[3];
    arg[0].val = 1; //mutex = 1;
    arg[1].val = 5; //empty = 5;
    arg[2].val = 0; //full = 0;
    for (i = 0; i < 3; i++)
    {
        semctl(semid, i, SETVAL, arg[i]);
    }
    for (i = 0; i < 3; i++)
    {
        printf("The semval(%d) = %d\n", i, semctl(semid, i, GETVAL, NULL));
    }
    pid_t p1, p2;
    if (0 == (p1 = fork()))
    {
        while (1)
        {
            semaphore_p(semid, 1); //P(empty)
            printf("1\n");
            semaphore_p(semid, 0); //P(mutex)
            printf("2\n");
            product++;
            printf("Producer %d: %d things", getpid(), product);
            semaphore_v(semid, 0); //V(mutex);
            semaphore_v(semid, 2); //V(full);
            sleep(2);
        }
    }
    else
    {
        if (0 == (p2 = fork()))
        {
            while (1)
            {
                sleep(2);
                semaphore_p(semid, 2); //p(full)
                printf("3\n");
                semaphore_p(semid, 0); //p(mutex)
                printf("4\n");
                product--;
                printf("Consumer1 %d: %d things", getpid(), product);
                semaphore_v(semid, 0); //v(mutex)
                semaphore_v(semid, 1); //v(empty)
                sleep(5);
            }
        }
        else
        {
            while (1)
            {
                sleep(2);
                semaphore_p(semid, 2); //p(full)
                printf("5\n");
                semaphore_p(semid, 0); //p(mutex)
                printf("6\n");
                product--;
                printf("Consumer2 %d: %d things", getpid(), product);
                semaphore_v(semid, 0); //v(mutex)
                semaphore_v(semid, 1); //v(empty)
                sleep(5);
            }
        }
    }
}
static void del_semvalue()
{
    union semun sem_union;
    if (-1 == semctl(sem_id, 0, IPC_RMID, sem_union))
        fprintf(stderr, "Failed to delete semaphore\n");
}
static int set_semvalue()
{
    union semun sem_union;
    sem_union.val = 1;
    if (-1 == semctl(sem_id, 0, SETVAL, sem_union))
        return 0;
    return 1;
}
void semaphore_p(int sem_id, int semNum)
{
    struct sembuf sem_b;
    sem_b.sem_num = semNum;
    sem_b.sem_op = -1; //P()
    sem_b.sem_flg = SEM_UNDO;
    if (-1 == semop(sem_id, &sem_b, 1))
    {
        fprintf(stderr, "semaphore_p failed\n");
    }
}
void semaphore_v(int sem_id, int semNum)
{
    struct sembuf sem_b;
    sem_b.sem_num = semNum;
    sem_b.sem_op = 1; //V()
    sem_b.sem_flg = SEM_UNDO;
    if (-1 == semop(sem_id, &sem_b, 1))
    {
        fprintf(stderr, "semaphore_p failed\n");
    }
}
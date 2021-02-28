#include <unistd.h>
#include <stdio.h>
#include <sys/sem.h>
#define KEY (key_t)14010322
void increase_and_print_sem_val(int semid)
{
    struct sembuf op;
    op.sem_num = 0;
    op.sem_op = +1;
    op.sem_flg = 0;
    semop(semid, &op, 1);
    int value = semctl(semid, 0, GETVAL, 0);
    printf("sem value of process %d :%d\n", getpid(), value);
}
void child_process(int semid)
{
    int pid = fork();
    if (pid == 0)
    {
        increase_and_print_sem_val(semid);
    }
    else
    {
        increase_and_print_sem_val(semid);
    }
}

int main(int argc, char const *argv[])
{
    int semid = -1;

    if (-1 == (semid = semget(KEY, 1, IPC_CREAT | 0660)))
    {
        printf("ERROR\n");
        return -1;
    }
    semctl(semid, 0, SETVAL, 0);
    int pid = fork();
    if (pid == 0)
    {
        child_process(semid);
    }
    else
    {
        child_process(semid);
    }

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
void thread(void *arg)
{
    int *number = (int *)((int **)arg);
    int i;
    for (i = 0; i < 3; i++)
    {
        *number += 1;
        printf("This is a pthread. number value:%d\n", *number);
    }
}
int main()
{
    pthread_t id;
    int i, ret;

    int *number = (int *)malloc(sizeof(int));
    *number = 0;

    ret = pthread_create(&id, NULL, (void *)thread, number);

    if (0 != ret)
    {
        printf("Create pthread error!\n");
        exit(1);
    }
    for (i = 0; i < 3; i++)
    {
        *number += 1;
        printf("This is the main process. number value:%d\n", *number);
        sleep(1);
    }
    pthread_join(id, NULL);
    return (0);
}

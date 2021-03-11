#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
void thread()
{
    int i;
    for (i = 0; i < 3; i++)
    {
        sleep(1);
        printf("This is a pthread.\n");
    }
}
int main()
{
    pthread_t id;
    int i, ret;
    ret = pthread_create(&id, NULL, (void *)thread, NULL);
    if (0 != ret)
    {
        printf("Create pthread error!\n");
        exit(1);
    }
    for (i = 0; i < 3; i++)
    {
        printf("This is the main process.\n");
    }
    pthread_join(id, NULL);
    return (0);
}

#include <stdio.h>
#include <stdlib.h>
int main()
{
    FILE *fp;
    int i, size = sizeof(int);
    if (NULL == (fp = fopen("temp.txt", "rb+")))
    {
        puts("Fail to open file!");
        exit(-1);
    }

    int num = 0;
    printf("input the size of the array:\n");
    scanf("%d", &num);
    if (num <= 0)
    {
        puts("Wrong size!");
        exit(-1);
    }

    int a[num], b[num];

    for (i = 0; i < num; i++)
    {
        scanf("%d", &a[i]);
    }

    fwrite(a, size, num, fp);

    rewind(fp);

    fread(b, size, num, fp);
    for (i = 0; i < num; i++)
    {
        printf("%d ", b[i]);
    }
    printf("\n");
    fclose(fp);
    return 0;
}

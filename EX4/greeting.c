#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main()
{
    void my_print(char *string);
    void my_print2(char *string);
    char my_string[] = "hello there";
    my_print(my_string);
    my_print2(my_string);
}
void my_print(char *string)
{
    printf("The string is %s\n", string);
}
void my_print2(char *string)
{
    char *string2;
    int size, size2, i;
    size = strlen(string);
    size2 = size - 1;
    string2 = (char *)malloc(size + 1);
    for (i = 0; i < size; i++)
    {
        string2[size2 - i] = string[i];
        string2[size + 1] = '0';
        printf("The string printed backwand is %s\n", string2);
    }
}

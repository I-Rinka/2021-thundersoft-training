#include <stdio.h>
#include <stdlib.h>
#include "sub.h"
int main()
{
    int a = 5;
    int b = 2;
    printf("a + b = %d.\n", add(a, b));
    printf("a - b = %d.\n", sub(a, b));
    return 0;
}

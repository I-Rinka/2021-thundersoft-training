#include <stdio.h>
#include "divide.h"
#include "minus.h"
#include "multiply.h"
#include "plus.h"
int main(int argc, char const *argv[])
{
    int a = 10, b = 2;
    printf("plus:%d\n", plus(a, b));
    printf("minus:%d\n", minus(a, b));
    printf("multiply:%d\n", multiply(a, b));
    printf("divide:%d\n", divide(a, b));
    return 0;
}

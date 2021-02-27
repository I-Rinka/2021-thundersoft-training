#include"minus.h"
int minus(int a, int b)
{
    return a - b;
}
#include"divide.h"
int divide(int a, int b)
{
    if (b != 0)
    {
        return a / b;
    }
    else
    {
        return __INT_MAX__;
    }
}
#include"plus.h"
int plus(int a,int b)
{
    return a+b;
}
#include"multiply.h"
int multiply(int a, int b)
{
    return a * b;
}
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
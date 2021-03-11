#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
int main()
{
  int p1 = fork();

  if (0 == p1)
  {
    int p2 = fork();

    if (0 == p2)
    {
      printf("c\n");
    }
    else
    {
      printf("b\n");
    }
  }
  else
  {
    printf("a\n");
  }

  return 0;
}

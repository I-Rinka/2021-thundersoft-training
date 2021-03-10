#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
int main()
{

  int pid1 = fork();

  if (0 == pid1)
  {
    int pid2 = fork();

    if (0 == pid2)
      printf("c\n");
    else
      printf("b\n");
  }
  else
  {
    printf("a\n");
  }

  return 0;
}

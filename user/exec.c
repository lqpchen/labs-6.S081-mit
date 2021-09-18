
// exec.c: replace a process with an executable file

#include "kernel/types.h"
#include "user/user.h"

int
main()
{
  char *argv[] = { "echo", "this", "is", "echo", 0 };

  //exec("echo", argv);
  exec(argv[0], argv);

  printf("exec failed!\n");

  exit(0);
}

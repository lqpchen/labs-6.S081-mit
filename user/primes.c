/*
Write a concurrent version of prime sieve using pipes. 
This idea is due to Doug McIlroy, inventor of Unix pipes. 
The picture halfway down this page: https://swtch.com/~rsc/thread/ 
and the surrounding text explain how to do it. 
Your solution should be in the file user/primes.c. 
*/

#include "kernel/types.h"
#include "user/user.h"

void
child(int *fds) 
{
  int num, first = 0, second = 0;
  int pfds[2];
  pipe(pfds);

  close(fds[1]);
  while(read(fds[0], &num, sizeof(num)) > 0) {
    if (first == 0) {
      printf("prime %d\n", num);
      first = num;
    } else if (num%first != 0) {
      //
      write(pfds[1], &num, sizeof(num));  
      if (second == 0) {
        second = num;
        if (fork() == 0) {
          child(pfds);
        } else {
          close(pfds[0]);
        }
      }
    }
  }
  close(fds[0]); // finish reading
  close(pfds[1]);
  
  wait(0);

  exit(0);
}

int
main(int argc, char *argv[])
{
  int fds[2];

  pipe(fds);
  if (fork() == 0) {
    // child process
    child(fds);
  } else {
    // main process feeding numbers to the pipeline
    close(fds[0]);
    for (int num = 2; num <= 35; ++num) {
      write(fds[1], &num, sizeof(num));
    }
    close(fds[1]); // finish writing
  }
  
  wait(0);

  exit(0);
}

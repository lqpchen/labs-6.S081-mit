/*
 Write a program that uses UNIX system calls to ''ping-pong'' a byte between two processes over a pair of pipes, 
 one for each direction. The parent should send a byte to the child; 
 the child should print "<pid>: received ping", where <pid> is its process ID, write the byte on the pipe to the parent, and exit; 
 the parent should read the byte from the child, print "<pid>: received pong", and exit. 
 Your solution should be in the file user/pingpong.c. 
*/

#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int ptc[2], ctp[2]; // parent to child pipe and vice versa
  int pid;
  char buf[10];

  pipe(ptc);
  pipe(ctp);

  if (fork() == 0) {
    // child
    pid = getpid();
    if (read(ptc[0], buf, sizeof(buf)) > 0) {
      //printf("%d: received ping\n", pid);
      fprintf(1, "%d: received ping\n", pid);
    }    
    write(ctp[1], "b", 1);
  } else {
    // parent
    pid = getpid();
    write(ptc[1], "b", 1);
    if (read(ctp[0], buf, sizeof(buf)) > 0) {
      //printf("%d: received pong\n", pid);
      fprintf(1, "%d: received pong\n", pid);
    }
  }

  exit(0);
}
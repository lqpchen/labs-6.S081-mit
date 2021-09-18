/*
Write a simple version of the UNIX xargs program: 
read lines from the standard input and run a command for each line, 
supplying the line as arguments to the command. 
Your solution should be in the file user/xargs.c. 
*/

#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h" // MAXARG

int
main(int argc, char *argv[])
{
  char str[256];
  //char str[256] = "12\n43\n657";  
  int sep[256], idx = 0, ncnt = 0, thr = 0, slen;

  // read from stdin
  if(read(0, str, sizeof(str)) < 0){
    printf("error reading str");
  }
  //printf("stdin: %s\n", str);

  slen = strlen(str);
  // count and get indexes of the seperator '\n'
  for(int i = 0; i < slen; ++i){
    if(str[i] == '\n'){
      sep[idx++] = i;
      ++ncnt;
      str[i] = 0;
    }    
  }
  sep[ncnt] = slen;

  char *xargv[argc + 1];
  for(int i = 0; i < argc - 1; ++i){
    xargv[i] = argv[i + 1];
    //printf("%s\n", argv[i]);
  }

  for(int p = 0, i = 0; p < slen; ){
    xargv[argc - 1] = str + p;
    xargv[argc] = 0;

    if(fork() == 0){
      // printf("%d:\n", getpid());
      // for(int i = 0; i < argc + 1; ++i) 
      //   printf("xarg[%d]: %s", i, xargv[i]);
      exec(xargv[0], xargv);
      printf("exec failed!\n");
      exit(0);
    } else {
      p = sep[i++] + 1;
      ++thr;
    }
  }

  for(int i = 0; i < thr; ++i){
    wait(0);
  }
  //printf("All good\n\n");

  exit(0);
}

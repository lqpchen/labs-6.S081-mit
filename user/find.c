/*
Write a simple version of the UNIX find program: 
find all the files in a directory tree with a specific name. 
Your solution should be in the file user/find.c. 
*/

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void
find(char* dir, char *file)
{
  int dirfd;
  struct dirent de;
  struct stat st;  
  char fullname[512], *p;

  if((dirfd = open(dir, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", dir);
  }

  // check each entry within the dir
  strcpy(fullname, dir);
  p = fullname+strlen(fullname);
  *p++ = '/';
  while(read(dirfd, &de, sizeof(de)) == sizeof(de)){
    if (de.inum == 0 || strcmp(de.name, ".") == 0 ||
        strcmp(de.name, "..") == 0){
      continue;
    }
    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;
    if(stat(fullname, &st) < 0){
      printf("ls: cannot stat %s\n", fullname);
      continue;
    }
    if (st.type == T_FILE && strcmp(de.name, file) == 0){
      printf("%s\n", fullname);
    } else if (st.type == T_DIR) {
      find(fullname, file);
    }
  }

  close(dirfd);

}

int
main(int argc, char *argv[])
{
  struct stat st;
  char *dir, *file;

  if(argc != 3){
    fprintf(2, "usage: find dirname filename\n");
    exit(1);
  } else {
    dir = argv[1];
    file = argv[2];
  }

  // ensure that dir is indeed a dir
  if(stat(dir, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", dir);
  }

  if (st.type != T_DIR) {
    fprintf(2, "find: %s is not a directory\n", dir);
  }

  find(dir, file);

  exit(0);
}

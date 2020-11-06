#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"

#include "user/user.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void 
find(char *directory, char *pattern)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(directory, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", directory);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", directory);
    close(fd);
    return;
  }

  if(st.type == T_FILE){
    fprintf(2, "find: %s should be a directory\n", directory);
    close(fd);
    return;
  }

  // strlen(directory) + / + len(name) + \0
  if(strlen(directory) + 1 + DIRSIZ + 1 > sizeof buf){
    printf("ls: path too long\n");
  }
  strcpy(buf, directory);
  p = buf+strlen(buf);
  *p++ = '/';
  while(read(fd, &de, sizeof(de)) == sizeof(de)){
    if(de.inum == 0)
      continue;
    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;
    if(stat(buf, &st) < 0){
      printf("ls: cannot stat %s\n", buf);
      continue;
    }
    if(strcmp(fmtname(buf), ".") == 0 || strcmp(fmtname(buf), "..") == 0){
      continue;
    }
    if(st.type == T_DIR){
      find(buf, pattern);
    }
    if(strcmp(fmtname(buf), pattern) == 0) {
      printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
    }
  }
  close(fd);
}

int 
main(int argc, char *argv[])
{
  int i = 0;
  if(argc < 3){
    fprintf(2, "Usage find directory pattern\n");
    exit(1);
  }

  find(argv[1], argv[2]);
  exit(0);
}
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int 
main(int argc, char *argv[])
{
  int p1[2], p2[2];
  int pid = fork();
  char c = 'c';

  pipe(p1);
  pipe(p2);
  if(pid < 0){
    fprintf(2, "fork panic\n");
    exit(1);
  }else if(pid > 0){
    // parent
    close(p1[0]);
    write(p1[1], &c, 1);
    close(p1[1]);

    close(p2[1]);
    read(p2[0], &c, 1);
    close(p2[0]);
    
    wait((int *)0);
    fprintf(1, "%d: received pong\n", getpid());
  }else{
    // child
    close(p1[1]);
    read(p1[0], &c, 1);
    close(p1[0]);

    fprintf(1, "%d: received ping\n", getpid());
    
    close(p2[0]);
    write(p2[1], &c, 1);
    close(p2[1]);
  }
  exit(0);
}
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int secs = 0;
  if(argc < 2){
    fprintf(2, "Usage: sleep secs\n");
    exit(1);
  }
  secs = atoi(argv[1]);
  if(sleep(secs) < 0){
    exit(1);
  }
  exit(0);
}

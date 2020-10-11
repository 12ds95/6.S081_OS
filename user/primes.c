#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void source() 
{
  int i = 0;
  for (i = 2; i <= 35; ++i) {
    write(1, (void *)&i, sizeof(int));
  }
  exit(0);
}

void cull(int p)
{
  int n;
  while (read(0, (void *)&n, sizeof(int)) != 0) {
    if (n % p != 0) {
      write(1, (void *)&n, sizeof(int));
    }
  }
  exit(0);
}

void redirect(int k, int p[]) 
{
  close(k);
  dup(p[k]);
  close(p[0]);
  close(p[1]);
}

void sink()
{
  int p[2];
  int n;
  while (read(0, (void *)&n, sizeof(int)) != 0) {
    fprintf(1, "prime %d\n", n);
    pipe(p);
    if (fork()) { // parent
      redirect(0, p);
      continue;
    } else {
      redirect(1, p);
      cull(n);
    }
  }
  while (wait((int *)0) >= 0) {}
}

int 
main(int argc, char *argv[])
{
  int p[2];
  pipe(p);
  if (fork()) { // parent
    redirect(0, p);
    sink();
  } else { 
    redirect(1, p);
    source();
  }
  exit(0);
}
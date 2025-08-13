#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int nproc = 4;    
  int loops = 10000; 
  if(argc == 3){
    nproc = atoi(argv[1]);
    loops = atoi(argv[2]);
  }

  uint64 before = getreadcount();

  for(int i = 0; i < nproc; i++){
    if(fork() == 0){
      char dummy;
      for(int j = 0; j < loops; j++){
        read(0, &dummy, 0); 
      }
      exit(0);
    }
  }

  for(int i = 0; i < nproc; i++)
    wait(0);

  uint64 after = getreadcount();
  uint64 delta = after - before;
  uint64 expected = (uint64)nproc * (uint64)loops;

  printf("before=%lu after=%lu delta=%lu expected=%lu\n",
         before, after, delta, expected);

  if(delta == expected)
    printf("PASS: atomic increments\n");
  else
    printf("FAIL: lost increments\n");

  exit(0);
}

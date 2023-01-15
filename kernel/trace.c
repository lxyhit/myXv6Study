#include "types.h"
#include "param.h"
#include "defs.h"
#include "proc.h"
#include "syscall.h"

int
trace(int mask){
  struct proc *p;
  int pid;

  struct proc *p = myproc();
  p->mask = mask;

  if(p->mask == mask){
    return 0;
  }else{
    return -1;
  }
}
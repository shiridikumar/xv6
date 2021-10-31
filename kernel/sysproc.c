#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
extern int traced[25];
extern int sys_length;
extern struct proc proc[NPROC];



uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  struct proc *p=myproc();
  if(strncmp(p->parent->name,"sh",strlen(p->parent->name))==0){
    for(int i=0;i<sys_length;i++){
      traced[i]=0;
    }
  }
  p->trace_flag=0;
  exit(n);
  return 0;  // not reached
}


uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_trace(void){

  //char buff[30];
  uint64 mask;
  if(argaddr(0,&mask)<0)
    return -1;
  int count=0;
  if(mask==0){
    for(int i=0;i<sys_length;i++){
      traced[i]=0;
    }
    return 0;
  }
  while(mask>0 && count<25){
    if((mask&1)==1){
      traced[count]=1;
    }
    else{
      traced[count]=0;
    }
    count++;
    mask=mask>>1;

  }
  //#endif
//  #endif



  struct proc *p=myproc();
  if(p->parent){
    p->parent->trace_flag=1;
  }
  return 0;

}

uint64
sys_waitx(void)
{
  uint64 wtime,rtime;
  uint64 arg1,arg2,arg3;
  if(argaddr(0,&arg1)<0){
    return -1;
  }
  if(argaddr(1,&arg2)<0){
    return -1;
  }  
  if(argaddr(2,&arg3)<0){
    return -1;
  }
  struct proc *p=myproc();
  int ret=waitx(arg1,&wtime,&rtime);
  if(copyout(p->pagetable,arg2,(char *)&wtime,sizeof(int))<0){
    return -1;
  }
  if(copyout(p->pagetable,arg3,(char *)&rtime,sizeof(int))<0){
    return -1;
  }
  return ret;
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}


uint64
sys_set_priority(void){
  uint64 priority;
  uint64 pid;
  if(argaddr(0,&priority)<0){
    return -1;
  }
  if(argaddr(1,&pid)<0){
    return -1;
  }
  struct proc *p;
  for(p=proc;p<&proc[NPROC];p++){
    if(p->pid==pid){
      p->sp=priority;
      p->sleep_time=0;
      p->dup_run=0;
      printf("changed static priority of pid %d to %d\n",pid,priority);
    }
  }
  return pid;
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

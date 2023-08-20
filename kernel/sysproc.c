#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
<<<<<<< Updated upstream
#include "param.h"
=======
#include "date.h"
>>>>>>> Stashed changes
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
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
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

<<<<<<< Updated upstream
  argint(0, &n);
=======
  if(argint(0, &n) < 0)
    return -1;
  
>>>>>>> Stashed changes
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

<<<<<<< Updated upstream
  argint(0, &n);
=======

  if(argint(0, &n) < 0)
    return -1;
>>>>>>> Stashed changes
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}


#ifdef LAB_PGTBL
int
sys_pgaccess(void)
{    
  // lab pgtbl: your code here.
  int num_check;
  uint64 va;
  uint64 dstva;
  pagetable_t pagetable = myproc()->pagetable;
  
  uint64 check_va;
  pte_t *pte;
  uint32 buf = 0; 
  
  if(argaddr(0, &va) < 0){
    return -1;
  }
  if(argint(1, &num_check) < 0){
    return -1;
  }
  if(argaddr(2, &dstva) < 0){
    return -1;
  }

  for(int i = 0; i < MAXSCAN && i < num_check; i++){
    check_va = va + (uint64)i * PGSIZE; 
    pte = walk(pagetable, check_va, 0);
    if(pte == 0){
      return -1;
    }
    if((*pte & PTE_V) == 0){
      return -1;
    }
    if((*pte & PTE_U) == 0){
      return -1;
    }
    if(*pte & PTE_A){
      *pte = *pte & ~PTE_A;
      buf |= (1 << i);  
    }
  }
  
  if(copyout(pagetable, dstva, (char *)&buf, sizeof(buf)) < 0){
    return -1;
  }

  return 0;
}
#endif

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
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

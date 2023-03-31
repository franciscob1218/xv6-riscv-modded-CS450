#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

int sys_whereIs(void)
{
  char *addr;
  if(argptr(0, &addr, sizeof(addr)) < 0)
    return -1;

  pde_t *pgdir = myproc()->pgdir;
  pte_t *pte = walkpgdir(pgdir, addr, 0);

  if(pte == 0)
    return -1;

  if(!(*pte & PTE_P))
    return -1;

  return PTE_ADDR(*pte) >> PGSHIFT;
}
int sys_isWritable(void *addr) {
  pte_t *pte = walkpgdir(myproc()->pgdir, addr, 0);
  if (!pte)
    return -1;
  return (*pte & PTE_W) ? 1 : 0;
}

int sys_notWritable(void *addr) {
  pte_t *pte = walkpgdir(myproc()->pgdir, addr, 0);
  if (!pte)
    return -1;
  *pte &= ~PTE_W;
  return 0;
}

int sys_yesWritable(void *addr) {
  pte_t *pte = walkpgdir(myproc()->pgdir, addr, 0);
  if (!pte)
    return -1;
  *pte |= PTE_W;
  return 0;
}


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

  argint(0, &n);
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

  argint(0, &n);
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

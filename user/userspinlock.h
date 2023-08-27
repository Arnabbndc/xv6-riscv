#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

struct thread_spinlock {
  uint8 locked;       // Is the lock held?
  int pid;            // id of the process holding the lock.
};

void
thread_spin_init(struct thread_spinlock *lk)
{
  lk->locked = 0;
  lk->pid=0;
}

int
thread_spin_holding(struct thread_spinlock *lk)
{
  int r;
  r = (lk->locked && lk->pid == getpid());
  return r;
}

void
thread_spin_lock(struct thread_spinlock *lk)
{
  if(thread_spin_holding(lk))
    printf("panic: thread_spinlock acquire\n");
  while(__sync_lock_test_and_set(&lk->locked, 1) != 0);
  __sync_synchronize();
  lk->pid = getpid();
}

// Release the lock.
void
thread_spin_unlock(struct thread_spinlock *lk)
{
  if(!thread_spin_holding(lk))
    printf("panic: thread_spinlock release\n");
  lk->pid = 0;
  __sync_synchronize();
  __sync_lock_release(&lk->locked);

}


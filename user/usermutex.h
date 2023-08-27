#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

struct thread_mutex {
  uint8 locked;       // Is the lock held?
  int pid;            // id of the process holding the lock.
};

void
thread_mutex_init(struct thread_mutex *m)
{
  m->locked = 0;
  m->pid=0;
}

int
thread_mutex_holding(struct thread_mutex *m)
{
  int r;
  r = (m->locked && m->pid == getpid());
  return r;
}

void
thread_mutex_lock(struct thread_mutex *m)
{
  if(thread_mutex_holding(m))
    printf("panic: mutex acquire\n");
  while(__sync_lock_test_and_set(&m->locked, 1) != 0){
    sleep(1);
  }
    ;
  __sync_synchronize();
  m->pid = getpid();

}

// Release the lock.
void
thread_mutex_unlock(struct thread_mutex *m)
{
  if(!thread_mutex_holding(m))
    printf("panic: mutex release\n");
  m->pid = 0;
  __sync_synchronize();
  __sync_lock_release(&m->locked);

}


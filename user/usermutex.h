#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

struct thread_mutex {
  uint8 locked;       // Is the lock held?
};

void
thread_mutex_init(struct thread_mutex *m)
{
  m->locked = 0;
}

void
thread_mutex_lock(struct thread_mutex *m)
{

  while(__sync_lock_test_and_set(&m->locked, 1) != 0){
    sleep(1);
  }
    ;
  __sync_synchronize();

}

// Release the lock.
void
thread_mutex_unlock(struct thread_mutex *m)
{

  __sync_synchronize();
  __sync_lock_release(&m->locked);

}
#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"
#include "user/queue.h"
#include "user/usermutex.h"

  struct thread_cond_var {
    struct queue q;
    struct thread_mutex mx; /*protects queue */
  };

void thread_cond_init(struct thread_cond_var *cv){
  queue(&(cv->q));
  thread_mutex_init(&(cv->mx));
}

void thread_cond_wait (struct thread_cond_var *cv,struct  thread_mutex *mutex) 
{
  thread_mutex_lock(&cv->mx);  /* protect the queue */
  push(&(cv->q),getpid()); /* enqueue */
  thread_mutex_unlock(&cv->mx); /* we're done with the list */

  /* The suspend and release_mutex() operation should be atomic */
  mutex->pid = 0;    // must be called from user space before using sleep_edited()
  sleep_edited(&(mutex->locked));  /* Sleep 'til someone wakes us */

  thread_mutex_lock (mutex); /* Woke up -- our turn, get resource lock */

  return;
}

void thread_cond_signal (struct thread_cond_var *cv)
{
  int thr_id;

  thread_mutex_lock(&cv->mx);  /* protect the queue */
  thr_id = front(&(cv->q));
  pop(&(cv->q));
  thread_mutex_unlock(&cv->mx); /* we're done with the list */

  if (thr_id>0)
    wakeup_edited(thr_id);

  return;
}
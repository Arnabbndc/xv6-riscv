#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"
#include "user/condVariable.h"

struct sem_t {
    int count; 
    struct thread_mutex m;
    struct thread_cond_var cv;
};

int sem_init(struct sem_t *s, int value) {
    s->count = value;
    thread_mutex_init(&s->m);
    thread_cond_init(&s->cv);
    return 0;
}


void sem_wait(struct sem_t *s) {
    thread_mutex_lock(&s->m);
    while (s->count <= 0) {
        thread_cond_wait(&s->cv, &s->m); /*unlock mutex, wait, relock mutex */
    }
    s->count--;
    thread_mutex_unlock(&s->m);
}

void sem_post(struct sem_t *s) {
    thread_mutex_lock(&s->m);
    s->count++;
    if (s->count == 1) /* Wake up one waiting thread! */
        thread_cond_signal(&s->cv);
    // thread_cond_signal(&s->cv); /* See note */
    /* A woken thread must acquire the lock, so it will also have to wait until we call unlock */

    thread_mutex_unlock(&s->m);
}


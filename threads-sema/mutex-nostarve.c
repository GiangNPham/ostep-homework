#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "common_threads.h"

//
// Here, you have to write (almost) ALL the code. Oh no!
// How can you show that a thread does not starve
// when attempting to acquire this mutex you build?
//

typedef struct __ns_mutex_t {
    int waiting1;
    int waiting2;
    sem_t lock;
    sem_t turnstile1;
    sem_t turnstile2;
} ns_mutex_t;

void ns_mutex_init(ns_mutex_t *m) {
    m->waiting1 = 0;
    m->waiting2 = 0;

    sem_init(&m->lock, 0, 1);
    sem_init(&m->turnstile1, 0, 1);
    sem_init(&m->turnstile2, 0, 0); // waiting room 2 is locked at first
}

void ns_mutex_acquire(ns_mutex_t *m) {
    sem_wait(&m->lock);
    m->waiting1++;
    sem_post(&m->lock); // have to release the lock here because 
    // if the old threads are in room2 and a new thread shows up and acquire the mutex but got blocked because of t1 => deadlock

    sem_wait(&m->turnstile1);

    m->waiting2++; // include this in the mutex lock is unncessary as 
    // only one thread can enter past the turnstile1 at a time

    // the reason why still need mutex lock for waiting1 is that to protect from new threads comming in (line 32)
    sem_wait(&m->lock);
    m->waiting1--;

    if (m->waiting1 == 0){
        sem_post(&m->lock);
        sem_post(&m->turnstile2);
    }
    else {
        sem_post(&m->lock);
        sem_post(&m->turnstile1);
    }

    sem_wait(&m->turnstile2);
    m->waiting2--;
}

void ns_mutex_release(ns_mutex_t *m) {
    if (m->waiting2 == 0)
        sem_post(&m->turnstile1);
    else {
        sem_post(&m->turnstile2);
    }
}


ns_mutex_t global_lock;
int shared_counter = 0;

void *worker(void *arg) {
    long id = (long)arg;

    for (int i = 0; i < 5; i++) {
        ns_mutex_acquire(&global_lock);

        // --- critical section ---
        int old = shared_counter;
        usleep(10000);  // force interleaving
        shared_counter = old + 1;

        printf("Thread %ld in CS, counter=%d\n", id, shared_counter);

        ns_mutex_release(&global_lock);
        // ------------------------

        usleep(rand() % 20000);
    }
    return NULL;
}

int main() {
    ns_mutex_init(&global_lock);

    pthread_t threads[10];

    for (long i = 0; i < 10; i++) {
        pthread_create(&threads[i], NULL, worker, (void *)i);
    }

    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Final counter = %d\n", shared_counter);
    return 0;
}

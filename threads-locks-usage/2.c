// Counter locking

#include <sys/time.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct __counter_t {
    int value;
    pthread_mutex_t lock;
} counter_t;

void init (counter_t * c){
    c->value = 0;
    pthread_mutex_init(&c->lock, NULL);
}

void increment(counter_t* c){
    pthread_mutex_lock(&c->lock);
    c->value++;
    pthread_mutex_unlock(&c->lock);
}

int get(counter_t *c) {
    pthread_mutex_lock(&c->lock);
    int rc = c->value;
    pthread_mutex_unlock(&c->lock);
    return rc;
}

void* worker(void* arg){
    counter_t* c = (counter_t*)arg;
    for (int i = 0; i < 10000; i++) increment(c);
}

int main() {
    struct timeval current_time;

    // Get the current time
    

    int num = 120;
    pthread_t *threads = malloc(sizeof(pthread_t) * num);
    counter_t *c = malloc(sizeof(counter_t));
    init(c);

    gettimeofday(&current_time, NULL);

    printf("Seconds since epoch: %ld\n", current_time.tv_sec);
    printf("%ld\n", current_time.tv_usec);


    for (int i = 0; i < num; i++){
        pthread_create(&threads[i], NULL, worker, c);
    }

    for (int i = 0; i < num; i++){
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&current_time, NULL);

    printf("Seconds since epoch: %ld\n", current_time.tv_sec);
    printf("%ld\n", current_time.tv_usec);

    free(threads);
    free(c);

    return 0;
}
// Hand-over-hand locking linked list

#include <sys/time.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct __node_t {
    int key;
    struct __node_t *next;
    pthread_mutex_t lock;
} node_t;

typedef struct __list_t {
    node_t *head;
} list_t;

void list_init(list_t *L){
    // dummy head
    node_t *dummy = malloc(sizeof(node_t));
    dummy->key = -1;
    dummy->next = NULL;
    pthread_mutex_init(&dummy->lock, NULL);
    L->head = dummy;
}

int list_insert(list_t* L, int key){
    node_t* new = malloc(sizeof(node_t));
    if (new == NULL){
        perror("malloc");
        return -1;
    }
    new->key = key;
    pthread_mutex_init(&new->lock, NULL);

    pthread_mutex_lock(&L->head->lock);
    new->next = L->head->next;
    L->head->next = new;
    pthread_mutex_unlock(&L->head->lock);
    return 0;
}

int list_lookup(list_t* L, int key){
    int rv = -1;
    pthread_mutex_lock(&L->head->lock);
    node_t *curr = L->head;
    while (curr){
        if (curr->key == key){
            rv = 0;
            break;
        }
        node_t *next = curr->next;
        if (next) pthread_mutex_lock(&next->lock);
        pthread_mutex_unlock(&curr->lock);
        curr = next;
    }
    return rv;
}

void* worker(void* arg){
    list_t *list = (list_t*) arg;
    for (int i = 0; i < 10000; i+=2){
        list_insert(list, i);
    }

    for (int i = 1; i < 100; i+=2){
        list_lookup(list, i);
    }
}


int main() {
    struct timeval current_time;

    // Get the current time
    

    int num = 120;
    pthread_t *threads = malloc(sizeof(pthread_t) * num);
    list_t *list = malloc(sizeof(list_t));
    list_init(list);

    gettimeofday(&current_time, NULL);

    printf("Seconds since epoch: %ld\n", current_time.tv_sec);
    printf("%ld\n", current_time.tv_usec);


    for (int i = 0; i < num; i++){
        pthread_create(&threads[i], NULL, worker, list);
    }

    for (int i = 0; i < num; i++){
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&current_time, NULL);

    printf("Seconds since epoch: %ld\n", current_time.tv_sec);
    printf("%ld\n", current_time.tv_usec);

    free(threads);
    free(list);

    return 0;
}
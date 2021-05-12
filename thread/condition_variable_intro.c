#include <stdio.h>
#include <pthread.h>

int done = 0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c = PTHREAD_COND_INITIALIZER;

void thread_exit() {
    pthread_mutex_lock(&m);
    done = 1;
    pthread_cond_signal(&c);
    pthread_mutex_unlock(&m);
}

void *childTask(void *arg) {
    printf("Child thread performing task\n");
    thread_exit();
    return NULL;
}

void thread_join() {
    pthread_mutex_lock(&m);
    while(done == 0)
        pthread_cond_wait(&c,&m);
    pthread_mutex_unlock(&m);
}

int main() {
    printf("Parent begins\n");
    pthread_t p;
    pthread_create(&p, NULL, childTask, NULL);

    thread_join();
    
    printf("Parent end\n");


    return 0;
}
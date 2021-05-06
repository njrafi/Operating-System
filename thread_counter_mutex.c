
#include <stdio.h>
#include <pthread.h>


int mx;
volatile int counter = 0; // shared global variable
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *mythread(void *arg) {
        char *letter = arg;
        int i; // stack (private per thread)
        printf("%s: begin [addr of i: %p]\n", letter, &i);

        pthread_mutex_lock(&mutex);
        for (i = 0; i < mx; i++) {
                counter = counter + 1; // shared: only one
        }
        pthread_mutex_unlock(&mutex);
        printf("%s: done\n", letter);
        return NULL;
}

int main() {

        printf("main: begin [counter = %d] [%x]\n", counter, (unsigned int) &counter);
        mx = 10000000;
        pthread_t p1,p2;

        pthread_create(&p1, NULL, mythread, "A");
        pthread_create(&p2, NULL, mythread, "B");
        printf("main: C\n");

        pthread_join(p1, NULL);
        pthread_join(p2, NULL);


        printf("main: done\n [counter: %d]\n [should: %d]\n", counter, mx*2);


        return 0;
}


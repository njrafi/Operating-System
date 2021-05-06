#include <stdio.h>
#include <pthread.h>


void *mythread(void *arg) {
        printf("%s\n", (char*) arg);
        return NULL;
}

int main() {

        printf("main: begin\n");
        pthread_t p1,p2;

        pthread_create(&p1, NULL, mythread, "A");
        pthread_create(&p2, NULL, mythread, "B");
        printf("main: C\n");

        pthread_join(p1, NULL);
        pthread_join(p2, NULL);

        printf("main: end\n");


        return 0;
}


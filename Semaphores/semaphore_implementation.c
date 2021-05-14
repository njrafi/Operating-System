#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

typedef struct __Zem_t {
        int value;
        pthread_cond_t cond;
        pthread_mutex_t lock;
} Zem_t;

void Zem_init(Zem_t *s,int value) {
        s->value = value;
        s->cond = PTHREAD_COND_INITIALIZER;
        s->lock = PTHREAD_MUTEX_INITIALIZER;
}

void Zem_wait(Zem_t *s) {
        pthread_mutex_lock(&s->lock);
        while(s->value <= 0)
                pthread_cond_wait(&s->cond, &s->lock);
        s->value--;
        pthread_mutex_unlock(&s->lock);
}


void Zem_post(Zem_t *s) {
        pthread_mutex_lock(&s->lock);
        s->value++;
        pthread_cond_signal(&s->cond);
        pthread_mutex_unlock(&s->lock);

}

int main() {

        return 0;
}

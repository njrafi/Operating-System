#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>


typedef struct _rwlock_t {
        sem_t mutex;
        sem_t writelock;
        int readers;
} rwlock_t;

void rwlock_init(rwlock_t *rw) {
        rw->readers = 0;
        sem_init(&rw->mutex, 0, 1);
        sem_init(&rw->writelock,0, 1);
}

void rwlock_acquire_readlock(rwlock_t *rw) {
        sem_wait(&rw->mutex);

        rw->readers++;
        if(rw->readers == 1)
                sem_wait(&rw->writelock);   // First reader get the write lock

        sem_post(&rw->mutex);
}

void rwlock_release_readlock(rwlock_t *rw) {
        sem_wait(&rw->mutex);

        rw->readers--;
        if(rw->readers == 0)
                sem_post(&rw->writelock);   // last reader release the write lock

        sem_post(&rw->mutex);
}


void rwlock_acquire_writelock(rwlock_t *rw) {
        sem_wait(&rw->writelock);
}

void rwlock_release_writelock(rwlock_t *rw) {
        sem_post(&rw->writelock);
}

int data = -1;
rwlock_t readWriteLock;

#define totalRead 20
#define totalWrite 20

void *readData(void *params) {
    int number = *((int*)params);
    for(int i = 0 ; i < totalRead ; i++) {

        rwlock_acquire_readlock(&readWriteLock);

        printf("Reader = %d, numOfReader = %d, Read =  %d\n", number,readWriteLock.readers,data);

        rwlock_release_readlock(&readWriteLock);
    }
}

void *writeData(void *params) {
    int number = *((int*)params);

    for(int i = 0 ; i < totalWrite ; i++) {

        rwlock_acquire_writelock(&readWriteLock);

        data = rand() % 100;
        printf("Writer = %d, numOfReader = %d, Write =  %d\n", number,readWriteLock.readers, data);

        rwlock_release_writelock(&readWriteLock);
    }
}

#define maxReader 3
#define maxWriter 2


int main() {
    srand(time(NULL));
    rwlock_init(&readWriteLock);
    pthread_t readers[maxReader],writers[maxWriter];
    int tem[maxReader + maxWriter];

    for(int i = 0 ; i < maxWriter; i++) {
        tem[i] = i+1;
        pthread_create(&writers[i],NULL,writeData,&tem[i]);
    }

   for(int i = 0 ; i < maxReader; i++) {
        tem[i] = i+1;
        pthread_create(&readers[i],NULL,readData,&tem[i]);
   }

    for(int i = 0 ; i < maxWriter; i++)
        pthread_join(writers[i],NULL);


    for(int i = 0 ; i < maxReader; i++)
        pthread_join(readers[i],NULL);


   return 0;
}


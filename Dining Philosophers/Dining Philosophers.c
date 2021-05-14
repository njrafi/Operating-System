#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>


#define NUMBER_OF_PHILOSOPHERS 5


sem_t forks[NUMBER_OF_PHILOSOPHERS];

int getLeftFork(int philosopherIndex) {
        return philosopherIndex;
}

int getRightFork(int philosopherIndex) {
        return (philosopherIndex + 1) % NUMBER_OF_PHILOSOPHERS;
}

void think(int philosopherIndex) {
        printf("Philosopher %d: Thinking\n", philosopherIndex);
}

void eat(int philosopherIndex) {
        printf("Philosopher %d: Eating\n", philosopherIndex);
}


void getForks(int philosopherIndex) {
        if(!philosopherIndex) {
                sem_wait(&forks[getRightFork(philosopherIndex)]);
                sem_wait(&forks[getLeftFork(philosopherIndex)]);
        } else {
                sem_wait(&forks[getLeftFork(philosopherIndex)]);
                sem_wait(&forks[getRightFork(philosopherIndex)]);
        }
}

void putForks(int philosopherIndex) {
        if(!philosopherIndex) {
                sem_post(&forks[getRightFork(philosopherIndex)]);
                sem_post(&forks[getLeftFork(philosopherIndex)]);
        } else {
                sem_post(&forks[getLeftFork(philosopherIndex)]);
                sem_post(&forks[getRightFork(philosopherIndex)]);
        }
}


// To understand dead lock comment out the below code
// There is a very high chance there will be no dead lock
/*
void getForks(int philosopherIndex) {
        sem_wait(&forks[getLeftFork(philosopherIndex)]);
        sem_wait(&forks[getRightFork(philosopherIndex)]);
}

void putForks(int philosopherIndex) {
        sem_post(&forks[getLeftFork(philosopherIndex)]);
        sem_post(&forks[getRightFork(philosopherIndex)]);
}
*/

void *thinkAndEat(void *arg) {
        int philosopherIndex = *((int*)arg);

        int duration = 100000000;
        while(duration--) {
                think(philosopherIndex);
                getForks(philosopherIndex);
                eat(philosopherIndex);
                putForks(philosopherIndex);
        }
}

int main() {
        int tem[NUMBER_OF_PHILOSOPHERS];
        pthread_t philosophers[NUMBER_OF_PHILOSOPHERS];

        for(int philosopher = 0; philosopher < NUMBER_OF_PHILOSOPHERS; philosopher++) {
                tem[philosopher] = philosopher;
                pthread_create(&philosophers[philosopher],NULL,thinkAndEat,&tem[philosopher]);
        }

        for(int philosopher = 0; philosopher < NUMBER_OF_PHILOSOPHERS; philosopher++)
                pthread_join(philosophers[philosopher], NULL);



        return 0;
}

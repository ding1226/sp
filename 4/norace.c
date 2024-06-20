#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4
#define NUM_OPERATIONS 1000000

double account_balance = 0.0;

pthread_mutex_t mutex;

void* deposit(void* arg) {
    for (int i = 0; i < NUM_OPERATIONS; i++) {
        pthread_mutex_lock(&mutex);
        account_balance += 1.0;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void* withdraw(void* arg) {
    for (int i = 0; i < NUM_OPERATIONS; i++) {
        pthread_mutex_lock(&mutex);
        account_balance -= 1.0;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < NUM_THREADS / 2; i++) {
        pthread_create(&threads[i], NULL, deposit, NULL);
    }

    for (int i = NUM_THREADS / 2; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, withdraw, NULL);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    printf("Final account balance: %.2f\n", account_balance);

    return 0;
}

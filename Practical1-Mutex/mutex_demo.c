#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 10
#define ITERATIONS  100000

int counter = 0;
pthread_mutex_t mutex;

void* increment_without_mutex(void* arg) {
    for (int i = 0; i < ITERATIONS; i++) {
        counter++;  // race condition
    }
    return NULL;
}

void* increment_with_mutex(void* arg) {
    for (int i = 0; i < ITERATIONS; i++) {
        pthread_mutex_lock(&mutex);
        counter++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int expected = NUM_THREADS * ITERATIONS;

    // ---- Without mutex ----
    counter = 0;
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_create(&threads[i], NULL, increment_without_mutex, NULL);
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);
    printf("Without mutex: counter = %d (expected %d)\n", counter, expected);

    // ---- With mutex ----
    pthread_mutex_init(&mutex, NULL);
    counter = 0;
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_create(&threads[i], NULL, increment_with_mutex, NULL);
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);
    pthread_mutex_destroy(&mutex);
    printf("With mutex:    counter = %d (expected %d)\n", counter, expected);

    return 0;
}

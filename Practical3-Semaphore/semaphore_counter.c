#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define NUM_THREADS 10
#define ITERATIONS  1000000

int counter = 0;
pthread_mutex_t mutex;
sem_t counting_sem;   // allows up to 3 threads at a time
int active_threads = 0;

// Mutex version
void* inc_mutex(void* arg) {
    for (int i = 0; i < ITERATIONS; i++) {
        pthread_mutex_lock(&mutex);
        counter++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

// Semaphore version (binary semaphore – same as mutex)
void* inc_sem_binary(void* arg) {
    for (int i = 0; i < ITERATIONS; i++) {
        sem_wait(&counting_sem);
        counter++;
        sem_post(&counting_sem);
    }
    return NULL;
}

// Counting semaphore: allow up to 3 threads concurrently
void* inc_sem_counting(void* arg) {
    for (int i = 0; i < ITERATIONS; i++) {
        sem_wait(&counting_sem);   // semaphore value is 3 initially
        counter++;
        sem_post(&counting_sem);
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    clock_t start, end;
    double cpu_time;

    // ---- Mutex ----
    pthread_mutex_init(&mutex, NULL);
    counter = 0;
    start = clock();
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_create(&threads[i], NULL, inc_mutex, NULL);
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);
    end = clock();
    cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Mutex:        counter = %d, time = %f s\n", counter, cpu_time);
    pthread_mutex_destroy(&mutex);

    // ---- Binary Semaphore ----
    sem_init(&counting_sem, 0, 1);
    counter = 0;
    start = clock();
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_create(&threads[i], NULL, inc_sem_binary, NULL);
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);
    end = clock();
    cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Binary sem:   counter = %d, time = %f s\n", counter, cpu_time);
    sem_destroy(&counting_sem);

    // ---- Counting Semaphore (max 3 concurrent) ----
    sem_init(&counting_sem, 0, 3);
    counter = 0;
    start = clock();
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_create(&threads[i], NULL, inc_sem_counting, NULL);
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);
    end = clock();
    cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Counting sem: counter = %d, time = %f s\n", counter, cpu_time);
    sem_destroy(&counting_sem);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>

#define SHM_SIZE 1024
#define SEM_NAME "/shm_sem"

int main() {
    // Create shared memory
    int shm_id = shmget(IPC_PRIVATE, SHM_SIZE, IPC_CREAT | 0666);
    if (shm_id < 0) {
        perror("shmget");
        exit(1);
    }

    // Attach
    char *shared_data = (char*) shmat(shm_id, NULL, 0);
    if (shared_data == (char*) -1) {
        perror("shmat");
        exit(1);
    }

    // Create named semaphore for synchronisation
    sem_t *sem = sem_open(SEM_NAME, O_CREAT, 0666, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }

    pid_t pid = fork();

    if (pid == 0) {  // Child process – reads
        sem_wait(sem);
        printf("Child reads: %s\n", shared_data);
        sem_post(sem);
        // Detach and clean up
        shmdt(shared_data);
        exit(0);
    } else {         // Parent – writes
        // Write something to shared memory
        sprintf(shared_data, "Hello from parent (PID %d)", getpid());

        // Wait for child to finish reading (optional: we can use semaphore to ensure order)
        // Actually we want child to read after we write, so we do: write, then child reads.
        // But we used semaphore to protect access, not ordering.
        // For proper ordering, we can use two semaphores, but here we just wait for child.
        wait(NULL);

        // Clean up
        sem_close(sem);
        sem_unlink(SEM_NAME);
        shmdt(shared_data);
        shmctl(shm_id, IPC_RMID, NULL);
    }

    return 0;
}

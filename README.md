# Operating-systems-practical

Student Information
	•	Name: Francis Charles Echezonachukwu 
	•	Reg no: 2023514097
	•	Assignment: Practical Sessions on Synchronization and Inter-Process Communication


Overview

This repository contains implementations of four Operating Systems practical sessions covering:
	1.	Mutex Lock Demonstration
	2.	Producer-Consumer Simulation
	3.	Semaphore Implementation in C
	4.	Shared Memory Programming

These practicals demonstrate thread synchronization, mutual exclusion, semaphores, and inter-process communication using C programming in a Linux/POSIX environment.


1. Mutex Lock Demonstration (mutex_demo.c)

· Spawns 10 threads, each incrementing a shared counter 100 000 times.
· Runs twice: without a mutex (race condition) and with a mutex.
· Expected output: without mutex, the counter is less than the expected value; with mutex, it is correct.
· Run the program multiple times to see the non‑deterministic results without synchronisation.

2. Producer‑Consumer Simulation (prod_cons.c)

· Implements a classic bounded‑buffer (circular queue) of size 5.
· One producer thread generates 20 items; one consumer consumes them.
· Uses three semaphores:
  · empty = number of free slots (initialised to buffer size)
  · full  = number of filled slots (initialised to 0)
  · mutex = protects critical sections (binary semaphore)
· The producer and consumer run at different speeds (usleep) to show buffer filling/emptying.

3. Semaphore vs Mutex Comparison (sem_vs_mutex.c)

· Compares three synchronisation approaches:
  1. pthread mutex – binary mutual exclusion.
  2. Binary semaphore (initial value 1) – identical to mutex.
  3. Counting semaphore (initial value 3) – allows up to 3 threads to enter the critical section simultaneously.
· All three protect the same shared counter with 10 threads and 1 000 000 iterations each.
· Prints the final counter value (should be correct in all cases) and the elapsed time.
· Demonstrates that counting semaphores can improve throughput when the resource can handle limited concurrency.

4. Shared Memory Programming (shm_demo.c)

· Creates a System V shared memory segment of 1024 bytes.
· Forks a child process.
· Parent writes a message into the shared memory.
· Child reads and prints the message.
· A named POSIX semaphore (/shm_sem) is used to synchronise access, though here we also use wait() to ensure the child reads after the parent writes.
· After the child exits, the parent cleans up the shared memory segment and unlinks the semaphore.

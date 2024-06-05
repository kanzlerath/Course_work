#include <semaphore.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
 
#define NUM_ITERATIONS 5
 
void access_server(sem_t *sem) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
 
    printf("Клиент %d пытается подключиться к серверу...\n", getpid());
    if (sem_wait(sem) < 0) {
        perror("sem_wait");
        exit(EXIT_FAILURE);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    double wait_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Клиент %d подключился к серверу. Время ожидания: %.6f секунд\n", getpid(), wait_time);
 
    // Эмуляция работы с сервером
    sleep(2);
    printf("Клиент %d завершил работу с сервером.\n", getpid());
 
    if (sem_post(sem) < 0) {
        perror("sem_post");
        exit(EXIT_FAILURE);
    }
}
 
int main() {
    sem_t *sem = sem_open("/posixsem", 0);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }
 
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        access_server(sem);
        sleep(1);  // Ожидание перед следующей попыткой подключения
    }
 
    sem_close(sem);
    return 0;
}

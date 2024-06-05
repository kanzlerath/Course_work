#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
 
#define NUM_ITERATIONS 10
 
// Определение union semun
union semun {
    int val;               // значение для SETVAL
    struct semid_ds *buf;  // буфер для IPC_STAT и IPC_SET
    unsigned short *array; // массив для GETALL и SETALL
};
 
void sem_op(int semid, int op) {
    struct sembuf sem;
    sem.sem_num = 0;
    sem.sem_op = op;
    sem.sem_flg = 0;
 
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    printf("Процесс %d пытается захватить семафор...\n", getpid());
    if (semop(semid, &sem, 1) < 0) {
        perror("semop");
        exit(1);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
 
    double wait_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Процесс %d захватил семафор. Время ожидания: %.6f секунд\n", getpid(), wait_time);
}
 
int main() {
    key_t key = ftok("semfile", 'a');
    int semid = semget(key, 1, 0666);
    if (semid < 0) {
        perror("semget");
        exit(1);
    }
 
    int fd = open("counter.txt", O_RDWR | O_CREAT, 0666);
    if (fd < 0) {
        perror("open");
        exit(1);
    }
 
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        sem_op(semid, -1);  // Захват семафора
 
        lseek(fd, 0, SEEK_SET);
        int counter;
        if (read(fd, &counter, sizeof(counter)) <= 0) {
            counter = 0;  // Если файл пуст, начинаем с нуля
        }
        counter++;
        lseek(fd, 0, SEEK_SET);
        write(fd, &counter, sizeof(counter));
 
        printf("Процесс %d увеличил счетчик до %d\n", getpid(), counter);
 
        sem_op(semid, 1);  // Освобождение семафора
        printf("Процесс %d освободил семафор.\n", getpid());
 
        sleep(1);
    }
 
    close(fd);
    return 0;
}

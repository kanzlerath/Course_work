#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
 
// Определение union semun
union semun {
    int val;               // значение для SETVAL
    struct semid_ds *buf;  // буфер для IPC_STAT и IPC_SET
    unsigned short *array; // массив для GETALL и SETALL
};
 
int main() {
    key_t key = ftok("semfile", 'a');
    int semid = semget(key, 1, 0666 | IPC_CREAT);
    if (semid < 0) {
        perror("semget");
        exit(1);
    }
 
    union semun arg;
    arg.val = 1;
    if (semctl(semid, 0, SETVAL, arg) < 0) {
        perror("semctl");
        exit(1);
    }
 
    printf("Семафор инициализирован.\n");
    return 0;
}

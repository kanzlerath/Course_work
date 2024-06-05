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
    int semid = semget(key, 1, 0666);
    if (semid < 0) {
        perror("semget");
        exit(1);
    }
 
    if (semctl(semid, 0, IPC_RMID) < 0) {
        perror("semctl");
        exit(1);
    }
 
    printf("Семафор удален.\n");
    return 0;
}

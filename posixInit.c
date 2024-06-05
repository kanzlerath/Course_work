#include <semaphore.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
 
int main() {
    sem_t *sem = sem_open("/posixsem", O_CREAT, 0644, 2);  // Ограничиваем до 2 одновременных подключений
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }
 
    printf("POSIX семафор инициализирован.\n");
    sem_close(sem);
    return 0;
}

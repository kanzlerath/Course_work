#include <semaphore.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
 
int main() {
    if (sem_unlink("/posixsem") < 0) {
        perror("sem_unlink");
        exit(EXIT_FAILURE);
    }
 
    printf("POSIX семафор удален.\n");
    return 0;
}
 

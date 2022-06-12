#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include <sys/types.h>

extern int errno;
#define SIZE 10 /* size of the shared buffer*/
#define VARSIZE 1 /* size of shared variable=1byte*/
#define INPUTSIZE 20
#define SHMPERM 0666 /* shared memory permissions */

int segid, empty_id, full_id, mutex_id;
int *buff;
sem_t *empty, *full, *mutex;

// Consumer function
void consume() {
    int c = 0;

    while (1) {
        sem_wait(full);
        sem_wait(mutex);

        if (*(buff) == -1) {
            printf("\n");
            sem_post(mutex);
            sem_post(empty);
            break;
        }
        
        printf("%d\n", *buff);

        sem_post(mutex);
        sem_post(empty);
    }
} 

//-------- Main function ---------
int main() {
    int i = 0;
    pid_t temp_pid;
    pthread_t id;

    segid = shmget(IPC_PRIVATE, SIZE, IPC_CREAT | IPC_EXCL | SHMPERM);
    empty_id = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | IPC_EXCL |
        SHMPERM);
    full_id = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | IPC_EXCL |
        SHMPERM);
    mutex_id = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | IPC_EXCL |
        SHMPERM);

    buff = shmat(segid, (char * ) 0, 0);
    empty = shmat(empty_id, (char * ) 0, 0);
    full = shmat(full_id, (char * ) 0, 0);
    mutex = shmat(mutex_id, (char * ) 0, 0);

    buff = shmat(segid, (int * ) 0, 0);
    empty = shmat(empty_id, (char * ) 0, 0);
    full = shmat(full_id, (char * ) 0, 0);
    mutex = shmat(mutex_id, (char * ) 0, 0);

    printf("\n Consumer Started \n");
    
    consume();

    shmdt(buff);
    shmdt(empty);
    shmdt(full);
    shmdt(mutex);
    shmctl(segid, IPC_RMID, NULL);
    semctl(empty_id, 0, IPC_RMID, NULL);
    semctl(full_id, 0, IPC_RMID, NULL);
    semctl(mutex_id, 0, IPC_RMID, NULL);
    sem_destroy(empty);
    sem_destroy(full);
    sem_destroy(mutex);
    printf("\n Consumer exited \n\n");
    return (0);
}
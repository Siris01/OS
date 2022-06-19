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
#define SIZE 10   /* size of the shared buffer*/
#define VARSIZE 1 /* size of shared variable=1byte*/
#define INPUTSIZE 20
#define SHMPERM 0666 /* shared memory permissions */

int segid, empty_id, full_id, mutex_id;
char *buff, *input_string;
sem_t *empty, *full, *mutex;

// Producer function
void produce()
{
    printf("Producer running...\n");
    int i = 0, p = 0;

    segid = shmget(IPC_PRIVATE, SIZE, IPC_CREAT | 0666);
    empty_id = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 0666);
    full_id = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 0666);
    mutex_id = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 0666);

    buff = shmat(segid, (char *)0, 0);
    empty = shmat(empty_id, (char *)0, 0);
    full = shmat(full_id, (char *)0, 0);
    mutex = shmat(mutex_id, (char *)0, 0);

    while (1)
    {
        if (i != 0)
        {
            sem_wait(empty);
            sem_wait(mutex);
        }

        if (i == strlen(input_string) + 2)
        {
            printf("Producer: Buffer is full.\n");
            sem_post(mutex);
            sem_post(full);

            break;
        }

        buff[p] = input_string[i];
        p = (p + 1) % SIZE;
        i++;

        sem_post(mutex);
        sem_post(full);
    }
}

// Consumer function
void *consume()
{
    printf("Consumer running...\n");
    int c = 0, i = 0;

    segid = shmget(IPC_PRIVATE, SIZE, IPC_CREAT | 0666);
    empty_id = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 0666);
    full_id = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 0666);
    mutex_id = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 0666);

    buff = shmat(segid, (char *)0, 0);
    empty = shmat(empty_id, (char *)0, 0);
    full = shmat(full_id, (char *)0, 0);
    mutex = shmat(mutex_id, (char *)0, 0);

    while (1)
    {
        sem_wait(full);
        sem_wait(mutex);

        if (i == strlen(input_string) + 2)
        {
            sem_post(mutex);
            sem_post(empty);
            break;
        }

        printf("%c\n", buff[c]);
        c = (c + 1) % SIZE;
        i++;

        sem_post(mutex);
        sem_post(empty);
    }
}

//-------- Main function ---------
int main()
{
    int i = 0;
    pid_t temp_pid;
    pthread_t id;

    segid = shmget(IPC_PRIVATE, SIZE, IPC_CREAT | 0666);
    empty_id = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 0666);
    full_id = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 0666);
    mutex_id = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 0666);

    buff = shmat(segid, (char *)0, 0);
    empty = shmat(empty_id, (char *)0, 0);
    full = shmat(full_id, (char *)0, 0);
    mutex = shmat(mutex_id, (char *)0, 0);

    input_string = (char *)malloc(INPUTSIZE * sizeof(char));

    sem_init(empty, 1, SIZE);
    sem_init(full, 1, 0);
    sem_init(mutex, 1, 1);

    printf("\n Main Process Started \n");
    printf("\n Enter the input string (20 characters MAX) : ");
    scanf("%[^\n]", input_string);
    printf("Input string is : %s\n", input_string);

    int ret = pthread_create(&id, NULL, &consume, NULL);
    if (ret != 0)
    {
        printf("\n Error in creating thread");
        exit(1);
    }

    produce();

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
    printf("\n Main process exited \n\n");
    return (0);
}
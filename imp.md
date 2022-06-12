# Getting string input
scanf("%[^\n]", path);

# Thread
pthread_t thread1;
pthread_attr_t attr;
    
pthread_attr_init(&attr);

pthread_create(&thread1, &attr, fun, arg); --> void *fun(void *arg){ ...; pthread_exit(0);}
pthread_join(thread1, NULL);

# pointer based array
int *val = calloc(100, sizeof(int));

# Spltting string
char* token = strtok(reference_string, " ");
    while( token != NULL ) {
        reference[ref_count++] = atoi(token);
        token = strtok(NULL, " ");
    }

# Sorting array of structs (used in cpu scheds)
qsort((void * )(p + 1), number_of_processes, sizeof(p[1]), cmp_burst_time);

int cmp_burst_time(const void * p, const void * q) {
    int b1 = ((struct process*)p)->burst_time;
    int b2 = ((struct process*)q)->burst_time;
    if (b1 < b2) return -1;
    return 1;
}

# H files
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h> 
#include <semaphore.h>
#include <pthread.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include <sys/types.h>

# Clearing shared mem
ipcs -m | grep `whoami` | awk '{ print $2 }' | xargs -n1 ipcrm -m
ipcs -s | grep `whoami` | awk '{ print $2 }' | xargs -n1 ipcrm -s
ipcs -q | grep `whoami` | awk '{ print $2 }' | xargs -n1 ipcrm -q

# Shared Memory
## Init
key_t key = ftok("shmfile", 65);  
int shmid = shmget(key, 1024, 0666|IPC_CREAT);
char *str = (char*) shmat(shmid, (void*)0, 0);

## Destroy
shmdt(str);
shmctl(shmid, IPC_RMID, NULL);
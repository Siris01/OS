# Getting string input
scanf("%[^\n]", path);

# Thread
pthread_t t1, t2, t3;
pthread_attr_t attr;
    
pthread_attr_init(&attr);

pthread_create(&t1, &attr, fun, arg); --> void *fun(void *arg){ ...; pthread_exit(0);}
pthread_join(t1, NULL);

# pointer based array
int *val = calloc(100, sizeof(int));

# Spltting string
char* token = strtok(reference_string, " ");
    while( token != NULL ) {
        reference[ref_count++] = atoi(token);
        token = strtok(NULL, " ");
    }

## Sub string
strstr(parent, child) == 1;

## Directory

DIR *fd = opendir(dir);
struct dirent *d;
while ((d = readdir(fd)) != NULL) { ... }

-- Readonly - 00
-- writeonly - 01
-- Readwrite - 02
-- create - 0100

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
#include <fcntl.h> 
#include <semaphore.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

# Clearing shared mem
ipcs -m | grep `whoami` | awk '{ print $2 }' | xargs -n1 ipcrm -m
ipcs -s | grep `whoami` | awk '{ print $2 }' | xargs -n1 ipcrm -s
ipcs -q | grep `whoami` | awk '{ print $2 }' | xargs -n1 ipcrm -q

# Shared Memory
## Init
int shmid = shmget(69, 1024, 0666 | IPC_CREAT);
char *str = (char *)shmat(shmid, (void *)0, 0);

## Destroy
shmdt(str);
shmctl(shmid, 0, NULL);;

# Semaphores

## Common 
shmget() x 4
shmat() x 4
sem_init(id, 1, <initial_value>) x 3
fun()
shmdt() x 4
shmctl() x 4
sem_destroy() x 3

## Producer
sem_wait( & (shmptr -> empty));
sem_wait( & (shmptr -> mutex));
...
sem_post( & (shmptr -> mutex));
sem_post( & (shmptr -> full));

## Cosumer
sem_wait( & (shmptr -> full));
sem_wait( & (shmptr -> mutex));
...
sem_post( & (shmptr -> mutex));
sem_post( & (shmptr -> empty));

# Deadlock

## Safety
System Model Deadlock Characterization Methods for Handling Deadlocks Deadlock Prevention Deadlock Detection
Safety Algorithm
1 Let Work and Finish be vectors of length m and n,
respectively. Initialize:
Work = Available
Finish [i] = false for i = 0, 1, , n - 1
2 Find an i such that both:
(a) Finish [i] = false
(b) Needi ≤ Work
If no such i exists, go to step 4
3 Work = Work + Allocationi
Finish[i] = true
go to step 2
4 If Finish [i] == true for all i, then the system is in a safe
state

## Resource req
System Model Deadlock Characterization Methods for Handling Deadlocks Deadlock Prevention Deadlock Detection
Resource-Request Algorithm for Process Pi
Requesti = request vector for process Pi . If Requesti [j ] = k
then process Pi wants k instances of resource type Rj
1 If Requesti ≤ Needi go to step 2. Otherwise, raise error
condition, since process has exceeded its maximum claim
2 If Requesti ≤ Available, go to step 3. Otherwise Pi must
wait, since resources are not available
3 Pretend to allocate requested resources to Pi by modifying
the state as follows:
Available = AvailableRequesti ;
Allocationi = Allocationi + Requesti ;
Needi = Needi Requesti ;
4 If safe → the resources are allocated to Pi
5 If unsafe → Pi must wait, and the old resource-allocation
state is restored

## Detection

Let Work and Finish be vectors of length m and n,
respectively Initialize: (a) Work = Available
(b) For i = 1,2, , n, if Allocationi 6 = 0, then
Finish[i] = false; otherwise, Finish[i] = true
2 Find an index i such that both:
(a) Finish[i] == false
(b) Requesti ≤ Work
If no such i exists, go to step 4
Unit-II Lecture -5
Deadlock
System Model Deadlock Characterization Methods for Handling Deadlocks Deadlock Prevention Deadlock Detection
Detection Algorithm
3 Work = Work + Allocationi
Finish[i] = true
go to step 2
4 If Finish[i] == false, for some i, 1 ≤ i ≤ n, then the system
is in deadlock state. Moreover, if Finish[i] == false, then Pi
is deadlocked


----

Normal (checking safe seq) -> work & finish
Detection -> work & finish & 2d req matrix
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h> 
  
void upload(char* path) {
    key_t key = ftok("shmfile", 65);  
    int shmid = shmget(key, 1024, 0666|IPC_CREAT);

    char *str = (char*) shmat(shmid, (void*)0, 0);

    int fd = open(path, O_RDONLY); 
    if (fd == -1) { 
        printf("Error while opening file!"); 
        exit(1); 
    }

    read(fd, str, 1024); 
    printf("[SERVER] Uploaded file!");
    
    close(fd);
    shmdt(str);
}

int main() {

    char* path = (char*) malloc(sizeof(char) * 100);
    printf("[MAIN] Enter path of the file that is to be uploaded: ");
    scanf("%s", path);

    upload(path);

    return 0;
}
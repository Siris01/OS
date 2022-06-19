#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>

void download(char *path)
{
    int shmid = shmget(69, 1024, 0666 | IPC_CREAT);

    char *str = (char *)shmat(shmid, (void *)0, 0);

    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd == -1)
    {
        printf("Error while opening file!");
        exit(1);
    }

    write(fd, str, strlen(str));

    printf("[CLIENT] File contents: %s\n", str);
    printf("[CLIENT] File saved in %s\n", path);

    close(fd);
    shmdt(str);
    shmctl(shmid, 0, NULL);
}

int main()
{

    char *path = (char *)malloc(sizeof(char) * 100);
    printf("[MAIN] Enter path where the file is to be downloaded: ");
    scanf("%s", path);

    download(path);

    return 0;
}
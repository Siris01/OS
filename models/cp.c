#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define MAX 4096

void cp(char *src, char *dest, int overwrite)
{
    int fd1 = open(src, 00);
    int fd2 = open(dest, 00);

    if (fd2 > -1 && overwrite == 0)
    {
        printf("Destination file already exists");
        return;
    }
    else
        fd2 = open(dest, 02 | 0100, 0666);

    char *buffer = malloc(MAX);

    int sz = read(fd1, buffer, MAX);
    buffer[sz] = '\0';
    write(fd2, buffer, strlen(buffer));

    free(buffer);
    close(fd1);
    close(fd2);

    return;
}

int main(int argc, char *argv[])
{
    if (argc < 3 || argc > 4)
    {
        printf("Usage: %s <src> <dest> [-n]\n", argv[0]);
        return 1;
    }

    int overwrite = 1;
    if (argv[3] && strcmp(argv[3], "-n") == 0)
        overwrite = 0;

    cp(argv[1], argv[2], overwrite);

    return 0;
}
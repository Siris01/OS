#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

void head(char *file, int lines)
{
    int fd = open(file, 0);
    if (fd == -1)
    {
        printf("Error: could not open file %s\n", file);
        exit(1);
    }

    char *buff = malloc(BUFFER_SIZE);
    if (read(fd, buff, BUFFER_SIZE) == 0)
    {
        printf("Error: could not read file %s. Check if the file has some content\n", file);
        exit(1);
    }

    char *line = strtok(buff, "\n");
    int n = 0;
    while (line != NULL && n < lines)
    {
        n++;
        printf("%s\n", line);
        line = strtok(NULL, "\n");
    }

    free(buff);
}

int main(int argc, char *argv[])
{
    head(argv[1], argc >= 3 ? atoi(argv[2]) : 10);

    return 0;
}
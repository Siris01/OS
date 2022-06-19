#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

void wc(char *file)
{
    int fd = open(file, 0);
    if (fd == -1)
    {
        printf("Error: could not open file %s\n", file);
        exit(1);
    }
    int l = 0, b = 0, c = 0, w = 0;

    char *buff = malloc(BUFFER_SIZE);

    b = read(fd, buff, BUFFER_SIZE);
    c = strlen(buff);

    char *line = strtok(buff, "\n");
    while (line != NULL)
    {
        l++;
        char *word = strtok(line, " ");
        while (word != NULL)
        {
            w++;
            word = strtok(NULL, " ");
        }
        line = strtok(NULL, "\n");
    }

    free(buff);

    printf("lines: %d\nBytes: %d\nCharacters: %d\nWords: %d\n", l, b, c, w);
}

int main(int argc, char *argv[])
{
    wc(argv[1]);

    return 0;
}
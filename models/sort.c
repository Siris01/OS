#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

void sort(char *file)
{
    int fd = open(file, 0);
    if (fd == -1)
    {
        printf("Error: could not open file %s\n", file);
        exit(1);
    }

    char *buff = malloc(BUFFER_SIZE);
    int l = 0;
    char *lines[BUFFER_SIZE];

    read(fd, buff, BUFFER_SIZE);
    char *line = strtok(buff, "\n");
    while (line != NULL)
    {
        lines[l] = malloc(strlen(line) + 1);
        strcpy(lines[l], line);
        l++;
        line = strtok(NULL, "\n");
    }

    free(buff);

    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < l - 1; j++)
        {
            if (strcmpi(lines[j], lines[j + 1]) > 0)
            {
                char *tmp = malloc(strlen(lines[j]) + 1);
                strcpy(tmp, lines[j]);
                strcpy(lines[j], lines[j + 1]);
                strcpy(lines[j + 1], tmp);
                free(tmp);
            }
        }
    }

    for (int i = 0; i < l; i++)
    {
        printf("%s\n", lines[i]);
    }
}

int main(int argc, char *argv[])
{
    sort(argv[1]);

    return 0;
}
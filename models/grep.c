#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

void grep(char *pattern, char *filename, int show_line_numbers)
{
    int fd = open(filename, 0);
    if (fd == -1)
    {
        printf("Error: could not open file %s\n", filename);
        exit(1);
    }

    char *buff = malloc(BUFFER_SIZE);
    if (read(fd, buff, BUFFER_SIZE) == 0)
    {
        printf("Error: could not read file %s. Check if the file has some content\n", filename);
        exit(1);
    }

    char *line = strtok(buff, "\n");
    int n = 0;
    while (line != NULL)
    {
        n = n + 1;

        if (strstr(line, pattern))
        {
            if (show_line_numbers)
            {
                printf("%d | %s\n", n, line);
            }
            else
            {
                printf("%s\n", line);
            }
        }
        line = strtok(NULL, "\n");
    }
}

int main(int argc, char *argv[])
{
    if (argc < 3 || argc > 4)
    {
        printf("Usage: %s <pattern> <file> [-n]\n", argv[0]);
        return 1;
    }

    char *pattern = argv[1];
    char *filename = argv[2];
    int show_line_numbers = 0;

    if (argv[3] && strcmp(argv[3], "-n") == 0)
        show_line_numbers = 1;

    grep(pattern, filename, show_line_numbers);

    return 0;
}
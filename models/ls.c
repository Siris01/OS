#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#define BUFFER_SIZE 1024

void ls(char *dir, int show_all_files)
{
    DIR *fd = opendir(dir);
    if (!fd)
    {
        printf("Error: Unable to access given directory");
        return;
    }

    struct dirent *d;

    while ((d = readdir(fd)) != NULL)
    {
        if (d->d_name[0] == '.')
        {
            if (show_all_files)
                printf("%s\n", d->d_name);
        }
        else
            printf("%s\n", d->d_name);
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2 || argc > 3)
    {
        printf("Usage: %s <directory> [-a]\n", argv[0]);
        return 1;
    }

    char *dir = argv[1];
    int show_all_files = 0;

    if (argv[2] && strcmp(argv[2], "-a") == 0)
        show_all_files = 1;

    ls(dir, show_all_files);

    return 0;
}
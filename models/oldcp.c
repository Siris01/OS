#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#define SIZE 2500

void copy(char *src, char *dest, int overwrite)
{

	int fd1 = open(src, O_RDONLY);
	if (fd1 == -1)
		return printf("\nUnable to open %s", src);
	int fd2 = overwrite == 1 ? open(dest, O_RDWR | O_CREAT) : creat(dest, O_CREAT);
	if (fd2 == -1 && overwrite == 0)
		return printf("\nDestination file already exists");
	if (fd2 == -1)
		return printf("\nUnable to create %s", dest);

	char *c = (char *)calloc(SIZE, sizeof(char));
	int sz = read(fd1, c, SIZE);
	if (sz == -1)
		return printf("\nUnable to read contents of %s", src);
	c[sz] = '\0';

	int res = write(fd2, c, strlen(c));
	if (res == -1)
		return printf("\nUnable to write contents to %s", dest);

	printf("Successfully written %d bytes from %s to %s!", res, src, dest);
}

int main(int argc, char *argv[])
{
	if (argc < 3 || argc > 4)
		return printf("Invalid Syntax! Correct syntax is:\n%s <source> <destination>\n", argv[0]);
	if (argc == 3)
	{
		copy(argv[1], argv[2], 1);
		return 0;
	}

	copy(argv[1], argv[2], (strcmp(argv[3], "-n") == 0 || strcmp(argv[3], "--no-overwrite") == 0) ? 0 : 1);
	return 0;
}

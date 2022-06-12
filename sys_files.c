#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <unistd.h>
#define SIZE 250

int main( int argc, char *argv[] ) {
    char *c = (char *) calloc(SIZE, sizeof(char)); 
    
    int fd = open(argv[1], O_RDONLY); 
    printf("Opened file with file descriptor %d\n\n", fd);
    int sz = read(fd, c, SIZE); 
    c[sz] = '\0';
    printf("Contents:\n\n\n%s\n\n", c);
    
    close(fd);
    printf("Closed file.");
    
    return 0;
}
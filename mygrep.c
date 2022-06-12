#include <stdio.h>
#include <stdlib.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <string.h> 
#define SIZE 2500 

void search (char* pattern, char* file, int show_line_numbers) {
	int fd = open(file, O_RDONLY);
	if (fd == -1) return printf("\nUnable to open %s", file);

	char* c=(char*) calloc(SIZE,sizeof(char));
	int sz = read(fd, c, SIZE);
	if (sz == -1) return printf("\nUnable to read contents of %s", file);
	c[sz] ='\0';

	int lc = 0;

	char * token = strtok(c, "\n");
   	while ( token != NULL ) {
		   lc++;
      		if (strstr(token, pattern) != NULL) {
			if (show_line_numbers == 1) printf("%d:%s\n", lc, token);
			else printf("%s\n", token);
		}
      		token = strtok(NULL, "\n");
   	}
}

int main (int argc, char* argv[]){
	if (argc > 4) return printf("Invalid Syntax! Correct syntax is:\n%s <pattern> <file>\n", argv[0]);
	if (argc == 3) search(argv[1], argv[2], 0);
	else search(argv[1], argv[2], strcmp(argv[3], "-n") == 0 ? 1 : 0);

	return 0;
}

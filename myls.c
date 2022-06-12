#include <stdio.h>
#include <stdlib.h> 
#include <dirent.h> 
#include <string.h> 
#include <string.h>

void list (char* src, int show_hidden) {
	DIR* dp = opendir(src);
 	struct dirent *dirp;

	if (dp == NULL) return printf("\nUnable to open directory %s", src);

	while ((dirp = readdir(dp)) != NULL)
        if (show_hidden == 0 && dirp->d_name[0] == '.') continue;
		else printf("%s\n", dirp->d_name);

    	closedir(dp);
}

int main (int argc, char* argv[]){
	if (argc > 3) return printf("Invalid Syntax! Correct syntax is:\n%s <source>\n", argv[0]);
	if (argc == 3) list(argv[1], strcmp(argv[2], "-a") == 0 ? 1 : 0);
	else list(argc == 1 ? "./": argv[1], 0);

	return 0;
}

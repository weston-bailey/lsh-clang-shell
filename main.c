#include <stdlib.h>
#include <stdio.h>

void lsh_loop(void);

int main (int argc, char *argv[]) {
	// Load config files, if any.
	
	// run command loop
	lsh_loop();	
	// perform any shutdown/cleanup
	return EXIT_SUCCESS;
}

void lsh_loop(void) 
{
	char *line;
	char **args;
	int status;

	do {
		printf("> ");
		/* line = lsh_read(); */
		/* args = lsh_split_lint(line); */
		/* status = lsh_exectute(args); */

		/* free(line); */
		/* free(args); */

	} while (status);
}

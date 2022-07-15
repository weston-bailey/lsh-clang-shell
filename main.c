#include <stdlib.h>
#include <stdio.h>

#define LSH_RL_BUFSIZE 1024

void lsh_loop(void);
char *lsh_read_line(void);

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
	/* char **args; */
	int status = 0;

	do {
		printf("> ");
		line = lsh_read_line();
		/* args = lsh_split_line(line); */
		/* status = lsh_exectute(args); */

		free(line);
		/* free(args); */

	} while (status);
}

char *lsh_read_line(void)
{
	int bufsize = LSH_RL_BUFSIZE;
	int position = 0;
	char *buffer = malloc(sizeof(char) * bufsize);
	int c = 0;

	if (!buffer) {
		// print to file
		fprintf(stderr, "lsh: allocation error\n:");
		exit(EXIT_FAILURE);
	}

	while (1) {
		// read a character
		c = getchar();
		// if we hit EOF, replace it with null character and return
		if (c == EOF || c == '\n') {
			buffer[position] = '\0';
			return buffer;
		} else {
			buffer[position] = c;
			printf("the buffer is %s\n", buffer);
		}

		position++;

		// if we have exceeded the buffer, reallocate.
		if (position >= bufsize) {
			bufsize += LSH_RL_BUFSIZE;
			buffer = realloc(buffer, bufsize);
			
			if (!buffer) {
				// print to file
				fprintf(stderr, "lsh: allocation error\n:");
				exit(EXIT_FAILURE);
			}
		}
	}
}

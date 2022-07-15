#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define LSH_RL_BUFSIZE 1024
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM "\t\r\n\a"

void lsh_loop(void);
char *lsh_read_line(void);
char **lsh_split_line(char *line);
int lsh_launch(char **args);
int lsh_execute(char **args);

// builtins 
int lsh_cd(char **args);
int lsh_help(char **args);
int lsh_exit(char **args);
int lsh_num_builtins(void);

// builtin commands and their corresponding functions
char *builtin_str[] = {
	"cd",
	"help",
	"exit"
};

// array of function pointers that take an array for strings and return an int
int (*builtin_func[]) (char **) = {
	&lsh_cd,
	&lsh_help,
	&lsh_exit
};

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
	int status = 0;

	do {
		printf("> ");
		line = lsh_read_line();
		args = lsh_split_line(line);
		status = lsh_execute(args);

		free(line);
		free(args);

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
			/* printf("the buffer is %s\n", buffer); */
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

char **lsh_split_line(char *line)
{
	int bufsize = LSH_TOK_BUFSIZE;
	int position = 0;
	char **tokens = malloc(bufsize * sizeof(char*));
	char *token;

	if (!tokens) {
		fprintf(stderr, "lsh: allocation error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, LSH_TOK_DELIM);
	while (token != NULL) {
		tokens[position] = token;
		position++;

		if (position >= bufsize) {
			bufsize += LSH_TOK_BUFSIZE;
			tokens = realloc(tokens, bufsize * sizeof(char*));
			if (!tokens) {
				fprintf(stderr, "lsh: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}
		
		token = strtok(NULL, LSH_TOK_DELIM);
	}
	
	tokens[position] = NULL;
	return tokens;
			
}

int lsh_launch(char **args)
{
	pid_t pid; 
	pid_t wpid;
	int status;

	pid = fork();

	if (pid == 0) {
		// child process
		if (execvp(args[0], args) == -1) {
			perror("lsh");
		}
		exit(EXIT_FAILURE);
	} else if (pid < 0) {
		// Error forking
		perror("lsh");
	} else {
		// parent process
		do {
			wpid = waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	
	return 1;		
}

int lsh_num_builtins(void) 
{
	return sizeof(builtin_str) / sizeof(char *);
}

int lsh_cd(char **args) {
	if (args[1] == NULL) {
		fprintf(stderr, "lsh: expected argument to \"cd\"\n");
	} else {
		if (chdir(args[1]) != 0) {
			perror("lsh");
		}
	}

	return 1;
}

int lsh_help(char **args)
{
  int i;
  printf("lsh shell\n");

  for (i = 0; i < lsh_num_builtins(); i++) {
    printf("  %s\n", builtin_str[i]);
  }

  printf("Use the man command for information on other programs.\n");
  return 1;
}

int lsh_exit(char **args) 
{
	return 0;
}

int lsh_execute(char **args)
{
	int i = 0;
	// an empty command was entered
	if (args[0] == NULL) {
		return 1;
	}

	for (i = 0; i < lsh_num_builtins(); i++) {
		if (strcmp(args[0], builtin_str[i]) == 0) {
			return (*builtin_func[i])(args);
		}
	}

	return lsh_launch(args);
}

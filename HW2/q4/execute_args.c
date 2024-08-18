//used resources 
//https://www.digitalocean.com/community/tutorials/execvp-function-c-plus-plus
// https://www.geeksforgeeks.org/chdir-in-c-language-with-examples/


#include "shell.h"
#include <unistd.h>

int execute_args(char **args)
{
	pid_t pid;
	int status;
	char *builtin_func_list[] = {
		// you should add something here
		"cd",
		"exit",
	};
	int (*builtin_func[])(char **) = {
		// you should add something here
		&own_cd,
		&own_exit,
	};

	long unsigned int i = 0;
	for (; i < sizeof(builtin_func_list) / sizeof(char *); i++)
	{
		/* if there is a match execute the builtin command */
		if (strcmp(args[0], builtin_func_list[i]) == 0)
		{
			return ((*builtin_func[i])(args));
		}
	}

	// TODO
	pid = fork();
	if (pid == 0)
	{
		int statusCode = execvp(args[0], args);
		if (statusCode == -1)
		{
			perror("faced with error while executing !!");
		}
		exit(0);
	}
	else if (pid < 0)
	{
		perror("Error forking process");
	}
	else
	{
		waitpid(pid, &status, 0); 
	}

	return (-1); // this status code means everything is ok
}
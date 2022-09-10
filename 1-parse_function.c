#include "shell.h"
/**
 * parse_line - Parses the command line looking for commands and argumements.
 * This function it is also in charged of freeing memory that is not longer
 * needed by the program.
 * @line: A pointer to a string. Will always be NULL upon function entry.
 * @size: A holder for numbers of size_t. Will always be initilized to 0.
 * @command_counter: A counter keeping track of how many commands have been
 * entered into the shell.
 * @av: Name of the program running the shell
 */
void parse_line(char *line, size_t size, int command_counter, char **av)
{
	int i;
	ssize_t read_len;
	int token_count;
	char **param_array;
	const char *delim = "\n\t ";

	token_count = 0;
	write(STDOUT_FILENO, PROMPT, str_len(PROMPT));
	read_len = getline(&line, &size, stdin);
	if (read_len != -1)
	{
		param_array = token_interface(line, delim, token_count);
		if (param_array[0] == NULL)
		{
			single_free(2, param_array, line);
			return;
		}
		i = built_in(param_array, line);
		if (i == -1)
			create_child(param_array, line, command_counter, av);
		for (i = 0; param_array[i] != NULL; i++)
			free(param_array[i]);
		single_free(2, param_array, line);
	}
	else
		exit_b(line);
}
/**
 * create_child - Creates a child in order to execute another program.
 * @param_array: An array of pointers to strings containing the possible name
 * of a program and its parameters. This array is NULL terminated.
 * @line: The contents of the read line.
 * @count: A counter keeping track of how many commands have been entered
 * into the shell.
 * @av: Name of the program running the shell
 */
void create_child(char **param_array, char *line, int count, char **av)
{
	pid_t id;
	int status;
	int i;
	int check;
	struct stat buf;
	char *tmp_command;
	char *command;

	id = fork();
	if (id == 0)
	{
		tmp_command = param_array[0];
		command = path_finder(param_array[0]);
		if (command == NULL)
		{
			/*Looking for file in current directory*/
			check = stat(tmp_command, &buf);
			if (check == -1)
			{
				error_printing(av[0], count, tmp_command);
				print_str(": not found", 0);
				single_free(2, line, tmp_command);
				for (i = 1; param_array[i]; i++)
					free(param_array[i]);
				free(param_array);
				exit(100);
			}
			/*file exist in cwd or has full path*/
			command = tmp_command;
		}
		param_array[0] = command;
		if (param_array[0] != NULL)
		{
			if (execve(param_array[0], param_array, environ) == -1)
				exec_error(av[0], count, tmp_command);
		}
	}
	else
		wait(&status);
}



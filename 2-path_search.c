#include "shell.h"
/**
 * search_directories - Looks through directories stored in path_tokens for a
 * specific file. aka commmand.
 * @path_tokens: A pointer to an array of strings representing the different
 * paths contained in the PATH environmental varible.
 * @command: Represents a command. For example ls, echo, pwd, /bin/ls etc.
 * Return: Upon success a string with the upper most directory containing
 * the command file. Otherwise returns NULL.
 */
char *search_directories(char **path_tokens, char *command)
{
	int i, s;
	char *cwd;
	char *buf;
	size_t size;
	struct stat stat_buf;

	buf = NULL;
	size = 0;
	cwd = getcwd(buf, size);
	if (cwd == NULL)
		return (NULL);
	if (command[0] == '/')
		command = command + 1;
	for (i = 0; path_tokens[i] != NULL; i++)
	{
		s = chdir(path_tokens[i]);
		if (s == -1)
		{
			perror("ERROR!");
			return (NULL);
		}
		s = stat(command, &stat_buf);
		if (s == 0)
		{
			chdir(cwd);
			free(cwd);
			return (path_tokens[i]);
		}
	}
	chdir(cwd);
	free(cwd);
	return (NULL);
}

/**
 * build_path - Combines two strings one representing the path directory and
 * one representing the command file.
 * @directory: Represents a directory in the path.
 * @command: Represents a file in a directory of the path.
 * Return: Upon success a string representing the full path of a command.
 * Otherwise NULL.
 */
char *build_path(char *directory, char *command)
{
	int i, j;
	int dir_len;
	int command_len;
	int len;
	char *built;

	if (directory == NULL || command == NULL)
		return (NULL);
	dir_len = str_len(directory) + 1;
	command_len = str_len(command) + 1;
	len = dir_len + command_len;
	built = malloc(sizeof(char) * len);
	if (built == NULL)
		return (NULL);
	for (i = 0; i < len; i++)
	{
		for (j = 0; directory[j] != '\0'; j++, i++)
			built[i] = directory[j];
		built[i] = '/';
		i++;
		for (j = 0; command[j] != '\0'; j++, i++)
			built[i] = command[j];
	}
	built[--i] = '\0';
	return (built);
}

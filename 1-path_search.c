#include "shell.h"

/**
 * path_finder - Acts as an interface for functions that will be able to
 * find the full path of a program.
 * @command: Represents a command. For example ls, echo, pwd, etc.
 * Return: Upon sucess a string with the full path of the program.
 * for example /bin/ls, /bin/echo, etc. Otherwise returns NULL.
 */
char *path_finder(char *command)
{
	char *str = "PATH";
	char *constructed;
	char **path_tokens;
	int index;
	char *directory;

	index = find_path(str);
	path_tokens = tokenize_path(index, str);
	if (path_tokens == NULL)
		return (NULL);

	directory = search_directories(path_tokens, command);
	if (directory == NULL)
	{
		double_free(path_tokens);
		return (NULL);
	}

	constructed = build_path(directory, command);
	if (constructed == NULL)
	{
		double_free(path_tokens);
		return (NULL);
	}
	double_free(path_tokens);
	return (constructed);
}

/**
 * find_path - Finds the index of an environmental variable.
 * @str: Environmental variable that needs to be found.
 * Return: Upon success returns the index of the environmental variable.
 * otherwise returns -1.
 */
int find_path(char *str)
{
	int i;
	int len;
	int j;

	len = str_len(str);
	for (i = 0; environ[i] != NULL; i++)
	{
		for (j = 0; j < len; j++)
		{
			if (environ[i][j] != str[j])
				break;
		}
		if (j == len && environ[i][j] == '=')
			return (i);
	return (-1);
}

/**
 * tokenize_path - Separates a string representing paths as an array
 * of strings contining the path directories.
 * @index: Index of the path in the environment variables.
 * @str: string to separate and tokenize.
 * Return: Upon success a NULL terminated array of pointer to strings.
 * Otherwise returns NULL. Note!: Do not forget to free alocated
 * memory on receiving function or when possible.
 */
char **tokenize_path(int index, char *str)
{
	char *env_var;
	int token_count;
	const char *delim = ":\n";
	char **path_tokens;
	int len;

	len = str_len(str);
	token_count = 0;
	/*Moving the pointer len of str plus = sign*/
	env_var = environ[index] + (len + 1);
	path_tokens = token_interface(env_var, delim, token_count);
	if (path_tokens == NULL)
		return (NULL);
       	return (path_tokens);
}


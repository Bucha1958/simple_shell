#include "shell.h"
#include <string.h>

/**
 * main - Entry point
 * @argc : command line argument int
 * @argv : Argument vector
 * Return : Always 0
 */
int main(int argc __attribute__((unused)), char *argv[])
{
	char *line;
	size_t size;
	int command_counter;

	command_counter = 0;
	signal(SIGINT, SIG_IGN);
	do {
		command_counter++;
		line = NULL;
		size = 0;
		parse_line(line, size, command_counter, argv);
	} while (1);
	return (0);
}
	

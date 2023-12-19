#include "minishell.h"
#include <stdbool.h>
#include "libft/libft.h"
#include <stdio.h>
#include <stdlib.h>

bool	is_builtin(char *command)
{
	if (ft_strncmp(command, "echo", 4) == 0
		|| ft_strncmp(command, "cd", 2) == 0
		|| ft_strncmp(command, "pwd", 3) == 0
		|| ft_strncmp(command, "export", 6) == 0
		|| ft_strncmp(command, "unset", 5) == 0
		|| ft_strncmp(command, "env", 3) == 0
		|| ft_strncmp(command, "exit", 4) == 0)
		return (true);
	return (false);
}

// n_switch is true if -n is passed, str is modified but not freed
int echo(char *str, const bool n_switch)
{
	if (!str || !str[0])
		str = "\n";
	else if (n_switch && str[ft_strlen(str) - 1] == 'n' && str[ft_strlen(str) - 2] == '\\')
		str = ft_substr(str, 0, ft_strlen(str) - 2);
	if (ft_printf(str) == -1)
	{
		perror("Minishell: echo: write error");
		return (1);
	}
	return (0);
}

int pwd(void)
{
	if (ft_printf(g_cwd) == -1)
		return (1);
	return (0);
}

void exit_shell(void)
{
	// TODO: freeing
	exit(0);
}

int builtin(char **command)
{
	if (ft_strncmp(command[0], "exit", 4) == 0)
		exit_shell();
	if (ft_strncmp(command[0], "pwd", 3) == 0)
		pwd();
	if (ft_strncmp(command[0], "exit", 4) == 0)
		exit_shell();
	if (ft_strncmp(command[0], "exit", 4) == 0)
		exit_shell();
	if (ft_strncmp(command[0], "exit", 4) == 0)
		exit_shell();
}

#include "minishell.h"
#include <stdbool.h>
#include "libft/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include "builtin.h"
#include <unistd.h>

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
	char	*w;

	w = getcwd(NULL, 0);
	if (ft_printf(w) == -1)
	{
		free(w);
		return (1);
	}
	free(w);
	return (0);
}

void exit_shell(void)
{
	// TODO: free data
	exit(0);
}

int builtin(char **command)
{
	if (ft_strncmp(command[0], "exit", 4) == 0)
		return (exit_shell(), 0);
	if (ft_strncmp(command[0], "pwd", 3) == 0)
		return (pwd());
	if (ft_strncmp(command[0], "cd", 2) == 0)
		cd(command[1]);
	if (ft_strncmp(command[0], "echo", 4) == 0)
	{
		if (ft_strncmp(command[1], "-n", 2) == 0)
			return (echo(command[2], true));
		if (command[1] && ft_strncmp(command[2], "-n", 2) == 0)
			return (echo(command[1], true));
		return (echo(command[1], false));
	}
	if (ft_strncmp(command[0], "export", 6) == 0)
		export(command[1], command[2]);
	if (ft_strncmp(command[0], "unset", 5) == 0)
		unset(command[1], __environ);
	if (ft_strncmp(command[0], "env", 3) == 0)
		return (env());
	return (0);
}

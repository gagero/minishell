#include <stdbool.h>
#include "libft/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include "builtin.h"
#include <unistd.h>
#include "minishell.h"

bool	is_builtin(char *command)
{
	if (!command)
		return (false);
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
int echo(char **command)
{
	int		i;
	char	*to_print;
	bool	trim;

	i = 0;
	trim = false;
	while(command[i])
		i++;
	if (i == 1)
		to_print = ft_strdup("\n");
	else if (i == 2)
	{
		if (ft_strncmp("-n", command[1], ft_strlen(command[1])) == 0)
			to_print = ft_strdup("\n");
		else
			to_print = ft_strdup(command[1]);
	}
	else
	{
		i = 1;
		if (ft_strncmp("-n", command[1], ft_strlen(command[1])) == 0)
		{
			i = 2;
			trim = true;
		}
		to_print = NULL;
		while(command[i])
		{
			to_print = ft_strjoin(to_print, command[i]);
			i++;
		}
	}
	if (ft_putstr_fd(to_print, 1) == -1 || (!trim && write(1, "\n", 1) == -1))
	{
		write(2, "Minishell: echo: write error\n", 29);
		return (1);
	}
	return (0);
}

int pwd(void)
{
	char	*w;
	char	*n;

	w = getcwd(NULL, 0);
	if (error((!w), "malloc error"))
		return (1);
	n = malloc(ft_strlen(w) + 2);
	ft_strlcpy(n, w, ft_strlen(w) + 1);
	ft_strlcat(n, "\n", ft_strlen(w) + 2);
	if (ft_printf(n) == -1)
	{
		free(w);
		free(n);
		return (1);
	}
	free(w);
	free(n);
	return (0);
}

void exit_shell(void)
{
	// TODO: free data
	exit(0);
}

int builtin(char **command)
{
	int ret;

	ret = 0;
	if (!ft_strncmp(command[0], "exit", 4))
		exit_shell();
	else if (!ft_strncmp(command[0], "pwd", 3))
		ret += pwd();
	else if (!ft_strncmp(command[0], "cd", 2))
		ret += cd(command[1]);
	else if (!ft_strncmp(command[0], "env", 3))
		ret += env();
	else if (!ft_strncmp(command[0], "echo", 4))
		ret += echo(command);
	// TODO: rest of the builtins
	else
		return (1);
	if (error((ret), "builtin error"))
		return (1);
	return (0);
}

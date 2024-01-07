#include "minishell.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "libft/libft.h"
#include <unistd.h>
#include "builtin.h"

// Adds to __environ if is_export is true, otherwise returns a copy with the variable added
char **putenviron(char *name, char *value, bool is_export, char ***last_environ)
{
	static int		env_len;
	char			**new_environ;
	char			*old;
	char			*new;

	while(*__environ && __environ[++env_len])
		;
	new_environ = malloc((env_len + 2) * sizeof(char*));
	old = ft_strjoin(name, "=");
	new = ft_strjoin(old, value);
	if (!new_environ || !old || !new)
	{
		free(old);
		free(new);
		return (NULL);
	}
	env_len = 0;
	while(__environ[env_len])
	{
		new_environ[env_len] = __environ[env_len];
		env_len++;
	}
	free(old);
	new_environ[env_len++] = new;
	new_environ[env_len] = NULL;
	if (is_export)
	{
		free(*last_environ);
		*last_environ = __environ;
		__environ = new_environ;
	}
	return (new_environ);
}

int	unset(char *name, char **old)
{
	int		env_len;
	int		name_len;
	bool	move_flag;

	env_len = -1;
	name_len = ft_strlen(name);
	move_flag = false;
	while(old && old[++env_len])
	{
		if (move_flag)
		{
			old[env_len] = old[env_len + 1];
			if (!old[env_len])
				break ;
		}
		if (ft_strncmp(old[env_len], name, name_len) == 0)
		{
			free(old[env_len]);
			move_flag = true;
		}
	}
	return (0);
}


int	env(void)
{
	int	i;

	i = 0;
	while(__environ[i])
	{
		if (ft_printf(__environ[i]) == -1)
		{
			perror("Minishell: env: write error");
			return (1);
		}
		if (ft_printf("\n") == -1)
		{
			perror("Minishell: env: write error");
			return (1);
		}
		i++;
	}
	return (0);
}

int cd(char *new)
{
	if (!new)
	{
		if (chdir(getenv("HOME")) == -1)
		{
			perror("Minishell");
			return (1);
		}
	}
	else if (chdir(new) == -1)
	{
		perror("Minishell");
		return (1);
	}
	return (0);
}

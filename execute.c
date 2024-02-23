#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "libft/libft.h"
#include "minishell.h"
#include <stdbool.h>
#include "builtin.h"
#include <errno.h>

// Frees handle if bool is true
void	free2d(char **ptr, bool handle)
{
	int	i;

	i = 0;
	while(ptr[i])
	{
		free(ptr[i]);
		i++;
	}
	if (handle)
		free(ptr);
}

static char	*append(char *command, char *split)
{
	char	*ret;
	char	*joined;

	joined = "";
	if (split[ft_strlen(split)] != '/')
		joined = ft_strjoin(split, "/");
	if (!joined)
		return (NULL);
	ret = ft_strjoin(joined, command);
	free(joined);
	return (ret);
}

static char	*find_path(char **split, char *command)
{
	int			i;
	static char	*path;

	i = 0;
	while (split[i])
	{
		path = append(command, split[i]);
		if (!path)
		{
			path = NULL;
			break ;
		}
		if (access(path, X_OK) == 0)
			break ;
		else
		{
			free(path);
			path = NULL;
		}
		free(path);
		i++;
	}
	/* free(command); */
	free2d(split, true);
	return (path);
}

static char	*find_command_path(char *command)
{
	char	**split;
	char	*cur;
	char	*joined;

	if (is_builtin(command))
		return (command);
	cur = getcwd(NULL, 0);
	joined = ft_strjoin(cur, command);
	free(cur);
	if (access(joined, X_OK) == 0)
		return (joined);
	free(joined);
	if (access(command, X_OK) == 0)
		return (command);
	split = ft_split(getenv("PATH"), ':');
	if (!split)
		return (NULL);
	return (find_path(split, command));
}

static void child_error(bool expr, int errno_value)
{
	if (expr)
	{
		errno = errno_value;
		perror("Minishell");
		exit(1);
	}
}

// TODO: error handling
static void child_init(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

static pid_t	run_command(char **command, int input, int output)
{
	char	*err_str;
	pid_t	ret;
	int		i;

	i = 0;
	while(g_running_processes[i])
		i++;
	ret = fork();
	g_running_processes[i] = ret;
	if (ret == 0)
	{
		child_init();
		if (input != STDIN_FILENO)
		{
			child_error((dup2(input, STDIN_FILENO) < 0), errno);
			close(input);
		}
		if (output != STDOUT_FILENO)
		{
			child_error((dup2(output, STDOUT_FILENO) < 0), errno);
			close(output);
		}
		if (execve(command[0], command, __environ) == -1)
		{
			err_str = ft_strjoin("Minishell: ", command[0]);
			perror(err_str);
			free(err_str);
			exit(1);
		}
	}
	if (ret < 0)
		perror("Minishell: fork error");
	g_running_processes[i] = 0;
	if (input != STDIN_FILENO)
		close(input);
	if (output != STDOUT_FILENO)
		close(output);
	return (ret);
}

int	wait_en_masse(void)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	while (g_running_processes[i])
	{
		waitpid(g_running_processes[i], &status, 0);
		if (WIFEXITED(status))
			status = WEXITSTATUS(status);
		i++;
	}
	return (status);
}

int	execute(char **command, int input, int output) // FIXME: command is NULL
{
	int	i;

	// TODO: place into parser
	/* if (command && is_builtin(command[0])) */
	/* 	return (builtin(command)); */
	command[0] = find_command_path(command[0]);
	if (command[0] == NULL)
	{
		errno = ENOENT;
		perror("Minishell");
		return (1);
	}
	run_command(command, input, output);
	i = 0;
	while (g_running_processes[i])
		i++;
	if (g_running_processes[i] < 0)
		return (1);
	free2d(command, true);
	return (0);
}

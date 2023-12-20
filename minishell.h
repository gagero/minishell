#ifndef MINISHELL_H
# define MINISHELL_H
#include <stdbool.h>
#include <sys/types.h>

extern char	*g_cwd;
extern char	**g_last_environ;
extern int	g_last_proc_code;
extern pid_t running_process;

void	free2d(char **ptr, bool handle);
int		execute(char **command, int input, int output, int *status, char **environment);
char	*substitute(char *buf);
#endif

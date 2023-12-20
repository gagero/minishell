#ifndef MINISHELL_H
# define MINISHELL_H
#include <stdbool.h>

extern char	*g_cwd;
extern char	**g_last_environ;
extern int	g_last_proc_code;

void	free2d(char **ptr, bool handle);
int		execute(char **command, int input, int output, int *status);

#endif

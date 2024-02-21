#ifndef MINISHELL_H
# define MINISHELL_H
#include <stdbool.h>
#include <sys/types.h>

extern pid_t *g_running_processes;

void	free2d(char **ptr, bool handle);
int		execute(char **command, int input, int output);
int		wait_en_masse(void);
char	*substitute(const char *buf);
int wait_for_proc(void);
#endif

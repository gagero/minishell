#ifndef MINISHELL_H
# define MINISHELL_H
#include "lexer.h"
#include <stdbool.h>
#include <sys/types.h>

extern pid_t *g_running_processes;

#define	ERROR(expr, message) error(expr, message, __FILE_NAME__, __LINE__)

void	free2d(char **ptr, bool handle);
int		execute(char **command, int input, int output);
int		wait_en_masse(void);
t_type *substitute(t_type *buf, int *last_code);
int	error(bool expr, char *message, char *file, int line);
void generic_sig_handler(int sig);
#endif

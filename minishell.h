#ifndef MINISHELL_H
# define MINISHELL_H
#include <stdbool.h>

extern char	*g_cwd;
extern char	**g_last_environ;
extern int	g_last_proc_code;

void free2d(char **ptr);
int builtin(char **command);
bool	is_builtin(char *command);
int	export(char *name, char *word);
int	unset(char *name, char **old);
int	env(void);
char	*find_command_path(char *command);
#endif

#ifndef BUILTIN_H
# define BUILTIN_H

char	**putenviron(char *name, char *value, bool is_export, char ***last_environ);
int		unset(char *name, char **old);
int		env(void);
int		cd(char *new_path);
bool	is_builtin(char *command);
void	exit_shell(void);

#endif

#ifndef BUILTIN_H
# define BUILTIN_H

char	**putenviron(char *name, char *value, bool is_export);
int		export(char *name, char *value);
int		unset(char *name, char **old);
int		env(void);
int		cd(char *new_path);
int		builtin(char **command);
bool	is_builtin(char *command);

#endif

#include "minishell.h"
#include "testing.h"
#include <readline/readline.h>
#include "libft/libft.h"
#include <stdlib.h>
#include <unistd.h>

char	**g_last_environ = NULL;
int		g_last_proc_code = 0;

// TODO: set terminal lflag to ISIG for SIGQUIT (termios_p), history

int main(void)
{
	char	*command;
	char	*prompt;
	char	**split;
	char	*cwd;

	if (chdir(getenv("HOME")) == -1)
	{
		perror("Minishell: init error");
		return (1);
	}
	while (1) // FIXME: while loop segfaults (readline returns NULL while execute runs (check waitpid)?)
	{
	cwd = getcwd(NULL, 0); // cosmetic
		prompt = ft_strjoin(cwd, "> ");
		command = readline(prompt);
		if (!command || !command[0])
			continue ;
		free(cwd);
		free(prompt);
		if (ft_strchr(command, '$'))
			command = substitute(command); // this is necessary, operates on the input string, not tokens. the rest of main is for testing and showcase, they will operate on tokens in some way.
		split = ft_split(command, ' '); // treat split as a stand-in for tokens
		execute(split, STDIN_FILENO, STDOUT_FILENO, &g_last_proc_code);
	}
}

#include "minishell.h"
#include "testing.h"
#include <readline/readline.h>
#include "libft/libft.h"
#include <stdlib.h>
#include <unistd.h>

char	**g_last_environ = NULL;
int		g_last_proc_code = 0;

int main(void)
{
	char	*command;
	char	*prompt;
	char	**split;
	int		ret;
	char	*cwd;

	if (chdir(getenv("HOME")) == -1)
	{
		perror("Minishell: init error");
		return (1);
	}
	/* while (1) */ // segfaults
	/* { */
		cwd = getcwd(NULL, 0);
		prompt = ft_strjoin(cwd, "> ");
		command = readline(prompt);
		free(cwd);
		free(prompt);
		split = ft_split(command, ' ');
		execute(split, STDIN_FILENO, STDOUT_FILENO, &ret);
	/* } */
}

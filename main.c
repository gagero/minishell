#include "minishell.h"
#include "testing.h"
#include <readline/readline.h>
#include "libft/libft.h"
#include <stdlib.h>
#include <unistd.h>

char	*g_cwd = NULL;
char	**g_last_environ = NULL;
int		g_last_proc_code = 0;

int main(void)
{
	char	*command;
	char	*prompt;

	g_cwd = getenv("HOME");
	while (1)
	{
		prompt = ft_strjoin(g_cwd, "> ");
		command = readline(prompt);
		free(prompt);
		char **split = ft_split(command, ' ');
		int ret;
		run_command(split, STDIN_FILENO, STDOUT_FILENO, &ret);
		ft_printf("ret: %d\n", ret);
	}
}

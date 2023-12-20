#include "minishell.h"
#include "testing.h"
#include <readline/readline.h>
#include <readline/history.h>
#include "libft/libft.h"
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

char	**g_last_environ = NULL;
int		g_last_proc_code = 0;
pid_t	running_process = 0;

void generic_sig_handler(int sig)
{
	if (running_process != 0) // TODO: don't use a global variable?
	{
		kill(running_process, sig);
		return ;
	}
	// proof of concept, need to implement other signals
	if (sig == SIGINT)
	{
		ft_printf("\n");
		rl_on_new_line();
	}
}

static void init(void)
{
	sigaction(SIGINT, SIG_IGN, NULL);
}

// TODO: set terminal lflag to ISIG for SIGQUIT (termios_p)

int main(void)
{
	char	*command;
	char	*prompt;
	char	**split;
	char	*cwd;

	init();
	if (chdir(getenv("HOME")) == -1)
	{
		perror("Minishell: init error");
		return (1);
	}
	while (1)
	{
		cwd = getcwd(NULL, 0); // cosmetic
		prompt = ft_strjoin(cwd, "> ");
		command = readline(prompt);
		if (!command || !command[0])
			continue ;
		add_history(command);
		free(cwd);
		free(prompt);
		/*
		This if is necessary, as substitute() operates on the input string, not tokens.
		The rest of main from this point forward is for testing and showcasing (although most of it will likely stay), as they will operate on tokens in some way.
		The split variable can be treated as a stand-in for tokens.
		*/
		if (ft_strchr(command, '$'))
			command = substitute(command);
		split = ft_split(command, ' ');
		execute(split, STDIN_FILENO, STDOUT_FILENO, &g_last_proc_code, NULL);
	}
}

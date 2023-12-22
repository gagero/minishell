#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include "libft/libft.h"
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "builtin.h"
#include <termios.h>
#include <fcntl.h>

char	**g_last_environ = NULL;
int		g_last_proc_code = 0;
pid_t	g_running_process = 0;

// proof of concept, need to implement other signals
void generic_sig_handler(int sig)
{
	if (g_running_process > 0) // TODO: don't use a global variable? (would need to establish signal disposition in run_command())
	{
		kill(g_running_process, sig);
		return ;
	}
	if (sig == SIGINT)
	{
		ft_printf("\n");
		rl_on_new_line();
		rl_replace_line("", 1);
		rl_redisplay();
	}
	if (sig == SIGQUIT)
	{
		rl_replace_line("", 1);
		rl_redisplay();
		return ;
	}
}

inline static void init_error(bool expr)
{
	if (expr)
	{
		perror("Minishell: init error");
		exit(0);
	}
}

inline static void init(void)
{
	struct sigaction	sa;
	struct termios		current;
	int					tty_fd;

	sa.sa_handler = generic_sig_handler;
	sa.sa_flags = SA_RESTART; // use SA_SIGINFO as well?
	init_error(sigaction(SIGINT, &sa, NULL) == -1);
	init_error(tcgetattr(0, &current) == -1);
	tty_fd = open(ttyname(0), O_RDONLY);
	current.c_lflag |= ISIG;
	init_error(tcsetattr(tty_fd, TCSANOW, &current) == -1);
	init_error(sigaction(SIGQUIT, &sa, NULL) == -1);
	close(tty_fd);
}

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
		cwd = getcwd(NULL, 0); // cosmetic, leaks
		prompt = ft_strjoin(cwd, "> "); // leaks
		command = readline(prompt);
		if (!command)
		{
			ft_printf("\n");
			exit_shell();
		}
		if (!command[0])
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
		{
			command = substitute(command);
			if (!command)
				exit_shell();
		}
		split = ft_split(command, ' '); // leaks
		execute(split, STDIN_FILENO, STDOUT_FILENO, &g_last_proc_code, NULL);
		free(command);
	}
}

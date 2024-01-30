#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include "libft/libft.h"
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include "builtin.h"
#include <termios.h>
#include <fcntl.h>
#include "lexer.h"
#include "debug.h"

int loop(void);

pid_t	*g_running_processes = NULL;

void generic_sig_handler(int sig)
{
	int	status;
	int	i;

	i = 0;
	while (g_running_processes[i])
	{
		waitpid(g_running_processes[i], &status, WNOHANG);
		if (!WIFEXITED(status))
			kill(g_running_processes[i], sig);
		i++;
	}
	if (sig == SIGINT)
	{
		/* ft_printf("\n"); */
		/* rl_on_new_line(); */
		/* rl_replace_line("", 1); */
		/* rl_redisplay(); */
		loop();
	}
	if (sig == SIGQUIT)
		;
}

inline static void init_error(bool expr)
{
	if (expr)
	{
		perror("Minishell: init error");
		exit(0);
	}
}

inline static void init(int reinit, char ***last_environ)
{
	struct sigaction	sa;
	struct termios		current;
	int					tty_fd;

	sa.sa_handler = generic_sig_handler;
	sa.sa_flags = SA_RESTART;
	init_error(sigaction(SIGINT, &sa, NULL) == -1);
	if (reinit == 0)
	{
		*last_environ = NULL;
		init_error(tcgetattr(0, &current) == -1);
		tty_fd = open(ttyname(0), O_RDONLY);
		current.c_lflag |= ISIG;
		init_error(tcsetattr(tty_fd, TCSANOW, &current) == -1);
		init_error(sigaction(SIGQUIT, &sa, NULL) == -1);
		close(tty_fd);
		init_error(chdir(getenv("HOME")) == -1);
	}
}

// TODO: error messages
int check_syntax(char *text)
{
	int	eq_count;
	char	*eq;

	eq_count = 0;
	eq = ft_strchr(text, '=');
	while (eq)
	{
		eq_count++;
		if (eq == text || !ft_isalpha(eq[1]) || !ft_isalpha(*(eq - 1)))
			return (1);
		text = eq;
	}
	if (eq_count > 1)
		return (1);
	// TODO: quote checking
	while (text++)
		if (ft_isascii(*text))
			return (1);
	return (0);
}

int loop(void)
{
	char	*cwd;
	t_list	*lexed;
	char	*command;
	char	*prompt;
	char	**last_environ;
	static int	runs;

	runs++;
	if (runs > 1)
		init(1, &last_environ);
	else
		init(0, &last_environ);

	while (1)
	{
		cwd = getcwd(NULL, 0);
		prompt = ft_strjoin(cwd, "> ");
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
		if (!check_syntax(command))
			exit(EXIT_SUCCESS);
		// testing
		lexed = lexer(command, last_environ);
		g_running_processes = ft_calloc(ft_lstsize(lexed) + 1, sizeof(pid_t));
		// for testing
		ft_lstiter(lexed, print_type);
		// parse()
		/* wait_en_masse(); */
		free(command);
	}
}

int main(void)
{
	loop();
}

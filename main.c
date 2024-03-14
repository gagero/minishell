#include <stdio.h>
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
#include <errno.h>
#include "parser.h"
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
		ft_printf("\n");
		rl_on_new_line();
		rl_replace_line("", 1);
		rl_redisplay();
	}
	if (sig == SIGQUIT)
	{}
}

inline static void init_error(bool expr)
{
	if (expr)
	{
		perror("Minishell: init error");
		exit(0);
	}
}

inline static void init(char ***last_environ)
{
	struct sigaction	sa;
	struct termios		current;
	int					tty_fd;

	sa.sa_handler = generic_sig_handler;
	sa.sa_flags = SA_RESTART;
	init_error(sigaction(SIGINT, &sa, NULL) == -1);
		*last_environ = NULL;
		init_error(tcgetattr(0, &current) == -1);
		tty_fd = open(ttyname(0), O_RDONLY);
		current.c_lflag |= ISIG;
		init_error(tcsetattr(tty_fd, TCSANOW, &current) == -1);
		init_error(sigaction(SIGQUIT, &sa, NULL) == -1);
		close(tty_fd);
		init_error(chdir(getenv("HOME")) == -1);
}

// FIXME: always errors out with no quotes
int check_syntax(char *text)
{
	int	eq_count;
	char	*eq;

	errno = EIO;
	eq_count = 0;
	eq = ft_strchr(text, '=');
	while (eq)
	{
		eq_count++;
		if (ERROR((eq == text || !ft_isalpha(eq[1]) || !ft_isalpha(*(eq - 1))), ""))
			return (1);
		text = eq;
	}
	if (ERROR((eq_count > 1), ""))
		return (1);
	eq = text;
	eq_count = 0;
	while (eq)
	{
		eq = ft_strchr(eq, '"');
		if (eq)
		{
			eq_count++;
			eq++;
		}
	}
	if (ERROR((eq_count && eq_count % 2 != 0), ""))
		return (1);
	eq = text;
	eq_count = 0;
	while (eq)
	{
		eq = ft_strchr(eq, '\'');
		if (eq)
		{
			eq_count++;
			eq++;
		}
	}
	if (ERROR((eq_count && eq_count % 2 != 0), ""))
		return (1);
	while (*text)
	{
		if (ERROR((!ft_isascii(*text)), ""))
			return (1);
		text++;
	}
	return (0);
}

int loop(char **last_environ)
{
	char	*cwd;
	t_list	*lexed;
	char	*command;
	char	*prompt;
	int		*last_code;

	last_code = 0;
	while (1)
	{
		cwd = getcwd(NULL, 0);
		prompt = ft_strjoin(cwd, "> ");
		free(cwd);
		command = readline(prompt);
		free(prompt);
		if (!command)
		{
			ft_printf("\n");
			exit_shell();
		}
		if (!command[0])
			continue ;
		if (check_syntax(command))
			return (1);
		add_history(command);
		lexed = lexer(command, last_environ);
		g_running_processes = ft_calloc(ft_lstsize(lexed) + 1, sizeof(pid_t));
		parse(lexed, last_code); // empty quotes fail here
		last_code = malloc(sizeof(*last_code));
		*last_code = wait_en_masse();
		free(command);
		free(g_running_processes);
	}
	free(last_code);
}

int main(void)
{
	char	**last_environ;

	init(&last_environ);
	loop(last_environ);
}

int	error(bool expr, char *message, char *file, int line)
{
	char *err = malloc(1000);
	sprintf(err, "%s:%d %s", file, line, message);
	if (expr)
	{
		perror(err);
		free(err);
		return (1);
	}
	free(err);
	return (0);
}

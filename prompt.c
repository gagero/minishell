#include <stdio.h>
#include <readline/readline.h>
#include "builtin.h"
#include "libft/libft.h"
#include "lexer.h"
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include "minishell.h"

void	pipe_prompt(t_list	**lexed)
{
	char	*cmd;
	t_type	*insert;

	insert = malloc(sizeof(t_type));
	cmd = readline("pipe> ");
	if (!cmd)
		exit_shell();
	insert = tokenize(cmd);
	ft_lstadd_back(lexed, ft_lstnew(insert));
}

static bool sig = true;

// FIXME: doesn't reprint prompt properly
void	heredoc_handler(int signum)
{
	if (signum == SIGINT)
		sig = false;
}

// FIXME: delim doesn't end prompt
char	*heredoc_prompt(const char *const delim, int *const len)
{
	char	*cmd;
	char	*ret;
	char	*little;
	char	*tmp;
	struct sigaction s;

	ret = "";
	cmd = (char *)1;
	*len = ft_strlen(delim);
	s.sa_handler = heredoc_handler;
	sigaction(SIGINT, &s, NULL);
	sig = true;
	while (cmd && sig)
	{
		cmd = readline("heredoc> ");
		if (!sig)
			return (NULL);
		little = ft_strnstr(cmd, delim, *len);
		if (little)
		{
			tmp = ft_substr(cmd, 0, little - cmd);
			ft_strjoin(ret, tmp);
			*len = ft_strlen(ret);
			free(cmd);
			free(tmp);
			s.sa_handler = generic_sig_handler;
			sigaction(SIGINT, &s, NULL);
			return (ret);
		}
		else
			ft_strjoin(ret, cmd);
		free(cmd);
	}
	s.sa_handler = generic_sig_handler;
	sigaction(SIGINT, &s, NULL);
	return (NULL);
}

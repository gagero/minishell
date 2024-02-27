#include <stdio.h>
#include <readline/readline.h>
#include "builtin.h"
#include "libft/libft.h"
#include "lexer.h"
#include <stdlib.h>
#include <stdbool.h>

// FIXME: this whole thing is bad

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

char	*heredoc_prompt(const char *const delim, int *const len)
{
	char	*cmd;
	char	*ret;
	char	*little;
	char	*tmp;

	ret = "";
	cmd = (char *)1;
	// TODO: SIGINT
	while (cmd)
	{
		cmd = readline("heredoc> ");
		*len = ft_strlen(cmd);
		little = ft_strnstr(cmd, delim, *len);
		if (little)
		{
			tmp = ft_substr(cmd, 0, little + ft_strlen(little) - cmd);
			if (ret[0])
				*len += ft_strlen(tmp);
			else
				*len = ft_strlen(tmp);
			ft_strjoin(ret, tmp);
			free(cmd);
			free(tmp);
			return (ret);
		}
		else
		{
			*len += ft_strlen(cmd);
			ft_strjoin(ret, cmd);
		}
		free(cmd);
	}
	return (NULL);
}

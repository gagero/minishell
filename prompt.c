#include <stdio.h>
#include <readline/readline.h>
#include "libft/libft.h"
#include "lexer.h"
#include <stdlib.h>
#include <stdbool.h>

// FIXME: this whole thing is bad

void	pipe_prompt(t_list	**lexed, int pos)
{
	char	*cmd;
	t_type	*insert;
	char	*end;

	insert = malloc(sizeof(t_type));
	cmd = readline("pipe> ");
	if (cmd)
	{
		end = cmd + ft_strlen(cmd) - 1;
		while (*end == ' ')
			end--;
		if (*end == '|')
		{
			pipe_prompt(lexed, pos);
			pos++;
		}
		insert = tokenize(cmd);
		*lexed = ft_lstinsert(*lexed, ft_lstnew(insert), pos);
	}
}

static char	*ft_strjoin_free(char *s1, char const *s2)
{
	char	*ret;
	int		i;
	int		j;

	if (!s1 || !s2)
		return (NULL);
	ret = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!ret)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		ret[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
	{
		ret[i + j] = s2[j];
		j++;
	}
	ret[i + j] = '\0';
	free(s1);
	return (ret);
}

void	quote_prompt(t_list	**lexed, const int pos, const bool dquote)
{
	char	*cmd;
	t_type	*insert;
	char	*full_cmd;

	insert = malloc(sizeof(t_type));
	full_cmd = "";
	if (dquote)
		while (!ft_strchr(full_cmd, '"'))
		{
			cmd = readline("dquote> ");
			full_cmd = ft_strjoin_free(full_cmd, cmd);
		}
	else
		while (!ft_strchr(full_cmd, '\''))
		{
			cmd = readline("quote> ");
			full_cmd = ft_strjoin_free(full_cmd, cmd);
		}
	insert->word.word = full_cmd;
	insert->word.is_quoted = true;
	*lexed = ft_lstinsert(*lexed, ft_lstnew(insert), pos);
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

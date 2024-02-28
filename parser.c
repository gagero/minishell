#include <stdio.h>
#include <unistd.h>
#include "libft/libft.h"
#include <stdint.h>
#include <stdlib.h>
#include "lexer.h"
#include <fcntl.h>
#include "parser.h"
#include "minishell.h"
#include <sys/ioctl.h>

intptr_t min(uintptr_t one, uintptr_t two)
{
	if (one < two && one)
		return (one);
	if (two)
		return (two);
	return (0);
}

t_type	*is_elem(t_type *t, const enum redir redir)
{
	if (t->redir == redir)
		return (t);
	else
		return (NULL);
}

t_list *lexed_find(const t_list *lexed, const enum redir elem)
{
	while (lexed && lexed->next)
	{
		if (is_elem(lexed->content, elem))
			return ((t_list *)lexed);
		lexed = lexed->next;
	}
	return (NULL);
}

int parse(t_list *lexed)
{
	t_list	*found;
	intptr_t		ret;
	int			pipefd[2];
	char			**c;
	int			out;

	lexed = ft_lstmap(lexed, (void *(*)(void *))substitute, free); // do prev pointers get created here?
	if (!lexed)
		return (1);
	if (ft_lstsize(lexed) > 2 && ((t_type *)ft_lstlast(lexed)->prev->content)->redir == HEREDOC)
		heredoc_prompt(((t_type *)ft_lstlast(lexed)->content)->word.word, &out);
	out = 1;
	found = (t_list *)min((uintptr_t)lexed_find(lexed, INPUT), (uintptr_t)lexed_find(lexed, HEREDOC));
	ret = pipe(pipefd);
	if (ret)
		return (1);
	if (found)
	{
		ret = handle_input_redir(found, pipefd);
		if (error((ret == 1), "redir error"))
			return (1);
	}
	else if ((lexed_find(lexed, PIPE)))
	{
		ret = process_pipes(lexed, pipefd);
		if (ret == 2)
			return (1);
	}
	else if (!lexed_find(lexed, OUTPUT) || !lexed_find(lexed, APPEND))
	{
		if (((t_type *)lexed->content)->redir > 4)
		{
			c = ft_split(((t_type *)lexed->content)->word.word, ' ');
			if (!c && write(STDERR_FILENO, "malloc error\n", 13))
				return (1);
			ret = execute(c, pipefd[0], STDOUT_FILENO);
			if (error((ret), "execution error"))
				return (1);
		}
		else
		{
			write(STDERR_FILENO, "Parse error: command not found\n", 31);
			return (1);
		}
	}
	else
		out = handle_output(lexed, pipefd);
	if (!out) // FIXME: proper outputting
	{
		ret = ioctl(pipefd[0], FIONREAD, &out);
		c = malloc(sizeof(*c));
		*c = malloc(out);
		out = read(pipefd[0], *c, out);
		if (error((out == -1), "pipe error"))
		{
			free(*c);
			free(c);
			return (1);
		}
		ft_printf("%s", *c);
		free(*c);
		free(c);
	}
	if (error((ret == 1), "output error"))
		return (1);
	return (0);
}

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
	intptr_t		ret;
	int			pipefd[2];
	char			**c;
	int			out;

	lexed = ft_lstmap(lexed, (void *(*)(void *))substitute, free);
	if (!lexed)
		return (1);
	out = 1;
	ret = pipe(pipefd);
	if (ret)
		return (1);
	ret = handle_input_redir((t_list *)min((uintptr_t)lexed_find(lexed, INPUT), (uintptr_t)(lexed_find(lexed, HEREDOC))), pipefd);
	if (error((ret == 1), "redir error"))
		return (1);
	// FIXME: put data into pipefd here?
	ret = process_pipes(lexed, pipefd);
	if (ret == 2)
		return (1);
	if (!lexed_find(lexed, OUTPUT) || !lexed_find(lexed, APPEND))
	{
		if ((uintptr_t)((t_type *)lexed->content)->word.word > (uintptr_t)4)
		{
			c = ft_split(((t_type *)lexed->content)->word.word, ' ');
			if (!c && write(STDERR_FILENO, "malloc error\n", 13))
				return (1);
			ret = execute(c, pipefd[0], STDOUT_FILENO);
			if (ret)
				return (1);
			return (0);
		}
		else
		{
			write(STDERR_FILENO, "Parse error: unhandled case; possible syntax error\n", 51);
			return (1);
		}
	}
	else
		out = handle_output(lexed, pipefd);
	// TODO: double check
	if (!out)
	{
		close(pipefd[1]);
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
	close(pipefd[0]);
	if (error((ret == 1), "output error"))
		return (1);
	return (0);
}

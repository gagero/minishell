#include <stdio.h>
#include <unistd.h>
#include "libft/libft.h"
#include <stdint.h>
#include <stdlib.h>
#include "lexer.h"
#include <fcntl.h>
#include "parser.h"
#include "minishell.h"

intptr_t max(uintptr_t one, uintptr_t two)
{
	if (one > two && one)
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

static bool is_pipe(void *void_node)
{
	t_type	*node;

	node = void_node;
	if (node->redir == PIPE)
		return (true);
	return (false);
}

int parse(t_list *lexed)
{
	intptr_t		ret;
	int					pipefd[2];
	char				**c;
	int					copy[2];
	int					count;
	int					i;

	count = ft_lstcount(lexed, is_pipe);
	i = 0;
	lexed = ft_lstmap(lexed, (void *(*)(void *))substitute, free);
	if (!lexed)
		return (1);
	while (i < count)
	{
		if (pipe(pipefd) == -1 || pipe(copy) == -1)
			return (1);
		ret = handle_input_redir((t_list *)max((uintptr_t)lexed_find(lexed, INPUT), (uintptr_t)(lexed_find(lexed, HEREDOC))), pipefd); // does this close?
		if (error((ret == 1), "redir error"))
			return (1);
		ret = process_pipes(lexed, pipefd, copy); // closes pipefd[READ_END]
		if (ret == 2)
			return (1);
		i++;
	}
	if (handle_output(lexed, copy))
		return (write(2, "output error\n", 13), 1);
	return (0);
}

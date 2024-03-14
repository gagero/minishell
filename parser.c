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

static t_list	*ft_lstmap_sub(t_list *lst, void *(*f)(void *, int *), void (*del)(void *), int *last_code)
{
	t_list	*ret;
	t_list	*beg;

	if (lst == NULL)
		return (NULL);
	ret = ft_lstnew(f(lst->content, last_code));
	if (ret == NULL)
		return (NULL);
	beg = ret;
	lst = lst->next;
	while (lst != NULL)
	{
		ft_lstadd_back(&ret, ft_lstnew(f(lst->content, last_code)));
		if (ret->next == NULL)
		{
			ft_lstclear(&beg, del);
			return (NULL);
		}
		lst = lst->next;
		ret = ret->next;
	}
	ret->next = NULL;
	return (beg);
}

int parse(t_list *lexed, int *last_code)
{
	intptr_t		ret;
	int					pipefd[2];
	int					copy[2];
	int					count;
	int					i;

	count = ft_lstcount(lexed, is_pipe);
	i = 0;
	lexed = ft_lstmap_sub(lexed, (void *(*)(void *, int *))substitute, free, last_code);
	if (!lexed || ERROR((pipe(pipefd) == -1), "pipe error") || ERROR((pipe(copy) == -1), "pipe error"))
		return (1);
	do
	{
		ret = handle_input_redir((t_list *)max((uintptr_t)lexed_find(lexed, INPUT), (uintptr_t)(lexed_find(lexed, HEREDOC))), pipefd); // does this close?
		if (ERROR((ret == 1), "redir error"))
			return (1);
		ret = process_pipes(lexed, pipefd, copy); // closes pipefd[READ_END] and copy[WRITE_END]
		if (ret == 2)
			return (1);
		if (ERROR((pipe(pipefd) == -1), "pipe error") || ERROR((pipe(copy) == -1), "pipe error"))
			return (1);
		i++;
	} while (i < count);
	if (handle_output(lexed, copy)) // copy invalid here
		return (write(2, "output error\n", 13), 1);
	return (0);
}

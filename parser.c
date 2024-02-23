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

t_list	*ft_lstmap_redir(t_list *lst, t_type *(*f)(t_type *, enum redir), void (*del)(void *), const enum redir redir)
{
	t_list	*ret;
	t_list	*beg;

	if (lst == NULL)
		return (NULL);
	ret = ft_lstnew(f(lst->content, redir));
	if (ret == NULL)
		return (NULL);
	beg = ret;
	lst = lst->next;
	while (lst != NULL)
	{
		ret->next = ft_lstnew(f(lst->content, redir));
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

t_type	*is_elem(t_type *t, const enum redir redir)
{
	if (t->redir == redir)
		return (t);
	else
		return (NULL);
}

t_list *lexed_find(const t_list *lexed, const enum redir elem)
{
	lexed = ft_lstmap_redir((t_list *)lexed, is_elem, free, elem);
	while (lexed && lexed->next)
	{
		if (lexed->content != NULL)
		    break ;
		lexed = lexed->next;
	}
	if (!lexed->content)
		return (NULL);
	return ((t_list *)lexed);
}

int parse(t_list *lexed)
{
	t_list	*found;
	intptr_t		ret;
	int			pipefd[2];
	char			**c;
	int			out;

	out = 1;
	lexed = ft_lstmap(lexed, (void *(*)(void *))substitute, free); // content pointer invalidated here
	if (!lexed)
		return (1);
	found = (t_list *)min((uintptr_t)lexed_find(lexed, INPUT), (uintptr_t)lexed_find(lexed, HEREDOC));
	ret = pipe(pipefd);
	if (ret)
		return (1);
	if (found)
	{
		ret = handle_input_redir(found, lexed, pipefd);
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
			execute(c, pipefd[0], STDOUT_FILENO);
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

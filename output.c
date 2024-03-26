#include <stdlib.h>
#include <unistd.h>
#include "lexer.h"
#include "libft/libft.h"
#include "minishell.h"
#include "parser.h"
#include <fcntl.h>

// TODO: validate logic
int handle_output(t_list *lexed)
{
	t_list	*found;
	int			fd;

	found = (t_list *)max((intptr_t)lexed_find(lexed, APPEND), (uintptr_t)lexed_find(lexed, OUTPUT));
	if (!found)
		return (STDOUT_FILENO);
	if (((t_type *)found->content)->redir == OUTPUT)
		fd = open(((t_type *)found->next->content)->word.word, O_WRONLY | O_CREAT, S_IWUSR);
	if (((t_type *)found->content)->redir == APPEND)
		fd = open(((t_type *)found->next->content)->word.word, O_APPEND | O_CREAT, S_IWUSR);
	return (fd);
}

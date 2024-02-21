#include "libft/libft.h"
#include "lexer.h"
#include "parser.h"

int process_pipes(t_list *lexed, int pipefd[2])
{
	t_list	*found;

	found = lexed;
	while (1)
	{
		found = lexed_find(found, PIPE);
	}
	return (0);
}

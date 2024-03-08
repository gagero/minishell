#include <stdlib.h>
#include <unistd.h>
#include "lexer.h"
#include "libft/libft.h"
#include "minishell.h"
#include "parser.h"
#include <fcntl.h>

// TODO: validate logic
//FIXME: rework
int handle_output(t_list *lexed, int pipefd[2])
{
	t_list	*found;
	int			fd;
	int			size;
	char		*buf;

	found = (t_list *)max((intptr_t)lexed_find(lexed, APPEND), (uintptr_t)lexed_find(lexed, OUTPUT));
	if (!found)
	{
		if (get_pipe_size(pipefd[READ_END], &fd)) // error here
			return (1);
		buf = malloc(fd);
		if (read(!buf || pipefd[READ_END], buf, fd) == -1 || write(STDOUT_FILENO, buf, fd) == -1)
			return (1);
		return (0);
	}
	if (((t_type *)found->content)->redir == OUTPUT)
		fd = open(((t_type *)found->next->content)->word.word, O_WRONLY | O_CREAT, S_IWUSR);
	if (((t_type *)found->content)->redir == APPEND)
		fd = open(((t_type *)found->next->content)->word.word, O_APPEND | O_CREAT, S_IWUSR);
	if (error((fd == -1), "Open error") || get_pipe_size(pipefd[READ_END], &size) == -1)
		return (write(2, "OUT\n", 4), 1);
	buf = malloc(size);
	if (error((!buf), "malloc error"))
	{
		free(buf);
		return (1);
	}
	read(pipefd[READ_END], buf, size);
	write(fd, buf, size);
	return (0);
}

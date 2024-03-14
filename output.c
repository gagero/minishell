#include <stdlib.h>
#include <unistd.h>
#include "lexer.h"
#include "libft/libft.h"
#include "minishell.h"
#include "parser.h"
#include <fcntl.h>

// TODO: validate logic
int handle_output(t_list *lexed, int pipefd[2])
{
	t_list	*found;
	int			fd;
	int			size;
	char		*buf;

	found = (t_list *)max((intptr_t)lexed_find(lexed, APPEND), (uintptr_t)lexed_find(lexed, OUTPUT));
	if (!found)
	{
		// FIXME: pipefd[READ_END] is empty here
		if (ERROR((get_pipe_size(pipefd[READ_END], &fd) == -1), "ioctl error"))
			return (1);
		buf = malloc(fd);
		if (!buf)
			return (1);
		if (read(pipefd[READ_END], buf, fd) == -1)
			return (1);
		if (write(STDOUT_FILENO, buf, fd) == -1)
			return (1);
		return (0);
	}
	if (((t_type *)found->content)->redir == OUTPUT)
		fd = open(((t_type *)found->next->content)->word.word, O_WRONLY | O_CREAT, S_IWUSR);
	if (((t_type *)found->content)->redir == APPEND)
		fd = open(((t_type *)found->next->content)->word.word, O_APPEND | O_CREAT, S_IWUSR);
	if (ERROR((fd == -1), "Open error") || ERROR((get_pipe_size(pipefd[READ_END], &size) == -1), "ioctl error"))
		return (write(2, "OUT\n", 4), 1);
	buf = malloc(size);
	if (ERROR((!buf), "malloc error"))
	{
		free(buf);
		return (1);
	}
	read(pipefd[READ_END], buf, size);
	write(fd, buf, size);
	return (0);
}

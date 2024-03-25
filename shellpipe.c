#include "libft/libft.h"
#include "lexer.h"
#include "minishell.h"
#include "parser.h"
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>

int	get_pipe_size(int pipe, int *size)
{
	int	ret;

	ret = ioctl(pipe, FIONREAD, size);
	if (ret == -1)
		return (-1);
	return (0);
}

int process_pipes(t_list *lexed, int pipefd[2])
{
	t_list	*found;
	char		**cmd;
	int size;
	int copy[2];
	char	*buf;

	buf = NULL;
  if (ERROR((pipe(copy) == -1), "Pipe error"))
		return (1);
	found = lexed_find(lexed, PIPE);
	if (!found)
	{
		// TODO: replace with quote-compliant version
		cmd = ft_split(((t_type *)lexed->content)->word.word, ' ');
		execute(cmd, pipefd[READ_END], copy[WRITE_END]);
		/* free(cmds[0]); */
		return (0);
	}
	if (found)
	{
		if (!found->prev)
			return (write(STDERR_FILENO, "syntax error\n", 13), 1);
		cmd = ft_split(((t_type *)found->prev->content)->word.word, ' ');
		execute(cmd, pipefd[READ_END], copy[WRITE_END]);
	}
	while (found)
	{
		if (buf)
		{
			write(copy[WRITE_END], buf, size);
			free(buf);
		}
		cmd = ft_split(((t_type *)found->next->content)->word.word, ' ');
		execute(cmd, copy[READ_END], copy[WRITE_END]);
		found = lexed_find(found->next, PIPE);
		if (ERROR((pipe(copy) == -1), "Pipe error"))
			return (1);
		get_pipe_size(copy[READ_END], &size);
		buf = malloc(size);
		read(copy[READ_END], buf, size);
	}
	write(pipefd[WRITE_END], buf, size);
	free(buf);
	return (0);
}

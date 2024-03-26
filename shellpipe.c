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

int process_pipes(t_list *lexed, int input, int output)
{
	t_list	*found;
	char		**cmd;
	int internal_pipe[2];

  if (ERROR((pipe(internal_pipe) == -1), "Pipe error"))
		return (1);
	found = lexed_find(lexed, PIPE);
	if (!found)
	{
		if (((t_type *)lexed->content)->word.word < (char *)4)
			return (write(2, "syntax error\n", 13), 1);
		// TODO: replace with quote-compliant version
		cmd = ft_split(((t_type *)lexed->content)->word.word, ' ');
		execute(cmd, input, output);
		/* free(cmds[0]); */
		return (0);
	}
	if (found)
	{
		if (!found->prev)
			return (write(STDERR_FILENO, "syntax error\n", 13), 1);
		cmd = ft_split(((t_type *)found->prev->content)->word.word, ' ');
		execute(cmd, input, internal_pipe[WRITE_END]);
	}
	while (found)
	{
		if (!found || !found->next || !lexed_find(found->next, PIPE))
		{
			if (((t_type *)ft_lstlast(lexed)->content)->word.word < (char *)4)
				return (write(2, "syntax error\n", 13), 1);
			cmd = ft_split(((t_type *)ft_lstlast(lexed)->content)->word.word, ' ');
			execute(cmd, internal_pipe[READ_END], output);
			return (0);
		}
		cmd = ft_split(((t_type *)found->next->content)->word.word, ' ');
		execute(cmd, internal_pipe[READ_END], internal_pipe[WRITE_END]);
		found = lexed_find(found->next, PIPE);
	}
	return (0);
}

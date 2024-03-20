#include "libft/libft.h"
#include "lexer.h"
#include "minishell.h"
#include "parser.h"
#include <unistd.h>
#include <sys/ioctl.h>

int	get_pipe_size(int pipe, int *size)
{
	int	ret;

	ret = ioctl(pipe, FIONREAD, size);
	if (ret == -1)
		return (-1);
	return (0);
}

int process_pipes(t_list *lexed, int pipefd[2], int copy[2])
{
	t_list	*found;
	int			internal_pipefd[2];
	int			ret;
	char		**cmds[2];

  if (ERROR((pipe(internal_pipefd) == -1 || pipe(copy) == -1), "Pipe error"))
		return (1);
	found = lexed;
	while (found)
	{
		found = lexed_find(found, PIPE);
		if (!found)
		{
			cmds[0]	 = ft_split(((t_type *)lexed->content)->word.word, ' ');
			execute(cmds[0], pipefd[READ_END], copy[WRITE_END]);
			/* free(cmds[0]); */
			return (0);
		}
		if (!found->prev)
			return (write(STDERR_FILENO, "syntax error\n", 13), 1);
		cmds[0] = ft_split(((t_type *)found->prev->content)->word.word, ' ');
		cmds[1] = ft_split(((t_type *)found->next->content)->word.word, ' ');
		ret = execute(cmds[0], pipefd[READ_END], internal_pipefd[WRITE_END]);
		/* free(cmds[0]); */
		if (ret)
			return (write(STDERR_FILENO, "execution error\n", 16), 1);
		ret = execute(cmds[1], internal_pipefd[READ_END], copy[WRITE_END]);
		/* free(cmds[1]); */
		if (ret)
			return (write(STDERR_FILENO, "execution error\n", 16), 1);
		found = found->next;
	}
	return (0);
}

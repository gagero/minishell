#include <stdlib.h>
#include "libft/libft.h"
#include "lexer.h"
#include "minishell.h"
#include "parser.h"
#include <unistd.h>
#include <sys/ioctl.h>

int process_pipes(t_list *lexed, int pipefd[2])
{
	t_list	*found;
	int			internal_pipefd[2];
	int			ret;
	char		*output;
	int			size;
	int			i;
	char		**cmds[2];

	ret = pipe(internal_pipefd);
  if (error((ret == -1), "Pipe error"))
		return (1);
	found = lexed;
	i = 0;
	while (1)
	{
		found = lexed_find(found, PIPE);
		if (!found)
			break ;
		cmds[0] = ft_split(((t_type *)found->prev->content)->word.word, ' ');
		cmds[1] = ft_split(((t_type *)found->next->content)->word.word, ' ');
		execute(cmds[0], pipefd[0], internal_pipefd[1]);
		execute(cmds[1], internal_pipefd[0], internal_pipefd[1]);
		free(cmds[0]);
		free(cmds[1]);
		i++;
	}
	if (i > 0)
	{
		ret = ioctl(internal_pipefd[0], FIONREAD, &size);
		output = malloc(size);
		if (error((!output), "malloc error"))
		{
			free(output);
			return (1);
		}
		read(internal_pipefd[0], output, size); // TODO: check if nul-terminated
		write(pipefd[1], output, size);
		if (error((ret == -1), "Pipe error"))
			return (1);
	}
	return (0);
}

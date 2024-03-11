#include <stdlib.h>
#include "libft/libft.h"
#include "lexer.h"
#include "minishell.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include "parser.h"

// TODO: break up into separate functions
// TODO: verify logic
intptr_t	handle_input_redir(t_list *found, int pipefd[2])
{
	int		fd;
	char	*copy;
	int		total_bytes_read;
	char	**split;

  if (found && ((t_type *)found->content)->redir == INPUT)
	{
		fd = open(((t_type *)found->next->content)->word.word, O_RDONLY);
		if (ERROR((fd == -1), "Open error") || ERROR((get_pipe_size(pipefd[READ_END], &total_bytes_read) == -1), "ioctl error"))
			return (write(2, "IN\n", 3), 1);
		copy = malloc(total_bytes_read);
		if (ERROR((!copy), "malloc error"))
		{
			free(copy);
			close(fd);
			return (1);
		}
		read(fd, copy, total_bytes_read);
		write(pipefd[WRITE_END], copy, total_bytes_read);
		close(fd);
	}
	else if (found && ((t_type *)found->content)->redir == HEREDOC)
	{
		if (((intptr_t)((t_type *)found->next->content)->word.word) > 4)
		{
			copy = heredoc_prompt(((t_type *)found->next->content)->word.word, &total_bytes_read);
			write(pipefd[WRITE_END], copy, ft_strlen(copy));
		}
		else
			return (1); // TODO: error message
	}
	else
		return (0);
	split = ft_split(((t_type *)found->prev->content)->word.word, ' ');
	if (!split)
		return (1);
	return ((intptr_t)split);
}

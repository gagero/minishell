#include <stdlib.h>
#include "libft/libft.h"
#include "lexer.h"
#include "minishell.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include "parser.h"
#include <sys/ioctl.h>

// TODO: break up into separate functions
intptr_t	handle_input_redir(t_list *found, int pipefd[2])
{
	int		fd;
	char	*copy;
	int		total_bytes_read;
	char	**split;
	int ret;

  if (((t_type *)found->content)->redir == INPUT)
	{
		fd = open(((t_type *)found->next->content)->word.word, O_RDONLY);
		if (error((fd == -1), "Open error"))
			return (1);
		ret = ioctl(pipefd[0], FIONREAD, &total_bytes_read);
		if (error((ret == -1), "ioctl error"))
			return (1);
		copy = malloc(total_bytes_read);
		if (error((!copy), "malloc error"))
		{
			free(copy);
			return (1);
		}
		read(pipefd[0], copy, total_bytes_read);
	}
	else if (((t_type *)found->content)->redir == HEREDOC)
	{
		if (((intptr_t)((t_type *)found->next->content)->word.word) > 4)
		{
			copy = heredoc_prompt(((t_type *)found->next->content)->word.word, &total_bytes_read);
			read(pipefd[0], copy, ft_strlen(copy));
		}
		else
			return (1); // TODO: error message
	}
	else
		return (1);
	split = ft_split(((t_type *)found->prev->content)->word.word, ' ');
	if (error((!split), "malloc error"))
		return (1);
	return ((intptr_t)split);
}

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
int	handle_input_redir(t_list *found, t_list *lexed, int pipefd[2])
{
	int		fd;
	char	*file_content;
	char	*copy;
	int		offset;
	int		total_bytes_read;
	int		read_bytes;
	char	**split;

  if (((t_type *)found->content)->redir == INPUT)
	{
		fd = access(((t_type *)found->next->content)->word.word, R_OK);
		if (!fd)
			return (1); // TODO: error message
		fd = open(((t_type *)found->next->content)->word.word, O_RDONLY);
		file_content = malloc(INITIAL_BUFFER_SIZE);
		offset = 1;
		total_bytes_read = 0;
		while (1)
		{
			read_bytes = read(fd, file_content, INITIAL_BUFFER_SIZE * offset);
			total_bytes_read += read_bytes;
			if (read_bytes > INITIAL_BUFFER_SIZE * offset)
			{
				offset++;
				copy = file_content;
				file_content = malloc(INITIAL_BUFFER_SIZE * offset);
				ft_memcpy(file_content, copy, INITIAL_BUFFER_SIZE * (offset - 1));
			}
		}
	}
	else if (((t_type *)found->content)->redir == HEREDOC)
	{
		if (((uintptr_t)((t_type *)found->next->content)->word.word) > 4)
			file_content = heredoc_prompt(((t_type *)found->next->content)->word.word, &total_bytes_read);
		else
			return (1); // TODO: error message
	}
	else
		return (1);
	read_bytes = write(pipefd[1], file_content, total_bytes_read);
	if (read_bytes)
		return (1);
	split = ft_split(((t_type *)found->prev->content)->word.word, ' ');
	lexed = lexed_find(lexed, PIPE);
	if (split && (lexed_find(lexed, PIPE) || lexed_find(lexed, OUTPUT) || lexed_find(lexed, APPEND)))
		execute(split, pipefd[0], pipefd[1]);
	else
		execute(split, pipefd[0], STDOUT_FILENO);
	return (0);
}

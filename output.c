#include <stdlib.h>
#include <unistd.h>
#include "lexer.h"
#include "libft/libft.h"
#include "minishell.h"
#include "parser.h"
#include <fcntl.h>
#include <sys/ioctl.h>

int handle_output(t_list *lexed, int pipefd[2])
{
	t_list	*found;
	int			ret;
	int			fd;
	int			size;
	char		*buf;

	found = lexed;
	found = (t_list *)min((uintptr_t)lexed_find(found, APPEND), (uintptr_t)lexed_find(found, OUTPUT));
	if (!found)
		return (2);
	if (((t_type *)found->content)->redir == OUTPUT)
		fd = open(((t_type *)found->next->content)->word.word, O_WRONLY);
	if (((t_type *)found->content)->redir == APPEND)
		fd = open(((t_type *)found->next->content)->word.word, O_APPEND);
	if (error((fd == -1), "Open error"))
		return (1);
	ret = ioctl(pipefd[0], FIONREAD, &size);
	if (error((ret == -1), "ioctl error"))
		return (1);
	buf = malloc(size);
	if (error((!buf), "malloc error"))
	{
		free(buf);
		return (1);
	}
	read(pipefd[0], buf, size);
	write(fd, buf, size);
	return (0);
}

#include "lexer.h"
#include <stdint.h>
#include "libft/libft.h"

void print_type(void *t)
{
	const char *redirs[] = {"OUTPUT", "APPEND", "INPUT", "HEREDOC", "PIPE"};
	if ((uintptr_t)((t_type *)t)->word.word > (uintptr_t)4)
		ft_printf("word: %s|quoted: %s\n", ((t_type *)t)->word.word, ((t_type *)t)->word.is_quoted ? "true" : "false");
	else
		ft_printf("%s\n", redirs[((t_type *)t)->redir]);
}

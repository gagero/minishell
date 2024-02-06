#include "lexer.h"
#include <stdint.h>
#include <stdio.h>

void print_type(void *t)
{
	const char *const redirs[] = {"OUTPUT", "APPEND", "INPUT", "HEREDOC", "PIPE"};
	if ((uintptr_t)((t_type *)t)->word.word > (uintptr_t)4)
		printf("word: %s|nquoted: %s\n", ((t_type *)t)->word.word, ((t_type *)t)->word.is_quoted ? "true" : "false");
	else
		printf("%s\n", redirs[((t_type *)t)->redir]);
}

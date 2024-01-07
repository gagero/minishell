#ifndef LEXER_H
# define LEXER_H
# include <stdbool.h>
# include "libft/libft.h"

enum redir
{
	OUTPUT,
	APPEND,
	INPUT,
	HEREDOC,
	PIPE,
};

struct s_word
{
	char *word;
	bool is_quoted;
};

typedef union s_type
{
	struct s_word word;
	enum redir redir;
}	t_type;

t_type	*tokenize(const char *text);
void	quote_prompt(t_list	**lexed, const int pos, const bool dquote);
void	pipe_prompt(t_list	**lexed, const int pos);
char	*heredoc_prompt(const char *const delim);
t_list	*lexer(char *text, char **last_environ);

#endif

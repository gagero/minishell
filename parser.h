#ifndef PARSER_H
# define PARSER_H
# include "libft/libft.h"
# include "lexer.h"
t_list *lexed_find(const t_list *lexed, enum redir elem);
int	handle_input_redir(t_list *found, t_list *lexed, int pipefd[2]);
int parse(t_list *lexed);
#endif

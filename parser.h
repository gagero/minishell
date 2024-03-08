#ifndef PARSER_H
# define PARSER_H
# include "libft/libft.h"
# include "lexer.h"
# include <stdint.h>
# define WRITE_END 1
# define READ_END 0
t_list *lexed_find(const t_list *lexed, enum redir elem);
intptr_t	handle_input_redir(t_list *found, int pipefd[2]);
int parse(t_list *lexed);
intptr_t max(uintptr_t one, uintptr_t two);
int process_pipes(t_list *lexed, int pipefd[2], int copy[2]);
int handle_output(t_list *lexed, int pipefd[2]);
int	get_pipe_size(int pipe, int *size);
#endif

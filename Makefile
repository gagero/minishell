NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -ggdb3 -O0 -lreadline -fno-omit-frame-pointer #-fsanitize=address,leak,undefined
SRCS = main.c execute.c builtin.c builtin2.c substitute.c lexer.c prompt.c debug.c parser.c input.c
OBJS = $(SRCS:.c=.o)
LIBFT = libft/libft.a

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $@

$(LIBFT):
	$(MAKE) -C libft

clean:
	$(MAKE) -C libft clean
	rm -f $(OBJS)

fclean: clean
	$(MAKE) -C libft fclean
	rm -f $(NAME)

re: fclean all

check-syntax:
	cppcheck --disable=variableScope --quiet ${CHK_SOURCES} 
	gcc -o /dev/null -S ${CHK_SOURCES} $(CFLAGS) || true
	#scan-build gcc -o /dev/null -S ${CHK_SOURCES} $(CFLAGS) || true

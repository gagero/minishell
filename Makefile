NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -g3 -O0 -fno-omit-frame-pointer -fPIE #-fsanitize=address,leak,undefined
LDFLAGS = -lreadline
SRCS = main.c execute.c builtin.c builtin2.c substitute.c lexer.c prompt.c parser.c input.c shellpipe.c output.c
OBJS = $(SRCS:.c=.o)
LIBFT = libft/libft.a

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(LIBFT) -o $@

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

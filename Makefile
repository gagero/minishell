NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -ggdb -O0 -lreadline -fno-omit-frame-pointer
SRCS = main.c execute.c env.c builtin.c builtin2.c
OBJS = $(SRCS:.c=.o)
LIBFT = libft/libft.a

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $@

$(LIBFT):
	$(MAKE) -C libft # doesn't recompile when necessary

clean:
	$(MAKE) -C libft clean
	rm -f $(OBJS)

fclean: clean
	$(MAKE) -C libft fclean
	rm -f $(NAME)

re: fclean all

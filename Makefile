NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -ggdb -O0 -lreadline -fsanitize=thread,undefined -fno-omit-frame-pointer
SRCS = main.c
OBJS = $(SRCS:.c=.o)

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

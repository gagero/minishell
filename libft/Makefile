CC = clang
CFLAGS = -Wall -Wextra -Werror -ggdb -O3 -fno-omit-frame-pointer
NAME = libft.a
SRCS = ft_putnbr_base.c ft_putnbr_base_ul.c ft_put_p.c ft_putnbr_p.c ft_put.c utils.c ft_printf.c ft_printf_internal.c ft_putnbr.c ft_putptr.c get_next_line_bonus.c get_next_line_utils_bonus.c ft_bonus.c ft_bonus2.c ft_bonus3.c ft_char.c ft_check.c ft_check2.c ft_malloc.c ft_map.c ft_mem.c ft_string.c ft_string2.c ft_string3.c ft_string4.c ft_itoa.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	ar rs $(NAME) $^

.PHONY: clean fclean re all

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

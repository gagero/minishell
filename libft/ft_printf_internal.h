/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_internal.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggero <ggero@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 07:33:45 by ggero             #+#    #+#             */
/*   Updated: 2023/10/25 07:33:48 by ggero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_INTERNAL_H
# define FT_PRINTF_INTERNAL_H
# include <stdarg.h>
# include <stdbool.h>

struct s_print
{
	int				printed_chars;
	unsigned long	fmt_printed_count;
	unsigned long	fmt_count;
};

struct s_format
{
	int		i;
	int		count;
	bool	is_percent;
};

struct s_number
{
	int		base_len;
	bool	wrong;
	int		i;
	bool	is_lowest;
};

struct s_args
{
	int		*nbr;
	char	*base;
};

void				ft_putchar_fd_p(char c, struct s_print *const print);
void				ft_putstr_fd_p(char *s, struct s_print *const print);
void				ft_putnbr_fd_p(int n, struct s_print *const print);
void				ft_putptr_p(void *ptr, struct s_print *const print);
unsigned long		ft_strlen(const char *s);
void				print_special_char(char c, struct s_print *print);
char				*ft_strjoin(char const *s1, char const *s2);
void				print_random(char c, struct s_print *print);
int					count_format(const char *fmt);
struct s_print		init_print(const char *str);
void				add_format(int to_add, struct s_print *print);
void				dispatch_print(char c, va_list argslist,
						struct s_print *print);
void				ft_putnbr_base_ul_p(unsigned long nbr, char *base,
						struct s_print *const print);
void				ft_putnbr_base_p(int nbr, char *base,
						struct s_print *const print);
int					check_base(char *base, bool *wrong);
void				print_result(int i, char *result, struct s_print *print);

#endif

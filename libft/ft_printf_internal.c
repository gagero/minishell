/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_internal.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggero <ggero@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 07:33:26 by ggero             #+#    #+#             */
/*   Updated: 2023/10/25 07:33:32 by ggero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_internal.h"

int	count_format(const char *fmt)
{
	struct s_format	args;
	char			c;

	args.i = 0;
	args.count = 0;
	args.is_percent = false;
	while (fmt[args.i])
	{
		c = fmt[args.i + 1];
		if (fmt[args.i] == '%' && c == '%' && args.is_percent == false)
		{
			args.is_percent = true;
			args.i++;
			continue ;
		}
		if (fmt[args.i] == '%' && (c == 'c' || c == 'd' || c == 'i' || c == 's'
				|| c == 'x' || c == 'X' || c == 'u' || c == 'p'))
		{
			args.count++;
			args.is_percent = false;
		}
		args.i++;
	}
	return (args.count);
}

void	print_random(char c, struct s_print *print)
{
	if (c == 'c')
		return (ft_putchar_fd_p(c + (long)&c, print));
	if (c == 'd' || c == 'i')
		return (ft_putnbr_fd_p((long)&c, print));
	if (c == 's')
		return (ft_putstr_fd_p(&c, print));
	if (c == 'x')
		return (ft_putnbr_base_ul_p((long)&c, "0123456789abcdef", print));
	if (c == 'X')
		return (ft_putnbr_base_ul_p((long)&c, "0123456789ABCDEF", print));
	if (c == 'u')
		return (ft_putnbr_base_ul_p((unsigned long)&c, "0123456789", print));
	if (c == 'p')
		return (ft_putptr_p(&c, print));
	print->printed_chars = -2;
}

struct s_print	init_print(const char *str)
{
	struct s_print	print;

	print.printed_chars = 0;
	print.fmt_printed_count = 0;
	print.fmt_count = count_format(str);
	return (print);
}

void	add_format(int to_add, struct s_print *print)
{
	print->fmt_printed_count += 1;
	if (to_add > 0)
		print->printed_chars += to_add;
	else
		print->printed_chars = -1;
}

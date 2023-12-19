/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggero <ggero@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 07:34:53 by ggero             #+#    #+#             */
/*   Updated: 2023/10/25 07:34:53 by ggero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_internal.h"
#include <stdlib.h>
#include <stdarg.h>

void	print_special_char(char c, struct s_print *print)
{
	if (c == 'a')
		return (ft_putchar_fd_p('\a', print));
	if (c == '\\')
		return (ft_putchar_fd_p('\\', print));
	if (c == 'b')
		return (ft_putchar_fd_p('\b', print));
	if (c == 'r')
		return (ft_putchar_fd_p('\r', print));
	if (c == '"')
		return (ft_putchar_fd_p('\"', print));
	if (c == 'f')
		return (ft_putchar_fd_p('\f', print));
	if (c == 't')
		return (ft_putchar_fd_p('\t', print));
	if (c == 'n')
		return (ft_putchar_fd_p('\n', print));
	if (c == '0')
		return (ft_putchar_fd_p('\0', print));
	if (c == '\'')
		return (ft_putchar_fd_p('\'', print));
	if (c == 'v')
		return (ft_putchar_fd_p('\v', print));
	if (c == '?')
		return (ft_putchar_fd_p('\?', print));
	ft_putchar_fd_p(c, print);
}

void	dispatch_print_check(char c, va_list argslist, struct s_print *print)
{
	if (print->fmt_printed_count < print->fmt_count)
	{
		if (c == 'c')
			ft_putchar_fd_p(va_arg(argslist, int), print);
		if (c == 'd' || c == 'i')
			ft_putnbr_fd_p(va_arg(argslist, int), print);
		if (c == 's')
			ft_putstr_fd_p(va_arg(argslist, char *), print);
		if (c == 'x')
			ft_putnbr_base_ul_p(va_arg(argslist, unsigned int),
				"0123456789abcdef", print);
		if (c == 'X')
			ft_putnbr_base_ul_p(va_arg(argslist, unsigned int),
				"0123456789ABCDEF", print);
		if (c == 'u')
			ft_putnbr_base_ul_p(va_arg(argslist, unsigned int),
				"0123456789", print);
		if (c == 'p')
			ft_putptr_p(va_arg(argslist, void *), print);
		print->fmt_printed_count += 1;
		return ;
	}
	print_random(c, print);
}

void	dispatch_print(char c, va_list argslist, struct s_print *print)
{
	if (c == '%')
		return (ft_putchar_fd_p('%', print));
	else if (c != 'c' && c != 'd' && c != 'i' && c != 's' && c != 'x'
		&& c != 'X' && c != 'u' && c != 'p')
	{
		ft_putchar_fd_p('%', print);
		ft_putchar_fd_p(c, print);
	}
	dispatch_print_check(c, argslist, print);
}

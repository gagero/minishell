/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggero <ggero@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 00:10:22 by ggero             #+#    #+#             */
/*   Updated: 2023/10/15 03:12:40 by ggero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include "ft_printf_internal.h"

/* batch printing? */

static void	printf_choice(const char *str, int *i,
		struct s_print *const printed, va_list argslist)
{
	while (str[*i])
	{
		if (str[*i] == '%')
		{
			dispatch_print(str[++(*i)], argslist, printed);
			if (printed->printed_chars == -2)
				ft_putstr_fd_p(ft_strjoin("%", str + *i), printed);
			else if (printed->printed_chars < 0)
				return ;
			(*i)++;
			continue ;
		}
		if (str[*i] == '\\')
		{
			print_special_char(str[++(*i)], printed);
			(*i)++;
			continue ;
		}
		ft_putchar_fd_p(str[*i], printed);
		if (printed->printed_chars < 0)
			return ;
		(*i)++;
	}
}

int	ft_printf(const char *str, ...)
{
	int				i;
	va_list			argslist;
	struct s_print	printed;

	va_start(argslist, str);
	i = 0;
	printed = init_print(str);
	printf_choice(str, &i, &printed, argslist);
	va_end(argslist);
	if (printed.printed_chars < 0)
		return (-1);
	return (printed.printed_chars);
}

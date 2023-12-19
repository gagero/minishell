/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_put_p.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggero <ggero@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 07:34:30 by ggero             #+#    #+#             */
/*   Updated: 2023/10/25 07:34:31 by ggero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "ft_printf_internal.h"

void	ft_putchar_fd_p(char c, struct s_print *const print)
{
	int	tmp;

	tmp = write(1, &c, 1);
	if (tmp >= 0)
		print->printed_chars += tmp;
	else
		print->printed_chars = -1;
}

void	ft_putstr_fd_p(char *s, struct s_print *const print)
{
	int	len;
	int	tmp;

	if (s == NULL)
		return (ft_putstr_fd_p("(null)", print));
	len = ft_strlen(s);
	tmp = write(1, s, len);
	if (tmp >= 0)
		print->printed_chars += tmp;
	else
		print->printed_chars = -1;
}

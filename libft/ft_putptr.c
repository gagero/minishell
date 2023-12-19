/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putptr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggero <ggero@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 07:34:43 by ggero             #+#    #+#             */
/*   Updated: 2023/10/25 07:34:44 by ggero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_internal.h"
#include <stddef.h>

void	ft_putptr_p(void *ptr, struct s_print *const print)
{
	if (ptr == NULL)
	{
		ft_putstr_fd_p("(nil)", print);
		return ;
	}
	ft_putstr_fd_p("0x", print);
	ft_putnbr_base_ul_p((unsigned long)ptr, "0123456789abcdef", print);
}

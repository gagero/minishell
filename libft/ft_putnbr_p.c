/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_p.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggero <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 04:11:54 by ggero             #+#    #+#             */
/*   Updated: 2023/10/25 04:11:56 by ggero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_internal.h"
#include "libft.h"

static unsigned int	num_len(int num)
{
	unsigned int	count;

	count = 0;
	while (num)
	{
		num = num / 10;
		count++;
	}
	return (count);
}

static void	reverse_array(char *arr)
{
	char	tmp;
	int		i;
	int		len;

	i = 0;
	len = ft_strlen(arr);
	while (i < len / 2)
	{
		tmp = arr[i];
		arr[i] = arr[len - i - 1];
		arr[len - i - 1] = tmp;
		i++;
	}
}

static void	check_num(const int n, char *const buf)
{
	long	num;
	int		i;
	int		len;

	len = num_len(n);
	num = (long)n;
	if (n < 0)
	{
		num = (long)n * -1;
		buf[len] = '-';
	}
	i = 0;
	while (num > 0)
	{
		buf[i] = (num % 10) + '0';
		num /= 10;
		i++;
	}
	if (n < 0)
		buf[len + 1] = '\0';
	else
		buf[len] = '\0';
}

void	ft_putnbr_fd_p(int n, struct s_print *const print)
{
	char	buf[12];

	if (n == 0)
		return (ft_putchar_fd_p('0', print));
	check_num(n, buf);
	reverse_array(buf);
	ft_putstr_fd_p(buf, print);
}

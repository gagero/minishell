/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggero <ggero@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 07:34:15 by ggero             #+#    #+#             */
/*   Updated: 2023/10/25 07:34:15 by ggero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>

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

void	ft_putnbr_fd(int n, int fd)
{
	char	buf[12];

	if (n == 0)
	{
		write(fd, "0", 1);
		return ;
	}
	check_num(n, buf);
	reverse_array(buf);
	write(fd, buf, ft_strlen(buf));
}

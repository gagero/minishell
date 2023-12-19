/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggero <ggero@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 23:58:57 by ggero             #+#    #+#             */
/*   Updated: 2023/09/15 19:03:05 by ggero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdbool.h>
#include <stdlib.h>

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

static char	*alloc_string(bool isminus, unsigned int count)
{
	char	*str;

	if (isminus)
	{
		str = malloc(count + 2);
		if (!str)
			return (NULL);
		str[count] = '-';
	}
	else
	{
		str = malloc(count + 1);
		if (!str)
			return (NULL);
	}
	if (isminus)
		str[count + 1] = '\0';
	else
		str[count] = '\0';
	return (str);
}

static void	process_numbers(const int n, char *str, bool isminus)
{
	int		i;
	long	tmp;

	tmp = n;
	if (isminus)
		tmp = (long)n * -1;
	i = 0;
	while (tmp)
	{
		str[i] = (tmp % 10) + '0';
		tmp /= 10;
		i++;
	}
}

char	*ft_itoa(int n)
{
	char	*str;
	bool	isminus;

	isminus = false;
	if (n < 0)
		isminus = true;
	if (n == 0)
	{
		str = malloc(2);
		if (!str)
			return (NULL);
		str[0] = '0';
		str[1] = '\0';
		return (str);
	}
	str = alloc_string(isminus, num_len(n));
	if (!str)
		return (NULL);
	process_numbers(n, str, isminus);
	reverse_array(str);
	return (str);
}

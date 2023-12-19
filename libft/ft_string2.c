/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_string2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggero <ggero@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 13:34:53 by ggero             #+#    #+#             */
/*   Updated: 2023/09/15 00:00:29 by ggero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdbool.h>
#include <stdlib.h>

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;

	if (!size)
		return (ft_strlen(src));
	i = 0;
	while (i < size - 1 && src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (ft_strlen(src));
}

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	dst_len;
	size_t	i;

	dst_len = ft_strlen(dst);
	i = 0;
	if (size <= dst_len)
		return (ft_strlen(src) + size);
	while (dst_len + i < size - 1 && src[i])
	{
		dst[dst_len + i] = src[i];
		i++;
	}
	dst[dst_len + i] = '\0';
	return (dst_len + ft_strlen(src));
}

void	ft_bzero(void *s, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		*(char *)(s + i) = 0;
		i++;
	}
}

static int	check_num(const char *const s, int *const i)
{
	bool	isminus;
	int		j;
	int		k;

	isminus = false;
	while (s[*i] == '\f' || s[*i] == '\n' || s[*i] == ' ' || s[*i] == '\r'
		|| s[*i] == '\t' || s[*i] == '\v')
		(*i)++;
	j = *i;
	while (s[j] == '-' || s[j] == '+')
	{
		if (s[j] == '-')
			isminus = true;
		j++;
	}
	k = *i;
	*i = j;
	if (j - k > 1)
		return (-1);
	return (isminus);
}

int	ft_atoi(const char *const nptr)
{
	int	i;
	int	start;
	int	end;
	int	prefix;
	int	ret;

	i = 0;
	if (*nptr == '\0')
		return (0);
	prefix = check_num(nptr, &i);
	if (prefix == -1)
		return (0);
	start = i;
	end = i;
	while (nptr[i] >= '0' && nptr[i++] <= '9')
		end++;
	i = start;
	ret = 0;
	while (nptr[i] && i < end)
		ret = ret * 10 + (nptr[i++] - '0');
	if (prefix == 1)
		return (ret * -1);
	return (ret);
}

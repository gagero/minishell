/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_string4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggero <ggero@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 17:36:03 by ggero             #+#    #+#             */
/*   Updated: 2023/09/15 19:04:06 by ggero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

char	*ft_strtrim(char const *s1, char const *set)
{
	unsigned int	beg;
	unsigned int	end;
	char			*ret;

	beg = 0;
	end = ft_strlen(s1);
	while (beg < end && ft_strchr(set, s1[beg]))
		beg++;
	while (beg < end && ft_strchr(set, s1[end - 1]))
		end--;
	ret = ft_substr(s1, beg, end - beg);
	if (!ret)
		return (NULL);
	return (ret);
}

static int	count_words(const char *str, char target)
{
	int	count;

	count = 0;
	if (!str)
		return (0);
	while (*str == target)
		str++;
	if (*str != '\0' && *str != target)
		count++;
	else
		return (0);
	while (*str != '\0')
	{
		if (*str == target)
		{
			while (*str == target)
				str++;
			if (*str == '\0')
				break ;
			count++;
			str--;
		}
		str++;
	}
	return (count);
}

static void	*free_arr(char **a, int s)
{
	int	i;

	i = 0;
	while (i < s)
		free(a[i++]);
	free(a);
	return (NULL);
}

// meat of ft_split
static struct s_not_tagged_union	meat(struct s_not_tagged_union data,
							const char *s, const char c, char **res)
{
	while (s[data.i] == c && s[data.i])
		data.i++;
	data.beg = data.i;
	while (s[data.i] != '\0')
	{
		if (s[data.i] == c)
		{
			res[data.j++] = ft_substr(s, data.beg, data.i - data.beg);
			if (!res[data.j - 1])
			{
				free_arr(res, data.j);
				data.tag = ERR;
				return (data);
			}
			while (s[data.i] == c)
				data.i++;
			data.beg = data.i;
		}
		else
			data.i++;
	}
	return (data);
}

char	**ft_split(char const *s, char c)
{
	char						**res;
	struct s_not_tagged_union	dt;

	res = malloc((count_words(s, c) + 1) * sizeof(char *));
	if (!res)
		return (NULL);
	dt.i = 0;
	dt.j = 0;
	dt.tag = DATA;
	dt.beg = 0;
	dt = meat(dt, s, c, res);
	if (dt.tag == ERR)
		return (NULL);
	if (s[dt.beg] != '\0')
	{
		res[dt.j++] = ft_substr(s, dt.beg, dt.i - dt.beg);
		if (!res[dt.j - 1])
			return (free_arr(res, dt.j));
	}
	res[dt.j] = NULL;
	return (res);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_map.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggero <ggero@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 10:41:54 by ggero             #+#    #+#             */
/*   Updated: 2023/09/15 19:12:14 by ggero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	size_t		i;
	char		*ret;

	if (f == NULL || s == NULL)
	{
		ret = malloc(1);
		ret[0] = '\0';
		return (ret);
	}
	i = 0;
	ret = malloc(ft_strlen(s) + 1);
	if (!ret)
		return (NULL);
	while (i < ft_strlen(s))
	{
		ret[i] = f(i, s[i]);
		i++;
	}
	ret[i] = '\0';
	return (ret);
}

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	char	*current;
	int		i;

	if (f == NULL || s == NULL )
		return ;
	i = 0;
	while (s[i])
	{
		current = &s[i];
		f(i, current);
		i++;
	}
}

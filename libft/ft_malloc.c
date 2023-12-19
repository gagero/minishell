/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggero <ggero@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 13:31:40 by ggero             #+#    #+#             */
/*   Updated: 2023/09/14 23:19:32 by ggero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include <limits.h>

char	*ft_strdup(const char *s)
{
	char	*buf;

	buf = malloc(ft_strlen(s) + 1);
	if (!buf)
		return (NULL);
	ft_strlcpy(buf, s, ft_strlen(s) + 1);
	buf[ft_strlen(s)] = '\0';
	return (buf);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*buf;

	if ((nmemb >= INT_MAX && size != 1) || (size >= INT_MAX && nmemb != 1))
		return (NULL);
	buf = malloc(nmemb * size);
	if (!buf)
		return (NULL);
	ft_bzero(buf, nmemb * size);
	return (buf);
}

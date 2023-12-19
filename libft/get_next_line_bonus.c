/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggero <ggero@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 21:19:54 by ggero             #+#    #+#             */
/*   Updated: 2023/10/15 04:59:15 by ggero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"
#include "libft.h"
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

// squish create_new_buffer and get_real_line into one function? (shared logic)
static char	*create_new_buffer(char *buf)
{
	int		prev_len;
	char	*ret;
	int		i;

	prev_len = ft_strlen_delimit(buf, '\n').result;
	if (!buf[0])
		return (NULL);
	i = 0;
	ret = ft_calloc(ft_strlen_delimit(buf, '\0').result
			+ 3 - prev_len++, 1);
	if (!ret)
		return (NULL);
	while (buf[prev_len])
		ret[i++] = buf[prev_len++];
	if (i == 0)
	{
		free(ret);
		ret = NULL;
	}
	free(buf);
	return (ret);
}

static char	*get_real_line(char *buf)
{
	char	*line;
	int		i;

	if (!buf[0])
		return (NULL);
	i = ft_strlen_delimit(buf, '\n').result;
	if (buf[i] == '\n')
		line = ft_calloc(i + 2, 1);
	else
		line = ft_calloc(i + 1, 1);
	if (!line)
		return (NULL);
	i = 0;
	while (buf[i] && buf[i] != '\n')
	{
		line[i] = buf[i];
		i++;
	}
	if (buf[i] == '\n')
		line[i] = '\n';
	return (line);
}

static char	*reader(const int fd, char *ret)
{
	char	*buf;

	buf = ft_calloc(BUFFER_SIZE + 1, 1);
	if (!buf)
		return (NULL);
	if (!ret)
	{
		ret = ft_calloc(1, 1);
		if (!ret)
		{
			free(buf);
			return (NULL);
		}
	}
	ret = loop(fd, ret, buf, 1);
	return (ret);
}

char	*get_next_line(int fd)
{
	static char	*buf[1024];
	char		*line;

	if (BUFFER_SIZE < 0 || fd < 0)
		return (NULL);
	buf[fd] = reader(fd, buf[fd]);
	if (!buf[fd])
		return (NULL);
	line = get_real_line(buf[fd]);
	buf[fd] = create_new_buffer(buf[fd]);
	return (line);
}

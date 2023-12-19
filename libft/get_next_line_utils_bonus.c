/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggero <ggero@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 21:38:19 by ggero             #+#    #+#             */
/*   Updated: 2023/10/15 04:54:31 by ggero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"
#include <unistd.h>
#include "libft.h"

// rewrite to use strlen and strchr?
struct s_len	ft_strlen_delimit(const char *s, char limit)
{
	unsigned int	i;
	struct s_len	ret;

	i = 0;
	ret.code = GOOD;
	while (s[i])
	{
		if (s[i] == limit)
		{
			ret.result = i;
			return (ret);
		}
		i++;
	}
	if (s[i] != limit)
		ret.code = ERROR;
	ret.result = i;
	return (ret);
}

static char	*ft_strjoin_gnl(char const *s1, char const *s2)
{
	char	*ret;
	int		i;
	int		j;

	if (ft_strlen_delimit(s1, '\0').result
		+ ft_strlen_delimit(s2, '\0').result == 0)
		return (NULL);
	ret = ft_calloc(ft_strlen_delimit(s1, '\0').result
			+ ft_strlen_delimit(s2, '\0').result + 2, 1);
	if (!ret)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		ret[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
	{
		ret[i + j] = s2[j];
		j++;
	}
	ret[i + j] = '\0';
	return (ret);
}

char	*loop(int fd, char *ret, char *buf, int len)
{
	char	*tmp;

	while (len > 0)
	{
		len = read(fd, buf, BUFFER_SIZE);
		if (len < 0)
		{
			free(buf);
			free(ret);
			return (NULL);
		}
		buf[len] = '\0';
		tmp = ret;
		ret = ft_strjoin_gnl(ret, buf);
		free(tmp);
		if (!ret)
		{
			free(buf);
			return (NULL);
		}
		if (ft_strlen_delimit(buf, '\n').code == GOOD)
			break ;
	}
	free(buf);
	return (ret);
}

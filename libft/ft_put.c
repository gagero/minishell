/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_put.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggero <ggero@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 07:34:00 by ggero             #+#    #+#             */
/*   Updated: 2023/10/25 07:34:01 by ggero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"

void	ft_putchar_fd(char c, int fd)
{
	if (ft_isprint(c))
		write(fd, &c, 1);
}

int	ft_putstr_fd(char *s, int fd)
{
	if (s)
		return (write(fd, s, ft_strlen(s)));
	return (-1);
}

void	ft_putendl_fd(char *s, int fd)
{
	int	len;

	len = ft_strlen(s);
	if (s)
	{
		write(fd, s, len);
		write(fd, "\n", 1);
	}
}

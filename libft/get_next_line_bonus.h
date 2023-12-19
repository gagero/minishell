/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggero <ggero@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 21:36:05 by ggero             #+#    #+#             */
/*   Updated: 2023/10/15 04:33:45 by ggero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H
# include <stddef.h>
# include <stdlib.h>
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1000
# endif

enum e_code
{
	GOOD,
	ERROR,
};

struct s_len
{
	enum e_code		code;
	unsigned long	result;
};

struct s_len	ft_strlen_delimit(const char *s, char limit);
char			*loop(int fd, char *ret, char *buf, int len);

#endif

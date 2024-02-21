/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_base.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggero <ggero@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 15:49:07 by ggero             #+#    #+#             */
/*   Updated: 2023/10/13 01:33:17 by ggero            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <unistd.h>
#include "ft_printf_internal.h"
#include <limits.h>
#include <stdlib.h>
#include "libft.h"

void	print_result(int i, char *result, struct s_print *print)
{
	int	a;

	a = 0;
	while (a <= i)
	{
		ft_putchar_fd_p(result[31 - i + a], print);
		a++;
	}
	print->printed_chars += a;
}

int	check_base(char *base, bool *wrong)
{
	int	i;
	int	j;

	i = 0;
	while (base[i] != '\0')
	{
		if (base[i + 1] == '\0')
			break ;
		j = i + 1;
		while (base[j] != '\0')
		{
			if (base[i] == base[j] || (base[i] == '+'
					|| base[i] == '-' || base[i] == ' '))
			{
				*wrong = 1;
			}
			j++;
		}
		i++;
	}
	if (i == 0)
		*wrong = 1;
	return (i + 1);
}

static bool	ft_handle_neg(struct s_args args, int *i, char *result,
				struct s_print *const print)
{
	long	lowest;
	int		base_len;
	bool	wrong_temp;

	wrong_temp = 0;
	base_len = check_base(args.base, &wrong_temp);
	if (*args.nbr == INT_MIN)
	{
		lowest = (long)(*args.nbr) *(-1);
		while (lowest != 0)
		{
			result[31 - *i] = (char)args.base[lowest % base_len];
			lowest = lowest / base_len;
			(*i)++;
		}
		ft_putchar_fd_p('-', print);
		return (1);
	}
	else
		*args.nbr *= -1;
	ft_putchar_fd_p('-', print);
	return (0);
}

static void	ft_putnbr_base_internal(int nbr, char *base,
								struct s_print *const print, struct s_args args)
{
	char			result[32];
	struct s_number	vars;

	vars.wrong = 0;
	vars.is_lowest = 0;
	vars.base_len = check_base(base, &vars.wrong);
	if (!vars.wrong)
	{
		if (nbr == 0)
			ft_putchar_fd_p('0', print);
		vars.i = 0;
		args.nbr = &nbr;
		args.base = base;
		if (nbr < 0)
			vars.is_lowest = ft_handle_neg(args, &vars.i, result, print);
		while (nbr != 0 && !vars.is_lowest)
		{
			result[31 - vars.i] = (char)base[nbr % vars.base_len];
			nbr = nbr / vars.base_len;
			vars.i++;
		}
		vars.i--;
		return (print_result(vars.i, result, print));
	}
	print->printed_chars = -1;
}

void	ft_putnbr_base(int nbr, char *base, struct s_print *const print)
{
	struct s_args	*args;

	args = ft_calloc(sizeof(*args), 1);
	ft_putnbr_base_internal(nbr, base, print, *args);
	free(args);
}

#include "ft_printf_internal.h"

void	ft_putnbr_base_ul_p(unsigned long nbr, char *base,
			struct s_print *const print)
{
	int		base_len;
	bool	wrong;
	char	result[32];
	int		i;

	wrong = 0;
	i = 0;
	base_len = check_base(base, &wrong);
	if (wrong)
	{
		print->printed_chars = -1;
		return ;
	}
	if (nbr == 0)
		ft_putchar_fd_p('0', print);
	while (nbr != 0)
	{
		result[31 - i] = (char)base[nbr % base_len];
		nbr = nbr / base_len;
		i++;
	}
	i--;
	print_result(i, result, print);
}

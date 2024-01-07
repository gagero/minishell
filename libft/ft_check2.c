#include "libft.h"

int	ft_isspace(int c)
{
	return (c == ' ' || c == '\f' || c == '\v' || c == '\n' || c == '\r' || c == '\t');
}

#include "libft.h"

int	ft_putstr(char *s, t_printf *x)
{
	if (!s)
		s = "(null)";
	while (*s)
		if (ft_putchar(*s++, x) == -1)
			return (-1);
	return (1);
}

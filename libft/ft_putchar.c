#include "libft.h"

int	ft_putchar(char c, t_printf *x)
{
	return (x->len++, write(1, &c, 1));
}

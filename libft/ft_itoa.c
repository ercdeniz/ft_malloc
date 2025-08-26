#include "libft.h"

int	ft_itoa(uintptr_t n, int base, char *s, t_printf *x)
{
	int	tab[50];
	int	mod;
	int	i;

	mod = (x->f == 'd' || x->f == 'i') + 2 * (x->f == 'p');
	i = 0;
	if (mod == 1 && (int)n < 0)
	{
		n = -n;
		if (ft_putchar('-', x) == -1)
			return (-1);
	}
	if (mod == 2 && ft_putstr("0x", x) == -1)
		return (-1);
	if (!n)
		return (ft_putchar('0', x));
	while (n)
	{
		tab[i++] = n % base;
		n /= base;
	}
	while (i--)
		if (ft_putchar(s[tab[i]], x) == -1)
			return (-1);
	return (1);
}

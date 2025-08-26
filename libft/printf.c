#include "libft.h"

static inline int	format(t_printf *x)
{
	if (x->f == 'c')
		return (ft_putchar(va_arg(x->args, int), x));
	else if (x->f == 's')
		return (ft_putstr(va_arg(x->args, char *), x));
	else if (x->f == 'u')
		return (ft_itoa(va_arg(x->args, unsigned int), 10, DEC, x));
	else if (x->f == 'd' || x->f == 'i')
		return (ft_itoa(va_arg(x->args, int), 10, DEC, x));
	else if (x->f == 'p')
		return (ft_itoa(va_arg(x->args, uintptr_t), 16, HEXLOW, x));
	else if (x->f == 'x')
		return (ft_itoa(va_arg(x->args, unsigned int), 16, HEXLOW, x));
	else if (x->f == 'X')
		return (ft_itoa(va_arg(x->args, unsigned int), 16, HEXUP, x));
	else if (x->f == '%')
		return (ft_putchar('%', x));
	return (-1);
}

int	ft_printf(const char *s, ...)
{
	t_printf	x;

	x.len = 0;
	va_start(x.args, s);
	while (*s)
	{
		if (*s == '%')
		{
			x.f = *++s;
			if (format(&x) == -1)
				return (-1);
		}
		else if (ft_putchar(*s, &x) == -1)
			return (-1);
		s++;
	}
	return (va_end(x.args), x.len);
}

#ifndef LIBFT_H
# define LIBFT_H

# include <stdarg.h>
# include <unistd.h>
# include <stdint.h>

// LIBFT
void		*ft_memcpy(void *dst, const void *src, size_t n);

// FT_PRINTF
# define DEC "0123456789"
# define HEXLOW "0123456789abcdef"
# define HEXUP "0123456789ABCDEF"

typedef struct s_printf
{
	va_list	args;
	char	f;
	int		len;
}			t_printf;

int			ft_printf(const char *s, ...);
int			ft_putchar(char c, t_printf *x);
int			ft_putstr(char *s, t_printf *x);
int			ft_itoa(uintptr_t n, int base, char *s, t_printf *x);

#endif

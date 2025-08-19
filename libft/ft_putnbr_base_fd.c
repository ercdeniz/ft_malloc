#include "libft.h"

void	ft_putnbr_base_fd(unsigned long nbr, char *base, int fd)
{
	size_t	base_len;

	base_len = ft_strlen(base);

	if (nbr >= base_len)
		ft_putnbr_base_fd(nbr / base_len, base, fd);

	ft_putchar_fd(base[nbr % base_len], fd);
}

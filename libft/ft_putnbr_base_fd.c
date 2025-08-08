#include "libft.h"

static int	ft_strlen(char *str)
{
	int	len;

	len = 0;
	while (str[len])
		len++;
	return (len);
}

void	ft_putnbr_base_fd(unsigned long nbr, char *base, int fd)
{
	int	base_len;

	base_len = ft_strlen(base);

	if (nbr >= (unsigned long)base_len)
		ft_putnbr_base_fd(nbr / base_len, base, fd);

	ft_putchar_fd(base[nbr % base_len], fd);
}

#ifndef LIBFT_H
# define LIBFT_H

# include <unistd.h>
# include <stdlib.h>

void	*ft_memcpy(void *dst, const void *src, size_t n);
void	ft_putstr_fd(const char *s, int fd);
void	ft_putchar_fd(char c, int fd);
void	ft_putnbr_fd(int n, int fd);
void	ft_putnbr_base_fd(unsigned long nbr, char *base, int fd);
size_t	ft_strlen(const char *s);

#endif
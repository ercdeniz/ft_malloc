NAME = libft.a

CC = gcc
CFLAGS = -Wall -Wextra -Werror

SRCDIR = .
OBJDIR = obj

SRC = 	./ft_memcpy.c	\
		./printf.c		\
		./ft_putchar.c	\
		./ft_putstr.c	\
		./ft_itoa.c

OBJS = $(SRC:%.c=$(OBJDIR)/%.o)

all: $(NAME)

$(NAME): $(OBJDIR) $(OBJS)
	@ar rcs $(NAME) $(OBJS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	@mkdir -p $(OBJDIR)

clean:
	@rm -rf $(OBJDIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
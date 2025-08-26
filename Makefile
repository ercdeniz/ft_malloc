ifndef HOSTTYPE
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

SYMLINK_NAME	= libft_malloc.so
LIBRARY_NAME	= libft_malloc_$(HOSTTYPE).so

SRCDIR			= src
INCDIR			= includes
LIBFTDIR		= libft
OBJDIR			= obj
CC				= cc
CFLAGS			= -Wall -Wextra -Werror -fPIC
LDFLAGS			= -shared
INCLUDES		= -I$(INCDIR) -I$(LIBFTDIR)

SRCS = $(SRCDIR)/malloc.c \
       $(SRCDIR)/free.c \
       $(SRCDIR)/realloc.c \
       $(SRCDIR)/zone_management.c \
       $(SRCDIR)/block_management.c \
       $(SRCDIR)/utils.c \
       $(SRCDIR)/alignment.c \
       $(SRCDIR)/show_alloc_mem.c

OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

LIBFT = $(LIBFTDIR)/libft.a

GREEN = \033[0;32m
RED = \033[0;31m
BLUE = \033[0;34m
NC = \033[0m

all: $(SYMLINK_NAME)

$(SYMLINK_NAME): $(LIBRARY_NAME)
	@ln -sf $(LIBRARY_NAME) $(SYMLINK_NAME)
	@echo "$(GREEN)✓ Created symbolic link: $(SYMLINK_NAME) -> $(LIBRARY_NAME)$(NC)"

$(LIBRARY_NAME): $(OBJDIR) $(OBJS) $(LIBFT)
	@$(CC) $(LDFLAGS) $(OBJS) $(LIBFT) -o $(LIBRARY_NAME)
	@echo "$(GREEN)✓ Built $(LIBRARY_NAME)$(NC)"

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	@echo "$(BLUE)Compiled: $<$(NC)"

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(LIBFT):
	@make -C $(LIBFTDIR) --no-print-directory

clean:
	@rm -rf $(OBJDIR)
	@make -C $(LIBFTDIR) clean --no-print-directory
	@echo "$(RED)✗ Cleaned object files$(NC)"

fclean: clean
	@rm -f *.so
	@make -C $(LIBFTDIR) fclean --no-print-directory
	@echo "$(RED)✗ Cleaned all$(NC)"

re: fclean all

test: all
	@echo "$(BLUE)🚀 Starting Interactive Test Runner...$(NC)"
	@./test_runner.sh

.PHONY: all clean fclean re test
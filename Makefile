ifndef HOSTTYPE
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

EXEC			= test_malloc

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
	@rm -f *.so $(EXEC)
	@make -C $(LIBFTDIR) fclean --no-print-directory
	@echo "$(RED)✗ Cleaned all$(NC)"

re: fclean all

_run_test:
	@if [ ! -f $(SYMLINK_NAME) ]; then \
		echo "$(RED)✗ Library $(SYMLINK_NAME) not found! Run 'make' first$(NC)"; \
		exit 1; \
	fi
	@$(CC) test/test_malloc.c -L. -lft_malloc -o $(EXEC) || { \
		echo "$(RED)✗ Failed to compile test$(NC)"; \
		exit 1; \
	}
	@echo "$(GREEN)✓ Built test executable$(NC)"
	@echo "$(BLUE)Running tests...$(NC)"
	@LD_LIBRARY_PATH=. $(TEST_CMD) ./$(EXEC) || { \
		echo "$(RED)✗ Test execution failed$(NC)"; \
		exit 1; \
	}

test: all
	@$(MAKE) --no-print-directory _run_test TEST_CMD=""

testv: all
	@$(MAKE) --no-print-directory _run_test TEST_CMD="valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=1"

.PHONY: all clean fclean re test testv _run_test
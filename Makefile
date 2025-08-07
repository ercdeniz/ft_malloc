# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::   #
#    Makefile                                           :+:      :+:    :+:   #
#                                                     +:+ +:+         +:+     #
#    By: malloc-project                             +#+  +:+       +#+        #
#                                                 +#+#+#+#+#+   +#+           #
#    Created: 2025/08/04                               #+#    #+#             #
#    Updated: 2025/08/04                              ###   ########.fr       #
#                                                                              #
# **************************************************************************** #

# HOSTTYPE environment variable check
ifndef HOSTTYPE
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

VLG	=
EXEC	=	test_malloc

# Library names
NAME = libft_malloc.so
TARGET = libft_malloc_$(HOSTTYPE).so

# Directories
SRCDIR = src
INCDIR = includes
LIBFTDIR = libft
OBJDIR = obj

# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -g -fPIC -g3
LDFLAGS = -shared
INCLUDES = -I$(INCDIR) -I$(LIBFTDIR)

# Source files
SRCS = $(SRCDIR)/malloc.c \
       $(SRCDIR)/free.c \
       $(SRCDIR)/realloc.c \
       $(SRCDIR)/show_alloc_mem.c \
       $(SRCDIR)/zone_management.c \
       $(SRCDIR)/block_management.c \
       $(SRCDIR)/utils.c \
       $(SRCDIR)/alignment.c

# Object files
OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Libft
LIBFT = $(LIBFTDIR)/libft.a

# Colors for output
GREEN = \033[0;32m
RED = \033[0;31m
BLUE = \033[0;34m
NC = \033[0m # No Color

# Rules
all: $(NAME)

$(NAME): $(TARGET)
	@ln -sf $(TARGET) $(NAME)
	@echo "$(GREEN)✓ Created symbolic link: $(NAME) -> $(TARGET)$(NC)"

$(TARGET): $(OBJDIR) $(OBJS) $(LIBFT)
	@$(CC) $(LDFLAGS) $(OBJS) $(LIBFT) -o $(TARGET)
	@echo "$(GREEN)✓ Built $(TARGET)$(NC)"

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	@echo "$(BLUE)Compiled: $<$(NC)"

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(LIBFT):
	@make -C $(LIBFTDIR)

clean:
	@rm -rf $(OBJDIR)
	@make -C $(LIBFTDIR) clean
	@echo "$(RED)✗ Cleaned object files$(NC)"

fclean: clean
	@rm -f $(TARGET) $(NAME) ${EXEC}
	@make -C $(LIBFTDIR) fclean
	@echo "$(RED)✗ Cleaned all$(NC)"

re: fclean all

test: $(NAME)
	@$(CC) -g3 test/test_malloc.c -L. -lft_malloc -o ${EXEC}
	@echo "$(GREEN)✓ Built test executable$(NC)"

run_test: test
	LD_LIBRARY_PATH=. ${VLG} ./${EXEC}

.PHONY: all clean fclean re test run_test

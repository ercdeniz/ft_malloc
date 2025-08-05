/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malloc-project                             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04                               #+#    #+#             */
/*   Updated: 2025/08/04                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

/**
 * Print information about a single zone
 */
void	print_zone_info(t_zone *zone, const char *type_name)
{
	t_block	*block;
	void	*data_ptr;

	ft_putstr_fd(type_name, 1);
	ft_putstr_fd(" : 0x", 1);
	ft_putnbr_base_fd((unsigned long)zone->start, "0123456789ABCDEF", 1);
	ft_putchar_fd('\n', 1);

	block = zone->blocks;
	while (block)
	{
		if (!block->is_free)
		{
			data_ptr = (char *)block + sizeof(t_block);
			ft_putstr_fd("0x", 1);
			ft_putnbr_base_fd((unsigned long)data_ptr, "0123456789ABCDEF", 1);
			ft_putstr_fd(" - 0x", 1);
			ft_putnbr_base_fd((unsigned long)data_ptr + block->size, "0123456789ABCDEF", 1);
			ft_putstr_fd(" : ", 1);
			ft_putnbr_fd(block->size, 1);
			ft_putstr_fd(" bytes\n", 1);
		}
		block = block->next;
	}
}

/**
 * Print information about all zones of a specific type
 */
static void	print_zones(t_zone *zones, const char *type_name, size_t *total)
{
	t_zone	*current;
	t_block	*block;

	current = zones;
	while (current)
	{
		print_zone_info(current, type_name);

		/* Add to total */
		block = current->blocks;
		while (block)
		{
			if (!block->is_free)
				*total += block->size;
			block = block->next;
		}

		current = current->next;
	}
}

/**
 * Main function to show all allocated memory
 */
void	show_alloc_mem(void)
{
	size_t	total;

	total = 0;

	/* Print TINY zones */
	if (g_malloc_state.tiny_zones)
		print_zones(g_malloc_state.tiny_zones, "TINY", &total);

	/* Print SMALL zones */
	if (g_malloc_state.small_zones)
		print_zones(g_malloc_state.small_zones, "SMALL", &total);

	/* Print LARGE zones */
	if (g_malloc_state.large_zones)
		print_zones(g_malloc_state.large_zones, "LARGE", &total);

	/* Print total */
	ft_putstr_fd("Total : ", 1);
	ft_putnbr_fd(total, 1);
	ft_putstr_fd(" bytes\n", 1);
}

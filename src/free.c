/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malloc-project                             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04                               #+#    #+#             */
/*   Updated: 2025/08/04                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

/**
 * Remove zone from the appropriate list
 */
static void	remove_zone_from_list(t_zone *zone_to_remove)
{
	t_zone	**zones;
	t_zone	*current;

	if (zone_to_remove->type == TINY)
		zones = &g_malloc_state.tiny_zones;
	else if (zone_to_remove->type == SMALL)
		zones = &g_malloc_state.small_zones;
	else
		zones = &g_malloc_state.large_zones;

	if (*zones == zone_to_remove)
	{
		*zones = zone_to_remove->next;
		return;
	}

	current = *zones;
	while (current && current->next != zone_to_remove)
		current = current->next;

	if (current)
		current->next = zone_to_remove->next;
}

/**
 * Free a large allocation (entire zone)
 */
static void	free_large(t_zone *zone)
{
	remove_zone_from_list(zone);
	destroy_zone(zone);
}

/**
 * Free a small/tiny allocation
 */
static void	free_small_tiny(t_zone *zone, t_block *block)
{
	block->is_free = true;
	zone->used_size -= block->size;

	/* Try to merge with adjacent free blocks */
	merge_blocks(block);

	/* If zone is completely empty, remove it */
	if (zone->used_size == 0)
	{
		remove_zone_from_list(zone);
		destroy_zone(zone);
	}
}

/**
 * Main free function
 */
void	free(void *ptr)
{
	t_zone	*zone;
	t_block	*block;

	if (!ptr)
		return;

	if (!is_valid_pointer(ptr))
		return;

	zone = find_zone_containing_ptr(ptr);
	if (!zone)
		return;

	block = get_block_header(ptr);
	if (!block || block->is_free)
		return;

	if (zone->type == LARGE)
		free_large(zone);
	else
		free_small_tiny(zone, block);
}

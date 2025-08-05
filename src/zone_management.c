/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zone_management.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malloc-project                             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04                               #+#    #+#             */
/*   Updated: 2025/08/04                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

/**
 * Create a new zone of specified type
 */
t_zone	*create_zone(t_zone_type type, size_t min_size)
{
	t_zone	*zone;
	size_t	zone_size;
	void	*zone_mem;

	zone_size = get_zone_size(type, min_size);
	zone_mem = safe_mmap(zone_size);
	if (!zone_mem)
		return (NULL);

	/* Zone header is at the beginning of the mapped memory */
	zone = (t_zone *)zone_mem;
	zone->start = zone_mem;
	zone->total_size = zone_size;
	zone->used_size = sizeof(t_zone);
	zone->type = type;
	zone->next = NULL;
	zone->blocks = NULL;

	return (zone);
}

/**
 * Destroy a zone and unmap its memory
 */
void	destroy_zone(t_zone *zone)
{
	if (!zone)
		return;

	safe_munmap(zone->start, zone->total_size);
}

/**
 * Find a zone with enough free space for the requested size
 */
t_zone	*find_zone_with_free_space(t_zone *zones, size_t size)
{
	t_zone	*current;
	t_block	*block;
	size_t	free_space;

	current = zones;
	while (current)
	{
		/* For LARGE zones, each zone contains only one allocation */
		if (current->type == LARGE)
		{
			current = current->next;
			continue;
		}

		/* Calculate available free space */
		free_space = current->total_size - current->used_size;

		/* Check if we have enough space for new block + header */
		if (free_space >= size + sizeof(t_block))
			return (current);

		/* Also check for free blocks that could be reused */
		block = current->blocks;
		while (block)
		{
			if (block->is_free && block->size >= size)
				return (current);
			block = block->next;
		}

		current = current->next;
	}

	return (NULL);
}

/**
 * Clean up empty zones (currently not used, but useful for optimization)
 */
void	cleanup_empty_zones(t_zone **zones)
{
	t_zone	*current;
	t_zone	*prev;
	t_zone	*next;

	if (!zones)
		return;

	prev = NULL;
	current = *zones;

	while (current)
	{
		next = current->next;

		/* If zone is empty (only contains the zone header) */
		if (current->used_size <= sizeof(t_zone))
		{
			if (prev)
				prev->next = next;
			else
				*zones = next;

			destroy_zone(current);
		}
		else
		{
			prev = current;
		}

		current = next;
	}
}

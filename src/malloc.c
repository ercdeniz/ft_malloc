/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malloc-project                             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04                               #+#    #+#             */
/*   Updated: 2025/08/04                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

/* Global state for malloc */
t_malloc_state g_malloc_state = {NULL, NULL, NULL, 0};

/**
 * Initialize malloc state if not already done
 */
static void init_malloc_state(void)
{
	if (g_malloc_state.page_size == 0)
	{
#ifdef __APPLE__
		g_malloc_state.page_size = getpagesize();
#else
		g_malloc_state.page_size = sysconf(_SC_PAGESIZE);
#endif
	}
}

/**
 * Allocate memory for LARGE requests (direct mmap)
 */
static void *malloc_large(size_t size)
{
	t_zone *zone;
	t_block *block;

	zone = create_zone(LARGE, size);
	if (!zone)
		return (NULL);

	block = create_block(zone, size);
	if (!block)
	{
		destroy_zone(zone);
		return (NULL);
	}

	zone->next = g_malloc_state.large_zones;
	g_malloc_state.large_zones = zone;

	return ((char *)block + sizeof(t_block)); // datanın başlangıcına ilerlet
}

/**
 * Allocate memory for TINY or SMALL requests
 */
static void *malloc_small_tiny(size_t size)
{
	t_zone_type type;
	t_zone **zones;
	t_zone *zone;
	t_block *block;

	type = get_zone_type(size);
	zones = (type == TINY) ? &g_malloc_state.tiny_zones : &g_malloc_state.small_zones;

	/* Try to find existing zone with free space */
	zone = find_zone_with_free_space(*zones, size);
	if (!zone)
	{
		/* Create new zone */
		zone = create_zone(type, size);
		if (!zone)
			return (NULL);
		zone->next = *zones;
		*zones = zone;
	}

	/* Find or create block in zone */
	block = find_free_block(zone, size);
	if (!block)
		block = create_block(zone, size);

	if (!block)
		return (NULL);

	/* Split block if too large */
	if (block->size > size + sizeof(t_block) + ALIGNMENT)
		split_block(block, size);

	block->is_free = false;
	zone->used_size += block->size;

	return ((char *)block + sizeof(t_block));
}

/**
 * Main malloc function
 */
void *malloc(size_t size)
{
	void *result;

	if (size == 0)
		return (NULL);

	init_malloc_state();
	size = align_size(size);

	if (size > SMALL_MAX_SIZE)
		result = malloc_large(size);
	else
		result = malloc_small_tiny(size);

	return (result);
}

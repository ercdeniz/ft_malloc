/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malloc-project                             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04                               #+#    #+#             */
/*   Updated: 2025/08/04                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

/**
 * Get the appropriate zone type based on size
 */
t_zone_type	get_zone_type(size_t size)
{
	return (size <= TINY_MAX_SIZE) ? TINY : SMALL;
}

/**
 * Calculate zone size based on type and minimum required size
 */
size_t	get_zone_size(t_zone_type type, size_t min_size)
{
	size_t	zone_size;
	size_t	page_size;

	page_size = g_malloc_state.page_size;

	if (type == TINY)
	{
		zone_size = page_size * TINY_ZONE_PAGES;
	}
	else if (type == SMALL)
	{
		zone_size = page_size * SMALL_ZONE_PAGES;
	}
	else /* LARGE */
	{
		/* For large allocations, use exactly what's needed (rounded to page size) */
		zone_size = sizeof(t_zone) + sizeof(t_block) + min_size;
		zone_size = ((zone_size + page_size - 1) / page_size) * page_size;
	}

	return (zone_size);
}

/**
 * Safe mmap wrapper with error handling
 */
void	*safe_mmap(size_t size)
{
	void	*addr;

	addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (addr == MAP_FAILED)
		return (NULL);

	return (addr);
}

/**
 * Safe munmap wrapper
 */
void	safe_munmap(void *addr, size_t size)
{
	if (addr && size > 0)
		munmap(addr, size);
}

/**
 * Find the zone that contains the given pointer
 */
t_zone	*find_zone_containing_ptr(void *ptr)
{
	t_zone	*zone;
	char	*char_ptr;

	char_ptr = (char *)ptr;

	/* Check TINY zones */
	zone = g_malloc_state.tiny_zones;
	while (zone)
	{
		if (char_ptr >= (char *)zone->start &&
			char_ptr < (char *)zone->start + zone->total_size)
			return (zone);
		zone = zone->next;
	}

	/* Check SMALL zones */
	zone = g_malloc_state.small_zones;
	while (zone)
	{
		if (char_ptr >= (char *)zone->start &&
			char_ptr < (char *)zone->start + zone->total_size)
			return (zone);
		zone = zone->next;
	}

	/* Check LARGE zones */
	zone = g_malloc_state.large_zones;
	while (zone)
	{
		if (char_ptr >= (char *)zone->start &&
			char_ptr < (char *)zone->start + zone->total_size)
			return (zone);
		zone = zone->next;
	}

	return (NULL);
}

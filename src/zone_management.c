#include "malloc.h"

t_zone	*create_zone(t_zone_type type, size_t min_size)
{
	t_zone	*zone;
	size_t	zone_size;
	void	*zone_mem;

	zone_size = get_zone_size(type, min_size);
	zone_mem = safe_mmap(zone_size);
	if (!zone_mem)
		return (NULL);

	zone = (t_zone *)zone_mem;
	zone->start = zone_mem;
	zone->total_size = zone_size;
	zone->used_size = sizeof(t_zone);
	zone->type = type;
	zone->next = NULL;
	zone->blocks = NULL;

	return (zone);
}

void	destroy_zone(t_zone *zone)
{
	if (!zone)
		return;

	safe_munmap(zone->start, zone->total_size);
}

t_zone	*find_zone_with_free_space(t_zone *zones, size_t size)
{
	t_zone	*current;
	t_block	*block;
	size_t	free_space;

	current = zones;
	while (current)
	{
		if (current->type == LARGE)
		{
			current = current->next;
			continue;
		}

		free_space = current->total_size - current->used_size;

		if (free_space >= size + sizeof(t_block))
			return (current);

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

		if (current->used_size <= sizeof(t_zone))
		{
			if (prev)
				prev->next = next;
			else
				*zones = next;

			destroy_zone(current);
		}
		else
			prev = current;

		current = next;
	}
}

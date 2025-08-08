#include "malloc.h"

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

static void	free_large(t_zone *zone)
{
	remove_zone_from_list(zone);
	destroy_zone(zone);
}

static void	free_small_tiny(t_zone *zone, t_block *block)
{
	block->is_free = true;
	zone->used_size -= block->size;

	merge_blocks(block);

	if (zone->used_size == 0)
	{
		remove_zone_from_list(zone);
		destroy_zone(zone);
	}
}

void	free(void *ptr)
{
	t_zone	*zone;
	t_block	*block;
	char	*block_start;

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

	block_start = (char *)block;
	
	if (block->size > zone->total_size - sizeof(t_block))
		return;
		
	if (block_start < (char *)zone->start || 
		block_start + sizeof(t_block) + block->size > (char *)zone->start + zone->total_size)
		return;

	if ((char *)ptr != (char *)block + sizeof(t_block))
		return;

	if (zone->type == LARGE)
		free_large(zone);
	else
		free_small_tiny(zone, block);
}

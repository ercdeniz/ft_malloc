#include "malloc.h"

static void	print_zone_header(t_zone_type type, void *start, void *end)
{
	if (type == TINY)
		ft_putstr_fd("TINY : ", 1);
	else if (type == SMALL)
		ft_putstr_fd("SMALL : ", 1);
	else
		ft_putstr_fd("LARGE : ", 1);
	
	ft_putnbr_base_fd((unsigned long)start, "0123456789ABCDEF", 1);
	ft_putstr_fd(" - ", 1);
	ft_putnbr_base_fd((unsigned long)end, "0123456789ABCDEF", 1);
	ft_putstr_fd("\n", 1);
}

static void	print_block_info(t_block *block)
{
	void	*block_start;
	void	*block_end;

	block_start = (char *)block + sizeof(t_block);
	block_end = (char *)block_start + block->size;
	
	ft_putnbr_base_fd((unsigned long)block_start, "0123456789ABCDEF", 1);
	ft_putstr_fd(" - ", 1);
	ft_putnbr_base_fd((unsigned long)block_end, "0123456789ABCDEF", 1);
	ft_putstr_fd(" : ", 1);
	ft_putnbr_fd((int)block->size, 1);
	ft_putstr_fd(" bytes\n", 1);
}

static void	show_zone_blocks(t_zone *zone)
{
	t_block	*block;
	void	*zone_end;

	zone_end = (char *)zone->start + zone->total_size;
	print_zone_header(zone->type, zone->start, zone_end);
	
	block = zone->blocks;
	while (block)
	{
		if (!block->is_free)
			print_block_info(block);
		block = block->next;
	}
}

static size_t	calculate_total_bytes(void)
{
	t_zone	*zone;
	t_block	*block;
	size_t	total;

	total = 0;
	
	zone = g_malloc_state.tiny_zones;
	while (zone)
	{
		block = zone->blocks;
		while (block)
		{
			if (!block->is_free)
				total += block->size;
			block = block->next;
		}
		zone = zone->next;
	}
	
	zone = g_malloc_state.small_zones;
	while (zone)
	{
		block = zone->blocks;
		while (block)
		{
			if (!block->is_free)
				total += block->size;
			block = block->next;
		}
		zone = zone->next;
	}
	
	zone = g_malloc_state.large_zones;
	while (zone)
	{
		block = zone->blocks;
		while (block)
		{
			if (!block->is_free)
				total += block->size;
			block = block->next;
		}
		zone = zone->next;
	}
	
	return (total);
}

void	show_alloc_mem(void)
{
	t_zone	*zone;

	zone = g_malloc_state.tiny_zones;
	while (zone)
	{
		show_zone_blocks(zone);
		zone = zone->next;
	}
	
	zone = g_malloc_state.small_zones;
	while (zone)
	{
		show_zone_blocks(zone);
		zone = zone->next;
	}
	
	zone = g_malloc_state.large_zones;
	while (zone)
	{
		show_zone_blocks(zone);
		zone = zone->next;
	}
	
	ft_putstr_fd("Total : ", 1);
	ft_putnbr_fd((int)calculate_total_bytes(), 1);
	ft_putstr_fd(" bytes\n", 1);
}
#include "malloc.h"

t_zone_type	get_zone_type(size_t size)
{
	return (size <= TINY_MAX_SIZE) ? TINY : SMALL;
}

size_t	get_zone_size(t_zone_type type, size_t min_size)
{
	size_t	zone_size;
	size_t	page_size;

	page_size = g_malloc_state.page_size;

	if (type == TINY)
		zone_size = page_size * TINY_ZONE_PAGES;
	else if (type == SMALL)
		zone_size = page_size * SMALL_ZONE_PAGES;
	else /* LARGE */
	{
		zone_size = sizeof(t_zone) + sizeof(t_block) + min_size;
		zone_size = ((zone_size + page_size - 1) / page_size) * page_size;
	}

	return (zone_size);
}

void	*safe_mmap(size_t size)
{
	void	*addr;

	addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (addr == MAP_FAILED)
		return (NULL);

	return (addr);
}

void	safe_munmap(void *addr, size_t size)
{
	if (addr && size > 0)
		munmap(addr, size);
}

static t_zone	*find_in_zone_list(t_zone *zone_list, char *ptr)
{
	while (zone_list)
	{
		if (ptr >= (char *)zone_list->start &&
			ptr < (char *)zone_list->start + zone_list->total_size)
			return (zone_list);
		zone_list = zone_list->next;
	}
	return (NULL);
}

t_zone	*find_zone_containing_ptr(void *ptr)
{
	char	*char_ptr = (char *)ptr;
	t_zone	*zone;

	zone = find_in_zone_list(g_malloc_state.tiny_zones, char_ptr);
	if (zone)
		return zone;
	zone = find_in_zone_list(g_malloc_state.small_zones, char_ptr);
	if (zone)
		return zone;
	return find_in_zone_list(g_malloc_state.large_zones, char_ptr);
}

#include "malloc.h"

/**
 * @brief Yeni bir zone oluşturur.
 * @param type Zone türü.
 * @param min_size Zone için minimum boyut.
 * @return Oluşturulan zone veya NULL (başarısız durum).
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
 * @brief Zone'u serbest bırakır.
 * @param zone Serbest bırakılacak zone.
 */
void	destroy_zone(t_zone *zone)
{
	if (!zone)
		return;

	safe_munmap(zone->start, zone->total_size);
}

/**
 * @brief İstenilen boyutta boş alanı olan bir zone bulur.
 * @param zones Zone listesi.
 * @param size Gerekli boyut.
 * @return Uygun zone veya NULL (bulunamadı).
 */
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

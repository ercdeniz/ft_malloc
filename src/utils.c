#include "malloc.h"

/**
 * @brief Verilen boyuta göre zone türünü döner.
 * @param size Tahsis edilecek boyut.
 * @return Zone türü.
 */
t_zone_type	get_zone_type(size_t size)
{
	if (size <= TINY_MAX_SIZE)
		return (TINY);
	else if (size <= SMALL_MAX_SIZE)
		return (SMALL);
	else
		return (LARGE);
}

/**
 * @brief Zone boyutunu hesaplar.
 * @param type Zone türü.
 * @param min_size Kullanılacak boyut.
 * @return Zone boyutu.
 */
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

/**
 * @brief Bellek alanı tahsis eder.
 * @details mmap sistem çağrısını sarmalayarak güvenli bir şekilde bellek alanı tahsis eder.
 * @param size Tahsis edilecek boyut.
 * @return Tahsis edilen bellek alanının başlangıç adresi veya NULL (başarısız durum).
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
 * @brief Bellek alanını serbest bırakır.
 * @details mmap sistem çağrısını sarmalayarak güvenli bir şekilde bellek alanını serbest bırakır.
 * @param addr Serbest bırakılacak bellek alanının başlangıç adresi.
 * @param size Serbest bırakılacak bellek alanının boyutu.
 */
void	safe_munmap(void *addr, size_t size)
{
	if (addr && size > 0)
		munmap(addr, size);
}

/**
 * @brief Verilen bir işaretçinin hangi zone içinde olduğunu bulur.
 * @param zone_list Aranacak zone listesi.
 * @param ptr Aranacak işaretçi.
 * @return Bulunan zone veya NULL(bulunamadı).
 */
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

/**
 * @brief Verilen bir işaretçinin hangi zone içinde olduğunu bulur.
 * @param ptr Aranacak işaretçi.
 * @return Bulunan zone veya NULL (bulunamadı).
 */
t_zone	*find_zone_containing_ptr(void *ptr)
{
    if (!ptr)
        return NULL;
    
    t_zone *zone_types[] = {
        g_malloc_state.tiny_zones,
        g_malloc_state.small_zones, 
        g_malloc_state.large_zones
    };

    t_zone *zone;
    for (size_t i = 0; i < 3; i++) {
        zone = find_in_zone_list(zone_types[i], (char *)ptr);
        if (zone)
            return zone;
    }
    
    return NULL;
}
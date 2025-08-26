#include "malloc.h"

/**
 * @brief Tahsisleri yöneten global değişken
 * @param İlk (NULL) tiny zone.
 * @param İkinci (NULL) small zone.
 * @param Üçüncü (NULL) large zone.
 * @param Dördüncü (0) sayfa boyutu 0 olarak ayarlanır (başlangıç değeri).
 */
t_malloc_state g_malloc_state = {NULL, NULL, NULL, 0};

/**
 * @brief Bellek tahsisi için sayfa boyutunu işletim sistemine göre ayarlar.
 * @details getpagesize() veya sysconf(_SC_PAGESIZE) kullanılarak sayfa boyutu alınır.
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
 * @brief Large bir bellek bloğu tahsis eder.
 * @param size Tahsis edilecek bellek bloğunun boyutu.
 * @return Tahsis edilen bellek bloğunun başlangıç adresi veya NULL(başarısız durum).
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
	zone->used_size += sizeof(t_block) + size;

	return ((char *)block + sizeof(t_block));
}

/**
 * @brief Small veya tiny zone'u tahsis eder.
 * @param size Tahsis edilecek bellek bloğunun boyutu.
 * @return Tahsis edilen bellek bloğunun başlangıç adresi veya NULL(başarısız durum).
 */
static void *malloc_small_tiny(size_t size)
{
	t_zone_type type;
	t_zone **zones;
	t_zone *zone;
	t_block *block;

	type = get_zone_type(size);
	zones = (type == TINY) ? &g_malloc_state.tiny_zones : &g_malloc_state.small_zones;

	zone = find_zone_with_free_space(*zones, size);
	if (!zone)
	{
		zone = create_zone(type, size);
		if (!zone)
			return (NULL);
		zone->next = *zones;
		*zones = zone;
	}

	block = find_free_block(zone, size);
	if (!block)
	{
		block = create_block(zone, size);
		if (!block)
			return (NULL);
		zone->used_size += sizeof(t_block);
	}
	else
	{
		block->is_free = false;
	}

	if (block->size > size + sizeof(t_block) + ALIGNMENT)
		split_block(block, size);

	zone->used_size += block->size;

	return ((char *)block + sizeof(t_block));
}

/**
 * @brief Bellek tahsisi için ana fonksiyon.
 * @param size Tahsis edilecek bellek bloğunun boyutu.
 * @return Tahsis edilen bellek bloğunun başlangıç adresi veya NULL(başarısız durum).
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
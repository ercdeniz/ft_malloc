#include "malloc.h"
#include "colors.h"

/**
 * @brief Zone başlığını ve başlangıç adresini yazdırır.
 * @param type Zone türü.
 * @param start Zone başlangıç adresi.
 */
static void	print_zone_header(t_zone_type type, void *start)
{
	const char *type_names[] = {"TINY", "SMALL", "LARGE"};
	ft_printf("%s%s%s%s : %s%p%s\n", BOLD, YELLOW, type_names[type], 
		NC, CYAN, start, NC);
}

/**
 * @brief Blok bilgilerini yazdırır.
 * @param block Yazdırılacak blok.
 */
static void	print_block_info(t_block *block)
{
	void *start = (char *)block + sizeof(t_block);
	void *end = (char *)start + block->size;
	char *unit = "bytes";
	unsigned int size = block->size;
	
	if (size >= 1024 * 1024 * 1024)
	{
		size /= (1024 * 1024 * 1024);
		unit = "GB";
	}
	else if (size >= 1024 * 1024)
	{
		size /= (1024 * 1024);
		unit = "MB";
	}
	
	ft_printf("%s%p%s - %s%p%s : %s%u%s %s%s%s\n", 
		BLUE, start, NC, BLUE, end, NC,
		block->is_free ? WHITE : MAGENTA, size, 
		block->is_free ? WHITE : MAGENTA, unit,
		block->is_free ? " (FREE)" : "", NC);
}

/**
 * @brief Toplam tahsis edilen bayt sayısını hesaplar.
 * @return Toplam tahsis edilen bayt sayısı.
 */
static size_t	calculate_total_bytes(void)
{
	t_zone *zone;
	t_block *block;
	size_t total = 0;
	
	for (zone = g_malloc_state.tiny_zones; zone; zone = zone->next)
		for (block = zone->blocks; block; block = block->next)
			if (!block->is_free)
				total += block->size;
				
	for (zone = g_malloc_state.small_zones; zone; zone = zone->next)
		for (block = zone->blocks; block; block = block->next)
			if (!block->is_free)
				total += block->size;
				
	for (zone = g_malloc_state.large_zones; zone; zone = zone->next)
		for (block = zone->blocks; block; block = block->next)
			if (!block->is_free)
				total += block->size;
	
	return (total);
}

/**
 * @brief Yapılan bellek tahsislerinin ve serbest bırakmalarının özetini gösterir.
 */
void	show_alloc_mem(void)
{
	t_zone *zone;
	size_t total;
	char *unit = "bytes";
	unsigned int size;

	for (zone = g_malloc_state.tiny_zones; zone; zone = zone->next)
	{
		print_zone_header(zone->type, zone->start);
		for (t_block *block = zone->blocks; block; block = block->next)
			print_block_info(block);
	}
	
	for (zone = g_malloc_state.small_zones; zone; zone = zone->next)
	{
		print_zone_header(zone->type, zone->start);
		for (t_block *block = zone->blocks; block; block = block->next)
			print_block_info(block);
	}
	
	for (zone = g_malloc_state.large_zones; zone; zone = zone->next)
	{
		print_zone_header(zone->type, zone->start);
		for (t_block *block = zone->blocks; block; block = block->next)
			print_block_info(block);
	}
	
	total = calculate_total_bytes();
	size = total;
	if (total >= 1024 * 1024 * 1024)
		size /= (1024 * 1024 * 1024), unit = "GB";
	else if (total >= 1024 * 1024)
		size /= (1024 * 1024), unit = "MB";
	else if (total >= 1024)
		size /= 1024, unit = "KB";
		
	ft_printf("%s%sTotal : %s%u%s %s%s\n", BOLD, WHITE, GREEN, size, WHITE, unit, NC);
}
#include "malloc.h"

/**
 * @brief Yeni bir blok oluşturur ve zone'a ekler.
 * @param zone Eklenecek zone.
 * @param size Oluşturulacak bloğun boyutu.
 * @return Oluşturulan blok.
 */
t_block	*create_block(t_zone *zone, size_t size)
{
	t_block	*block;
	char	*block_start;

	if (!zone->blocks)
		block_start = (char *)zone->start + sizeof(t_zone);
	else
	{
		t_block *last_block = zone->blocks;
		while (last_block->next)
			last_block = last_block->next;

		block_start = (char *)last_block + sizeof(t_block) + last_block->size;
	}

	if (block_start + sizeof(t_block) + size > (char *)zone->start + zone->total_size)
		return (NULL);

	block = (t_block *)block_start;
	block->size = size;
	block->is_free = false;
	block->next = NULL;
	block->prev = NULL;

	if (!zone->blocks)
		zone->blocks = block;
	else
	{
		t_block *last_block = zone->blocks;
		while (last_block->next)
			last_block = last_block->next;

		last_block->next = block;
		block->prev = last_block;
	}

	return (block);
}

/**
 * @brief Uygun bir boş blok bulur.
 * @param zone Aranacak zone.
 * @param size İstenen blok boyutu.
 * @return Bulunan boş blok veya NULL(hiçbir blok bulunamazsa).
 */
t_block	*find_free_block(t_zone *zone, size_t size)
{
	t_block	*current;

	current = zone->blocks;
	while (current)
	{
		if (current->is_free && current->size >= size)
			return (current);
		current = current->next;
	}

	return (NULL);
}

/**
 * @brief Tahsis edilen alan bloğun hepsini kaplamıyorsa bölünür, kalan alan yeni bir blok olarak ayrılır.
 * @param block Bölünecek blok.
 * @param size Yeni bloğun boyutu.
 */
void	split_block(t_block *block, size_t size)
{
	t_block	*new_block;
	char	*new_block_start;

	if (!block || block->size <= size + sizeof(t_block))
		return;

	new_block_start = (char *)block + sizeof(t_block) + size;
	new_block = (t_block *)new_block_start;

	new_block->size = block->size - size - sizeof(t_block);
	new_block->is_free = true;
	new_block->next = block->next;
	new_block->prev = block;

	if (block->next)
		block->next->prev = new_block;
	block->next = new_block;

	block->size = size;
}

/**
 * @brief Tahsis edilen alan serbest bırakıldığında blokları birleştirir.
 * @param block Birleştirilecek blok.
 */
void	merge_blocks(t_block *block)
{
	t_block	*next_block;

	if (!block)
		return;

	next_block = block->next;
	if (next_block && next_block->is_free)
	{
		if ((char *)block + sizeof(t_block) + block->size == (char *)next_block)
		{
			block->size += sizeof(t_block) + next_block->size;
			block->next = next_block->next;
			if (next_block->next)
				next_block->next->prev = block;
		}
	}

	if (block->prev && block->prev->is_free)
	{
		if ((char *)block->prev + sizeof(t_block) + block->prev->size == (char *)block)
		{
			block->prev->size += sizeof(t_block) + block->size;
			block->prev->next = block->next;
			if (block->next)
				block->next->prev = block->prev;
		}
	}
}

/**
 * @brief Bloğun başlık bilgilerini alır.
 * @param ptr Bloğun başlangıç adresi.
 * @return Bloğun başlık bilgileri.
 */
t_block	*get_block_header(void *ptr)
{
	if (!ptr)
		return (NULL);

	return ((t_block *)((char *)ptr - sizeof(t_block)));
}

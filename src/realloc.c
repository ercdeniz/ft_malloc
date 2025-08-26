#include "malloc.h"

/**
 * @brief Bellek alanını kopyalar.
 * @param dst Hedef bellek alanı.
 * @param src Kaynak bellek alanı.
 * @param old_size Eski bellek alanının boyutu.
 * @param new_size Yeni bellek alanının boyutu.
 */
static void	copy_data(void *dst, void *src, size_t old_size, size_t new_size)
{
	size_t	copy_size;

	copy_size = (old_size < new_size) ? old_size : new_size;
	ft_memcpy(dst, src, copy_size);
}

/**
 * @brief Bellek bloğunu bulunduğu zone'da genişletmeye çalışır.
 * @param block Genişletilecek bellek bloğu.
 * @param new_size Yeni boyut.
 * @return Başarılı ise true, aksi halde false.
 */
static bool	try_expand_in_place(t_block *block, size_t new_size)
{
	t_block	*next_block;
	size_t	available_size;

	next_block = block->next;
	available_size = block->size;

	if (next_block && next_block->is_free)
		if ((char *)block + sizeof(t_block) + block->size == (char *)next_block)
			available_size += sizeof(t_block) + next_block->size;

	if (available_size >= new_size)
	{
		if (next_block && next_block->is_free &&
			(char *)block + sizeof(t_block) + block->size == (char *)next_block)
		{
			block->size += sizeof(t_block) + next_block->size;
			block->next = next_block->next;
			if (next_block->next)
				next_block->next->prev = block;
		}

		if (block->size > new_size + sizeof(t_block) + ALIGNMENT)
			split_block(block, new_size);
		else
			block->size = new_size;

		return (true);
	}

	return (false);
}

/**
 * @brief Large bir bellek bloğunu yeniden tahsis eder.
 * @param ptr Yeniden tahsis edilecek bellek bloğunun başlangıç adresi.
 * @param size Yeni boyut.
 * @return Tahsis edilen bellek bloğunun başlangıç adresi veya NULL(başarısız durum).
 */
static void	*realloc_large(void *ptr, size_t size)
{
	void	*new_ptr;
	t_block	*block;

	block = get_block_header(ptr);
	if (!block)
		return (NULL);

	new_ptr = malloc(size);
	if (!new_ptr)
		return (NULL);

	copy_data(new_ptr, ptr, block->size, size);
	free(ptr);

	return (new_ptr);
}

/**
 * @brief Small veya tiny bir bellek bloğunu yeniden tahsis eder.
 * @param ptr Yeniden tahsis edilecek bellek bloğunun başlangıç adresi.
 * @param size Yeni boyut.
 * @return Tahsis edilen bellek bloğunun başlangıç adresi veya NULL(başarısız durum).
 */
static void	*realloc_small_tiny(void *ptr, size_t size)
{
	t_block	*block;
	void	*new_ptr;

	block = get_block_header(ptr);
	if (!block)
		return (NULL);

	if (size <= block->size)
	{
		if (block->size > size + sizeof(t_block) + ALIGNMENT)
			split_block(block, size);
		return (ptr);
	}

	if (try_expand_in_place(block, size))
		return (ptr);

	new_ptr = malloc(size);
	if (!new_ptr)
		return (NULL);

	copy_data(new_ptr, ptr, block->size, size);
	free(ptr);

	return (new_ptr);
}

/**
 * @brief Yeniden tahsis için ana fonksiyon.
 * @param ptr Yeniden tahsis edilecek bellek bloğunun başlangıç adresi.
 * @param size Yeni boyut.
 * @return Tahsis edilen bellek bloğunun başlangıç adresi veya NULL(başarısız durum).
 */
void	*realloc(void *ptr, size_t size)
{
	t_zone	*zone;
	void	*result;

	if (!ptr)
		return (malloc(size));

	if (size == 0)
	{
		free(ptr);
		return (NULL);
	}

	zone = find_zone_containing_ptr(ptr);
	if (!zone)
		return (NULL);

	size = align_size(size);

	if (zone->type == LARGE)
		result = realloc_large(ptr, size);
	else
		result = realloc_small_tiny(ptr, size);

	return (result);
}

#include "../includes/malloc.h"

static void	copy_data(void *dst, void *src, size_t old_size, size_t new_size)
{
	size_t	copy_size;

	copy_size = (old_size < new_size) ? old_size : new_size;
	ft_memcpy(dst, src, copy_size);
}

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

	if (!is_valid_pointer(ptr))
		return (NULL);

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

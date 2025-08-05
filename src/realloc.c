/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malloc-project                             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04                               #+#    #+#             */
/*   Updated: 2025/08/04                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

/**
 * Copy data from old pointer to new pointer
 */
static void	copy_data(void *dst, void *src, size_t old_size, size_t new_size)
{
	size_t	copy_size;

	copy_size = (old_size < new_size) ? old_size : new_size;
	ft_memcpy(dst, src, copy_size);
}

/**
 * Try to expand block in place
 */
static bool	try_expand_in_place(t_block *block, size_t new_size)
{
	t_block	*next_block;
	size_t	available_size;

	next_block = block->next;
	available_size = block->size;

	/* Check if next block is free and adjacent */
	if (next_block && next_block->is_free)
	{
		/* Check if blocks are adjacent */
		if ((char *)block + sizeof(t_block) + block->size == (char *)next_block)
		{
			available_size += sizeof(t_block) + next_block->size;
		}
	}

	if (available_size >= new_size)
	{
		/* Merge with next block if needed */
		if (next_block && next_block->is_free &&
			(char *)block + sizeof(t_block) + block->size == (char *)next_block)
		{
			block->size += sizeof(t_block) + next_block->size;
			block->next = next_block->next;
			if (next_block->next)
				next_block->next->prev = block;
		}

		/* Split if too large */
		if (block->size > new_size + sizeof(t_block) + ALIGNMENT)
			split_block(block, new_size);
		else
			block->size = new_size;

		return (true);
	}

	return (false);
}

/**
 * Handle realloc for large allocations
 */
static void	*realloc_large(void *ptr, size_t size)
{
	void	*new_ptr;
	t_block	*block;

	block = get_block_header(ptr);
	if (!block)
		return (NULL);

	/* For large allocations, always allocate new and copy */
	new_ptr = malloc(size);
	if (!new_ptr)
		return (NULL);

	copy_data(new_ptr, ptr, block->size, size);
	free(ptr);

	return (new_ptr);
}

/**
 * Handle realloc for small/tiny allocations
 */
static void	*realloc_small_tiny(void *ptr, size_t size)
{
	t_block	*block;
	void	*new_ptr;

	block = get_block_header(ptr);
	if (!block)
		return (NULL);

	/* If new size is same or smaller, try to shrink */
	if (size <= block->size)
	{
		if (block->size > size + sizeof(t_block) + ALIGNMENT)
			split_block(block, size);
		return (ptr);
	}

	/* Try to expand in place */
	if (try_expand_in_place(block, size))
		return (ptr);

	/* Allocate new block and copy */
	new_ptr = malloc(size);
	if (!new_ptr)
		return (NULL);

	copy_data(new_ptr, ptr, block->size, size);
	free(ptr);

	return (new_ptr);
}

/**
 * Main realloc function
 */
void	*realloc(void *ptr, size_t size)
{
	t_zone	*zone;
	void	*result;

	/* Handle special cases */
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

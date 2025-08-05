/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_management.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malloc-project                             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04                               #+#    #+#             */
/*   Updated: 2025/08/04                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

/**
 * Create a new block in the specified zone
 */
t_block	*create_block(t_zone *zone, size_t size)
{
	t_block	*block;
	char	*block_start;

	/* Find position for new block */
	if (!zone->blocks)
	{
		/* First block in zone - place after zone header */
		block_start = (char *)zone->start + sizeof(t_zone);
	}
	else
	{
		/* Find end of last block */
		t_block *last_block = zone->blocks;
		while (last_block->next)
			last_block = last_block->next;

		block_start = (char *)last_block + sizeof(t_block) + last_block->size;
	}

	/* Check if we have enough space */
	if (block_start + sizeof(t_block) + size > (char *)zone->start + zone->total_size)
		return (NULL);

	/* Initialize block */
	block = (t_block *)block_start;
	block->size = size;
	block->is_free = false;
	block->next = NULL;
	block->prev = NULL;

	/* Add to zone's block list */
	if (!zone->blocks)
	{
		zone->blocks = block;
	}
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
 * Find a free block of sufficient size
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
 * Split a block into two parts
 */
void	split_block(t_block *block, size_t size)
{
	t_block	*new_block;
	char	*new_block_start;

	if (!block || block->size <= size + sizeof(t_block))
		return;

	/* Calculate position for new block */
	new_block_start = (char *)block + sizeof(t_block) + size;
	new_block = (t_block *)new_block_start;

	/* Initialize new block */
	new_block->size = block->size - size - sizeof(t_block);
	new_block->is_free = true;
	new_block->next = block->next;
	new_block->prev = block;

	/* Update links */
	if (block->next)
		block->next->prev = new_block;
	block->next = new_block;

	/* Update original block */
	block->size = size;
}

/**
 * Merge a block with adjacent free blocks
 */
void	merge_blocks(t_block *block)
{
	t_block	*next_block;

	if (!block)
		return;

	/* Merge with next block if it's free and adjacent */
	next_block = block->next;
	if (next_block && next_block->is_free)
	{
		/* Check if blocks are adjacent */
		if ((char *)block + sizeof(t_block) + block->size == (char *)next_block)
		{
			block->size += sizeof(t_block) + next_block->size;
			block->next = next_block->next;
			if (next_block->next)
				next_block->next->prev = block;
		}
	}

	/* Merge with previous block if it's free and adjacent */
	if (block->prev && block->prev->is_free)
	{
		/* Check if blocks are adjacent */
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
 * Get block header from data pointer
 */
t_block	*get_block_header(void *ptr)
{
	if (!ptr)
		return (NULL);

	return ((t_block *)((char *)ptr - sizeof(t_block)));
}

/**
 * Check if pointer is valid (basic validation)
 */
bool	is_valid_pointer(void *ptr)
{
	t_block	*block;

	if (!ptr)
		return (false);

	/* Get block header */
	block = get_block_header(ptr);

	/* Basic sanity checks */
	if (block->size == 0 || block->size > (1024 * 1024 * 1024)) /* 1GB max */
		return (false);

	return (true);
}

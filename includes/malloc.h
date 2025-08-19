#ifndef MALLOC_H
# define MALLOC_H

#define _GNU_SOURCE
# include <unistd.h>
# include <sys/mman.h>
# include <sys/resource.h>
# include <stdbool.h>
# include "libft.h"

# define ALIGNMENT			16
# define TINY_MAX_SIZE		128
# define SMALL_MAX_SIZE		1024

# define TINY_ZONE_PAGES	4
# define SMALL_ZONE_PAGES	16
# define MAX_ALLOC_SIZE		1024 * 1024 * 1024

typedef enum e_zone_type
{
	TINY,
	SMALL,
	LARGE
}	t_zone_type;

typedef struct s_block
{
	size_t				size;
	bool				is_free;
	struct s_block		*next;
	struct s_block		*prev;
}	t_block;

typedef struct s_zone
{
	void				*start;
	size_t				total_size;
	size_t				used_size;
	t_zone_type			type;
	struct s_zone		*next;
	t_block				*blocks;
}	t_zone;

typedef struct s_malloc_state
{
	t_zone				*tiny_zones;
	t_zone				*small_zones;
	t_zone				*large_zones;
	size_t				page_size;
}	t_malloc_state;

extern t_malloc_state	g_malloc_state;

void					*malloc(size_t size);
void					free(void *ptr);
void					*realloc(void *ptr, size_t size);
void					show_alloc_mem(void);

t_zone					*create_zone(t_zone_type type, size_t min_size);
void					destroy_zone(t_zone *zone);
t_zone					*find_zone_with_free_space(t_zone *zones, size_t size);
t_zone					*find_zone_containing_ptr(void *ptr);
void					cleanup_empty_zones(t_zone **zones);

t_block					*create_block(t_zone *zone, size_t size);
t_block					*find_free_block(t_zone *zone, size_t size);
void					split_block(t_block *block, size_t size);
void					merge_blocks(t_block *block);
t_block					*get_block_header(void *ptr);
bool					is_valid_pointer(void *ptr);

size_t					align_size(size_t size);
size_t					get_zone_size(t_zone_type type, size_t min_size);
t_zone_type				get_zone_type(size_t size);
void					*safe_mmap(size_t size);
void					safe_munmap(void *addr, size_t size);
#endif
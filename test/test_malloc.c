#include <string.h>
#include <assert.h>
#include "malloc.h"
#include "colors.h"

void	test_malloc(void)
{
	void *ptr1 = malloc(42);
	ft_printf("malloc(42): " YELLOW "%p" NC "\n", ptr1);
	assert(ptr1 != NULL);

	void *ptr2 = malloc(1024);
	ft_printf("malloc(1024): " YELLOW "%p" NC "\n", ptr2);
	assert(ptr2 != NULL);

	void *ptr3 = malloc(2048);
	ft_printf("malloc(2048): " YELLOW "%p" NC "\n", ptr3);
	assert(ptr3 != NULL);
	
	ft_printf(CYAN "=== Before Free ===" NC "\n");
	show_alloc_mem();
	
	free(ptr1);
	free(ptr2);
	free(ptr3);
	
	ft_printf(CYAN "\n=== After Free ===" NC "\n");
	show_alloc_mem();
}

int main(void)
{
	test_malloc();
	return (0);
}

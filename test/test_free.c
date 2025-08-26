#include <string.h>
#include <assert.h>
#include "malloc.h"
#include "colors.h"

void	test_free(void)
{
	void *ptr = malloc(100);
	ft_printf("Allocated 100 bytes at: " YELLOW "%p" NC "\n", ptr);
	assert(ptr != NULL);

	ft_printf(CYAN "\n--- Before Free ---" NC "\n");
	show_alloc_mem();

	free(ptr);
	free(NULL);
	
	ft_printf(CYAN "\n--- After Free ---" NC "\n");
	show_alloc_mem();
}

int main(void)
{
	test_free();
	return (0);
}

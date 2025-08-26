#include <string.h>
#include <assert.h>
#include "malloc.h"
#include "colors.h"

void	test_realloc(void)
{
	void *ptr = malloc(50);
	strcpy((char*)ptr, "Hello World!");
	ft_printf("Original: " MAGENTA "%s" NC "\n", (char*)ptr);

	ft_printf(CYAN "\n--- After malloc(50) ---" NC "\n");
	show_alloc_mem();

	ptr = realloc(ptr, 100);
	ft_printf("\nAfter realloc(100): " MAGENTA "%s" NC "\n", (char*)ptr);
	
	ft_printf(CYAN "\n--- After realloc(100) ---" NC "\n");
	show_alloc_mem();

	ptr = realloc(ptr, 25);
	ft_printf("\nAfter realloc(25): " MAGENTA "%s" NC "\n", (char*)ptr);
	
	ft_printf(CYAN "\n--- After realloc(25) ---" NC "\n");
	show_alloc_mem();

	free(ptr);
	
	ft_printf(CYAN "\n--- After final free ---" NC "\n");
	show_alloc_mem();
}

int main(void)
{
	test_realloc();
	return (0);
}

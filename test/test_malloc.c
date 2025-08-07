#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../includes/malloc.h"

void	test_basic_malloc(void)
{
	printf("=== Testing Basic Malloc ===\n");

	void *ptr1 = malloc(42);
	printf("malloc(42): %p\n", ptr1);
	assert(ptr1 != NULL);

	void *ptr2 = malloc(1024);
	printf("malloc(1024): %p\n", ptr2);
	assert(ptr2 != NULL);

	void *ptr3 = malloc(2048);
	printf("malloc(2048): %p\n", ptr3);
	assert(ptr3 != NULL);

	free(ptr1);
	free(ptr2);
	free(ptr3);

	printf("✓ Basic malloc tests passed\n\n");
}

void	test_free(void)
{
	printf("=== Testing Free ===\n");

	void *ptr = malloc(100);
	printf("Allocated 100 bytes at: %p\n", ptr);

	assert(ptr != NULL);
	free(ptr);
	printf("Freed pointer\n");

	free(NULL);
	printf("✓ Free tests passed\n\n");
}

void	test_realloc(void)
{
	printf("=== Testing Realloc ===\n");

	void *ptr = malloc(50);
	strcpy((char*)ptr, "Hello World!");
	printf("Original: %s\n", (char*)ptr);
	show_alloc_mem();


	ptr = realloc(ptr, 100);
	printf("After realloc(100): %s\n", (char*)ptr);

	ptr = realloc(ptr, 25);
	printf("After realloc(25): %s\n", (char*)ptr);

	free(ptr);
	printf("✓ Realloc tests passed\n\n");
}

void	test_show_alloc_mem(void)
{
	printf("=== Testing show_alloc_mem ===\n");

	void *ptrs[5];
	ptrs[0] = malloc(42);
	ptrs[1] = malloc(100);
	ptrs[2] = malloc(1000);
	ptrs[3] = malloc(5000);
	ptrs[4] = malloc(10);

	printf("\nMemory state after allocations:\n");
	show_alloc_mem();

	free(ptrs[1]);
	free(ptrs[3]);

	printf("\nMemory state after some frees:\n");
	show_alloc_mem();

	free(ptrs[0]);
	free(ptrs[2]);
	free(ptrs[4]);

	printf("✓ show_alloc_mem tests passed\n\n");
}

void	test_edge_cases(void)
{
	printf("=== Testing Edge Cases ===\n");

	void *ptr1 = malloc(0);
	printf("malloc(0): %p\n", ptr1);

	void *ptr2 = realloc(NULL, 50);
	printf("realloc(NULL, 50): %p\n", ptr2);
	assert(ptr2 != NULL);

	void *ptr3 = realloc(ptr2, 0);
	printf("realloc(ptr, 0): %p\n", ptr3);

	if (ptr1 != NULL)
		free(ptr1);

	printf("✓ Edge case tests passed\n\n");
}

int	main(void)
{
	printf("🧪 Starting Malloc Tests\n");
	printf("========================\n\n");

	test_basic_malloc();
	test_free();
	test_realloc();
	test_show_alloc_mem();
	test_edge_cases();

	printf("🎉 All tests completed!\n");
	return (0);
}

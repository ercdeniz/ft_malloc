#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "malloc.h"

#define GREEN  "\033[0;32m"
#define RED    "\033[0;31m"
#define BLUE   "\033[0;34m"
#define YELLOW "\033[0;33m"
#define CYAN   "\033[0;36m"
#define PURPLE "\033[0;35m"
#define NC     "\033[0m"

void	test_basic_malloc(void)
{
	printf(CYAN "=== Testing Basic Malloc ===" NC "\n");

	void *ptr1 = malloc(42);
	printf("malloc(42): " YELLOW "%p" NC "\n", ptr1);
	assert(ptr1 != NULL);

	void *ptr2 = malloc(1024);
	printf("malloc(1024): " YELLOW "%p" NC "\n", ptr2);
	assert(ptr2 != NULL);

	void *ptr3 = malloc(2048);
	printf("malloc(2048): " YELLOW "%p" NC "\n", ptr3);
	assert(ptr3 != NULL);

	free(ptr1);
	free(ptr2);
	free(ptr3);

	printf(GREEN "✓ Basic malloc tests passed" NC "\n\n");
}

void	test_free(void)
{
	printf(CYAN "=== Testing Free ===" NC "\n");

	void *ptr = malloc(100);
	printf("Allocated 100 bytes at: " YELLOW "%p" NC "\n", ptr);

	assert(ptr != NULL);
	free(ptr);
	printf("Freed pointer\n");

	free(NULL);
	printf(GREEN "✓ Free tests passed" NC "\n\n");
}

void	test_realloc(void)
{
	printf(CYAN "=== Testing Realloc ===" NC "\n");

	void *ptr = malloc(50);
	strcpy((char*)ptr, "Hello World!");
	printf("Original: " PURPLE "%s" NC "\n", (char*)ptr);

	ptr = realloc(ptr, 100);
	printf("After realloc(100): " PURPLE "%s" NC "\n", (char*)ptr);

	ptr = realloc(ptr, 25);
	printf("After realloc(25): " PURPLE "%s" NC "\n", (char*)ptr);

	free(ptr);
	printf(GREEN "✓ Realloc tests passed" NC "\n\n");
}

void	test_edge_cases(void)
{
	printf(CYAN "=== Testing Edge Cases ===" NC "\n");

	void *ptr1 = malloc(0);
	printf("malloc(0): " YELLOW "%p" NC "\n", ptr1);

	void *ptr2 = realloc(NULL, 50);
	printf("realloc(NULL, 50): " YELLOW "%p" NC "\n", ptr2);
	assert(ptr2 != NULL);

	void *ptr3 = realloc(ptr2, 0);
	printf("realloc(ptr, 0): " YELLOW "%p" NC "\n", ptr3);

	if (ptr1 != NULL)
		free(ptr1);

	printf(GREEN "✓ Edge case tests passed" NC "\n\n");
}

int	main(void)
{
	printf(BLUE "🧪 Starting Malloc Tests" NC "\n");
	printf(BLUE "========================" NC "\n\n");

	test_basic_malloc();
	test_free();
	test_realloc();
	test_edge_cases();

	printf(GREEN "🎉 All tests completed!" NC "\n");
	return (0);
}

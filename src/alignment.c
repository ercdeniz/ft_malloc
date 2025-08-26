#include "malloc.h"

/**
 * @brief Verilen boyutu ALIGNMENT'a hizalar.
 * @param size Hizalanacak boyut.
 * @return Hizalanmış boyut.
 */
size_t	align_size(size_t size)
{
	if (size == 0)
		return (0);
	return (size + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1);
}

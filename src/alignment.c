/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alignment.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malloc-project                             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04                               #+#    #+#             */
/*   Updated: 2025/08/04                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

/**
 * Align size to the specified alignment boundary
 */
size_t	align_size(size_t size)
{
	if (size == 0)
	return (0);
	return (size + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1);
}

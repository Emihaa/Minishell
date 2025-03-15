/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lt_alloc_arena.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 16:43:43 by ltaalas           #+#    #+#             */
/*   Updated: 2025/03/15 18:22:52 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/lt_alloc.h"

void	*arena_alloc(t_arena *a, uint64_t size)
{
	void *result;

	if (a->size + size > a->capacity)
	{
		errno = ENOMEM;
		return (NULL);
	}
	result = &a->data[a->size];
	a->size += size;
	while (size--)
		((uint8_t *)result)[size] = 0;
	return (result);
}

void	*arena_alloc_no_zero(t_arena *a, uint64_t size)
{
	void *result;

	if (a->size + size > a->capacity)
	{
		errno = ENOMEM;
		return (NULL);
	}
	result = &a->data[a->size];
	a->size += size;
	return (result);
}

int	arena_unalloc(t_arena *a, uint64_t size)
{
	if (a->size - size > size)
	{
		errno = ERANGE;
		return (-1);	
	}
	a->size -= size;
	return (0);
}

void arena_reset(t_arena *a)
{
	a->size = 0;
}





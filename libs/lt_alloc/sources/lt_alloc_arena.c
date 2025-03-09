/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lt_alloc_arena.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 16:43:43 by ltaalas           #+#    #+#             */
/*   Updated: 2025/03/09 20:49:16 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/lt_alloc.h"

void	*arena_alloc(t_arena *a, t_u64 size)
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
		((t_u8 *)result)[size] = 0;
	return (result);
}

void	*arena_alloc_no_zero(t_arena *a, t_u64 size)
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

int	arena_unalloc(t_arena *a, t_u64 size)
{
	if (a->size - size > size)
	{
		errno = ERANGE;
		return (-1);	
	}
	a->size -= size;
	return (0);
}

// this probably is kind of useless. maybe?
void	arena_realloc(t_arena *a, void *data, t_u64 size)
{
	uint8_t *data_ptr;

	data_ptr = ((uint8_t *)data) + size;
	a->size += size;
	while (size--)
		data_ptr[size] = 0;
}

void arena_reset(t_arena *a)
{
	a->size = 0;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lt_alloc_arena.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 16:43:43 by ltaalas           #+#    #+#             */
/*   Updated: 2025/02/26 19:05:43 by ltaalas          ###   ########.fr       */
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

void arena_reset(t_arena *a)
{
	a->size = 0;
}

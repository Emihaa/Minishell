/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lt_arena.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 16:43:43 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/13 19:14:04 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/lt_arena.h"

void	*arena_alloc(t_arena *a, uint64_t size)
{
	void		*result;
	uint64_t	capacity;

	if (a->capacity < a->size + size)
	{
		while (a->next != NULL && a->next->capacity < a->next->size + size)
			a = a->next;
		if (a->next == NULL)
		{
			capacity = DEFAULT_ARENA_CAPACITY; 
			if (capacity < size)
				capacity = size;
			a->next = arena_new(capacity);
			if (a->next == NULL)
				return (NULL);
		}
		a = a->next;
	}
	result = &a->data[a->size];
	a->size += size;
	while (size--)
		((uint8_t *)result)[size] = 0;
	return (result);
}

void	*arena_alloc_no_zero(t_arena *a, uint64_t size)
{
	void		*result;
	uint64_t	capacity;

	if (a->capacity < a->size + size)
	{
		while (a->next != NULL && a->next->capacity < a->next->size + size)
			a = a->next;
		if (a->next == NULL)
		{
			capacity = DEFAULT_ARENA_CAPACITY; 
			if (capacity < size)
				capacity = size;
			a->next = arena_new(capacity);
			if (a->next == NULL)
				return (NULL);
		}
		a = a->next;
	}
	result = &a->data[a->size];
	a->size += size;
	return (result);
}

int	arena_unalloc(t_arena *a, uint64_t size)
{
	if (a->size - size > size)
	{
		return (-1);	
	}
	a->size -= size;
	return (0);
}

void	*arena_realloc(t_arena *arena, void *old, uint64_t old_siz, uint64_t new_siz)
{
	void	*new;

	if (new_siz <= old_siz)
		return (old);
	new = arena_alloc(arena, new_siz);
	if (new == NULL)
		return (NULL);
	(void)arena_memmove(new, old, old_siz);
	return (new);
}

void arena_reset(t_arena *a)
{
	while (a != NULL)
	{
		a->size = 0;
		a = a->next;
	}
}

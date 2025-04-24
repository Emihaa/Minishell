/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lt_arena_new_delete.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 23:54:14 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/24 17:53:23 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/lt_arena.h"

t_arena	*arena_new(uint64_t cap)
{
	t_arena	*arena;

	arena = malloc(sizeof(*arena) + cap);
	if (arena == NULL)
		return (NULL);
	arena->capacity = cap;
	arena->size = 0;
	arena->next = NULL;
	return (arena);
}

void	arena_delete(t_arena *arena)
{
	t_arena	*a0;
	t_arena	*a1;

	a0 = arena->next;
	while (a0 != NULL)
	{
		a1 = a0;
		a0 = a0->next;
		free(a1);
		a1 = NULL;
	}
	free(arena);
	arena = NULL;
}

void	arena_trim(t_arena *arena)
{
	t_arena	*a0;
	t_arena	*a1;

	a0 = arena->next;
	while (a0 != NULL)
	{
		a1 = a0;
		a0 = a0->next;
		free(a1);
		a1 = NULL;
	}
	arena->next = NULL;
}

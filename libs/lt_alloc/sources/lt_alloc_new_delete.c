/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lt_alloc_new_delete.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 23:54:14 by ltaalas           #+#    #+#             */
/*   Updated: 2025/02/26 19:05:37 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/lt_alloc.h"

t_arena arena_new(t_u64 cap)
{
	t_arena a;

	a.data = malloc(cap);
	if (a.data = NULL)
		return ((t_arena){0});
	a.capacity = cap;
	a.size = 0;
	return (a);
}

void arena_delete(t_arena *a)
{
	free(a->data);
	a->capacity = 0;
	a->size = 0;
}

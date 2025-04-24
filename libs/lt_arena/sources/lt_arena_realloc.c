/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lt_arena_realloc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 22:32:36 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/24 17:52:13 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/lt_arena.h"

void	*arena_memmove(void *dest, const void *src, size_t n)
{
	size_t		i;
	char		*d;
	const char	*s;

	if (dest == NULL || src == NULL)
		return (NULL);
	d = dest;
	s = src;
	if ((uintptr_t)d < (uintptr_t)s)
	{
		i = 0;
		while (i < n)
		{
			d[i] = s[i];
			i += 1;
		}
	}
	else
	{
		while (n-- > 0)
			d[n] = s[n];
	}
	return (dest);
}

void	*arena_realloc(t_arena *arena, void *old,
			uint64_t old_siz, uint64_t new_siz)
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

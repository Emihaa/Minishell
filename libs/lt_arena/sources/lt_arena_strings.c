/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lt_arena_strings.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 15:00:49 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/13 21:12:04 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/lt_arena.h"

size_t	arena_strlen(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

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

void	*arena_realloc_str(t_arena *arena, char *old, t_string *str, uint64_t new_size)
{
	const t_arena *backing_arena = str->backing_arena;
	char	*new;

	if (new_size <= str->capacity)
		return (old);
	if (backing_arena->capacity < new_size)
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
	new = arena_alloc(arena, new_size);
	if (new == NULL)
		return (NULL);
	(void)arena_memmove(new, old, old_siz);
	return (new);
}

void	arena_append_str_buf(t_arena *arena, t_string *dest, char *src, size_t src_len)
{
#define	ARENA_INIT_CAP 256

	size_t new_capacity;

	if (dest->size + src_len >= dest->capacity)
	{
		new_capacity = dest->capacity;
		if (new_capacity == 0)
			new_capacity = ARENA_INIT_CAP;
		while (dest->size + src_len > new_capacity)
			new_capacity *= 2;
		dest->base = arena_realloc(
			arena, 
			dest->base, 
			dest->capacity, 
			new_capacity);
		dest->capacity = new_capacity;
	}
	arena_memmove(&dest->base[dest->size], src, sizeof(*dest->base) * src_len);
	dest->size += src_len;
}
// seems problematic
void	arena_null_terminate_string(t_arena *arena, t_string *str)
{
	if (str->size >= str->capacity)
	{
		str->base = arena_realloc(arena, str->base, str->size, str->size + 1);
		if (str->base == NULL)
			; // PANIC
	}
	arena_unalloc(arena, str->capacity);
	arena_alloc_no_zero(arena, str->size + 1);
	str->base[str->size++] = '\0';
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lt_arena_strings.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 15:00:49 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/14 03:04:47 by ltaalas          ###   ########.fr       */
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

// void	arena_append_str_buf(t_arena *arena, t_string *dest, char *src, size_t src_len)
// {
// #define	ARENA_INIT_CAP 16

// 	size_t new_capacity;

// 	if (dest->size + src_len >= dest->capacity)
// 	{
// 		new_capacity = dest->capacity;
// 		if (new_capacity == 0)
// 			new_capacity = ARENA_INIT_CAP;
// 		while (dest->size + src_len > new_capacity)
// 			new_capacity *= 2;
// 		dest->base = arena_realloc(
// 			arena,
// 			dest->base,
// 			dest->capacity,
// 			new_capacity);
// 		dest->capacity = new_capacity;
// 	}
// 	arena_memmove(&dest->base[dest->size], src, sizeof(*dest->base) * src_len);
// 	dest->size += src_len;
// }
// // seems problematic
// void	arena_null_terminate_string(t_arena *arena, t_string *str)
// {
// 	if (str->size >= str->capacity)
// 	{
// 		str->base = arena_realloc(arena, str->base, str->size, str->size + 1);
// 		if (str->base == NULL)
// 		{
// 			; // PANIC
// 		}
// 	}
// 	arena_unalloc(arena, str->capacity);
// 	arena_alloc_no_zero(arena, str->size + 1);
// 	str->base[str->size++] = '\0';
// }



t_string start_string(t_arena *a, char *src, size_t len)
{
	t_string string;

	while (a->capacity < a->size + len)
		a = a->next;
	string.memory = a;
	string.base = arena_alloc(a, len);
	if (string.base == NULL)
		return ((t_string){0});
	(void)arena_memmove(string.base, src, len);
	string.size = len;
	return (string);
}

int	string_find_new_memory(t_arena *a, t_string *str, size_t new_size)
{
	size_t capacity;
	char	*new;

	if (a->capacity < new_size)
	{
		while (a->next != NULL && a->next->capacity < a->next->size + new_size)
			a = a->next;
		if (a->next == NULL)
		{
			capacity = DEFAULT_ARENA_CAPACITY;
			if (capacity < new_size)
				capacity = new_size;
			a->next = arena_new(capacity);
			if (a->next == NULL)
				return (-1);
			a = a->next;
		}
	}
	new = arena_alloc(a, new_size);
	arena_memmove(new, str->base, str->size);
	arena_unalloc(str->memory, str->size);
	str->memory = a;
	str->base = new;
	return (0);
}

int	append_to_string(t_arena *a, t_string *str, char *src, size_t src_len)
{
	size_t new_size;

	new_size = str->size + src_len;
	if (str->memory->capacity < str->memory->size + new_size)
	{
		if (string_find_new_memory(a, str, new_size))
			return (-1);
	}
	arena_memmove(&str->base[str->size], src, src_len);
	str->size += src_len;
	//str->memory->size += src_len;
	return (0);
}

void string_reset(t_arena *a, t_string *str)
{
	arena_unalloc(str->memory, str->size);
	str->memory = a;
	str->base = NULL;
	str->size = 0;
}
// terminate and resrve
int terminate_and_truncate_string(t_arena *a, t_string *str)
{
	#include <stdio.h>
	printf("<%lu> <%lu> <%lu>\n", str->size, str->memory->size, str->memory->capacity);
	if (str->memory->capacity < str->memory->size + str->size + 1)
	{
		if (string_find_new_memory(a, str, str->size + 1))
			return (-1);
	}
	str->base[str->size] = '\0';
	str->size += 1;
	str->memory->size += 1;
	return (0);
}

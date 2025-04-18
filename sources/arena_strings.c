/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena_strings.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 15:00:49 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/18 20:56:06 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

t_arena *find_free_arena(t_arena *a, size_t size)
{
	size_t	capacity;

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
	return (a);
}

int	string_find_new_memory(t_arena *a, t_string *str, size_t new_size)
{
	t_arena	*backing_memory;
	char	*new;

	backing_memory = find_free_arena(a, new_size);
	if (backing_memory == NULL)
		return (-1);
	new = arena_alloc(backing_memory, 0);
	arena_memmove(new, str->base, str->size);
	str->memory = backing_memory;
	str->base = new;
	str->capacity = backing_memory->capacity - backing_memory->size;
	return (0);
}

int	append_to_string(t_arena *a, t_string *str, char *src, size_t src_len)
{
	if (str->capacity < str->size + src_len)
	{
		if (string_find_new_memory(a, str, str->size + src_len	))
			return (-1);
	}
	arena_memmove(&str->base[str->size], src, src_len);
	str->size += src_len;
	return (0);
}

// terminate and resrve
int terminate_and_commit_string(t_arena *a, t_string *str)
{
	#include <stdio.h>
	printf("<%lu> <%lu> <%lu>\n", str->size, str->memory->size, str->memory->capacity);
	if (str->capacity < str->size + 1)
	{
		if (string_find_new_memory(a, str, str->size + 1))
			return (-1);
	}
	str->base[str->size] = '\0';
	str->size += 1;
	str->memory->size += str->size;
	str->capacity = str->size;
	return (0);
}

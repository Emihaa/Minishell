/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena_strings.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 15:00:49 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/26 00:16:41 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/// @TODO: check comment
t_arena	*find_free_arena(t_arena *a, size_t size)
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
			a->next = xarena_new(capacity);
		}
		a = a->next;
	}
	return (a);
}

void	string_find_new_memory(t_arena *a, t_string *str, size_t new_size)
{
	t_arena	*backing_memory;
	char	*new;

	backing_memory = find_free_arena(a, new_size);
	new = xarena_alloc(backing_memory, 0);
	ft_memmove(new, str->base, str->size);
	str->memory = backing_memory;
	str->base = new;
	str->capacity = backing_memory->capacity - backing_memory->size;
}

void	append_to_string(t_arena *a, t_string *str, char *src, size_t src_len)
{
	if (str->capacity < str->size + src_len)
	{
		string_find_new_memory(a, str, str->size + src_len);
	}
	ft_memmove(&str->base[str->size], src, src_len);
	str->size += src_len;
}

/// @brief terminate and reserve
void	terminate_and_commit_string(t_arena *a, t_string *str)
{
	if (str->capacity < str->size + 1)
	{
		string_find_new_memory(a, str, str->size + 1);
	}
	str->base[str->size] = '\0';
	str->size += 1;
	str->memory->size += str->size;
	str->capacity = str->size;
}

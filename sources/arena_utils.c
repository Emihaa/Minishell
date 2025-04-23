/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 17:43:04 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/23 19:48:50 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	*xarena_alloc(t_arena *arena, uint64_t size)
{
	void	*result;

	result = arena_alloc(arena, size);
	if (result == NULL)
	{
		(void)put_str(2, "minishell: Allocation failure\n");
		error_exit(get_minishell(NULL), 1);
	}
	return (result);
}

void	*xarena_alloc_no_zero(t_arena *arena, uint64_t size)
{
	void	*result;

	result = arena_alloc_no_zero(arena, size);
	if (result == NULL)
	{
		(void)put_str(2, "minishell: Allocation failure\n");
		error_exit(get_minishell(NULL), 1);
	}
	return (result);
}

t_arena	*xarena_new(uint64_t cap)
{
	t_arena	*arena;

	arena = malloc(sizeof(*arena) + cap);
	if (arena == NULL)
	{
		(void)put_str(2, "minishell: Allocation failure\n");
		error_exit(get_minishell(NULL), 1);
	}
	arena->capacity = cap;
	arena->size = 0;
	arena->next = NULL;
	return (arena);
}

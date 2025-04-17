/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 17:43:04 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/17 23:05:42 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void *xarena_alloc(t_arena *arena, uint64_t size)
{
	void *value;
	
	value = arena_alloc(arena, size);
	if (value == NULL)
	{
		(void)put_str(2, "arena allocation failure\n");
		error_exit(get_minishell(NULL), 1);
	}
	return (value);
}

void *xarena_alloc_no_zero(t_arena *arena, uint64_t size)
{
	void *value;
	
	value = arena_alloc_no_zero(arena, size);
	if (value == NULL)
	{
		(void)put_str(2, "arena allocation failure\n");
		error_exit(get_minishell(NULL), 1);
	}
	return (value);
}
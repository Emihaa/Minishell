/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lt_arena_temp.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehaanpaa <ehaanpaa@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 16:43:43 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/24 17:52:58 by ehaanpaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/lt_arena.h"

t_arena_temp	arena_temp_begin(t_arena *a)
{
	t_arena_temp	temp;

	temp.arena = a;
	temp.pos = a->size;
	return (temp);
}

void	arena_temp_end(t_arena_temp *a)
{
	a->arena->size = a->pos;
	a->arena = NULL;
	a->pos = 0;
}

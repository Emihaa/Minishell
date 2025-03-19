/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lt_alloc.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 21:59:14 by ltaalas           #+#    #+#             */
/*   Updated: 2025/03/19 23:18:55 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LT_ALLOC_H
# define LT_ALLOC_H

#	include <stdlib.h>
#	include <stddef.h>
#	include <stdint.h>
#	include <errno.h>

#define DEFAULT_ARENA_CAPACITY (1024 * 1024 * 256)

typedef struct s_arena
{
	uint64_t	capacity;
	uint64_t	size;
	uint8_t		*data;
}	t_arena;

typedef struct s_arena_temp
{
	t_arena *arena;
	uint64_t	pos;
}	t_arena_temp;

t_arena			arena_new(uint64_t cap);
void			arena_delete(t_arena *a);

void			*arena_alloc(t_arena *a, uint64_t size);
int				arena_unalloc(t_arena *a, uint64_t size);
void			arena_reset(t_arena *a);

t_arena_temp	arena_temp_begin(t_arena *a);
void			arena_temp_end(t_arena_temp *a);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lt_arena.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltaalas <ltaalas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 21:59:14 by ltaalas           #+#    #+#             */
/*   Updated: 2025/04/28 18:51:42 by ltaalas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LT_ARENA_H
# define LT_ARENA_H

# include <stdlib.h> // malloc, free
# include <stddef.h> // size_t
# include <stdint.h> // sized int types

# define DEFAULT_ARENA_CAPACITY 67108864 //(1024 * 1024 * 64)

typedef struct s_arena
{
	struct s_arena	*next;
	uint64_t		capacity;
	uint64_t		size;
	uint8_t			data[];
}	t_arena;

typedef struct s_arena_temp
{
	t_arena		*arena;
	uint64_t	pos;
}	t_arena_temp;

t_arena			*arena_new(uint64_t cap);
void			arena_delete(t_arena *a);
void			arena_trim(t_arena *arena);

void			*arena_alloc(t_arena *a, uint64_t size);
void			*arena_alloc_no_zero(t_arena *a, uint64_t size);
int				arena_unalloc(t_arena *a, uint64_t size);
void			*arena_realloc(t_arena *arena, void *old,
					uint64_t old_siz, uint64_t new_siz);

void			*arena_memmove(void *dest, const void *src, size_t n);
void			*arena_realloc(t_arena *arena, void *old,
					uint64_t old_siz, uint64_t new_siz);

void			arena_reset(t_arena *a);

t_arena_temp	arena_temp_begin(t_arena *a);
void			arena_temp_end(t_arena_temp *a);

#endif
